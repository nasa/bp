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
    BPNode_AppData.ClaInData[ContactInfo.ContactID].PerfId = BPNODE_CLA_IN_PERF_ID_BASE + ContactInfo.ContactID;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME,
                                            &BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation.PspModuleId);

    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Couldn't find I/O driver. Error = %d",
                            ContactInfo.ContactID, PspStatus);

        Status = BPLIB_ERROR;
    }

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
        BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation.SubchannelId = BPNODE_CLA_PSP_INPUT_SUBCHANNEL;

        /* Configure Port Number */
        snprintf(Str, sizeof(Str), "port=%d", ContactInfo.PortNum);

        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA In #%d]: Couldn't set port number configuration. Error = %d",
                                ContactInfo.ContactID, PspStatus);

            Status = BPLIB_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        /* Configure IP Address */
        snprintf(Str, sizeof(Str), "IpAddr=%s", ContactInfo.CLAddr);

        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation,
                                                CFE_PSP_IODriver_SET_CONFIGURATION,
                                                CFE_PSP_IODriver_CONST_STR(Str));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_IP_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA In #%d]: Couldn't set IP address configuration. Error = %d",
                                ContactInfo.ContactID, PspStatus);

            Status = BPLIB_ERROR;
        }
    }

    if (Status == BPLIB_SUCCESS)
    {
        OS_printf("[CLA In #%d]: Receiving on %s:%d\n", ContactInfo.ContactID, ContactInfo.CLAddr, ContactInfo.PortNum);

        /* Set direction to input only */
        PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContactInfo.ContactID].PspLocation,
                                                CFE_PSP_IODriver_SET_DIRECTION,
                                                CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));

        if (PspStatus != CFE_PSP_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[CLA In #%d]: Couldn't set I/O direction to input. Error = %d",
                                ContactInfo.ContactID, PspStatus);

            Status = BPLIB_ERROR;
        }
    }

    return Status;
}