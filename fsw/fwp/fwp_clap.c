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

#include "fwp_clap.h"

/* =================== */
/* Function Prototypes */
/* =================== */

BPLib_Status_t BPA_CLAP_ContactSetup(BPLib_CLA_ContactsSet_t ContactInfo)
{
    BPLib_Status_t Status;
    int32_t        PspStatus;
    char           Str[100];

    Status = BPLIB_SUCCESS;
    BPNode_AppData.ClaInData[ContactInfo.ContactId].PerfId = BPNODE_CLA_IN_PERF_ID_BASE + ContactInfo.ContactId;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME,
                                            &BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation.PspModuleId);

    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLAP_IO_FIND_NAME_DBG_EID, BPLib_EM_EventType_ERROR,
                            "[Contact ID #%d]: Couldn't find I/O driver. Error = %d",
                            ContactInfo.ContactId,
                            PspStatus);

        Status = BPLIB_CLA_IO_ERROR;
    }

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
        BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation.SubchannelId = BPNODE_CLA_PSP_INPUT_SUBCHANNEL;

        /* Configure Port Number */
        snprintf(Str, sizeof(Str), "port=%d", ContactInfo.PortNum);

        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLAP_IO_PORT_DBG_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set port number configuration. Error = %d",
                                ContactInfo.ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Configure IP Address */
        snprintf(Str, sizeof(Str), "IpAddr=%s", ContactInfo.CLAddr);

        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLAP_IO_IP_DBG_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set IP address configuration. Error = %d",
                                ContactInfo.ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        OS_printf("[Contact ID #%d]: Setup on %s:%d\n", ContactInfo.ContactId, ContactInfo.CLAddr, ContactInfo.PortNum);

        /* Set direction to input only */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_DIRECTION,
                                                CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLAP_IO_DIR_DBG_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set I/O direction to input. Error = %d",
                                ContactInfo.ContactId,
                                PspStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }
    }

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactStart(BPLib_CLA_ContactsSet_t ContactInfo)
{
    int32_t OtherStatus;
    BPLib_Status_t Status;
    BPLib_CLA_ContactRunState_t RunState;

    /* Default to success */
    Status = BPLIB_SUCCESS;

    /* Verify that the contact is in the correct state before starting it */
    RunState = BPLib_CLA_GetContactRunState(ContactInfo.ContactId);
    if (RunState != BPLIB_CLA_TORNDOWN)
    {
        /* Set I/O to running */
        OtherStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactId].PspLocation,
                                                CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));

        if (OtherStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLAP_IO_SET_RUN_DBG_EID, BPLib_EM_EventType_ERROR,
                                "[Contact ID #%d]: Couldn't set I/O state to running. Error = %d",
                                ContactInfo.ContactId,
                                OtherStatus);

            Status = BPLIB_CLA_IO_ERROR;
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Start performance log */
            BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContactInfo.ContactId].PerfId);

            /* Notify main task that child task is running */
            BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContactInfo.ContactId].PerfId);
            OtherStatus = OS_BinSemGive(BPNode_AppData.ClaInData[ContactInfo.ContactId].InitSemId);
            BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContactInfo.ContactId].PerfId);

            if (OtherStatus != OS_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLAP_INIT_SEM_DBG_EID, BPLib_EM_EventType_ERROR,
                                    "[Contact ID #%d]: Failed to give init semaphore. Error = %d",
                                    ContactInfo.ContactId,
                                    OtherStatus);

                Status = BPLIB_CLA_INIT_SEM_ERROR;
            }
        }

        if (Status == BPLIB_SUCCESS)
        {
            /* Set the run state to started */
            Status = BPLib_CLA_SetContactRunState(ContactInfo.ContactId, BPLIB_CLA_STARTED);
        }
    }
    else
    {
        Status = BPLIB_CLA_INCORRECT_STATE;

        BPLib_EM_SendEvent(BPNODE_CLAP_INIT_STATE_DBG_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Task must be setup before starting",
                            ContactInfo.ContactId);
    }

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactStop(void)
{
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactTeardown(void)
{
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    return Status;
}