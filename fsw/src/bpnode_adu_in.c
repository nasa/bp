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
#include "fwp.h"


/*
** Function Definitions
*/

/* Create all ADU In child task(s) */
int32 BPNode_AduInCreateTasks(void)
{
    int32  Status;
    uint8  i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;
    
    /* Create all of the ADU In task(s) */
    for (i = 0; i < BPNODE_MAX_NUM_CHANNELS; i++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_INIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                        "Failed to create the ADU In #%d task init semaphore. Error = %d.", 
                        i, Status);
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
            CFE_EVS_SendEvent(BPNODE_ADU_IN_CREATE_ERR_EID, CFE_EVS_EventType_ERROR,
                            "Failed to create the ADU In #%d child task. Error = %d.", 
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the init semaphore */
        CFE_ES_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduInData[i].InitSemId, 
                                                                BPNODE_SEM_WAIT_MSEC);
        CFE_ES_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_IN_RUN_ERR_EID, CFE_EVS_EventType_ERROR,
                            "ADU In #%d task not running. Init Sem Error = %d.", 
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for ADU In task(s) */
int32 BPNode_AduIn_TaskInit(uint8 *ChanId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    uint8           i;
    char            NameBuff[OS_MAX_API_NAME];

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_IN_NO_ID_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU In #?]: Failed to get task ID. Error = %d", Status);
        return Status;        
    }

    /* Map this task's ID to a channel ID */
    for (i = 0; i < BPNODE_MAX_NUM_CHANNELS; i++)
    { 
        if (TaskId == BPNode_AppData.AduInData[i].TaskId)
        {
            *ChanId = i;
        }
    }

    if (*ChanId == BPNODE_MAX_NUM_CHANNELS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_IN_INV_ID_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU In #?]: Task ID does not match any known task IDs. ID = %d", 
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.AduInData[*ChanId].PerfId = BPNODE_ADU_IN_PERF_ID_BASE + *ChanId;

    /* Start performance log */
    CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[*ChanId].PerfId);

    /* Create ADU ingest pipe */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_PIPE_BASE_NAME, *ChanId);
    Status = CFE_SB_CreatePipe(&BPNode_AppData.AduInData[*ChanId].AduPipe, 
                                    BPNODE_ADU_PIPE_DEPTH, NameBuff);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_IN_CR_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                        "[ADU In #%d]: Error creating SB ADU Pipe, Error = %d", 
                        *ChanId, Status);
        return Status;
    }

    /* Notify main task that child task is running */
    CFE_ES_PerfLogExit(BPNode_AppData.AduInData[*ChanId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.AduInData[*ChanId].InitSemId);
    CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[*ChanId].PerfId);

    if (Status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU In #%d]: Failed to give init semaphore. Error = %d", 
                          *ChanId, Status);
        return Status;
    }

    BPNode_AppData.AduInData[*ChanId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "[ADU In #%d]: Child Task Initialized.", *ChanId);

    return CFE_SUCCESS;
}

/* Main loop for ADU In task(s) */
void BPNode_AduIn_AppMain(void)
{
    int32 Status;
    CFE_SB_Buffer_t *BufPtr = NULL;
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS; /* Set to garbage value */

    /* Perform task-specific initialization */
    Status = BPNode_AduIn_TaskInit(&ChanId);

    if (Status != CFE_SUCCESS)
    {
        /* Channel ID can't be determined, shut down immediately */
        if (ChanId == BPNODE_MAX_NUM_CHANNELS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_IN_UNK_EXIT_CRIT_EID, CFE_EVS_EventType_CRITICAL,
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
        if (BPNode_AppData.AduConfigs[ChanId].AppState == BPA_ADUP_APP_STARTED)
        {
            /* Check for ADUs to ingest */
            do
            {
                CFE_ES_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

                Status = CFE_SB_ReceiveBuffer(&BufPtr, 
                                              BPNode_AppData.AduInData[ChanId].AduPipe,
                                              BPNODE_ADU_IN_SB_TIMEOUT);

                CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[ChanId].PerfId);

                if (Status == CFE_SUCCESS && BufPtr != NULL)
                {
                    Status = BPA_ADUP_In((void *) BufPtr, ChanId);
                }

            } while (Status == BPLIB_SUCCESS);
        }
        else 
        {
            /* Check if the application was recently stopped and the pipe needs to be cleared */
            if (BPNode_AppData.AduInData[ChanId].ClearPipe == true)
            {
                CFE_ES_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

                while (Status == CFE_SUCCESS)
                {
                    Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.AduInData[ChanId].AduPipe,
                                                CFE_SB_POLL);
                }

                CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[ChanId].PerfId);
            }

            /* Sleep until app is started again */
            (void) OS_TaskDelay(BPNODE_ADU_IN_SLEEP_MSEC);
        }
    }

    /* Exit gracefully */
    BPNode_AduIn_TaskExit(ChanId);

    return;
}

/* Exit child task */
void BPNode_AduIn_TaskExit(uint8 ChanId)
{
    CFE_EVS_SendEvent(BPNODE_ADU_IN_EXIT_CRIT_EID, CFE_EVS_EventType_CRITICAL,
                      "[ADU In #%d]: Terminating Task. RunStatus = %d.",
                      ChanId, BPNode_AppData.AduInData[ChanId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[ADU In #%d]: Terminating Task. RunStatus = %d.\n",
                         ChanId, BPNode_AppData.AduInData[ChanId].RunStatus);

    /* Exit the perf log */
    CFE_ES_PerfLogExit(BPNode_AppData.AduInData[ChanId].PerfId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
