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
    // BPLib_Status_t    Status;

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

        BPLib_AS_Increment(0, BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT);
    }

    return Result;
}

/* Ground command processing */
void BPA_DP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode;
    BPLib_Status_t    Status;

    CommandCode = 0;
    Status      = BPLIB_UNKNOWN;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /* Process ground command */
    switch (CommandCode)
    {
        case BPNODE_NOOP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_NoopCmd_t)))
            {
                char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
                char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];

                Status = BPLib_NC_Noop();

                (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
                                BPNODE_MAJOR_VERSION,
                                BPNODE_MINOR_VERSION,
                                BPNODE_REVISION);

                CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                            BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

                BPLib_EM_SendEvent(BPNODE_NOOP_INF_EID, BPLib_EM_EventType_INFORMATION,
                                    "No-op command. %s", VersionString);
            }
            break;

        case BPNODE_ADD_ALL_APPLICATIONS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAllApplicationsCmd_t)))
            {
                Status = BPLib_NC_AddAllApplications();
            }
            break;

        case BPNODE_START_ALL_APPLICATIONS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartAllApplicationsCmd_t)))
            {
                Status = BPLib_NC_StartAllApplications();
            }
            break;

        case BPNODE_VERIFY_BUNDLE_STORAGE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleStorageCmd_t)))
            {
                Status = BPLib_NC_VerifyBundleStorage();
            }
            break;

        case BPNODE_INIT_BUNDLE_STORAGE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_InitBundleStorageCmd_t)))
            {
                Status = BPLib_NC_InitBundleStorage();
            }
            break;

        case BPNODE_VERIFY_BUNDLE_METADATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_VerifyBundleMetadataCmd_t)))
            {
                Status = BPLib_NC_VerifyBundleMetadata();
            }
            break;

        case BPNODE_REBUILD_BUNDLE_METADATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RebuildBundleMetadataCmd_t)))
            {
                Status = BPLib_NC_RebuildBundleMetadata();
            }
            break;

        case BPNODE_CLEAR_VOLATILE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ClearVolatileCmd_t)))
            {
                Status = BPLib_NC_ClearVolatile();
            }
            break;

        case BPNODE_RELOAD_SAVED_DATA_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ReloadSavedDataCmd_t)))
            {
                Status = BPLib_NC_ReloadSavedData();
            }
            break;

        case BPNODE_RESET_ALL_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetAllCountersCmd_t)))
            {
                BPLib_NC_ResetAllCounters();
            }
            break;

        case BPNODE_RESET_COUNTER_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetCounterCmd_t)))
            {
                const BPNode_ResetCounterCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ResetCounterCmd_t*) SBBufPtr;

                Status = BPLib_NC_ResetCounter(MsgPtr->Payload);
            }
            break;

        case BPNODE_RESET_SOURCE_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetSourceCountersCmd_t)))
            {
                const BPNode_ResetSourceCountersCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ResetSourceCountersCmd_t*) SBBufPtr;

                Status = BPLib_NC_ResetSourceCounters(MsgPtr->Payload);
            }
            break;

        case BPNODE_RESET_BUNDLE_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetBundleCountersCmd_t)))
            {
                const BPNode_ResetBundleCountersCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ResetBundleCountersCmd_t*) SBBufPtr;

                Status = BPLib_NC_ResetBundleCounters(MsgPtr->Payload);
            }
            break;

        case BPNODE_RESET_ERROR_COUNTERS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetErrorCountersCmd_t)))
            {
                const BPNode_ResetErrorCountersCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ResetErrorCountersCmd_t*) SBBufPtr;

                Status = BPLib_NC_ResetErrorCounters(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddApplicationCmd_t)))
            {
                const BPNode_AddApplicationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddApplicationCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddApplication(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveApplicationCmd_t)))
            {
                const BPNode_RemoveApplicationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveApplicationCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveApplication(MsgPtr->Payload);
            }
            break;

        case BPNODE_SET_REGISTRATION_STATE_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetRegistrationStateCmd_t)))
            {
                const BPNode_SetRegistrationStateCmd_t* MsgPtr;
                MsgPtr = (const BPNode_SetRegistrationStateCmd_t*) SBBufPtr;

                Status = BPLib_NC_SetRegistrationState(MsgPtr->Payload);
            }
            break;

        case BPNODE_START_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StartApplicationCmd_t)))
            {
                const BPNode_StartApplicationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_StartApplicationCmd_t*) SBBufPtr;

                Status = BPLib_NC_StartApplication(MsgPtr->Payload);
            }
            break;

        case BPNODE_STOP_APPLICATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_StopApplicationCmd_t)))
            {
                const BPNode_StopApplicationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_StopApplicationCmd_t*) SBBufPtr;

                Status = BPLib_NC_StopApplication(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_AUTH_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthSourcesCmd_t)))
            {
                const BPNode_AddAuthSourcesCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddAuthSourcesCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddAuthSources(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_AUTH_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthSourcesCmd_t)))
            {
                const BPNode_RemoveAuthSourcesCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveAuthSourcesCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveAuthSources(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodySourcesCmd_t)))
            {
                const BPNode_AddAuthCustodySourcesCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddAuthCustodySourcesCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddAuthCustodySources(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodySourcesCmd_t)))
            {
                const BPNode_RemoveAuthCustodySourcesCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveAuthCustodySourcesCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveAuthCustodySources(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_AUTH_CUSTODIANS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthCustodiansCmd_t)))
            {
                const BPNode_AddAuthCustodiansCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddAuthCustodiansCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddAuthCustodians(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_AUTH_CUSTODIANS_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthCustodiansCmd_t)))
            {
                const BPNode_RemoveAuthCustodiansCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveAuthCustodiansCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveAuthCustodians(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_AUTH_REPORT_TO_EID_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddAuthReportToEidCmd_t)))
            {
                const BPNode_AddAuthReportToEidCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddAuthReportToEidCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddAuthReportToEid(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveAuthReportToEidCmd_t)))
            {
                const BPNode_RemoveAuthReportToEidCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveAuthReportToEidCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveAuthReportToEid(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_LATENCY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddLatencyCmd_t)))
            {
                const BPNode_AddLatencyCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddLatencyCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddLatency(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_LATENCY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveLatencyCmd_t)))
            {
                const BPNode_RemoveLatencyCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveLatencyCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveLatency(MsgPtr->Payload);
            }
            break;

        case BPNODE_CONTACT_SETUP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactSetupCmd_t)))
            {
                const BPNode_ContactSetupCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ContactSetupCmd_t*) SBBufPtr;

                Status = BPLib_NC_ContactSetup(MsgPtr->Payload);
            }
            break;

        case BPNODE_CONTACT_START_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStartCmd_t)))
            {
                const BPNode_ContactStartCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ContactStartCmd_t*) SBBufPtr;

                Status = BPLib_NC_ContactStart(MsgPtr->Payload);
            }
            break;

        case BPNODE_CONTACT_STOP_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactStopCmd_t)))
            {
                const BPNode_ContactStopCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ContactStopCmd_t*) SBBufPtr;

                Status = BPLib_NC_ContactStop(MsgPtr->Payload);
            }
            break;

        case BPNODE_CONTACT_TEARDOWN_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ContactTeardownCmd_t)))
            {
                const BPNode_ContactTeardownCmd_t* MsgPtr;
                MsgPtr = (const BPNode_ContactTeardownCmd_t*) SBBufPtr;

                Status = BPLib_NC_ContactTeardown(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_MIB_ARRAY_KEY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddMibArrayKeyCmd_t)))
            {
                const BPNode_AddMibArrayKeyCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddMibArrayKeyCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddMibArrayKey(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_MIB_ARRAY_KEY_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveMibArrayKeyCmd_t)))
            {
                const BPNode_RemoveMibArrayKeyCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveMibArrayKeyCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveMibArrayKey(MsgPtr->Payload);
            }
            break;

        case BPNODE_SET_MIB_ITEM_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SetMibItemCmd_t)))
            {
                const BPNode_SetMibItemCmd_t* MsgPtr;
                MsgPtr = (const BPNode_SetMibItemCmd_t*) SBBufPtr;

                Status = BPLib_NC_SetMibItem(MsgPtr->Payload);
            }
            break;

        case BPNODE_ADD_STORAGE_ALLOCATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_AddStorageAllocationCmd_t)))
            {
                const BPNode_AddStorageAllocationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_AddStorageAllocationCmd_t*) SBBufPtr;

                Status = BPLib_NC_AddStorageAllocation(MsgPtr->Payload);
            }
            break;

        case BPNODE_REMOVE_STORAGE_ALLOCATION_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_RemoveStorageAllocationCmd_t)))
            {
                const BPNode_RemoveStorageAllocationCmd_t* MsgPtr;
                MsgPtr = (const BPNode_RemoveStorageAllocationCmd_t*) SBBufPtr;

                Status = BPLib_NC_RemoveStorageAllocation(MsgPtr->Payload);
            }
            break;

        case BPNODE_PERFORM_SELF_TEST_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_PerformSelfTestCmd_t)))
            {
                Status = BPLib_NC_PerformSelfTest();
            }
            break;

        case BPNODE_SEND_NODE_MIB_CONFIG_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibConfigHkCmd_t)))
            {
                Status = BPLib_NC_SendNodeMibConfigHk();
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibConfigHkCmd_t)))
            {
                Status = BPLib_NC_SendSourceMibConfigHk();
            }
            break;

        case BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendNodeMibCountersHkCmd_t)))
            {
                uint32_t ADU_Delivered;
                uint32_t ADU_Received;
                uint8 i;

                /* Reset ADU counters so += can be used in a loop*/
                ADU_Delivered = 0;
                ADU_Received  = 0;

                /* Get ADU counts for all ADU child tasks */
                for(i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
                {
                    ADU_Delivered += BPNode_AppData.AduOutData[i].AduCountDelivered;
                    ADU_Received  += BPNode_AppData.AduInData[i].AduCountReceived;
                }

                /* Set the node's ADUs delivered counter to the new value */
                Status = BPLib_AS_Set(0, ADU_COUNT_DELIVERED, ADU_Delivered);

                if (Status == BPLIB_SUCCESS)
                { /* ADUs delivered counter was successfully set*/

                    /* Set the node's ADUs received to the new value */
                    Status = BPLib_AS_Set(0, ADU_COUNT_RECEIVED, ADU_Received);

                    if (Status == BPLIB_SUCCESS)
                    { /* ADUs received counter was successfully set */

                        /* Send the node MIB counters HK */
                        BPLib_NC_SendNodeMibCountersHk();
                    }
                    else
                    {
                        /*
                        ** The call to BPLib_AS_Set() is so controlled that it seems unlikely that an error
                        ** will occur from its use. This is also very difficult to test from a code coverage
                        ** standpoint but an indication of an error is still needed in case the worst happens
                        */

                        BPLib_EM_SendEvent(BPNODE_DP_SEND_NODE_CNTRS_ERR_EID,
                                            BPLib_EM_EventType_ERROR,
                                            "Can't send node MIB counters; error setting ADUs received counter, RC = %d",
                                            Status);
                    }
                }
                else
                {
                    /*
                    ** The call to BPLib_AS_Set() is so controlled that it seems unlikely that an error
                    ** will occur from its use. This is also very difficult to test from a code coverage
                    ** standpoint but an indication of an error is still needed in case the worst happens
                    */

                    BPLib_EM_SendEvent(BPNODE_DP_SEND_NODE_CNTRS_ERR_EID,
                                        BPLib_EM_EventType_ERROR,
                                        "Can't send node MIB counters; error setting ADUs delivered counter, RC = %d",
                                        Status);
                }
            }
            break;

        case BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendSourceMibCountersHkCmd_t)))
            {
                Status = BPLib_NC_SendSourceMibCountersHk();
            }
            break;

        case BPNODE_SEND_STORAGE_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendStorageHkCmd_t)))
            {
                Status = BPLib_NC_SendStorageHk();
            }
            break;

        case BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC:
            if (BPA_DP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendChannelContactStatHkCmd_t)))
            {
                uint8 i;

                /* Get ADU status from all child tasks */
                for(i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
                {
                    BPLib_AS_ChannelContactStatsPayload.ChannelStatus[i].Status = BPNode_AppData.AduState[i].AppState;
                }

                Status = BPLib_NC_SendChannelContactStatHk();

                if (Status != BPLIB_SUCCESS)
                {
                    /* Failed to send channel contact stats HK */
                }
            }
            break;

        /* Default case already found during FC vs length test */
        default:
            Status = BPLIB_ERROR;

            BPLib_EM_SendEvent(BPNODE_CC_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Invalid ground command code: CC = %d", CommandCode);
            break;
    }

    if (Status == BPLIB_SUCCESS)
    {
        BPLib_AS_Increment(0, BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT);
    }
    else if (Status != BPLIB_UNKNOWN)
    {
        BPLib_AS_Increment(0, BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT);
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
            BPNode_AppData.NodeMibCountersHkTlm.Payload.BundleAgentRejectedDirectiveCount++;

            BPLib_EM_SendEvent(BPNODE_MID_ERR_EID, BPLib_EM_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x",
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}