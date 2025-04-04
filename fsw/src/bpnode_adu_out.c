/*
 * NASA Docket No. GSC-18,587-1 and identified as “The Bundle Protocol Core Flight
 * System Application (BP) v6.5”
 *
 * Copyright © 2020 United States Government as represented by the Administrator of
 * the National Aeronautics and Space Administration. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * \file
 *   This file contains the source code for the BPNode ADU Out Child Task(s)
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_adu_out.h"


/*
** Function Definitions
*/

/* Create all ADU Out child task(s) */
int32 BPNode_AduOutCreateTasks(void)
{
    int32  Status;
    uint8  i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;

    /* Create all of the ADU Out task(s) */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_ADU_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[ADU Out #%d]: Failed to create initialization semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);
            return Status;
        }

        /* Create wakeup semaphore so main task can control workflow */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_ADU_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].WakeupSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[ADU Out #%d]: Failed to create wakeup semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create exit semaphore so main task knows when child finished shutdown */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_ADU_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_EXIT_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[ADU Out #%d]: Failed to create exit semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_OUT_BASE_NAME, i);
        TaskPriority = BPNODE_ADU_IN_PRIORITY_BASE + i;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.AduOutData[i].TaskId,
                                NameBuff, BPNode_AduOut_AppMain,
                                0, BPNODE_ADU_OUT_STACK_SIZE, TaskPriority, 0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU Out #%d]: Failed to create child task. Error = %d.",
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduOutData[i].InitSemId, BPNODE_ADU_OUT_SEM_INIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU Out #%d]: Task not running. Init Sem Error = %d.",
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for ADU Out task(s) */
int32 BPNode_AduOut_TaskInit(uint8 *ChanId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    uint8           i;

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_NO_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU Out #?]: Failed to get task ID. Error = %d", Status);
        return Status;
    }

    /* Map this task's ID to a channel ID */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        if (TaskId == BPNode_AppData.AduOutData[i].TaskId)
        {
            *ChanId = i;
        }
    }

    if (*ChanId == BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU Out #?]: Task ID does not match any known task IDs. ID = %d",
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    /* Initialize generic output buffer with a dummy msgid and max possible size */
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.AduOutData[*ChanId].OutBuf.TelemetryHeader), 
            CFE_SB_ValueToMsgId(1), sizeof(BPNode_AduOutBuf_t));

    BPNode_AppData.AduOutData[*ChanId].PerfId = BPNODE_ADU_OUT_PERF_ID_BASE + *ChanId;

    /* Start performance log */
    BPLib_PL_PerfLogEntry(BPNode_AppData.AduOutData[*ChanId].PerfId);

    /* Notify main task that child task is running */
    BPLib_PL_PerfLogExit(BPNode_AppData.AduOutData[*ChanId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.AduOutData[*ChanId].InitSemId);
    BPLib_PL_PerfLogEntry(BPNode_AppData.AduOutData[*ChanId].PerfId);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU Out #%d]: Failed to give init semaphore. Error = %d",
                          *ChanId, Status);
        return Status;
    }

    BPNode_AppData.AduOutData[*ChanId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    BPLib_EM_SendEvent(BPNODE_ADU_OUT_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                      "[ADU Out #%d]: Child Task Initialized.", *ChanId);

    return CFE_SUCCESS;
}

/* Main loop for ADU Out task(s) */
void BPNode_AduOut_AppMain(void)
{
    int32 Status;
    BPLib_Status_t BpStatus = BPLIB_SUCCESS;
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS; /* Set to garbage value */
    BPLib_NC_ApplicationState_t AppState;

    /* Perform task-specific initialization */
    Status = BPNode_AduOut_TaskInit(&ChanId);

    if (Status != CFE_SUCCESS)
    {
        /* Channel ID can't be determined, shut down immediately */
        if (ChanId == BPLIB_MAX_NUM_CHANNELS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "Terminating Unknown ADU Out Task.");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("Terminating Unknown ADU Out Task.\n");

            CFE_ES_ExitChildTask();

            return;
        }
        /* If channel ID can be determined, ready normal shutdown */
        else
        {
            BPNode_AppData.AduOutData[ChanId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* ADU Out task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.AduOutData[ChanId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        /* Attempt to take the wakeup semaphore */
        BPLib_PL_PerfLogExit(BPNode_AppData.AduOutData[ChanId].PerfId);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduOutData[ChanId].WakeupSemId, BPNODE_ADU_OUT_SEM_WAKEUP_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNode_AppData.AduOutData[ChanId].PerfId);

        if (Status == OS_SUCCESS)
        {
            do
            {
                AppState = BPLib_NC_GetAppState(ChanId);
                if (AppState == BPLIB_NC_APP_STATE_STARTED)
                {
                    /* Poll bundle from PI out queue */
                    BpStatus = BPA_ADUP_Out(ChanId, BPNODE_ADU_IN_PI_Q_TIMEOUT);
                    if (BpStatus == BPLIB_SUCCESS)
                    {
                        /* Success */
                    }
                    else if (BpStatus == BPLIB_PI_TIMEOUT)
                    {
                        /* This is ok, don't need to break */
                    }
                    else
                    {
                        // Event message
                        break;
                    }
                }
                else
                {
                    /* By breaking, we avoid busy-polling NC_GetAppState() */
                    break;
                }
            } while (BPNode_NotifIsSet(&BPNode_AppData.ChildStopWorkNotif) == false);
        }
        else if (Status == OS_SEM_TIMEOUT)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_SEM_TK_TIMEOUT_INF_EID,
                                BPLib_EM_EventType_INFORMATION,
                                "[ADU Out #%d]: Timed out while waiting for the wakeup semaphore",
                                ChanId);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_WAKEUP_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[ADU Out #%d]: Failed to take wakeup semaphore, RC = %d",
                                ChanId,
                                Status);
        }
    }

    /* Exit gracefully */
    BPNode_AduOut_TaskExit(ChanId);

    return;
}

/* Exit child task */
void BPNode_AduOut_TaskExit(uint8 ChanId)
{
    BPLib_EM_SendEvent(BPNODE_ADU_OUT_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "[ADU Out #%d]: Terminating Task. RunStatus = %d.",
                      ChanId, BPNode_AppData.AduOutData[ChanId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[ADU Out #%d]: Terminating Task. RunStatus = %d.\n",
                         ChanId, BPNode_AppData.AduOutData[ChanId].RunStatus);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.AduOutData[ChanId].PerfId);

    /* Signal to the main task that the child task has exited */
    (void) OS_BinSemGive(BPNode_AppData.AduOutData[ChanId].ExitSemId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
