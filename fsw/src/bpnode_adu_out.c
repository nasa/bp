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
** Global Data
*/

BPNode_AppData_t BPNode_AppData;

/* Create all ADU Out child task(s) */
int32 BPNode_AduOutCreateTasks(void)
{
    int32 Status;
    uint8 i;
    char  NameBuff[OS_MAX_API_NAME];
    
    /* Create all of the ADU Out task(s) */
    for (i = 0; i < BPNODE_TOTAL_ADU_PROXIES; i++)
    {
        /*
        ** Create an initialization and an exit semaphore for each ADU Out Task, to signal 
        ** that the task has completed its initialization procedures or exited, respectively. 
        ** The child task  returns the semaphore after completing the relevant procedure.
        */

        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_OUT_INIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_OUT_INIT_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                        "Failed to create the ADU Out #%d task init semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }

        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_OUT_EXIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduOutData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_OUT_EXIT_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                        "Failed to create the ADU Out #%d task exit semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_OUT_BASE_NAME, i);
        Status = CFE_ES_CreateChildTask(&BPNode_AppData.AduOutData[i].TaskId,
                                NameBuff, BPNode_AduOut_AppMain,
                                0, BPNODE_ADU_OUT_STACK_SIZE, BPNODE_ADU_OUT_PRIORITY, 0);

        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_OUT_CREATE_ERR_EID, CFE_EVS_EventType_ERROR,
                            "Failed to create the ADU Out #%d child task. Error = %d.", 
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the init semaphore */
        CFE_ES_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.AduOutData[i].InitSemId, 
                                                                BPNODE_SEM_WAIT_MSEC);
        CFE_ES_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_OUT_RUN_ERR_EID, CFE_EVS_EventType_ERROR,
                            "ADU Out #%d task not running. Init Sem Error = %d.", 
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for ADU Out task(s) */
int32 BPNode_AduOut_TaskInit(uint32 *AduOutId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    uint8           i;

    /* Set to invalid value */
    *AduOutId = BPNODE_TOTAL_ADU_PROXIES; 

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_OUT_NO_ID_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU Out #?]: Failed to get task ID. Error = %d", Status);
        return Status;        
    }

    /* Map this task's ID to an ADU Out ID */
    for (i = 0; i < BPNODE_TOTAL_ADU_PROXIES; i++)
    {
        if (TaskId == BPNode_AppData.AduOutData[i].TaskId)
        {
            *AduOutId = i;
        }
    }

    if (*AduOutId == BPNODE_TOTAL_ADU_PROXIES)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_OUT_INV_ID_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU Out #?]: Task ID does not match any known task IDs. ID = %d", 
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.AduOutData[*AduOutId].PerfId = BPNODE_ADU_OUT_PERF_ID_BASE + *AduOutId;

    /* Start performance log */
    CFE_ES_PerfLogEntry(BPNode_AppData.AduOutData[*AduOutId].PerfId);

    /* Notify main task that child task is running */
    CFE_ES_PerfLogExit(BPNode_AppData.AduOutData[*AduOutId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.AduOutData[*AduOutId].InitSemId);
    CFE_ES_PerfLogEntry(BPNode_AppData.AduOutData[*AduOutId].PerfId);

    if (Status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU Out #%d]: Failed to give init semaphore. Error = %d", 
                          *AduOutId, Status);
        return Status;
    }

    BPNode_AppData.AduOutData[*AduOutId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_EVS_SendEvent(BPNODE_ADU_OUT_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "[ADU Out #%d]: Child Task Initialized.", *AduOutId);

    return CFE_SUCCESS;
}

/* Main loop for ADU Out task(s) */
void BPNode_AduOut_AppMain(void)
{
    int32  Status;
    uint32 AduOutId;

    /* Perform task-specific initialization */
    Status = BPNode_AduOut_TaskInit(&AduOutId);

    if (Status != CFE_SUCCESS)
    {
        BPNode_AppData.AduOutData[AduOutId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /* ADU Out task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.AduOutData[AduOutId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        /* TODO */
    }

    /* Exit gracefully */
    BPNode_AduOut_TaskExit(AduOutId);

    return;
}

/* Exit child task */
void BPNode_AduOut_TaskExit(uint32 AduOutId)
{
    /* Signal to the main task that the child task has exited */
    CFE_ES_PerfLogExit(BPNode_AppData.AduOutData[AduOutId].PerfId);
    (void) OS_BinSemGive(BPNode_AppData.AduOutData[AduOutId].ExitSemId);
    CFE_ES_PerfLogEntry(BPNode_AppData.AduOutData[AduOutId].PerfId);

    CFE_EVS_SendEvent(BPNODE_ADU_OUT_EXIT_CRIT_EID, CFE_EVS_EventType_CRITICAL,
                      "[ADU Out #%d]: Terminating Task. RunStatus = %d.",
                      AduOutId, BPNode_AppData.AduOutData[AduOutId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[ADU Out #%d]: Terminating Task. RunStatus = %d.\n",
                         AduOutId, BPNode_AppData.AduOutData[AduOutId].RunStatus);

    /* Exit the perf log */
    CFE_ES_PerfLogExit(BPNode_AppData.AduOutData[AduOutId].PerfId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
