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
** Global Data
*/

BPNode_AppData_t BPNode_AppData;


/* Create all ADU In child task(s) */
int32 BPNode_AduInCreateTasks(void)
{
    int32 Status;
    uint8 i;
    char  NameBuff[OS_MAX_API_NAME];
    
    /* Create all of the ADU In task(s) */
    for (i = 0; i < BPNODE_TOTAL_ADU_PROXIES; i++)
    {
        /*
        ** Create an initialization and an exit semaphore for each ADU In Task, to signal 
        ** that the task has completed its initialization procedures or exited, respectively. 
        ** The child task  returns the semaphore after completing the relevant procedure.
        */

        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_INIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                        "Failed to create the ADU In #%d task init semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }

        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_EXIT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.AduInData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_IN_EXIT_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                        "Failed to create the ADU In #%d task exit semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_ADU_IN_BASE_NAME, i);
        Status = CFE_ES_CreateChildTask(&BPNode_AppData.AduInData[i].TaskId,
                                NameBuff, BPNode_AduIn_AppMain,
                                0, BPNODE_ADU_IN_STACK_SIZE, BPNODE_ADU_IN_PRIORITY, 0);

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
int32 BPNode_AduIn_TaskInit(uint32 *AduInId)
{
    int32  Status;

    *AduInId = 0; /* TODO set ID properly */

    BPNode_AppData.AduInData[*AduInId].PerfId = BPNODE_ADU_IN_PERF_ID_BASE + *AduInId;

    /* Start performance log */
    CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[*AduInId].PerfId);

    /* Notify main task that child task is running */
    CFE_ES_PerfLogExit(BPNode_AppData.AduInData[*AduInId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.AduInData[*AduInId].InitSemId);
    CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[*AduInId].PerfId);

    if (Status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID, CFE_EVS_EventType_ERROR,
                          "[ADU In #%d]: Failed to give init semaphore. Error = %d", 
                          *AduInId, Status);
        return Status;
    }

    BPNode_AppData.AduInData[*AduInId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    CFE_EVS_SendEvent(BPNODE_ADU_IN_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "[ADU In #%d]: Child Task Initialized.", *AduInId);

    return CFE_SUCCESS;
}

/* Main loop for ADU In task(s) */
void BPNode_AduIn_AppMain(void)
{
    int32  Status;
    uint32 AduInId;

    /* Perform task-specific initialization */
    Status = BPNode_AduIn_TaskInit(&AduInId);

    if (Status != CFE_SUCCESS)
    {
        BPNode_AppData.AduInData[AduInId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /* ADU In task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.AduInData[AduInId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        /* TODO */
    }

    /* Exit gracefully */
    BPNode_AduIn_TaskExit(AduInId);

    return;
}

/* Exit child task */
void BPNode_AduIn_TaskExit(uint32 AduInId)
{
    /* Signal to the main task that the child task has exited */
    CFE_ES_PerfLogExit(BPNode_AppData.AduInData[AduInId].PerfId);
    (void) OS_BinSemGive(BPNode_AppData.AduInData[AduInId].ExitSemId);
    CFE_ES_PerfLogEntry(BPNode_AppData.AduInData[AduInId].PerfId);

    CFE_EVS_SendEvent(BPNODE_ADU_IN_EXIT_CRIT_EID, CFE_EVS_EventType_CRITICAL,
                      "[ADU In #%d]: Terminating Task. RunStatus = %d.",
                      AduInId, BPNode_AppData.AduInData[AduInId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[ADU In #%d]: Terminating Task. RunStatus = %d.\n",
                         AduInId, BPNode_AppData.AduInData[AduInId].RunStatus);

    /* Exit the perf log */
    CFE_ES_PerfLogExit(BPNode_AppData.AduInData[AduInId].PerfId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
