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
 *   This file contains the source code for the BPNode ADU In Child Task(s)
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_adu_in.h"


/*
** Function Definitions
*/

/* Create all ADU In child task(s) */
int32 BPNode_AduInCreateTasks(void)
{
    int32  Status;
    uint32 i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;

    /* Create all of the ADU In task(s) */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_ADU_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[ADU In #%d]: Failed to create task initialization semaphore, %s. Error = %d",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create wakeup semaphore so main task can control ADU In tasks */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_ADU_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].WakeupSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[ADU In #%d]: Failed to create wakeup semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create exit semaphore so main task knows when child finished shutdown */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_ADU_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_EXIT_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[ADU In #%d]: Failed to create exit semaphore. Error = %d.",
                                i,
                                Status);

            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_BASE_NAME, i);
        TaskPriority = BPNODE_ADU_IN_PRIORITY_BASE + i;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.AduInData[i].TaskId,
                                NameBuff, BPNode_AduIn_AppMain,
                                0, BPNODE_ADU_IN_STACK_SIZE, TaskPriority, 0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU In #%d]: Failed to create child task. Error = %d.",
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduInData[i].InitSemId, BPNODE_ADU_IN_SEM_INIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU In #%d]: Task not running. Init Sem Error = %d.",
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for ADU In task(s) */
int32 BPNode_AduIn_TaskInit(uint32 *ChanId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    uint32          i;
    char            NameBuff[OS_MAX_API_NAME];

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_IN_NO_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU In #?]: Failed to get task ID. Error = %d", Status);
        return Status;
    }

    /* Map this task's ID to a channel ID */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        if (TaskId == BPNode_AppData.AduInData[i].TaskId)
        {
            *ChanId = i;
        }
    }

    if (*ChanId == BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_IN_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU In #?]: Task ID does not match any known task IDs. ID = %d",
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.AduInData[*ChanId].PerfId = BPNODE_ADU_IN_PERF_ID_BASE + *ChanId;

    /* Start performance log */
    BPLib_PL_PerfLogEntry(BPNode_AppData.AduInData[*ChanId].PerfId);

    /* Create ADU ingest pipe */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_PIPE_BASE_NAME, *ChanId);
    Status = CFE_SB_CreatePipe(&BPNode_AppData.AduInData[*ChanId].AduPipe,
                                    BPNODE_ADU_PIPE_DEPTH, NameBuff);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_IN_CR_PIPE_ERR_EID, BPLib_EM_EventType_ERROR,
                        "[ADU In #%d]: Error creating SB ADU Pipe, Error = %d",
                        *ChanId, Status);
        return Status;
    }

    /* Notify main task that child task is running */
    BPLib_PL_PerfLogExit(BPNode_AppData.AduInData[*ChanId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.AduInData[*ChanId].InitSemId);
    BPLib_PL_PerfLogEntry(BPNode_AppData.AduInData[*ChanId].PerfId);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU In #%d]: Failed to give init semaphore. Error = %d",
                          *ChanId, Status);
        return Status;
    }

    BPNode_AppData.AduInData[*ChanId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    BPLib_EM_SendEvent(BPNODE_ADU_IN_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                      "[ADU In #%d]: Child Task Initialized.", *ChanId);

    return CFE_SUCCESS;
}

/* Main loop for ADU In task(s) */
void BPNode_AduIn_AppMain(void)
{
    int32 Status;
    CFE_SB_Buffer_t *BufPtr = NULL;
    uint32 ChanId = BPLIB_MAX_NUM_CHANNELS; /* Set to garbage value */
    BPLib_NC_ApplicationState_t AppState;

    /* Perform task-specific initialization */
    Status = BPNode_AduIn_TaskInit(&ChanId);

    if (Status != CFE_SUCCESS)
    {
        /* Channel ID can't be determined, shut down immediately */
        if (ChanId == BPLIB_MAX_NUM_CHANNELS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "Terminating Unknown ADU In Task.");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("Terminating Unknown ADU In Task.\n");

            CFE_ES_ExitChildTask();

            return;
        }
        /* If channel ID can be determined, ready normal shutdown */
        else
        {
            BPNode_AppData.AduInData[ChanId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* ADU In task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.AduInData[ChanId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        BPLib_PL_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduInData[ChanId].WakeupSemId, BPNODE_ADU_IN_SEM_WAKEUP_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNode_AppData.AduInData[ChanId].PerfId);

        if (Status == OS_SUCCESS)
        {
            AppState = BPLib_NC_GetAppState(ChanId);
            if (AppState == BPLIB_NC_APP_STATE_STARTED)
            {
                /* Check for ADUs to ingest */
                do
                {
                    BPLib_PL_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

                    Status = CFE_SB_ReceiveBuffer(&BufPtr,
                                                BPNode_AppData.AduInData[ChanId].AduPipe,
                                                BPNODE_ADU_IN_SB_TIMEOUT);

                    BPLib_PL_PerfLogEntry(BPNode_AppData.AduInData[ChanId].PerfId);

                    if (Status == CFE_SUCCESS && BufPtr != NULL)
                    {
                        Status = BPA_ADUP_In((void *) BufPtr, ChanId);
                    }
                    else if (Status == CFE_SB_TIME_OUT)
                    {
                        /* This is ok, not a break condition */
                    }
                    else
                    {
                        break;
                    }
                } while (BPNode_NotifIsSet(&BPNode_AppData.ChildStopWorkNotif) == false);
            }
            else
            {
                /* Check if the application was recently stopped and the pipe needs to be cleared */
                if (BPNode_AppData.AduInData[ChanId].ClearPipe == true)
                {
                    BPLib_PL_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

                    while (Status == CFE_SUCCESS)
                    {
                        Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.AduInData[ChanId].AduPipe,
                                                    CFE_SB_POLL);
                    }

                    BPLib_PL_PerfLogEntry(BPNode_AppData.AduInData[ChanId].PerfId);

                    BPNode_AppData.AduInData[ChanId].ClearPipe = false;
                }
            }
        }
        else if (Status == OS_SEM_TIMEOUT)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_SEM_TK_TIMEOUT_INF_EID,
                                BPLib_EM_EventType_INFORMATION,
                                "[ADU In #%d]: Timed out while waiting for the wakeup semaphore",
                                ChanId);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_ADU_IN_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[ADU In #%d]: Failed to take wakeup semaphore, RC = %d",
                                ChanId,
                                Status);
        }
    }

    /* Exit gracefully */
    BPNode_AduIn_TaskExit(ChanId);

    return;
}

/* Exit child task */
void BPNode_AduIn_TaskExit(uint32 ChanId)
{
    BPLib_EM_SendEvent(BPNODE_ADU_IN_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "[ADU In #%d]: Terminating Task. RunStatus = %d.",
                      ChanId, BPNode_AppData.AduInData[ChanId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[ADU In #%d]: Terminating Task. RunStatus = %d.\n",
                         ChanId, BPNode_AppData.AduInData[ChanId].RunStatus);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

    /* Signal to the main task that the child task has exited */
    (void) OS_BinSemGive(BPNode_AppData.AduInData[ChanId].ExitSemId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
