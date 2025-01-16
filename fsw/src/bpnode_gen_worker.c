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
 *   This file contains the source code for the BPNode Generic Worker Child Task(s)
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_gen_worker.h"


/*
** Function Definitions
*/

/* Create all Generic Worker child task(s) */
int32 BPNode_GenWorkerCreateTasks(void)
{
    int32  Status;
    uint8  i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;

    /* Create all of the Generic Worker task(s) */
    for (i = 0; i < BPNODE_NUM_GEN_WRKR_TASKS; i++)
    {
        /* Create initialization semaphore for main task control */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_GEN_WRKR_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.GenWorkerData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_SEM_CR_ERR_EID, BPLib_EM_EventType_ERROR,
                        "[Generic Worker #%d]: Failed to create initialization semaphore. Error = %d.",
                        i, Status);
            return Status;
        }

        /* Create wakeup semaphore for main task control */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKEUP_%d", BPNODE_GEN_WRKR_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.GenWorkerData[i].WakeupSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_SEM_CR_ERR_EID, BPLib_EM_EventType_ERROR,
                        "[Generic Worker #%d]: Failed to create wakeup semaphore. Error = %d.",
                        i, Status);
            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_GEN_WRKR_BASE_NAME, i);
        TaskPriority = BPNODE_ADU_IN_PRIORITY_BASE + i;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.GenWorkerData[i].TaskId,
                                NameBuff, BPNode_GenWorker_AppMain,
                                0, BPNODE_GEN_WRKR_STACK_SIZE, TaskPriority, 0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[Generic Worker #%d]: Failed to create child task. Error = %d.",
                            i, Status);
            return Status;
        }

        /* Verify initialization by trying to take the semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.GenWorkerData[i].InitSemId, BPNODE_GEN_WRKR_SEM_INIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[Generic Worker #%d]: Task not running. Init Sem Error = %d.",
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for Generic Worker task(s) */
int32 BPNode_GenWorker_TaskInit(uint8 *WorkerId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    uint8           i;

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_GEN_WRKR_NO_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[Generic Worker #?]: Failed to get task ID. Error = %d", Status);
        return Status;
    }

    /* Map this task's ID to a worker ID */
    for (i = 0; i < BPNODE_NUM_GEN_WRKR_TASKS; i++)
    {
        if (TaskId == BPNode_AppData.GenWorkerData[i].TaskId)
        {
            *WorkerId = i;
        }
    }

    if (*WorkerId == BPNODE_NUM_GEN_WRKR_TASKS)
    {
        BPLib_EM_SendEvent(BPNODE_GEN_WRKR_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[Generic Worker #?]: Task ID does not match any known task IDs. ID = %d",
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.GenWorkerData[*WorkerId].PerfId = BPNODE_GEN_WRKR_PERF_ID_BASE + *WorkerId;

    /* Notify main task that child task is running */
    Status = OS_BinSemGive(BPNode_AppData.GenWorkerData[*WorkerId].InitSemId);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_GEN_WRKR_SEM_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[Generic Worker #%d]: Failed to give init semaphore. Error = %d",
                          *WorkerId, Status);
        return Status;
    }

    BPNode_AppData.GenWorkerData[*WorkerId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    BPLib_EM_SendEvent(BPNODE_GEN_WRKR_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                      "[Generic Worker #%d]: Child Task Initialized.", *WorkerId);

    /* Start performance log */
    BPLib_PL_PerfLogEntry(BPNode_AppData.GenWorkerData[*WorkerId].PerfId);

    return CFE_SUCCESS;
}

/* Main loop for Generic Worker task(s) */
void BPNode_GenWorker_AppMain(void)
{
    int32 Status;
    BPLib_Status_t BpStatus = BPLIB_SUCCESS;
    uint8 WorkerId = BPNODE_NUM_GEN_WRKR_TASKS; /* Set to garbage value */
    uint32 NumJobsComplete = 0;

    /* Perform task-specific initialization */
    Status = BPNode_GenWorker_TaskInit(&WorkerId);

    if (Status != CFE_SUCCESS)
    {
        /* Worker ID can't be determined, shut down immediately */
        if (WorkerId == BPNODE_NUM_GEN_WRKR_TASKS)
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "Terminating Unknown Generic Worker Task.");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("Terminating Unknown Generic Worker Task.\n");

            CFE_ES_ExitChildTask();

            return;
        }
        /* If worker ID can be determined, ready normal shutdown */
        else
        {
            BPNode_AppData.GenWorkerData[WorkerId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* Generic Worker task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.GenWorkerData[WorkerId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        /* Take semaphore from main task */
        BPLib_PL_PerfLogExit(BPNode_AppData.GenWorkerData[WorkerId].PerfId);
        Status = OS_BinSemTimedWait(BPNode_AppData.GenWorkerData[WorkerId].WakeupSemId, BPNODE_GEN_WRKR_SEM_WAKEUP_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNode_AppData.GenWorkerData[WorkerId].PerfId);

        /* Process one cycle's worth of jobs */
        if (Status == OS_SUCCESS)
        {
            while (BpStatus == BPLIB_SUCCESS && NumJobsComplete < BPNODE_NUM_JOBS_PER_CYCLE)
            {
                /*
                ** TODO call the relevant BPLib JS API to process one job
                */

                BPLib_PL_PerfLogExit(BPNode_AppData.GenWorkerData[WorkerId].PerfId);
                (void) OS_TaskDelay(BPNODE_GEN_WRKR_SLEEP_MSEC);
                BPLib_PL_PerfLogEntry(BPNode_AppData.GenWorkerData[WorkerId].PerfId);

                NumJobsComplete++;
            }

            NumJobsComplete = 0;
        }
        else if (Status == OS_SEM_TIMEOUT)
        {
            BPLib_EM_SendEvent(BPNODE_SEM_TAKE_TIMEOUT_ERR_EID,
                                BPLib_EM_EventType_INFORMATION,
                                "[Generic Worker #%d]: Timed out while waiting for the wakeup semaphore",
                                WorkerId);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_GEN_WRKR_SEM_TK_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[Generic Worker #%d]: Failure to take semaphore. Sem Error = %d.",
                            WorkerId, Status);
        }
    }

    /* Exit gracefully */
    BPNode_GenWorker_TaskExit(WorkerId);

    return;
}

/* Exit child task */
void BPNode_GenWorker_TaskExit(uint8 WorkerId)
{
    BPLib_EM_SendEvent(BPNODE_GEN_WRKR_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "[Generic Worker #%d]: Terminating Task. RunStatus = %d.",
                      WorkerId, BPNode_AppData.GenWorkerData[WorkerId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[Generic Worker #%d]: Terminating Task. RunStatus = %d.\n",
                         WorkerId, BPNode_AppData.GenWorkerData[WorkerId].RunStatus);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.GenWorkerData[WorkerId].PerfId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}
