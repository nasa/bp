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
 *   This file contains the source code for the BPNode EDS bindings
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_eventids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

#include "bpnode_eds_dispatcher.h"
#include "bpnode_eds_dictionary.h"

#include "fwp_dp.h"

/**
 * If there are mismatched names in the build, check bpnode/eds/bpnode.xml
 * See also bpnode/config/default_bpnode_msgstruct.h
 */
/* Command code lookup table */
static const EdsDispatchTable_BPNode_Application_CFE_SB_Telecommand_t BPNODE_TC_DISPATCH_TABLE = {
    .CMD = {
                .NoopCmd_indication = BPA_DP_NoopCmd,
                .AddAllApplicationsCmd_indication = BPA_DP_AddAllApplicationsCmd,
                .StartAllApplicationsCmd_indication = BPA_DP_StartAllApplicationsCmd,
                .VerifyBundleStorageCmd_indication = BPA_DP_VerifyBundleStorageCmd,
                .InitBundleStorageCmd_indication = BPA_DP_InitBundleStorageCmd,
                .VerifyBundleMetadataCmd_indication = BPA_DP_VerifyBundleMetadataCmd,
                .RebuildBundleMetadataCmd_indication = BPA_DP_RebuildBundleMetadataCmd,
                .ClearVolatileCmd_indication = BPA_DP_ClearVolatileCmd,
                .ReloadSavedDataCmd_indication = BPA_DP_ReloadSavedDataCmd,
                .ResetAllCountersCmd_indication = BPA_DP_ResetAllCountersCmd,
                .ResetCounterCmd_indication = BPA_DP_ResetCounterCmd,
                .ResetSourceCountersCmd_indication = BPA_DP_ResetSourceCountersCmd,
                .ResetBundleCountersCmd_indication = BPA_DP_ResetBundleCountersCmd,
                .ResetErrorCountersCmd_indication = BPA_DP_ResetErrorCountersCmd,
                .AddApplicationCmd_indication = BPA_DP_AddApplicationCmd,
                .RemoveApplicationCmd_indication = BPA_DP_RemoveApplicationCmd,
                .SetRegistrationStateCmd_indication = BPA_DP_SetRegistrationStateCmd,
                .StartApplicationCmd_indication = BPA_DP_StartApplicationCmd,
                .StopApplicationCmd_indication = BPA_DP_StopApplicationCmd,
                .AddAuthSourcesCmd_indication = BPA_DP_AddAuthSourcesCmd,
                .RemoveAuthSourcesCmd_indication = BPA_DP_RemoveAuthSourcesCmd,
                .AddAuthCustodySourcesCmd_indication = BPA_DP_AddAuthCustodySourcesCmd,
                .RemoveAuthCustodySourcesCmd_indication = BPA_DP_RemoveAuthCustodySourcesCmd,
                .AddAuthCustodiansCmd_indication = BPA_DP_AddAuthCustodiansCmd,
                .RemoveAuthCustodiansCmd_indication = BPA_DP_RemoveAuthCustodiansCmd,
                .AddAuthReportToEidCmd_indication = BPA_DP_AddAuthReportToEidCmd,
                .RemoveAuthReportToEidCmd_indication = BPA_DP_RemoveAuthReportToEidCmd,
                .AddLatencyCmd_indication = BPA_DP_AddLatencyCmd,
                .RemoveLatencyCmd_indication = BPA_DP_RemoveLatencyCmd,
                .ContactSetupCmd_indication = BPA_DP_ContactSetupCmd,
                .ContactStartCmd_indication = BPA_DP_ContactStartCmd,
                .ContactStopCmd_indication = BPA_DP_ContactStopCmd,
                .ContactTeardownCmd_indication = BPA_DP_ContactTeardownCmd,
                .AddMibArrayKeyCmd_indication = BPA_DP_AddMibArrayKeyCmd,
                .RemoveMibArrayKeyCmd_indication = BPA_DP_RemoveMibArrayKeyCmd,
                .SetMibItemCmd_indication = BPA_DP_SetMibItemCmd,
                .AddStorageAllocationCmd_indication = BPA_DP_AddStorageAllocationCmd,
                .RemoveStorageAllocationCmd_indication = BPA_DP_RemoveStorageAllocationCmd,
                .PerformSelfTestCmd_indication = BPA_DP_PerformSelfTestCmd,
                .SendNodeMibConfigHkCmd_indication = BPA_DP_SendNodeMibConfigHkCmd,
                .SendSourceMibConfigHkCmd_indication = BPA_DP_SendSourceMibConfigHkCmd,
                .SendNodeMibCountersHkCmd_indication = BPA_DP_SendNodeMibCountersHkCmd,
                .SendSourceMibCountersHkCmd_indication = BPA_DP_SendSourceMibCountersHkCmd,
                .SendStorageHkCmd_indication = BPA_DP_SendStorageHkCmd,
                .SendChannelContactStatusHk_indication = BPA_DP_SendChannelContactStatHkCmd
            }
};


/*
** Function Definitions
*/

/* Process packets received on command pipe */
void BPNode_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_Status_t      Status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    Status = EdsDispatch_BPNode_Application_Telecommand(SBBufPtr, &BPNODE_TC_DISPATCH_TABLE);

    /* Invalid packet */
    if (Status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);
        CFE_MSG_GetSize(&SBBufPtr->Msg, &MsgSize);
        CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &MsgFc);

        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        /* Invalid message ID */
        if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            BPLib_EM_SendEvent(BPNODE_MID_ERR_EID, BPLib_EM_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x", 
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
        }
        /* Invalid message length */
        else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            BPLib_EM_SendEvent(BPNODE_CMD_LEN_ERR_EID, BPLib_EM_EventType_ERROR,
                              "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u",
                              (uint16) CFE_SB_MsgIdToValue(MsgId), 
                              (uint8) MsgFc, (uint16) MsgSize);
        }
        /* Invalid command code */
        else
        {
            BPLib_EM_SendEvent(BPNODE_CC_ERR_EID, BPLib_EM_EventType_ERROR,
                              "Invalid ground command code: CC = %d", (uint8) MsgFc);
        }
    }
}
