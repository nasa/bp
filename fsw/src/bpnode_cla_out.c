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
int32 BPNode_ClaOut_ProcessBundleOutput(uint32 ContId, size_t *MsgSize)
{
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;
    int32                                Status;
    BPLib_Status_t                       BpStatus;

    Status  = CFE_PSP_SUCCESS;
    *MsgSize = 0;

    /* Get next bundle from CLA */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

    BpStatus = BPLib_CLA_Egress(&BPNode_AppData.BplibInst,
                                ContId,
                                BPNode_AppData.ClaOutData[ContId].OutBuffer.Payload,
                                MsgSize,
                                BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE,
                                BPNODE_CLA_OUT_QUEUE_PEND_TIME);

    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);

    if (BpStatus != BPLIB_SUCCESS && BpStatus != BPLIB_CLA_TIMEOUT)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_LIB_LOAD_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA Out #%d]: Failed to get bundle for egress. Error = %d",
                            ContId,
                            BpStatus);

        Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Send egress bundle onto CL */
    if (BpStatus == BPLIB_SUCCESS)
    {
        if (ContId == BPNODE_CLA_SB_CONTACT_ID)
        { /* Contact is SB-type */
            /* Set the MID for the outbound bundle */
            CFE_MSG_SetMsgId(CFE_MSG_PTR(BPNode_AppData.ClaOutData[ContId].OutBuffer.TelemetryHeader),
                                CFE_SB_ValueToMsgId(BPNODE_CLA_OUT_BUNDLE_MID));

            /* Set the size of the message */
            CFE_MSG_SetSize(CFE_MSG_PTR(BPNode_AppData.ClaOutData[ContId].OutBuffer.TelemetryHeader),
                            *MsgSize + sizeof(CFE_MSG_TelemetryHeader_t));

            /* Timestamp message before transmitting */
            CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.ClaOutData[ContId].OutBuffer.TelemetryHeader));

            /* Send the wrapped bundle onto the Software Bus */
            CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.ClaOutData[ContId].OutBuffer.TelemetryHeader), true);
        }
        else
        {
            WrBuf.OutputSize = *MsgSize;
            WrBuf.BufferMem  = BPNode_AppData.ClaOutData[ContId].OutBuffer.Payload;

            BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

            /* This does not check return code here, it is "best effort" at this stage.
            * bplib should retry based on custody signals if this does not work. */
            (void) CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContId].PspLocation,
                                                    CFE_PSP_IODriver_PACKET_IO_WRITE,
                                                    CFE_PSP_IODriver_VPARG(&WrBuf));

            BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);
        }

        CFE_MSG_SetSize(CFE_MSG_PTR(BPNode_AppData.ClaOutData[ContId].OutBuffer.TelemetryHeader), 0);
    }

    return Status;
}

CFE_Status_t BPNode_ClaOutCreateTasks(void)
{
    CFE_Status_t Status;
    uint32       ContactId;
    char         NameBuff[OS_MAX_API_NAME];
    uint16       TaskPriority;

    Status = CFE_SUCCESS;

    /* Create all of the CLA Out task(s) */
    for (ContactId = 0; ContactId < BPLIB_MAX_NUM_CONTACTS; ContactId++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Failed to create init semaphore, %s, for CLA Out #%d. Error = %d",
                                NameBuff,
                                ContactId,
                                Status);

            /* Stop creating tasks and return error code */
            break;
        }
        else
        {
            /* Create wakeup semaphore so main task can control workflow */
            snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
            Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].WakeupSemId, NameBuff, 0, 0);

            if (Status != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Failed to create wakeup semaphore, %s, for CLA Out #%d. Error = %d",
                                    ContactId,
                                    NameBuff,
                                    Status);

                /* Stop creating tasks and return error code */
                break;
            }
            else
            {
                /* Create exit semaphore so main task knows when child finished shutdown */
                snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, ContactId);
                Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContactId].ExitSemId, NameBuff, 0, 0);

                if (Status != OS_SUCCESS)
                {
                    BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                        "Failed to create exit semaphore, %s, for CLA Out #%d. Error = %d",
                                        ContactId,
                                        NameBuff,
                                        Status);

                    /* Stop creating tasks and return error code */
                    break;
                }
                else
                {
                    BPNode_AppData.ClaOutData[ContactId].OutBuffer.Payload = malloc(BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE);
                    if (BPNode_AppData.ClaOutData[ContactId].OutBuffer.Payload == NULL)
                    {
                        printf("FRIG!\n");
                        break;
                    }

                    /* Create child task */
                    snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_CLA_OUT_BASE_NAME, ContactId);
                    TaskPriority = BPNODE_CLA_OUT_PRIORITY_BASE + ContactId;

                    Status = CFE_ES_CreateChildTask(&BPNode_AppData.ClaOutData[ContactId].TaskId,
                                                    NameBuff,
                                                    BPNode_ClaOut_AppMain,
                                                    0,
                                                    BPNODE_CLA_OUT_STACK_SIZE,
                                                    TaskPriority,
                                                    0);

                    if (Status != CFE_SUCCESS)
                    {
                        BPLib_EM_SendEvent(BPNODE_CLA_OUT_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                                            "Failed to create child task for CLA Out #%d. Error = %d",
                                            ContactId,
                                            Status);

                        /* Stop creating tasks and return error code */
                        break;
                    }
                    else
                    {
                        /* Verify initialization by trying to take the init semaphore */
                        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
                        Status = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[ContactId].InitSemId, BPNODE_CLA_OUT_SEM_INIT_WAIT_MSEC);
                        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

                        if (Status != OS_SUCCESS)
                        {
                            BPLib_EM_SendEvent(BPNODE_CLA_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                                "CLA Out task #%d not running. Init Sem Error = %d.",
                                                ContactId,
                                                Status);

                            /* Stop creating tasks and return error code */
                            break;
                        }
                    }
                }
            }
        }
    }

    return Status;
}

CFE_Status_t BPNode_ClaOut_TaskInit(uint32 ContactId)
{
    CFE_Status_t Status;

    /* Set performance ID */
    BPNode_AppData.ClaOutData[ContactId].PerfId = BPNODE_CLA_OUT_PERF_ID_BASE + ContactId;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    Status = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME,
                                            &BPNode_AppData.ClaOutData[ContactId].PspLocation.PspModuleId);

    if (Status != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA Out #%d]: Couldn't find I/O driver. Error = %d",
                            ContactId,
                            Status);
    }
    else
    {
        BPNode_AppData.ClaOutData[ContactId].PspLocation.SubsystemId = 2 - (CFE_PSP_GetProcessorId() & 1);

        /* Set direction to output only */
        Status = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                            CFE_PSP_IODriver_SET_DIRECTION,
                                            CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_OUTPUT_ONLY));

        if (Status != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA Out #%d]: Couldn't set I/O direction to output. Error = %d",
                                ContactId,
                                Status);
        }
        else
        {
            /* Verify initialization by trying to give on the init semaphore */
            BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
            Status = OS_BinSemGive(BPNode_AppData.ClaOutData[ContactId].InitSemId);
            BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

            if (Status != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "[CLA Out #%d]: Task not running. Error = %d",
                                    ContactId,
                                    Status);
            }
            else
            {
                BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_INF_EID,
                                    BPLib_EM_EventType_INFORMATION,
                                    "[CLA Out #%d]: Child task initialized",
                                    ContactId);
            }
        }
    }

    return Status;
}

BPLib_Status_t BPNode_ClaOut_Setup(uint32 ContactId, int32 PortNum, char* IpAddr)
{
    BPLib_Status_t  Status;
    int32           PspStatus;
    char            Str[100];

    Status = BPLIB_SUCCESS;

    if (ContactId != BPNODE_CLA_SB_CONTACT_ID)
    {
        #ifdef BPNODE_CLA_UDP_DRIVER
            /* Configure Port Number */
            snprintf(Str, sizeof(Str), "port=%d", PortNum);

            PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                    CFE_PSP_IODriver_SET_CONFIGURATION,
                                                    CFE_PSP_IODriver_CONST_STR(Str));

            if (PspStatus != CFE_PSP_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Couldn't configure port number for CLA Out #%d. Error = %d",
                                    ContactId,
                                    PspStatus);

                Status = BPLIB_CLA_IO_ERROR;
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
                                        "Couldn't configure IP address for CLA Out #%d. Error = %d",
                                        ContactId,
                                        PspStatus);

                    Status = BPLIB_CLA_IO_ERROR;
                }
                else
                {
                    OS_printf("CLA Out #%d sending on %s:%d\n", ContactId, IpAddr, PortNum);
                }
            }
        #endif
    }

    return Status;
}

BPLib_Status_t BPNode_ClaOut_Start(uint32 ContactId)
{
    int32 PspStatus;
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    if (ContactId != BPNODE_CLA_SB_CONTACT_ID)
    {
        /* Set I/O to running */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_RUNNING,
                                                CFE_PSP_IODriver_U32ARG(true));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Couldn't set I/O state for CLA Out #%d to running. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    return Status;
}

BPLib_Status_t BPNode_ClaOut_Stop(uint32 ContactId)
{
    BPLib_Status_t Status;
    int32 PspStatus;

    Status = BPLIB_SUCCESS;

    if (ContactId != BPNODE_CLA_SB_CONTACT_ID)
    {
        /* Set I/O to stop running */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_RUNNING,
                                                CFE_PSP_IODriver_U32ARG(false));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Couldn't set I/O state to stop for CLA Out #%d. Error = %d",
                                ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    return Status;
}

void BPNode_ClaOut_Teardown(uint32 ContactId)
{
    /*
    ** Disestablish CLA (notify that ACK isn't coming)
    ** Free all CLA resources
    ** Discard output queue
    ** Delete custody timers
    */

    return;
}

/* Main loop for CLA Out task(s) */
void BPNode_ClaOut_AppMain(void)
{
    int32                       OsStatus;
    CFE_Status_t                CFE_Status;
    BPLib_Status_t              Status;
    CFE_ES_TaskId_t             TaskId;
    uint32                      ContactId;
    BPLib_CLA_ContactRunState_t RunState;
    size_t                      BundleSize;
    size_t                      BytesEgressed;

    /* Get this tasks ID to reference later */
    CFE_Status = CFE_ES_GetTaskID(&TaskId);
    if (CFE_Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID,
                            BPLib_EM_EventType_CRITICAL,
                            "[CLA Out #?]: Terminating unknown task");

        /* In case event services is not working, add a message to the system log */
        CFE_ES_WriteToSysLog("Terminating unknown task");

        /* Stop execution */
        CFE_ES_ExitChildTask();
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

        /* Only move toward processing bundles if the task ID has an associated contact ID */
        if (ContactId != BPLIB_MAX_NUM_CONTACTS)
        {
            CFE_Status = BPNode_ClaOut_TaskInit(ContactId);

            /* Initialization must succeed to start processing, exit task if unsuccessful */
            if (CFE_Status == CFE_SUCCESS)
            {
                /* Confirm initialization with give on init semaphore */
                (void) OS_BinSemGive(BPNode_AppData.ClaOutData[ContactId].InitSemId);

                /* The contact task must not be exited */
                Status = BPLib_CLA_GetContactRunState(ContactId, &RunState);
                while (RunState != BPLIB_CLA_EXITED && Status == BPLIB_SUCCESS)
                {
                    /* Attempt to take the wake up semaphore */
                    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContactId].PerfId);
                    OsStatus = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[ContactId].WakeupSemId, BPNODE_CLA_OUT_SEM_WAKEUP_WAIT_MSEC);
                    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContactId].PerfId);

                    if (OsStatus == OS_SUCCESS)
                    {
                        /* Ingress bundles only when the contact has been started */
                        if (RunState == BPLIB_CLA_STARTED)
                        {
                            BytesEgressed = 0;

                            do
                            {
                                CFE_Status = BPNode_ClaOut_ProcessBundleOutput(ContactId, &BundleSize);
                                if (CFE_Status == CFE_SUCCESS)
                                {
                                    BytesEgressed += BundleSize;

                                    if ((BytesEgressed * 8) >= 
                                        BPNode_AppData.ConfigPtrs.ContactsConfigPtr->ContactSet[ContactId].EgressBitsPerCycle)
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                                
                            } while (BPNode_NotifIsSet(&BPNode_AppData.ChildStopWorkNotif) == false);
                        }
                    }
                    else if (OsStatus == OS_SEM_TIMEOUT)
                    {
                        BPLib_EM_SendEvent(BPNODE_CLA_OUT_SEM_TK_TIMEOUT_INF_EID,
                                            BPLib_EM_EventType_INFORMATION,
                                            "[CLA Out #%d]: Wakeup semaphore wait timed out",
                                            ContactId);
                    }
                    else
                    {
                        BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                            BPLib_EM_EventType_ERROR,
                                            "[CLA Out #%d]: Wakeup semaphore take failed, RC = %d",
                                            ContactId,
                                            OsStatus);
                    }

                    /* Update run state of the contact task */
                    Status = BPLib_CLA_GetContactRunState(ContactId, &RunState);
                }
            }

            /* Exit gracefully */
            BPNode_ClaOut_TaskExit(ContactId);
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_INV_ID_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "[CLA Out #?] Could not find a CLA Out task to process bundles with");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("[CLA Out #?] Could not find a CLA Out task to process bundles with");

            /* Stop execution */
            CFE_ES_ExitChildTask();
        }
    }

    return;
}

void BPNode_ClaOut_TaskExit(uint32 ContactId)
{
    BPLib_CLA_ContactRunState_t RunState = BPLIB_CLA_EXITED;

    /* Teardown CLA Out task, in case that hasn't been done already */
    BPNode_ClaOut_Teardown(ContactId);

    (void) BPLib_CLA_GetContactRunState(ContactId, &RunState);
    BPLib_EM_SendEvent(BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                        "[CLA Out #%d]: Terminating Task. Run state = %d.",
                        ContactId,
                        RunState);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[CLA Out #%d]: Terminating Task. Run state = %d.",
                            ContactId,
                            RunState);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContactId].PerfId);

    /* Confirm exit with give on exit semaphore */
    (void) OS_BinSemGive(BPNode_AppData.ClaOutData[ContactId].ExitSemId);

    /* Return semaphores */
    BPNode_ClaOut_DeleteSems(ContactId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}

void BPNode_ClaOut_DeleteSems(uint32 ContactId)
{
    CFE_Status_t Status;

    Status = OS_BinSemDelete(BPNode_AppData.ClaOutData[ContactId].InitSemId);
    if (Status == CFE_SUCCESS)
    {
        Status = OS_BinSemDelete(BPNode_AppData.ClaOutData[ContactId].WakeupSemId);
        if (Status == CFE_SUCCESS)
        {
            Status = OS_BinSemDelete(BPNode_AppData.ClaOutData[ContactId].ExitSemId);
            if (Status != CFE_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_SEM_ERR_EID,
                                    BPLib_EM_EventType_ERROR,
                                    "Could not delete exit semaphore");
            }
        }
        else
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Could not delete wake up semaphore");
        }
    }
    else
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_ERR_EID,
                            BPLib_EM_EventType_ERROR,
                            "Could not delete init semaphore");
    }
}