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
 *   This file contains the source code for the BPNode command-handling functions
 */


/* ======== */
/* Includes */
/* ======== */

#include "fwp_dp.h"
#include "bpnode_app.h"
#include "bpnode_eventids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Verify command packet length */
bool BPA_DP_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
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

        BPLib_EM_SendEvent(BPNODE_CMD_LEN_ERR_EID, BPLib_EM_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (uint16) CFE_SB_MsgIdToValue(MsgId), (uint8) FcnCode, 
                          (uint16) ActualLength, (uint16) ExpectedLength);

        Result = false;

        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }

    return Result;
}

/* Ground command processing */
void BPA_DP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /* Process ground command */
    switch (CommandCode)
    {
        case BPNODE_NOOP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_NoopCmd_t)))
            {
                BPA_DP_NoopCmd();
            }
            break;

        case BPNODE_ADD_ALL_APPLICATIONS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAllApplicationsCmd_t)))
            {
                BPA_DP_AddAllApplicationsCmd();
            }
            break;

        case BPNODE_START_ALL_APPLICATIONS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartAllApplicationsCmd_t)))
            {
                BPA_DP_StartAllApplicationsCmd();
            }
            break;

        case BPNODE_VERIFY_BUNDLE_STORAGE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleStorageCmd_t)))
            {
                BPA_DP_VerifyBundleStorageCmd();
            }
            break;

        case BPNODE_INIT_BUNDLE_STORAGE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_InitBundleStorageCmd_t)))
            {
                BPA_DP_InitBundleStorageCmd();
            }
            break;

        case BPNODE_VERIFY_BUNDLE_METADATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleMetadataCmd_t)))
            {
                BPA_DP_VerifyBundleMetadataCmd();
            }
            break;

        case BPNODE_REBUILD_BUNDLE_METADATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RebuildBundleMetadataCmd_t)))
            {
                BPA_DP_RebuildBundleMetadataCmd();
            }
            break;

        case BPNODE_CLEAR_VOLATILE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ClearVolatileCmd_t)))
            {
                BPA_DP_ClearVolatileCmd();
            }
            break;

        case BPNODE_RELOAD_SAVED_DATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ReloadSavedDataCmd_t)))
            {
                BPA_DP_ReloadSavedDataCmd();
            }
            break;

        case BPNODE_RESET_ALL_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetAllCountersCmd_t)))
            {
                BPA_DP_ResetAllCountersCmd();
            }
            break;

        case BPNODE_RESET_COUNTER_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetCounterCmd_t)))
            {
                BPA_DP_ResetCounterCmd((const BPNode_ResetCounterCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_SOURCE_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetSourceCountersCmd_t)))
            {
                BPA_DP_ResetSourceCountersCmd((const BPNode_ResetSourceCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_BUNDLE_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetBundleCountersCmd_t)))
            {
                BPA_DP_ResetBundleCountersCmd();
            }
            break;

        case BPNODE_RESET_ERROR_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetErrorCountersCmd_t)))
            {
                BPA_DP_ResetErrorCountersCmd();
            }
            break;

        case BPNODE_ADD_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddApplicationCmd_t)))
            {
                BPA_DP_AddApplicationCmd((const BPNode_AddApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveApplicationCmd_t)))
            {
                BPA_DP_RemoveApplicationCmd((const BPNode_RemoveApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SET_REGISTRATION_STATE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetRegistrationStateCmd_t)))
            {
                BPA_DP_SetRegistrationStateCmd((const BPNode_SetRegistrationStateCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_START_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartApplicationCmd_t)))
            {
                BPA_DP_StartApplicationCmd((const BPNode_StartApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_STOP_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StopApplicationCmd_t)))
            {
                BPA_DP_StopApplicationCmd((const BPNode_StopApplicationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthSourcesCmd_t)))
            {
                BPA_DP_AddAuthSourcesCmd((const BPNode_AddAuthSourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthSourcesCmd_t)))
            {
                BPA_DP_RemoveAuthSourcesCmd((const BPNode_RemoveAuthSourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodySourcesCmd_t)))
            {
                BPA_DP_AddAuthCustodySourcesCmd((const BPNode_AddAuthCustodySourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodySourcesCmd_t)))
            {
                BPA_DP_RemoveAuthCustodySourcesCmd((const BPNode_RemoveAuthCustodySourcesCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODIANS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodiansCmd_t)))
            {
                BPA_DP_AddAuthCustodiansCmd((const BPNode_AddAuthCustodiansCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODIANS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodiansCmd_t)))
            {
                BPA_DP_RemoveAuthCustodiansCmd((const BPNode_RemoveAuthCustodiansCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_AUTH_REPORT_TO_EID_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthReportToEidCmd_t)))
            {
                BPA_DP_AddAuthReportToEidCmd((const BPNode_AddAuthReportToEidCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthReportToEidCmd_t)))
            {
                BPA_DP_RemoveAuthReportToEidCmd((const BPNode_RemoveAuthReportToEidCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_LATENCY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddLatencyCmd_t)))
            {
                BPA_DP_AddLatencyCmd((const BPNode_AddLatencyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_LATENCY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveLatencyCmd_t)))
            {
                BPA_DP_RemoveLatencyCmd((const BPNode_RemoveLatencyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_SETUP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactSetupCmd_t)))
            {
                BPA_DP_ContactSetupCmd((const BPNode_ContactSetupCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_START_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStartCmd_t)))
            {
                BPA_DP_ContactStartCmd((const BPNode_ContactStartCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_STOP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStopCmd_t)))
            {
                BPA_DP_ContactStopCmd((const BPNode_ContactStopCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_CONTACT_TEARDOWN_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactTeardownCmd_t)))
            {
                BPA_DP_ContactTeardownCmd((const BPNode_ContactTeardownCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_MIB_ARRAY_KEY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddMibArrayKeyCmd_t)))
            {
                BPA_DP_AddMibArrayKeyCmd((const BPNode_AddMibArrayKeyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_MIB_ARRAY_KEY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveMibArrayKeyCmd_t)))
            {
                BPA_DP_RemoveMibArrayKeyCmd((const BPNode_RemoveMibArrayKeyCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SET_MIB_ITEM_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetMibItemCmd_t)))
            {
                BPA_DP_SetMibItemCmd((const BPNode_SetMibItemCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_ADD_STORAGE_ALLOCATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddStorageAllocationCmd_t)))
            {
                BPA_DP_AddStorageAllocationCmd((const BPNode_AddStorageAllocationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_REMOVE_STORAGE_ALLOCATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveStorageAllocationCmd_t)))
            {
                BPA_DP_RemoveStorageAllocationCmd((const BPNode_RemoveStorageAllocationCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_PERFORM_SELF_TEST_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_PerformSelfTestCmd_t)))
            {
                BPA_DP_PerformSelfTestCmd();
            }
            break;

        case BPNODE_SEND_NODE_MIB_CONFIG_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibConfigHkCmd_t)))
            {
                BPA_DP_SendNodeMibConfigHkCmd();
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibConfigHkCmd_t)))
            {
                BPA_DP_SendSourceMibConfigHkCmd();
            }
            break;

        case BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibCountersHkCmd_t)))
            {
                BPA_DP_SendNodeMibCountersHkCmd();
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibCountersHkCmd_t)))
            {
                BPA_DP_SendSourceMibCountersHkCmd();
            }
            break;

        case BPNODE_SEND_STORAGE_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendStorageHkCmd_t)))
            {
                BPA_DP_SendStorageHkCmd();
            }
            break;

        case BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendChannelContactStatHkCmd_t)))
            {
                BPA_DP_SendChannelContactStatHkCmd();
            }
            break;

        /* Default case already found during FC vs length test */
        default:
            BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

            BPLib_EM_SendEvent(BPNODE_CC_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "Invalid ground command code: CC = %d", CommandCode);
            break;
    }
}

/* Process packets received on command pipe */
void BPA_DP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case BPNODE_CMD_MID:
            BPA_DP_ProcessGroundCommand(SBBufPtr);
            break;

        default:
            BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

            BPLib_EM_SendEvent(BPNODE_MID_ERR_EID, BPLib_EM_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x", 
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}

/* Noop command */
void BPA_DP_NoopCmd(void)
{
    char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
    char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];

    BPLib_NC_Noop();

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
        BPNODE_MAJOR_VERSION,
        BPNODE_MINOR_VERSION,
        BPNODE_REVISION);

    CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

    BPLib_EM_SendEvent(BPNODE_NOOP_INF_EID, BPLib_EM_EventType_INFORMATION,
                        "No-op command. %s", VersionString);
}

/* Add all applications command */
void BPA_DP_AddAllApplicationsCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAllApplications();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Start all applications command */
void BPA_DP_StartAllApplicationsCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StartAllApplications();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Verify bundle storage command */
void BPA_DP_VerifyBundleStorageCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_VerifyBundleStorage();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Initialize bundle storage command */
void BPA_DP_InitBundleStorageCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_InitBundleStorage();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Verify bundle metadata (7.2) command */
void BPA_DP_VerifyBundleMetadataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_VerifyBundleMetadata();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Rebuild bundle metadata command */
void BPA_DP_RebuildBundleMetadataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RebuildBundleMetadata();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Clear volatile command */
void BPA_DP_ClearVolatileCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ClearVolatile();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Reload saved data (7.2) command */
void BPA_DP_ReloadSavedDataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ReloadSavedData();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Reset all counters command */
void BPA_DP_ResetAllCountersCmd(void)
{
    uint8 i;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived = 0;

    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPNode_AppData.AduInData[i].AduCountReceived = 0;
        BPNode_AppData.AduOutData[i].AduCountDelivered = 0;
    }

    BPLib_NC_ResetAllCounters();
}

/* Reset counter command */
void BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetCounter(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Reset source counters command */
void BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetSourceCounters(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Reset bundle counters command */
void BPA_DP_ResetBundleCountersCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetBundleCounters();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Reset error counters command */
void BPA_DP_ResetErrorCountersCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetErrorCounters();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add application command */
void BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddApplication(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove application command */
void BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveApplication(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Set registration state command */
void BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SetRegistrationState(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Start application command */
void BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StartApplication(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Stop application command */
void BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StopApplication(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add authorized sources command */
void BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthSources(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove authorized sources command */
void BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthSources(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add authorized custody sources command */
void BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthCustodySources(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove authorized custody sources command */
void BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthCustodySources(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add authorized custodians command */
void BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthCustodians(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove authorized custodians command */
void BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthCustodians(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add authorized report-to eid command */
void BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthReportToEid(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove authorized report-to eid command */
void BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthReportToEid(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add latency command */
void BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddLatency(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove latency command */
void BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveLatency(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Contact setup command */
void BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactSetup(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Contact start command */
void BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactStart(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Contact stop command */
void BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactStop(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Contact teardown command */
void BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactTeardown(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add mib array key command */
void BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddMibArrayKey(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove mib array key command */
void BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveMibArrayKey(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Set mib item command */
void BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SetMibItem(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Add storage allocation command */
void BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddStorageAllocation(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Remove storage allocation command */
void BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveStorageAllocation(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Perform self test (7.2) command */
void BPA_DP_PerformSelfTestCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_PerformSelfTest();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Send node mib config hk command */
void BPA_DP_SendNodeMibConfigHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendNodeMibConfigHk();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Send per-source mib config hk command */
void BPA_DP_SendSourceMibConfigHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendSourceMibConfigHk();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Send Node MIB Counters housekeeping command */
void BPA_DP_SendNodeMibCountersHkCmd(void)
{
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    uint8 i;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived = 0;

    /* Get ADU counts for all ADU child tasks */
    for(i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered += BPNode_AppData.AduOutData[i].AduCountDelivered;
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived += BPNode_AppData.AduInData[i].AduCountReceived;
    }

    /* Get DTN time data */
    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.NodeMibCountersHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader), true);

    BPLib_NC_SendNodeMibCountersHk();
}

/* Send per-source mib counter hk command */
void BPA_DP_SendSourceMibCountersHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendSourceMibCountersHk();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Send storage hk command */
void BPA_DP_SendStorageHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendStorageHk();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}

/* Send channel/contact status hk command */
void BPA_DP_SendChannelContactStatHkCmd(void)
{
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    uint8 i;

    /* Get ADU status from all child tasks */
    for(i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPNode_AppData.ChannelContactStatHkTlm.Payload.ChannelStatus[i].Status = BPNode_AppData.AduState[i].AppState;
    }

    /* Get DTN time data */
    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.ChannelContactStatHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.ChannelContactStatHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.ChannelContactStatHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactStatHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactStatHkTlm.TelemetryHeader), true);

    BPLib_NC_SendChannelContactStatHk();
}
