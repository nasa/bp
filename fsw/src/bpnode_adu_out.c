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
    for (i = 0; i < BPNODE_MAX_NUM_CHANNELS; i++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_OUT_INIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Failed to create the ADU Out #%d task init semaphore. Error = %d.", 
                        i, Status);
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
                            "Failed to create the ADU Out #%d child task. Error = %d.", 
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduOutData[i].InitSemId, 
                                                                BPNODE_SEM_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "ADU Out #%d task not running. Init Sem Error = %d.", 
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
    for (i = 0; i < BPNODE_MAX_NUM_CHANNELS; i++)
    {
        if (TaskId == BPNode_AppData.AduOutData[i].TaskId)
        {
            *ChanId = i;
        }
    }

    if (*ChanId == BPNODE_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[ADU Out #?]: Task ID does not match any known task IDs. ID = %d", 
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

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
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS; /* Set to garbage value */
    bool  NewMsgRecvd;
    CFE_SB_Buffer_t *Buf;

    /* Perform task-specific initialization */
    Status = BPNode_AduOut_TaskInit(&ChanId);

    if (Status != CFE_SUCCESS)
    {
        /* Channel ID can't be determined, shut down immediately */
        if (ChanId == BPNODE_MAX_NUM_CHANNELS)
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
        if (BPNode_AppData.AduConfigs[ChanId].AppState == BPA_ADUP_APP_STARTED)
        {
            /* TODO Pend on bundle from PI out queue */
            (void) OS_TaskDelay(BPNODE_ADU_OUT_SLEEP_MSEC);
            NewMsgRecvd = false;
            Buf = NULL;

            if (NewMsgRecvd == true)
            {
                (void) BPA_ADUP_Out((void *) Buf, ChanId);
            }
        }
        else 
        {
            (void) OS_TaskDelay(BPNODE_ADU_OUT_SLEEP_MSEC);
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

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
