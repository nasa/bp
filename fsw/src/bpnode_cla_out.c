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
 *   This file contains the source code for the BPNode CLA Out Child Task(s)
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_cla_out.h"


/*
** Function Definitions
*/

/* Receive bundles from CLA and send egress bundles to network CL */
int32 BPNode_ClaOut_ProcessBundleOutput(uint8 ContId)
{
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;
    int32                                Status = CFE_PSP_SUCCESS;
    BPLib_Status_t                       BpStatus;

    /* Get next bundle from CLA */
    if (BPNode_AppData.ClaOutData[ContId].CurrentBufferSize == 0)
    {
        BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

        BpStatus = BPLib_CLA_Egress(&BPNode_AppData.BplibInst, ContId,
                                    BPNode_AppData.ClaOutData[ContId].BundleBuffer,
                                    &BPNode_AppData.ClaOutData[ContId].CurrentBufferSize,
                                    BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE,
                                    BPNODE_CLA_OUT_QUEUE_PEND_TIME);

        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);

        if (BpStatus != BPLIB_SUCCESS && BpStatus != BPLIB_CLA_TIMEOUT)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_LIB_LOAD_ERR_EID, BPLib_EM_EventType_ERROR,
                               "[CLA Out #%d]: Failed to get bundle for egress. Error = %d",
                               ContId, Status);
            return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
        }
    }

    /* Send egress bundle onto CL */
    if (BPNode_AppData.ClaOutData[ContId].CurrentBufferSize != 0)
    {
        WrBuf.OutputSize = BPNode_AppData.ClaOutData[ContId].CurrentBufferSize;
        WrBuf.BufferMem  = BPNode_AppData.ClaOutData[ContId].BundleBuffer;

        BPLib_AS_Increment(BPLIB_EID_INSTANCE, BUNDLE_COUNT_DELIVERED, 1);

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

        /* This does not check return code here, it is "best effort" at this stage.
         * bplib should retry based on custody signals if this does not work. */
        CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContId].PspLocation,
                                    CFE_PSP_IODriver_PACKET_IO_WRITE,
                                    CFE_PSP_IODriver_VPARG(&WrBuf));

        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);

        BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = 0;
    }

    return CFE_SUCCESS;
}

BPLib_Status_t BPNode_ClaOut_Setup(uint32_t ContactId, int32 PortNum, char* IpAddr)
{
    BPLib_Status_t  Status;
    int32           PspStatus;
    int32           OsStatus;
    char            Str[100];

    Status = BPLIB_SUCCESS;
    BPNode_AppData.ClaOutData[ContactId].PerfId = BPNODE_CLA_OUT_PERF_ID_BASE + ContactId;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME,
                                            &BPNode_AppData.ClaOutData[ContactId].PspLocation.PspModuleId);

    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[Contact ID #%d]: Couldn't find CLA Out I/O driver. Error = %d",
                            ContactId,
                            PspStatus);

        Status = BPLIB_CLA_IO_ERROR;
    }

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.ClaOutData[ContactId].PspLocation.SubsystemId  = 2 - (CFE_PSP_GetProcessorId() & 1);
        BPNode_AppData.ClaOutData[ContactId].PspLocation.SubchannelId = BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL;

    #ifdef BPNODE_CLA_UDP_DRIVER
        /* Configure Port Number */
        snprintf(Str, sizeof(Str), "port=%d", PortNum);

        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't configure CLA Out port number. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Configure IP Address */
        snprintf(Str, sizeof(Str), "IpAddr=%s", IpAddr);
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_IP_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't configure IP address for CLA Out. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
        else
        {
            OS_printf("[Contact ID #%d]: CLA Out receiving on %s:%d\n", ContactId, IpAddr, PortNum);
        }
    }
#endif

    if (Status == BPLIB_SUCCESS)
    {
        /* Set direction to output only */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_DIRECTION,
                                                CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_OUTPUT_ONLY));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set CLA Out I/O direction to output. Error = %d",
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Set I/O to running */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_RUNNING,
                                                CFE_PSP_IODriver_U32ARG(true));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set CLA Out I/O state to running. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Start performance log */
        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContactId].PerfId);

        /* Notify main task that child task is running */
        BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContactId].PerfId);
        OsStatus = OS_BinSemGive(BPNode_AppData.ClaOutData[ContactId].InitSemId);
        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContactId].PerfId);

        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Failed to give CLA Out init semaphore. Error = %d",
                                ContactId,
                                OsStatus);

            Status = BPLIB_CLA_INIT_SEM_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                            "[Contact ID #%d]: CLA Out set up",
                            ContactId);
    }

    return Status;
}

BPLib_Status_t BPNode_ClaOut_Start(uint32_t ContactId)
{
    BPLib_Status_t             Status;
    CFE_Status_t               CFE_Status;
    int32                      OsStatus;
    char                       NameBuff[OS_MAX_API_NAME];
    CFE_ES_TaskPriority_Atom_t TaskPriority;

    Status = BPLIB_SUCCESS;

    /* Create init semaphore so main task knows when child initialized */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
    OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].InitSemId, NameBuff, 0, 0);

    if (OsStatus != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_ERR_EID,
                            BPLib_EM_EventType_ERROR,
                            "[Contact ID #%d]: Failed to create CLA Out init semaphore, %s. Error = %d.",
                            ContactId,
                            NameBuff,
                            OsStatus);

        Status = BPLIB_CLA_INIT_SEM_ERROR;
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Create wakeup semaphore so main task can control CLA Out tasks */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
        OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].WakeupSemId, NameBuff, 0, 0);

        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Failed to create CLA Out wakeup semaphore, %s. Error = %d.",
                                ContactId,
                                NameBuff,
                                OsStatus);

            Status = BPLIB_CLA_WAKEUP_SEM_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Create exit semaphore so main task knows when child finished shutdown */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
        OsStatus = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Failed to create CLA Out exit semaphore. Error = %d.",
                                ContactId,
                                OsStatus);

            Status = BPLIB_CLA_EXIT_SEM_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_CLA_OUT_BASE_NAME, ContactId);
        TaskPriority = BPNODE_CLA_IN_PRIORITY_BASE + ContactId;

        CFE_Status = CFE_ES_CreateChildTask(&BPNode_AppData.ClaOutData[ContactId].TaskId,
                                            NameBuff,
                                            BPNode_ClaOut_AppMain,
                                            0,
                                            BPNODE_CLA_OUT_STACK_SIZE,
                                            TaskPriority,
                                            0);

        if (CFE_Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Failed to create CLA Out child task. Error = %d.",
                                ContactId,
                                CFE_Status);

            Status = BPLIB_CLA_TASK_CREATE_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        OsStatus = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[ContactId].InitSemId, BPNODE_CLA_OUT_SEM_INIT_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (OsStatus != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: CLA Out task not running. Init Sem Error = %d.",
                                ContactId,
                                OsStatus);

            Status = BPLIB_CLA_INIT_SEM_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Enable egress */
        BPNode_AppData.ClaOutData[ContactId].EgressServiceEnabled = true;
    }

    return Status;
}

void BPNode_ClaOut_Stop(uint32_t ContactId)
{
    /* Set I/O to stop running */
    (void) CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                    CFE_PSP_IODriver_SET_RUNNING,
                                    CFE_PSP_IODriver_U32ARG(false));

    BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                        "[Contact ID #%d]: Terminating CLA Out task",
                        ContactId);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[Contact ID #%d]: Terminating CLA Out task",
                            ContactId);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContactId].PerfId);

    /* Signal to the main task that the child task has exited */
    (void) OS_BinSemGive(BPNode_AppData.ClaOutData[ContactId].ExitSemId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}

/* Main loop for CLA Out task(s) */
void BPNode_ClaOut_AppMain(void)
{
    int32                       OsStatus;
    CFE_Status_t                CFE_Status;
    CFE_ES_TaskId_t             TaskId;
    uint32                      BundlesForwarded;
    uint32                      ContactId;
    BPLib_CLA_ContactRunState_t RunState;

    CFE_Status = CFE_ES_GetTaskID(&TaskId);
    if (CFE_Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_NO_ID_ERR_EID,
                            BPLib_EM_EventType_ERROR,
                            "[Contact ID #?]: Failed to get CLA Out task ID. Error = %d",
                            CFE_Status);
    }
    else
    {
        /* Find a contact whose task ID matches the calling task */
        for (ContactId = 0; ContactId < BPLIB_MAX_NUM_CONTACTS; ContactId++)
        {
            if (BPNode_AppData.ClaOutData[ContactId].TaskId == TaskId)
            {
                /* break to preserve ContactId */
                break;
            }
        }

        if (ContactId != BPLIB_MAX_NUM_CONTACTS)
        {
            /* CLA Out task loop */
            do
            { /* At least one loop will always occur, even if you start the contact, then quickly stop it */
                BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContactId].PerfId);
                OsStatus = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[ContactId].WakeupSemId, BPNODE_CLA_OUT_SEM_WAKEUP_WAIT_MSEC);
                BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContactId].PerfId);

                if (OsStatus == OS_SUCCESS)
                {
                    if (BPNode_AppData.ClaOutData[ContactId].EgressServiceEnabled)
                    {
                        BundlesForwarded = 0;

                        do
                        {
                            CFE_Status = BPNode_ClaOut_ProcessBundleOutput(ContactId);
                            if (CFE_Status == CFE_SUCCESS)
                            {
                                BundlesForwarded++;
                            }
                        } while ((CFE_Status == CFE_SUCCESS) && (BundlesForwarded < BPNODE_CLA_OUT_MAX_BUNDLES_PER_CYCLE));
                    }
                }
                else if (OsStatus == OS_SEM_TIMEOUT)
                {
                    BPLib_EM_SendEvent(BPNODE_CLA_OUT_SEM_TK_TIMEOUT_INF_EID,
                                        BPLib_EM_EventType_INFORMATION,
                                        "[Contact ID #%d]: CLA Out task timed out while waiting for the wakeup semaphore",
                                        ContactId);
                }
                else
                {
                    BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                        BPLib_EM_EventType_ERROR,
                                        "[Contact ID #%d]: CLA Out task failed to take wakeup semaphore, RC = %d",
                                        ContactId,
                                        OsStatus);
                }

                RunState = BPLib_CLA_GetContactRunState(ContactId);
            } while (RunState == BPLIB_CLA_STARTED);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_INV_ID_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[Contact ID #?] Could not find a CLA Out task to process bundles with");
        }
    }

    return;
}