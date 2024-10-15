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

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Noop command */
void BPA_DP_NoopCmd(void)
{
    char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
    char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];

    BPLib_NC_NoopCmd();

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
        BPNODE_MAJOR_VERSION,
        BPNODE_MINOR_VERSION,
        BPNODE_REVISION);

    CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

    BPLib_EM_SendEvent(BPLIB_NOOP_EID, BPLib_EM_EventType_INFORMATION,
                        "No-op command. %s", VersionString);
}

/* Add all applications command */
void BPA_DP_AddAllApplicationsCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAllApplicationsCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_ALL_APPS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add all applications command is unimplemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_ALL_APPS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add all applications command is unimplemented");
    }
}

/* Start all applications command */
void BPA_DP_StartAllApplicationsCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StartAllApplicationsCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_START_ALL_APPS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Start all applications command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_START_ALL_APPS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Start all applications command not implemented");
    }
}

/* Verify bundle storage command */
void BPA_DP_VerifyBundleStorageCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_VerifyBundleStorageCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_VERIF_BNDL_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Verify bundle storage command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_VERIF_BNDL_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Verify bundle storage command not implemented");
    }
}

/* Initialize bundle storage command */
void BPA_DP_InitBundleStorageCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_InitBundleStorageCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_INIT_BNDL_STOR_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Initialize bundle storage command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_INIT_BNDL_STOR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Initialize bundle storage command not implemented");
    }
}

/* Verify bundle metadata (7.2) command */
void BPA_DP_VerifyBundleMetadataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_VerifyBundleMetadataCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_VERIF_BNDL_META_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Verify bundle metadata (7.2) command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_VERIF_BNDL_META_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Verify bundle metadata (7.2) command not implemented");
    }
}

/* Rebuild bundle metadata command */
void BPA_DP_RebuildBundleMetadataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RebuildBundleMetadataCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_REBUILD_BNDL_META_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Rebuild bundle metadata command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_REBUILD_BNDL_META_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Rebuild bundle metadata command not implemented");
    }
}

/* Clear volatile command */
void BPA_DP_ClearVolatileCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ClearVolatileCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CLR_VOLATILE_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Clear volatile command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
        
        BPLib_EM_SendEvent(BPLIB_CLR_VOLATILE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Clear volatile command not implemented");
    }
}

/* Reload saved data (7.2) command */
void BPA_DP_ReloadSavedDataCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ReloadSavedDataCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RELOAD_SVD_DATA_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Reload saved data (7.2) command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RELOAD_SVD_DATA_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Reload saved data (7.2) command not implemented");
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

    BPLib_NC_ResetAllCountersCmd();

    BPLib_EM_SendEvent(BPLIB_RESET_ALL_CTRS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION, 
                        "Reset all counters command");
}

/* Reset counter command */
void BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetCounterCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_CTR_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Reset counter command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_CTR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Reset counter command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Reset source counters command */
void BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetSourceCountersCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_SRC_CTRS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Reset source counters command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_SRC_CTRS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Reset source counters command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Reset bundle counters command */
void BPA_DP_ResetBundleCountersCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetBundleCountersCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_BNDL_CTRS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Reset bundle counters command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_BNDL_CTRS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Reset bundle counters command not implemented");
    }
}

/* Reset error counters command */
void BPA_DP_ResetErrorCountersCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetErrorCountersCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_ERR_CTRS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Reset error counters command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RESET_ERR_CTRS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Reset error counters command not implemented");
    }
}

/* Add application command */
void BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddApplicationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_APP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Successful add-application directive for ChanId=%d",
                            Msg->Payload.ChanId);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        switch (Status)
        {
        case BPLIB_ADU_ADD_CHAN_ERR:
            BPLib_EM_SendEvent(BPLIB_ADU_ADD_CHAN_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error with add-application directive, invalid ChanId=%d",
                                Msg->Payload.ChanId);

            break;
        case BPLIB_ADU_ADD_STAT_ERR:
            BPLib_EM_SendEvent(BPLIB_ADU_ADD_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error with add-application directive, invalid AppState=%d for ChanId=%d", 
                                BPNode_AppData.AduState[Msg->Payload.ChanId].AppState,
                                Msg->Payload.ChanId);

            break;
        default:
            break;
        }
    }
}

/* Remove application command */
void BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveApplicationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_APP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove application command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_APP_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove application command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Set registration state command */
void BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SetRegistrationStateCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SET_REGI_STAT_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Set registration state command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SET_REGI_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Set registration state command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Start application command */
void BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StartApplicationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_START_APP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Successful start-application directive for ChanId=%d",
                            Msg->Payload.ChanId);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        switch (Status)
        {
            case BPLIB_ADU_START_CHAN_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_START_CHAN_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error with start-application directive, invalid ChanId=%d",
                                    Msg->Payload.ChanId);

                break;
            case BPLIB_ADU_START_STAT_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_START_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error with start-application directive, invalid AppState=%d for ChanId=%d", 
                                    BPNode_AppData.AduState[Msg->Payload.ChanId].AppState,
                                    Msg->Payload.ChanId);

                break;
            case BPLIB_ADU_START_SUB_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_START_SUB_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error subscribing to ADU on channel #%d",
                                    Msg->Payload.ChanId);

                break;
            default:
                break;
        }
    }
}

/* Stop application command */
void BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_StopApplicationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_START_APP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Successful stop-application directive for ChanId=%d",
                            Msg->Payload.ChanId);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        switch (Status)
        {
            case BPLIB_ADU_STOP_CHAN_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_STOP_CHAN_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error with stop-application directive, invalid ChanId=%d",
                                    Msg->Payload.ChanId);

                break;
            case BPLIB_ADU_STOP_STAT_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_STOP_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error with stop-application directive, invalid AppState=%d for ChanId=%d", 
                                    BPNode_AppData.AduState[Msg->Payload.ChanId].AppState,
                                    Msg->Payload.ChanId);

                break;
            case BPLIB_ADU_STOP_UNSUB_ERR:
                BPLib_EM_SendEvent(BPLIB_ADU_UNSUB_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error unsubscribing from ADU on channel #%d", 
                                    Msg->Payload.ChanId);

                break;
            default:
                break;
        }
    }
}

/* Add authorized sources command */
void BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthSourcesCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_SRCS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add authorized sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_SRCS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add authorized sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove authorized sources command */
void BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthSourcesCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_SRCS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove authorized sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_SRCS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove authorized sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Add authorized custody sources command */
void BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthCustodySourcesCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_CUST_SRCS_SUCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add authorized custody sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_CUST_SRCS_SUCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add authorized custody sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove authorized custody sources command */
void BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthCustodySourcesCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_CUST_SRCS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove authorized custody sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_CUST_SRCS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove authorized custody sources command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Add authorized custodians command */
void BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthCustodiansCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_CUSTODIANS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add authorized custodians command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_CUSTODIANS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add authorized custodians command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove authorized custodians command */
void BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthCustodiansCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_CUSTODIANS_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove authorized custodians command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_CUSTODIANS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove authorized custodians command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Add authorized report-to eid command */
void BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddAuthReportToEidCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_RPT_EID_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add authorized report-to eid command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_AUTH_RPT_EID_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add authorized report-to eid command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove authorized report-to eid command */
void BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveAuthReportToEidCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_RPT_EID_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove authorized report-to eid command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_AUTH_RPT_EID_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove authorized report-to eid command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Add latency command */
void BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddLatencyCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_LATENCY_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add latency command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_LATENCY_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add latency command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove latency command */
void BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveLatencyCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_LATENCY_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove latency command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_LATENCY_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove latency command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Contact setup command */
void BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactSetupCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_SETUP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Contact setup command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_SETUP_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Contact setup command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Contact start command */
void BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactStartCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_START_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Contact start command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_START_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Contact start command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Contact stop command */
void BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactStopCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_STOP_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Contact stop command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_STOP_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Contact stop command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Contact teardown command */
void BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ContactTeardownCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_TEARDOWN_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Contact teardown command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_CONTACT_TEARDOWN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Contact teardown command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Add mib array key command */
void BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddMibArrayKeyCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_MIB_ARR_KEY_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add mib array key command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_MIB_ARR_KEY_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Add mib array key command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Remove mib array key command */
void BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveMibArrayKeyCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_MIB_ARR_KEY_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove mib array key command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_MIB_ARR_KEY_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove mib array key command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Set mib item command */
void BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SetMibItemCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SET_MIB_ITEM_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Set mib item command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        switch (Status)
        {
            case BPLIB_NC_INVALID_MIB_ITEM_INDEX:
                // BPLib_EM_SendEvent(BPLIB_SET_MIB_ITEM_INVALID_INDEX_ERR_EID, BPLib_EM_EventType_ERROR,
                //                     "Given index (%d) was out of bounds, expected value in range [0, %d]",
                //                     Msg->Payload.Index, MAX_MIB_ARR_SIZE);

                break;
            case BPLIB_NC_INVALID_MID_VALUE:
                // BPLib_EM_SendEvent(BPLIB_SET_MID_ITEM_INVALID_VALUE_ERR_EID,
                //                     BPLib_EM_EventType_ERROR,
                //                     "Given MIB value (%d) was invalid, expected value in range [%d, %d]",
                //                     Msg->Payload.Value, MAX_MIB_VALUE, MIN_MID_VALUE);

                break;
            // case BPLIB_TABLE_UPDATE_ERR:
            //     BPLib_EM_SendEvent(BPLIB_SET_MIB_ITEM_TBL_UPDATE_FAIL, BPLib_EM_EventType_ERROR,
            //                         "Failed to update the MIB configuration")

            //     break;
            default:
                break;
        }
    }
}

/* Add storage allocation command */
void BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_AddStorageAllocationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_ADD_STOR_ALLOC_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Add storage allocation command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        switch (Status)
        {
            // case BPLIB_STOR_ADD_ALLOC_ERR:
            //     BPLib_EM_SendEvent(BPLIB_ADD_STOR_ALLOC_ERR_EID,
            //                         BPLib_EM_EventType_ERROR,
            //                         "Could not update storage allocation of size %d for EID %d",
            //                         Msg->Payload.AllocSize, Msg->Payload.EID);
            //     break;
            // case BPLIB_TABLE_UPDATE_ERR:
            //     BPLib_EM_SendEvent(BPLIB_ADD_STOR_ALLOC_ERR_EID,
            //                         BPLib_EM_EventType_ERROR,
            //                         "Failed to update the MIB configuration");

            //     break;
            default:
                break;
        }
    }
}

/* Remove storage allocation command */
void BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_RemoveStorageAllocationCmd(Msg->Payload);

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_STOR_ALLOC_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Remove storage allocation command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_RM_STOR_ALLOC_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Remove storage allocation command not implemented, received %d in payload",
                            Msg->Payload.ExampleParameter);
    }
}

/* Perform self test (7.2) command */
void BPA_DP_PerformSelfTestCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_PerformSelfTestCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_PERFORM_SELF_TEST_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Perform self test (7.2) command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_PERFORM_SELF_TEST_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Perform self test (7.2) command not implemented");
    }
}

/* Send node mib config hk command */
void BPA_DP_SendNodeMibConfigHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendNodeMibConfigHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_NODE_MIB_CFG_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send node mib config hk command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_NODE_MIB_CFG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send node mib config hk command not implemented");
    }
}

/* Send per-source mib config hk command */
void BPA_DP_SendSourceMibConfigHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendSourceMibConfigHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_SRC_MIB_CFG_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send per-source mib config hk command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_SRC_MIB_CFG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send per-source mib config hk command not implemented");
    }
}

/* Send Node MIB Counters housekeeping command */
void BPA_DP_SendNodeMibCountersHkCmd(void)
{
    BPLib_Status_t Status;
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

    Status = BPLib_NC_SendNodeMibCountersHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_NODE_MIB_CTRS_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send node mib counters HK command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_NODE_MIB_CTRS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send node mib counters HK command not implemented");
    }
}

/* Send per-source mib counter hk command */
void BPA_DP_SendSourceMibCountersHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendSourceMibCountersHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_SRC_MIB_CTRS_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send per-source mib counter hk command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_SRC_MIB_CTRS_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send per-source mib counter hk command not implemented");
    }
}

/* Send storage hk command */
void BPA_DP_SendStorageHkCmd(void)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_SendStorageHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_STOR_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send storage hk command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_STOR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send storage hk command not implemented");
    }
}

/* Send channel/contact status hk command */
void BPA_DP_SendChannelContactStatHkCmd(void)
{
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    uint8 i;
    BPLib_Status_t Status;

    /* Get ADU status from all child tasks */
    for(i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        BPNode_AppData.ChannelContactStatHkTlm.Payload.ChannelStats[i].State = BPNode_AppData.AduState[i].AppState;
    }

    /* Get DTN time data */
    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.ChannelContactStatHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.ChannelContactStatHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.ChannelContactStatHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactStatHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactStatHkTlm.TelemetryHeader), true);

    Status = BPLib_NC_SendChannelContactStatHkCmd();

    if (Status == BPLIB_SUCCESS)
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_CHAN_CONTACT_STAT_HK_SUCCESS_EID, BPLib_EM_EventType_INFORMATION,
                            "Send channel/contact status hk command not implemented");
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        BPLib_EM_SendEvent(BPLIB_SEND_CHAN_CONTACT_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Send channel/contact status hk command not implemented");
    }
}
