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
#include "bpnode_cmds.h"
#include "bpnode_eventids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

#include "bpnode_eds_dispatcher.h"
#include "bpnode_eds_dictionary.h"

/**
 * If there are mismatched names in the build, check bpnode/eds/bpnode.xml
 * See also bpnode/config/default_bpnode_msgstruct.h
 */
/* Command code lookup table */
static const EdsDispatchTable_BPNode_Application_CFE_SB_Telecommand_t BPNODE_TC_DISPATCH_TABLE = {
    .CMD = {
                .NoopCmd_indication = BPNode_NoopCmd,
                .AddAllApplicationsCmd_indication = BPNode_AddAllApplicationsCmd,
                .StartAllApplicationsCmd_indication = BPNode_StartAllApplicationsCmd,
                .VerifyBundleStorageCmd_indication = BPNode_VerifyBundleStorageCmd,
                .InitBundleStorageCmd_indication = BPNode_InitBundleStorageCmd,
                .VerifyBundleMetadataCmd_indication = BPNode_VerifyBundleMetadataCmd,
                .RebuildBundleMetadataCmd_indication = BPNode_RebuildBundleMetadataCmd,
                .ClearVolatileCmd_indication = BPNode_ClearVolatileCmd,
                .ReloadSavedDataCmd_indication = BPNode_ReloadSavedDataCmd,
                .ResetAllCountersCmd_indication = BPNode_ResetAllCountersCmd,
                .ResetCounterCmd_indication = BPNode_ResetCounterCmd,
                .ResetSourceCountersCmd_indication = BPNode_ResetSourceCountersCmd,
                .ResetBundleCountersCmd_indication = BPNode_ResetBundleCountersCmd,
                .ResetErrorCountersCmd_indication = BPNode_ResetErrorCountersCmd,
                .AddApplicationCmd_indication = BPNode_AddApplicationCmd,
                .RemoveApplicationCmd_indication = BPNode_RemoveApplicationCmd,
                .SetRegistrationStateCmd_indication = BPNode_SetRegistrationStateCmd,
                .StartApplicationCmd_indication = BPNode_StartApplicationCmd,
                .StopApplicationCmd_indication = BPNode_StopApplicationCmd,
                .AddAuthSourcesCmd_indication = BPNode_AddAuthSourcesCmd,
                .RemoveAuthSourcesCmd_indication = BPNode_RemoveAuthSourcesCmd,
                .AddAuthCustodySourcesCmd_indication = BPNode_AddAuthCustodySourcesCmd,
                .RemoveAuthCustodySourcesCmd_indication = BPNode_RemoveAuthCustodySourcesCmd,
                .AddAuthCustodiansCmd_indication = BPNode_AddAuthCustodiansCmd,
                .RemoveAuthCustodiansCmd_indication = BPNode_RemoveAuthCustodiansCmd,
                .AddAuthReportToEidCmd_indication = BPNode_AddAuthReportToEidCmd,
                .RemoveAuthReportToEidCmd_indication = BPNode_RemoveAuthReportToEidCmd,
                .AddLatencyCmd_indication = BPNode_AddLatencyCmd,
                .RemoveLatencyCmd_indication = BPNode_RemoveLatencyCmd,
                .ContactSetupCmd_indication = BPNode_ContactSetupCmd,
                .ContactStartCmd_indication = BPNode_ContactStartCmd,
                .ContactStopCmd_indication = BPNode_ContactStopCmd,
                .ContactTeardownCmd_indication = BPNode_ContactTeardownCmd,
                .AddMibArrayKeyCmd_indication = BPNode_AddMibArrayKeyCmd,
                .RemoveMibArrayKeyCmd_indication = BPNode_RemoveMibArrayKeyCmd,
                .SetMibItemCmd_indication = BPNode_SetMibItemCmd,
                .AddStorageAllocationCmd_indication = BPNode_AddStorageAllocationCmd,
                .RemoveStorageAllocationCmd_indication = BPNode_RemoveStorageAllocationCmd,
                .PerformSelfTestCmd_indication = BPNode_PerformSelfTestCmd,
                .SendNodeMibConfigHkCmd_indication = BPNode_SendNodeMibConfigHkCmd,
                .SendSourceMibConfigHkCmd_indication = BPNode_SendSourceMibConfigHkCmd,
                .SendNodeMibCountersHkCmd_indication = BPNode_SendNodeMibCountersHkCmd,
                .SendSourceMibCountersHkCmd_indication = BPNode_SendSourceMibCountersHkCmd,
                .SendStorageHkCmd_indication = BPNode_SendStorageHkCmd,
                .SendChannelContactStatusHk_indication = BPNode_SendChannelContactStatHkCmd
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
