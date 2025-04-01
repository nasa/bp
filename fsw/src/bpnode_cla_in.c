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
 *   This file contains the source code for the BPNode CLA In Child Task(s)
 */

/*
** Include Files
*/
#include "bpnode_app.h"
#include "bpnode_cla_in.h"

/*
** Function Definitions
*/

/* Receive bundles from network CL and forward ingress bundles to CLA  */
int32 BPNode_ClaIn_ProcessBundleInput(uint32 ContId)
{
    CFE_PSP_IODriver_ReadPacketBuffer_t RdBuf;
    int32                               Status = CFE_PSP_SUCCESS;
    BPLib_Status_t                      BpStatus;

    /* Read next bundle from CL */
    if (BPNode_AppData.ClaInData[ContId].CurrentBufferSize == 0)
    {
        RdBuf.BufferSize = sizeof(BPNode_AppData.ClaInData[ContId].BundleBuffer);
        RdBuf.BufferMem  = BPNode_AppData.ClaInData[ContId].BundleBuffer;

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);

        Status = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContId].PspLocation,
                                          CFE_PSP_IODriver_PACKET_IO_READ,
                                          CFE_PSP_IODriver_VPARG(&RdBuf));

        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);

        if (Status == CFE_PSP_SUCCESS)
        {
            BPNode_AppData.ClaInData[ContId].CurrentBufferSize = RdBuf.BufferSize;

            Status = CFE_SUCCESS;
        }

    }

    /* Ingress received bundle to bplib CLA */
    if (Status == CFE_SUCCESS && BPNode_AppData.ClaInData[ContId].CurrentBufferSize != 0)
    {
        BPLib_AS_Increment(BPLIB_EID_INSTANCE, BUNDLE_COUNT_RECEIVED, 1);

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);

        BpStatus = BPLib_CLA_Ingress(&BPNode_AppData.BplibInst, ContId,
                                    BPNode_AppData.ClaInData[ContId].BundleBuffer,
                                    BPNode_AppData.ClaInData[ContId].CurrentBufferSize, 0);

        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);

        /* If CLA did not timeout during ingress, zero out current buffer size */
        if (BpStatus != BPLIB_CLA_TIMEOUT)
        {
            BPNode_AppData.ClaInData[ContId].CurrentBufferSize = 0;
            if (BpStatus != BPLIB_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_LIB_PROC_ERR_EID, BPLib_EM_EventType_ERROR,
                                  "[CLA In #%d]: Failed to ingress bundle. Error = %d",
                                  ContId,
                                  BpStatus);

                Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
            }
        }
    }

    return Status;
}

BPLib_Status_t BPNode_ClaInCreateTasks(void)
{
    BPLib_Status_t Status;
    int32          OsStatus;
    int32          PspStatus;
    CFE_Status_t   CFE_Status;
    uint32         ContactId;
    char           NameBuff[OS_MAX_API_NAME];
    uint16         TaskPriority;

    Status = BPLIB_SUCCESS;

    /* Create all of the CLA In task(s) */
    for (ContactId = 0; ContactId < BPLIB_MAX_NUM_CONTACTS; ContactId++)
    {
        /* Set performance ID */
        BPNode_AppData.ClaInData[ContactId].PerfId = BPNODE_CLA_IN_PERF_ID_BASE + ContactId;

        /* Get PSP module ID for either the Unix or UDP socket driver */
        PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME,
                                                &BPNode_AppData.ClaInData[ContactId].PspLocation.PspModuleId);

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA In #%d]: Couldn't find I/O driver. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }

        if (Status == BPLIB_SUCCESS)
        {
            BPNode_AppData.ClaInData[ContactId].PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
            BPNode_AppData.ClaInData[ContactId].PspLocation.SubchannelId = BPNODE_CLA_PSP_INPUT_SUBCHANNEL;

            /* Set direction to input only */
            PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactId].PspLocation,
                                                    CFE_PSP_IODriver_SET_DIRECTION,
                                                    CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));

            if (PspStatus != CFE_PSP_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Couldn't set I/O direction to input. Error = %d",
                                    ContactId,
                                    PspStatus);

                Status = BPLIB_CLA_IO_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Create init semaphore so main task knows when child initialized */
            snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
            OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

            if (OsStatus != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Failed to create init semaphore, %s. Error = %d",
                                    ContactId,
                                    NameBuff,
                                    OsStatus);

                Status = BPLIB_CLA_INIT_SEM_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Create wakeup semaphore so main task can control workflow */
            snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
            OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

            if (OsStatus != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_ADU_OUT_WAKEUP_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Failed to create wakeup semaphore, %s. Error = %d",
                                    ContactId,
                                    NameBuff,
                                    OsStatus);

                Status = BPLIB_CLA_WAKEUP_SEM_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Create exit semaphore so main task knows when child finished shutdown */
            snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
            OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

            if (OsStatus != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_EXIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Failed to create exit semaphore, %s. Error = %d",
                                    ContactId,
                                    NameBuff,
                                    OsStatus);

                Status = BPLIB_CLA_EXIT_SEM_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Create child task */
            snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_CLA_IN_BASE_NAME, ContactId);
            TaskPriority = BPNODE_CLA_IN_PRIORITY_BASE + ContactId;

            CFE_Status = CFE_ES_CreateChildTask(&BPNode_AppData.ClaInData[ContactId].TaskId,
                                                NameBuff,
                                                BPNode_ClaIn_AppMain,
                                                0,
                                                BPNODE_CLA_IN_STACK_SIZE,
                                                TaskPriority,
                                                0);

            if (CFE_Status != CFE_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Failed to create child task. Error = %d",
                                    ContactId,
                                    Status);

                Status = BPLIB_CLA_TASK_CREATE_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Verify initialization by trying to take the init semaphore */
            BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
            OsStatus = OS_BinSemTimedWait(BPNode_AppData.ClaInData[ContactId].InitSemId, BPNODE_CLA_IN_SEM_INIT_WAIT_MSEC);
            BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

            if (OsStatus != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA In #%d]: Task not running. Error = %d",
                                    ContactId,
                                    OsStatus);

                Status = BPLIB_CLA_INIT_SEM_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_INIT_INF_EID,
                                BPLib_EM_EventType_INFORMATION,
                                "[CLA In #%d]: Child task initialized",
                                ContactId);
        }
        else
        {
            /* Exit function upon an error */
            break;
        }
    }

    return Status;
}

BPLib_Status_t BPNode_ClaIn_Setup(uint32 ContactId, int32 PortNum, char* IpAddr)
{
    BPLib_Status_t  Status;
    int32           PspStatus;
    char            Str[100];

    Status = BPLIB_SUCCESS;

#ifdef BPNODE_CLA_UDP_DRIVER
    /* Configure Port Number */
    snprintf(Str, sizeof(Str), "port=%d", PortNum);
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactId].PspLocation,
                                            CFE_PSP_IODriver_SET_CONFIGURATION,
                                            CFE_PSP_IODriver_CONST_STR(Str));

    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Couldn't configure port number. Error = %d",
                            ContactId,
                            PspStatus);

        Status = BPLIB_CLA_IO_ERROR;
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Configure IP Address */
        snprintf(Str, sizeof(Str), "IpAddr=%s", IpAddr);
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_IP_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA In #%d]: Couldn't configure IP address. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
        else
        {
            OS_printf("[CLA In #%d]: Receiving on %s:%d\n", ContactId, IpAddr, PortNum);
        }
    }
#endif

    return Status;
}

BPLib_Status_t BPNode_ClaIn_Start(uint32 ContactId)
{
    int32 PspStatus;
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    /* Set I/O to running */
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactId].PspLocation,
                                            CFE_PSP_IODriver_SET_RUNNING,
                                            CFE_PSP_IODriver_U32ARG(true));

    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_SET_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Couldn't set I/O state to running. Error = %d",
                            ContactId,
                            PspStatus);

        Status = BPLIB_CLA_IO_ERROR;
    }

    return Status;
}

void BPNode_ClaIn_Stop(uint32 ContactId)
{
    /* Set I/O to stop running */
    (void) CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactId].PspLocation,
                                    CFE_PSP_IODriver_SET_RUNNING,
                                    CFE_PSP_IODriver_U32ARG(false));

    return;
}

void BPNode_ClaIn_Teardown(uint32 ContactId)
{
    /*
    ** Disestablish CLA
    ** Free all CLA resources
    ** Discard output queue
    ** Delete custody timers
    */

    return;
}

/* Main loop for CLA In task(s) */
void BPNode_ClaIn_AppMain(void)
{
    int32                       OsStatus;
    CFE_Status_t                CFE_Status;
    BPLib_Status_t              Status;
    CFE_ES_TaskId_t             TaskId;
    uint32                      BundlesReceived;
    uint32                      ContactId;
    BPLib_CLA_ContactRunState_t RunState;

    CFE_Status = CFE_ES_GetTaskID(&TaskId);
    if (CFE_Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_NO_ID_ERR_EID,
                            BPLib_EM_EventType_ERROR,
                            "[CLA In #?]: Failed to get task ID. Error = %d",
                            CFE_Status);
    }
    else
    {
        for (ContactId = 0; ContactId < BPLIB_MAX_NUM_CONTACTS; ContactId++)
        {
            if (BPNode_AppData.ClaInData[ContactId].TaskId == TaskId)
            {
                /* break to preserve ContactId */
                break;
            }
        }

        if (ContactId != BPLIB_MAX_NUM_CONTACTS)
        {
            /* Confirm initialization with give on init semaphore */
            (void) OS_BinSemGive(BPNode_AppData.ClaInData[ContactId].InitSemId);

            Status = BPLib_CLA_GetContactRunState(ContactId, &RunState);
            while (RunState != BPLIB_CLA_EXITED && Status == BPLIB_SUCCESS)
            {
                /* Attempt to take the wakeup semaphore */
                BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContactId].PerfId);
                OsStatus = OS_BinSemTimedWait(BPNode_AppData.ClaInData[ContactId].WakeupSemId, BPNODE_CLA_IN_SEM_WAKEUP_WAIT_MSEC);
                BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContactId].PerfId);

                if (OsStatus == OS_SUCCESS)
                {
                    if (RunState == BPLIB_CLA_STARTED)
                    {
                        BundlesReceived = 0;

                        do
                        {
                            CFE_Status = BPNode_ClaIn_ProcessBundleInput(ContactId);
                            if (CFE_Status == CFE_SUCCESS)
                            {
                                BundlesReceived++;
                            }
                        } while ((CFE_Status == CFE_SUCCESS) && (BundlesReceived < BPNODE_CLA_IN_MAX_BUNDLES_PER_CYCLE));
                    }
                }
                else if (OsStatus == OS_SEM_TIMEOUT)
                {
                    BPLib_EM_SendEvent(BPNODE_CLA_IN_SEM_TK_TIMEOUT_INF_EID,
                                        BPLib_EM_EventType_INFORMATION,
                                        "[CLA In #%d]: Wakeup semaphore wait timed out",
                                        ContactId);
                }
                else
                {
                    BPLib_EM_SendEvent(BPNODE_CLA_IN_WAKEUP_SEM_ERR_EID,
                                        BPLib_EM_EventType_ERROR,
                                        "[CLA In #%d]: Wakeup semaphore take failed, RC = %d",
                                        ContactId,
                                        OsStatus);
                }

                Status = BPLib_CLA_GetContactRunState(ContactId, &RunState);
            }

            /* Teardown CLA In task, in case that hasn't been done already */
            BPNode_ClaIn_Teardown(ContactId);

            /* Confirm exit with give on exit semaphore */
            (void) OS_BinSemGive(BPNode_AppData.ClaInData[ContactId].ExitSemId);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_INV_ID_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[CLA In #?] Could not find a task to process bundles with");
        }
    }

    return;
}