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
 *   This file contains the source code for the BPNode App.
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_utils.h"
#include "bpnode_eventids.h"
#include "bpnode_tbl.h"
#include "bpnode_version.h"

#include "bplib.h"
#include "fwp.h"

/*
** Global Data
*/

BPNode_AppData_t BPNode_AppData;


/*
** Function Definitions
*/

/* Application entry point and main process loop */
void BPNode_AppMain(void)
{
    CFE_Status_t     Status;
    CFE_SB_Buffer_t *BufPtr;

    /* Create the first Performance Log entry */
    BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

    /*
    ** Perform application-specific initialization
    ** If the initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the app will not enter the run loop
    */
    Status = BPNode_AppInit();
    if (Status != CFE_SUCCESS)
    {
        BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /* BPNode run loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.RunStatus) == true)
    {
        /* Performance Log Exit Stamp */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);

        /* Pend on receipt of wakeup message */
        Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.WakeupPipe,
                                                BPNODE_WAKEUP_PIPE_TIMEOUT);

        /* Performance Log Entry Stamp */
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status == CFE_SUCCESS || Status == CFE_SB_TIME_OUT || Status == CFE_SB_NO_MESSAGE)
        {
            /* Process wakeup tasks */
            Status = BPNode_WakeupProcess();
        }

        /* Exit upon pipe read error */
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_PIPE_ERR_EID, BPLib_EM_EventType_ERROR,
                              "SB Pipe Read Error, App Will Exit");

            BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    CFE_ES_ExitApp(BPNode_AppData.RunStatus);
}


/* Perform wakeup processing */
CFE_Status_t BPNode_WakeupProcess(void)
{
    CFE_Status_t                Status;
    BPLib_Status_t              BpStatus;
    int32                       OsStatus;
    CFE_SB_Buffer_t*            BufPtr;
    uint32                      ContactNum;

    BufPtr = NULL;

    /* Check if any bundles are in cache, routing them to an egress route */
    (void) BPLib_STOR_ScanCache(&BPNode_AppData.BplibInst, BPNODE_MAX_BUNDLES_TO_ENQUEUE_DURING_CACHE_SCAN);

    /* Request the event loop to run up to 'BPNODE_NUM_JOBS_PER_CYCLE' */
    BPLib_QM_SortJobs(&BPNode_AppData.BplibInst, BPNODE_NUM_JOBS_PER_CYCLE);

    /* Wake up the ADU In and ADU Out tasks */
    for (ContactNum = 0; ContactNum < BPLIB_MAX_NUM_CHANNELS; ContactNum++)
    {
        OsStatus = OS_BinSemGive(BPNode_AppData.AduInData[ContactNum].WakeupSemId);
        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_WKP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Error giving ADU In Task #%d its wakeup semaphore, RC = %d",
                                ContactNum,
                                OsStatus);
        }

        OsStatus = OS_BinSemGive(BPNode_AppData.AduOutData[ContactNum].WakeupSemId);
        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_WKP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Error giving ADU Out Task #%d its wakeup semaphore, RC = %d",
                                ContactNum,
                                OsStatus);
        }
    }

    /* Wake up the CLA In and CLA Out tasks */
    for (ContactNum = 0; ContactNum < BPLIB_MAX_NUM_CONTACTS; ContactNum++)
    {
        OsStatus = OS_BinSemGive(BPNode_AppData.ClaInData[ContactNum].WakeupSemId);
        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_WKP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Could not wake up CLA In task #%d, RC = %d",
                                ContactNum,
                                OsStatus);
        }

        OsStatus = OS_BinSemGive(BPNode_AppData.ClaOutData[ContactNum].WakeupSemId);
        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_WKP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Could not wake up CLA Out task #%d, RC = %d",
                                ContactNum,
                                OsStatus);
        }
    }

    /* Update time as needed */
    BpStatus = BPLib_TIME_MaintenanceActivities();

    if (BpStatus != BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TIME_WKP_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error doing time maintenance activities, RC = %d", BpStatus);
    }

    /* Call NC to update configurations */
    BpStatus = BPLib_NC_ConfigUpdate();
    if (BpStatus != BPLIB_SUCCESS && BpStatus != BPLIB_TBL_UPDATED)
    {
        BPLib_EM_SendEvent(BPNODE_NC_CFG_UPDATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error managing configurations on wakeup, Status=0x%08X",
                            BpStatus);
    }

    /* Update the ADUP configuration individually since it's owned by BPNode */
    BpStatus = BPA_TABLEP_TableUpdate(BPLIB_ADU_PROXY, (void**) &BPNode_AppData.AduProxyTablePtr);
    if (BpStatus != BPLIB_SUCCESS && BpStatus != BPLIB_TBL_UPDATED)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error managing the configuration: ADUProxyTable on wakeup, Status=0x%08X",
                            BpStatus);
    }

    /* Check for pending commands */
    do
    {
        Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.CommandPipe, CFE_SB_POLL);

        if (Status == CFE_SUCCESS && BufPtr != NULL)
        {
            BPA_DP_TaskPipe(BufPtr);
        }
    } while (Status == CFE_SUCCESS);

    /* Not an error case */
    if (Status == CFE_SB_NO_MESSAGE)
    {
        Status = CFE_SUCCESS;
    }

    return Status;
}


/* App initialization activities */
CFE_Status_t BPNode_AppInit(void)
{
    CFE_Status_t Status;
    BPLib_Status_t BpStatus;
    char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
    char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];
    uint8 i;

    BPLib_FWP_ProxyCallbacks_t Callbacks = {
        /* Time Proxy */
        .BPA_TIMEP_GetMonotonicTime = BPA_TIMEP_GetMonotonicTime,
        .BPA_TIMEP_GetHostEpoch = BPA_TIMEP_GetHostEpoch,
        .BPA_TIMEP_GetHostClockState = BPA_TIMEP_GetHostClockState,
        .BPA_TIMEP_GetHostTime = BPA_TIMEP_GetHostTime,
        /* Perf Log Proxy */
        .BPA_PERFLOGP_Entry = BPA_PERFLOGP_Entry,
        .BPA_PERFLOGP_Exit = BPA_PERFLOGP_Exit,
        /* Table Proxy */
        .BPA_TABLEP_TableUpdate       = BPA_TABLEP_TableUpdate,
        /* Event Proxy */
        .BPA_EVP_Init = BPA_EVP_Init,
        .BPA_EVP_SendEvent = BPA_EVP_SendEvent,
        /* ADU Proxy */
        .BPA_ADUP_AddApplication = BPA_ADUP_AddApplication,
        .BPA_ADUP_StartApplication = BPA_ADUP_StartApplication,
        .BPA_ADUP_StopApplication = BPA_ADUP_StopApplication,
        .BPA_ADUP_RemoveApplication = BPA_ADUP_RemoveApplication,
        /* Telemetry Proxy */
        .BPA_TLMP_SendNodeMibConfigPkt = BPA_TLMP_SendNodeMibConfigPkt,
        .BPA_TLMP_SendPerSourceMibConfigPkt = BPA_TLMP_SendPerSourceMibConfigPkt,
        .BPA_TLMP_SendNodeMibCounterPkt = BPA_TLMP_SendNodeMibCounterPkt,
        .BPA_TLMP_SendPerSourceMibCounterPkt = BPA_TLMP_SendPerSourceMibCounterPkt,
        .BPA_TLMP_SendChannelContactPkt = BPA_TLMP_SendChannelContactPkt,
        .BPA_TLMP_SendStoragePkt = BPA_TLMP_SendStoragePkt,
        /* CLA Proxy */
        .BPA_CLAP_ContactSetup    = BPA_CLAP_ContactSetup,
        .BPA_CLAP_ContactStart    = BPA_CLAP_ContactStart,
        .BPA_CLAP_ContactStop     = BPA_CLAP_ContactStop,
        .BPA_CLAP_ContactTeardown = BPA_CLAP_ContactTeardown,
    };

    /* Zero out the global data structure */
    CFE_PSP_MemSet(&BPNode_AppData, 0, sizeof(BPNode_AppData));

    /* Initialize the FWP before using BPLib functions */
    BpStatus = BPLib_FWP_Init(&Callbacks);
    if (BpStatus != BPLIB_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BPNode: Failure initializing function callbacks, RC = 0x%08lX\n",
                                (unsigned long) BpStatus);

        /* Use CFE_EVS_SendEvent() rather than BPLib_EM_SendEvent() since callbacks weren't initialized */
        CFE_EVS_SendEvent(BPNODE_FWP_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "BPNode: Failure initializing function callbacks, RC = 0x%08lX",
                            (unsigned long) BpStatus);

        return BpStatus;
    }

    /* Register with Event Services */
    BpStatus = BPLib_EM_Init();
    if (BpStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BPNode: Error Registering Events, RC = 0x%08lX\n",
                                (unsigned long)BpStatus);

        return BpStatus;
    }

    /* Call Table Proxy Init Function Here to load default configurations*/
    BpStatus = BPA_TABLEP_TableInit();
    if (BpStatus != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error getting configuration from Table Proxy, RC = 0x%08lX",
                            (unsigned long)BpStatus);

        return BpStatus;
    }

    BpStatus = BPLib_TIME_Init();
    if (BpStatus != BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TIME_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error initializing BPLib Time Management, RC = %d", BpStatus);

        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Initialize configurations and counters */
    BpStatus = BPLib_NC_Init(&BPNode_AppData.ConfigPtrs);
    if (BpStatus != BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_NC_AS_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error initializing NC/AS, RC = %d", BpStatus);

        return BpStatus;
    }

    /* Initialize MEM and QM */
    BpStatus = BPLib_QM_QueueTableInit(&BPNode_AppData.BplibInst, BPNODE_MAX_UNSORTED_JOBS);
    if (BpStatus != BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_QM_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error initializing QM, RC = %d", BpStatus);

        return BpStatus;
    }

    BpStatus = BPLib_MEM_PoolInit(&BPNode_AppData.BplibInst.pool, (void *)BPNode_AppData.pool_mem,
        (size_t)BPNODE_MEM_POOL_LEN);
    if (BpStatus != BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_MEM_INIT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error initializing MEM, RC = %d", BpStatus);

        return BpStatus;
    }

    /* Create command pipe */
    Status = CFE_SB_CreatePipe(&BPNode_AppData.CommandPipe, BPNODE_CMD_PIPE_DEPTH,
                                "BPNODE_CMD_PIPE");

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CR_CMD_PIPE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error creating SB Command Pipe, RC = 0x%08lX", (unsigned long)Status);

        return Status;
    }

    /* Create wakeup pipe */
    Status = CFE_SB_CreatePipe(&BPNode_AppData.WakeupPipe, BPNODE_WAKEUP_PIPE_DEPTH,
                                "BPNODE_WAKEUP_PIPE");

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CR_WKP_PIPE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error creating SB Wakeup Pipe, RC = 0x%08lX", (unsigned long)Status);

        return Status;
    }

    /* Subscribe to ground command packets */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_CMD_MID),
                                BPNode_AppData.CommandPipe);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_SUB_CMD_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error Subscribing to Commands, RC = 0x%08lX", (unsigned long)Status);

        return Status;
    }

    /* Subscribe to wakeup messages on the wakeup pipe */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_WAKEUP_MID),
                                BPNode_AppData.WakeupPipe);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_SUB_WKP_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error Subscribing to wakeup messages, RC = 0x%08lX",
                            (unsigned long)Status);

        return Status;
    }

    /* Call Telemetry Proxy Init Function */
    BPA_TLMP_Init();

    /* Create ADU In child tasks */
    Status = BPNode_AduInCreateTasks();

    if (Status != CFE_SUCCESS)
    {
        /* Event message handled in task creation function */
        return Status;
    }

    /* Create ADU Out child tasks */
    Status = BPNode_AduOutCreateTasks();

    if (Status != CFE_SUCCESS)
    {
        /* Event message handled in task creation function */
        return Status;
    }


    /* Create CLA In child tasks */
    Status = BPNode_ClaInCreateTasks();

    if (Status != CFE_SUCCESS)
    {
        /* Event message handled in task creation function */
        return Status;
    }

    /* Create CLA Out child tasks */
    Status = BPNode_ClaOutCreateTasks();

    if (Status != CFE_SUCCESS)
    {
        /* Event message handled in task creation function */
        return Status;
    }

    /* Create Generic Worker child tasks */
    Status = BPNode_GenWorkerCreateTasks();

    if (Status != CFE_SUCCESS)
    {
        /* Event message handled in task creation function */
        return Status;
    }

    /* Register delete handler for graceful app shutdowns */
    Status = OS_TaskInstallDeleteHandler(&BPNode_AppExit);
    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_DEL_HANDLER_ERR_EID, CFE_EVS_EventType_ERROR,
                            "Failed to install delete handler. Error = 0x%08X", Status);

        return Status;
    }

    /* Add and start all applications set to be loaded at startup */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        if (BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[i].AddAutomatically == true)
        {
            /* Ignore return value, no failure conditions are possible here */
            (void) BPA_ADUP_AddApplication(i);

            BpStatus = BPA_ADUP_StartApplication(i);

            if (BpStatus != BPLIB_SUCCESS)
            {
                /* Error event message handled by ADU Proxy */
                return BpStatus;
            }
            else
            {
                BPLib_EM_SendEvent(BPNODE_AUTO_ADD_APP_INF_EID, BPLib_EM_EventType_INFORMATION,
                                    "Automatically added app configurations for ChanId=%d", i);
            }

        }
    }

    /* App has initialized properly */

    BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
                    BPNODE_MAJOR_VERSION,
                    BPNODE_MINOR_VERSION,
                    BPNODE_REVISION);

    CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

    BPLib_EM_SendEvent(BPNODE_INIT_INF_EID, BPLib_EM_EventType_INFORMATION, "BPNode Initialized: %s",
                        VersionString);

    return CFE_SUCCESS;
}

/* Exit app */
void BPNode_AppExit(void)
{
    uint8  i;
    uint32 ContactId;

    BPLib_EM_SendEvent(BPNODE_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                        "App terminating, error = %d", BPNode_AppData.RunStatus);

    CFE_ES_WriteToSysLog("BPNode app terminating, error = %d", BPNode_AppData.RunStatus);

    /* Signal to ADU child tasks to exit */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPNode_AppData.AduOutData[i].RunStatus = CFE_ES_RunStatus_APP_EXIT;
        BPNode_AppData.AduInData[i].RunStatus = CFE_ES_RunStatus_APP_EXIT;
    }

    /* Signal to CLA child tasks to exit */
    for (ContactId = 0; ContactId < BPLIB_MAX_NUM_CONTACTS; ContactId++)
    {
        /* Exit all CLA child tasks upon exit */
        BPLib_CLA_SetContactRunState(ContactId, BPLIB_CLA_EXITED);
    }

    /* Signal to generic worker tasks to exit */
    for (i = 0; i < BPNODE_NUM_GEN_WRKR_TASKS; i++)
    {
        BPNode_AppData.GenWorkerData[i].RunStatus = CFE_ES_RunStatus_APP_EXIT;
    }

    /* Wait on the ADU task exit semaphores */
    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        (void) OS_BinSemTimedWait(BPNode_AppData.AduInData[i].ExitSemId, BPNODE_ADU_IN_SEM_EXIT_WAIT_MSEC);
        (void) OS_BinSemTimedWait(BPNode_AppData.AduOutData[i].ExitSemId, BPNODE_ADU_OUT_SEM_EXIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);
    }

    /* Wait on the CLA task exit semaphores */
    for (i = 0; i < BPLIB_MAX_NUM_CONTACTS; i++)
    {
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        (void) OS_BinSemTimedWait(BPNode_AppData.ClaInData[i].ExitSemId, BPNODE_CLA_IN_SEM_EXIT_WAIT_MSEC);
        (void) OS_BinSemTimedWait(BPNode_AppData.ClaOutData[i].ExitSemId, BPNODE_CLA_OUT_SEM_EXIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);
    }

    /* Wait on the generic worker task exit semaphores */
    for (i = 0; i < BPNODE_NUM_GEN_WRKR_TASKS; i++)
    {
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        (void) OS_BinSemTimedWait(BPNode_AppData.GenWorkerData[i].ExitSemId, BPNODE_GEN_WRKR_SEM_EXIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);
    }

    /* Cleanup QM and MEM */
    BPLib_QM_QueueTableDestroy(&BPNode_AppData.BplibInst);
    BPLib_MEM_PoolDestroy(&BPNode_AppData.BplibInst.pool);

    /* Performance Log Exit Stamp */
    BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
}
