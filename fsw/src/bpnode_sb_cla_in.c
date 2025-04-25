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

/* ======== */
/* Includes */
/* ======== */
#include "bpnode_app.h"
#include "bpnode_sb_cla_in.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

CFE_Status_t BPNode_SB_ClaInCreateTasks(void)
{
    CFE_Status_t Status;
    char         NameBuff[OS_MAX_API_NAME];
    uint16       TaskPriority;

    /* Create wakeup semaphore so main task can control workflow */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE", BPNODE_SB_CLA_IN_SEM_BASE_NAME);
    Status = OS_BinSemCreate(&BPNode_AppData.SB_ClaInData.WakeupSemId, NameBuff, 0, 0);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_WAKEUP_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Failed to create wakeup semaphore, %s, for CLA In #%d. Error = %d",
                            NameBuff,
                            Status);
    }
    else
    {
        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s", BPNODE_SB_CLA_IN_BASE_NAME);
        TaskPriority = BPNODE_CLA_IN_PRIORITY_BASE;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.SB_ClaInData.TaskId,
                                        NameBuff,
                                        BPNode_SB_ClaIn_AppMain,
                                        0,
                                        BPNODE_CLA_IN_STACK_SIZE,
                                        TaskPriority,
                                        0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Failed to create child task for SB CLA In task. Error = %d",
                                Status);
        }
    }

    return Status;
}

/* Main loop for CLA In task(s) */
void BPNode_SB_ClaIn_AppMain(void)
{
    int32                       OsStatus;
    CFE_Status_t                CFE_Status;
    BPLib_Status_t              Status;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;

    /* Get this tasks ID to reference later */
    CFE_Status = CFE_ES_GetTaskID(&TaskId);
    if (CFE_Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_UNK_EXIT_CRIT_EID,
                            BPLib_EM_EventType_CRITICAL,
                            "[SB CLA In]: Terminating unknown task");

        /* In case event services is not working, add a message to the system log */
        CFE_ES_WriteToSysLog("Terminating unknown task");

        /* Stop execution */
        CFE_ES_ExitChildTask();
    }
    else
    {
        if (BPNode_AppData.SB_ClaInData.TaskId == TaskId)
        {
            /* Tie this SB CLA In task to the CLA In task on channel 0 */
            Status = BPLib_CLA_GetContactRunState(0, &RunState);
            while (RunState != BPLIB_CLA_EXITED && Status == BPLIB_SUCCESS)
            {
                /* Attempt to take the wakeup semaphore */
                BPLib_PL_PerfLogExit(BPNode_AppData.SB_ClaInData.PerfId);
                OsStatus = OS_BinSemTimedWait(BPNode_AppData.SB_ClaInData.WakeupSemId, BPNODE_CLA_IN_SEM_WAKEUP_WAIT_MSEC);
                BPLib_PL_PerfLogEntry(BPNode_AppData.SB_ClaInData.PerfId);

                if (OsStatus == OS_SUCCESS)
                {
                    /* Ingress bundles only when the contact has been started */
                    if (RunState == BPLIB_CLA_STARTED)
                    {
                        printf("Process bundles here\n");
                    }
                }
                else if (OsStatus == OS_SEM_TIMEOUT)
                {
                    BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_SEM_TK_TIMEOUT_INF_EID,
                                        BPLib_EM_EventType_INFORMATION,
                                        "[SB CLA In]: Wakeup semaphore wait timed out");
                }
                else
                {
                    BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_WAKEUP_SEM_ERR_EID,
                                        BPLib_EM_EventType_ERROR,
                                        "[SB CLA In]: Wakeup semaphore take failed, RC = %d",
                                        OsStatus);
                }

                /* Update run state of the contact task */
                Status = BPLib_CLA_GetContactRunState(0, &RunState);
            }

            /* Exit gracefully */
            BPNode_SB_ClaIn_TaskExit();
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_INV_ID_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[SB CLA In] Could not find an SB CLA In task to process bundles with");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("[SB CLA In] Could not find an SB CLA In task to process bundles with");

            /* Stop execution */
            CFE_ES_ExitChildTask();
        }
    }

    return;
}

void BPNode_SB_ClaIn_TaskExit(void)
{
    BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_UNK_EXIT_CRIT_EID,
                        BPLib_EM_EventType_CRITICAL,
                        "[SB CLA In]: Terminating Task.");

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[SB CLA In]: Terminating Task.");

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.SB_ClaInData.PerfId);

    /* Return semaphores */
    BPNode_SB_ClaIn_DeleteSems();

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}

void BPNode_SB_ClaIn_DeleteSems(void)
{
    CFE_Status_t Status;

    Status = OS_BinSemDelete(BPNode_AppData.SB_ClaInData.WakeupSemId);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_SB_CLA_IN_WAKEUP_SEM_ERR_EID,
                            BPLib_EM_EventType_ERROR,
                            "Could not delete wake up semaphore");
    }
}