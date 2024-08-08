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
 *   This file contains the source code for the BPNode software bus processing.
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


/*
** Function Definitions
*/

/* Verify command packet length */
bool BPNode_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              Result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /* Check that actual length matches expected length */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(BPNODE_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (uint16) CFE_SB_MsgIdToValue(MsgId), (uint8) FcnCode, 
                          (uint16) ActualLength, (uint16) ExpectedLength);

        Result = false;

        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }

    return Result;
}

/* Ground command processing */
void BPNode_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /* Process ground command */
    switch (CommandCode)
    {
        case BPNODE_NOOP_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_NoopCmd_t)))
            {
                BPNode_NoopCmd((const BPNode_NoopCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_ALL_APPLICATIONS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAllApplicationsCmd_t)))
            {
                BPNode_AddAllApplicationsCmd((const BPNode_AddAllApplicationsCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_START_ALL_APPLICATIONS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartAllApplicationsCmd_t)))
            {
                BPNode_StartAllApplicationsCmd((const BPNode_StartAllApplicationsCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_VERIFY_BUNDLE_STORAGE_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleStorageCmd_t)))
            {
                BPNode_VerifyBundleStorageCmd((const BPNode_VerifyBundleStorageCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_INIT_BUNDLE_STORAGE_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_InitBundleStorageCmd_t)))
            {
                BPNode_InitBundleStorageCmd((const BPNode_InitBundleStorageCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_VERIFY_BUNDLE_METADATA_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleMetadataCmd_t)))
            {
                BPNode_VerifyBundleMetadataCmd((const BPNode_VerifyBundleMetadataCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REBUILD_BUNDLE_METADATA_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RebuildBundleMetadataCmd_t)))
            {
                BPNode_RebuildBundleMetadataCmd((const BPNode_RebuildBundleMetadataCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CLEAR_VOLATILE_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ClearVolatileCmd_t)))
            {
                BPNode_ClearVolatileCmd((const BPNode_ClearVolatileCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RELOAD_SAVED_DATA_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ReloadSavedDataCmd_t)))
            {
                BPNode_ReloadSavedDataCmd((const BPNode_ReloadSavedDataCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_ALL_COUNTERS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetAllCountersCmd_t)))
            {
                BPNode_ResetAllCountersCmd((const BPNode_ResetAllCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_COUNTER_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetCounterCmd_t)))
            {
                BPNode_ResetCounterCmd((const BPNode_ResetCounterCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_SOURCE_COUNTERS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetSourceCountersCmd_t)))
            {
                BPNode_ResetSourceCountersCmd((const BPNode_ResetSourceCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_BUNDLE_COUNTERS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetBundleCountersCmd_t)))
            {
                BPNode_ResetBundleCountersCmd((const BPNode_ResetBundleCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_ERROR_COUNTERS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetErrorCountersCmd_t)))
            {
                BPNode_ResetErrorCountersCmd((const BPNode_ResetErrorCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_APPLICATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddApplicationCmd_t)))
            {
                BPNode_AddApplicationCmd((const BPNode_AddApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_APPLICATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveApplicationCmd_t)))
            {
                BPNode_RemoveApplicationCmd((const BPNode_RemoveApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SET_REGISTRATION_STATE_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetRegistrationStateCmd_t)))
            {
                BPNode_SetRegistrationStateCmd((const BPNode_SetRegistrationStateCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_START_APPLICATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartApplicationCmd_t)))
            {
                BPNode_StartApplicationCmd((const BPNode_StartApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_STOP_APPLICATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StopApplicationCmd_t)))
            {
                BPNode_StopApplicationCmd((const BPNode_StopApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_SOURCES_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthSourcesCmd_t)))
            {
                BPNode_AddAuthSourcesCmd((const BPNode_AddAuthSourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_SOURCES_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthSourcesCmd_t)))
            {
                BPNode_RemoveAuthSourcesCmd((const BPNode_RemoveAuthSourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodySourcesCmd_t)))
            {
                BPNode_AddAuthCustodySourcesCmd((const BPNode_AddAuthCustodySourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodySourcesCmd_t)))
            {
                BPNode_RemoveAuthCustodySourcesCmd((const BPNode_RemoveAuthCustodySourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODIANS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodiansCmd_t)))
            {
                BPNode_AddAuthCustodiansCmd((const BPNode_AddAuthCustodiansCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODIANS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodiansCmd_t)))
            {
                BPNode_RemoveAuthCustodiansCmd((const BPNode_RemoveAuthCustodiansCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_REPORT_TO_EID_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthReportToEidCmd_t)))
            {
                BPNode_AddAuthReportToEidCmd((const BPNode_AddAuthReportToEidCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthReportToEidCmd_t)))
            {
                BPNode_RemoveAuthReportToEidCmd((const BPNode_RemoveAuthReportToEidCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_LATENCY_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddLatencyCmd_t)))
            {
                BPNode_AddLatencyCmd((const BPNode_AddLatencyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_LATENCY_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveLatencyCmd_t)))
            {
                BPNode_RemoveLatencyCmd((const BPNode_RemoveLatencyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_SETUP_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactSetupCmd_t)))
            {
                BPNode_ContactSetupCmd((const BPNode_ContactSetupCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_START_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStartCmd_t)))
            {
                BPNode_ContactStartCmd((const BPNode_ContactStartCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_STOP_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStopCmd_t)))
            {
                BPNode_ContactStopCmd((const BPNode_ContactStopCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_TEARDOWN_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactTeardownCmd_t)))
            {
                BPNode_ContactTeardownCmd((const BPNode_ContactTeardownCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_MIB_ARRAY_KEY_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddMibArrayKeyCmd_t)))
            {
                BPNode_AddMibArrayKeyCmd((const BPNode_AddMibArrayKeyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_MIB_ARRAY_KEY_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveMibArrayKeyCmd_t)))
            {
                BPNode_RemoveMibArrayKeyCmd((const BPNode_RemoveMibArrayKeyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SET_MIB_ITEM_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetMibItemCmd_t)))
            {
                BPNode_SetMibItemCmd((const BPNode_SetMibItemCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_STORAGE_ALLOCATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddStorageAllocationCmd_t)))
            {
                BPNode_AddStorageAllocationCmd((const BPNode_AddStorageAllocationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_STORAGE_ALLOCATION_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveStorageAllocationCmd_t)))
            {
                BPNode_RemoveStorageAllocationCmd((const BPNode_RemoveStorageAllocationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_PERFORM_SELF_TEST_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_PerformSelfTestCmd_t)))
            {
                BPNode_PerformSelfTestCmd((const BPNode_PerformSelfTestCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_NODE_MIB_CONFIG_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibConfigHkCmd_t)))
            {
                BPNode_SendNodeMibConfigHkCmd((const BPNode_SendNodeMibConfigHkCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibConfigHkCmd_t)))
            {
                BPNode_SendSourceMibConfigHkCmd((const BPNode_SendSourceMibConfigHkCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibCountersHkCmd_t)))
            {
                BPNode_SendNodeMibCountersHkCmd((const BPNode_SendNodeMibCountersHkCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibCountersHkCmd_t)))
            {
                BPNode_SendSourceMibCountersHkCmd((const BPNode_SendSourceMibCountersHkCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_STORAGE_HK:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendStorageHk_t)))
            {
                BPNode_SendStorageHk((const BPNode_SendStorageHk_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendChannelContactStatHkCmd_t)))
            {
                BPNode_SendChannelContactStatHkCmd((const BPNode_SendChannelContactStatHkCmd_t *)SBBufPtr);
            }
            break;

        /* Default case already found during FC vs length test */
        default:
            BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

            CFE_EVS_SendEvent(BPNODE_CC_ERR_EID, CFE_EVS_EventType_ERROR, 
                            "BPNode_ProcessGroundCommand: Invalid ground command code: CC = %d", CommandCode);
            break;
    }
}

/* Process packets received on command pipe */
void BPNode_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case BPNODE_CMD_MID:
            BPNode_ProcessGroundCommand(SBBufPtr);
            break;

        default:
            BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

            CFE_EVS_SendEvent(BPNODE_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x", 
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}
