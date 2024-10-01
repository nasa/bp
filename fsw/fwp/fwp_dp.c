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
CFE_Status_t BPA_DP_NoopCmd(void)
{
    char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
    char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];
    int32 Status;

    Status = BPLib_NC_NoopCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
        BPNODE_MAJOR_VERSION,
        BPNODE_MINOR_VERSION,
        BPNODE_REVISION);

    CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

    BPLib_EM_SendEvent(BPLIB_NOOP_EID, BPLib_EM_EventType_INFORMATION,
                        "No-op command. %s", VersionString);

    return (CFE_Status_t)Status;
}

/* Add all applications command */
CFE_Status_t BPA_DP_AddAllApplicationsCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAllApplicationsCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Start all applications command */
CFE_Status_t BPA_DP_StartAllApplicationsCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartAllApplicationsCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Verify bundle storage command */
CFE_Status_t BPA_DP_VerifyBundleStorageCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleStorageCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Initialize bundle storage command */
CFE_Status_t BPA_DP_InitBundleStorageCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_InitBundleStorageCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Verify bundle metadata (7.2) command */
CFE_Status_t BPA_DP_VerifyBundleMetadataCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleMetadataCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Rebuild bundle metadata command */
CFE_Status_t BPA_DP_RebuildBundleMetadataCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RebuildBundleMetadataCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Clear volatile command */
CFE_Status_t BPA_DP_ClearVolatileCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ClearVolatileCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reload saved data (7.2) command */
CFE_Status_t BPA_DP_ReloadSavedDataCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ReloadSavedDataCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reset all counters command */
CFE_Status_t BPA_DP_ResetAllCountersCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetAllCountersCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reset counter command */
CFE_Status_t BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetCounterCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reset source counters command */
CFE_Status_t BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetSourceCountersCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reset bundle counters command */
CFE_Status_t BPA_DP_ResetBundleCountersCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetBundleCountersCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Reset error counters command */
CFE_Status_t BPA_DP_ResetErrorCountersCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetErrorCountersCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add application command */
CFE_Status_t BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddApplicationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove application command */
CFE_Status_t BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveApplicationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Set registration state command */
CFE_Status_t BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetRegistrationStateCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Start application command */
CFE_Status_t BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartApplicationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Stop application command */
CFE_Status_t BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StopApplicationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add authorized sources command */
CFE_Status_t BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthSourcesCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove authorized sources command */
CFE_Status_t BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthSourcesCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add authorized custody sources command */
CFE_Status_t BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodySourcesCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove authorized custody sources command */
CFE_Status_t BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodySourcesCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add authorized custodians command */
CFE_Status_t BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodiansCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove authorized custodians command */
CFE_Status_t BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodiansCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add authorized report-to eid command */
CFE_Status_t BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthReportToEidCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove authorized report-to eid command */
CFE_Status_t BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthReportToEidCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add latency command */
CFE_Status_t BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddLatencyCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove latency command */
CFE_Status_t BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveLatencyCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Contact setup command */
CFE_Status_t BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactSetupCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Contact start command */
CFE_Status_t BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStartCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Contact stop command */
CFE_Status_t BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStopCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Contact teardown command */
CFE_Status_t BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactTeardownCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add mib array key command */
CFE_Status_t BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddMibArrayKeyCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove mib array key command */
CFE_Status_t BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveMibArrayKeyCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Set mib item command */
CFE_Status_t BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetMibItemCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Add storage allocation command */
CFE_Status_t BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddStorageAllocationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Remove storage allocation command */
CFE_Status_t BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveStorageAllocationCmd(Msg->Payload);

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Perform self test (7.2) command */
CFE_Status_t BPA_DP_PerformSelfTestCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_PerformSelfTestCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send node mib config hk command */
CFE_Status_t BPA_DP_SendNodeMibConfigHkCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendNodeMibConfigHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send per-source mib config hk command */
CFE_Status_t BPA_DP_SendSourceMibConfigHkCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibConfigHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send Node MIB Counters housekeeping command */
CFE_Status_t BPA_DP_SendNodeMibCountersHkCmd(void)
{
    int32 Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.NodeMibCountersHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader), true);

    Status = BPLib_NC_SendNodeMibCountersHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send per-source mib counter hk command */
CFE_Status_t BPA_DP_SendSourceMibCountersHkCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibCountersHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send storage hk command */
CFE_Status_t BPA_DP_SendStorageHkCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendStorageHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}

/* Send channel/contact status hk command */
CFE_Status_t BPA_DP_SendChannelContactStatHkCmd(void)
{
    int32 Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendChannelContactStatHkCmd();

    switch (Status)
    {
        case BPLIB_SUCCESS:
            Status = CFE_SUCCESS;
            break;
        default:
            Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return (CFE_Status_t)Status;
}
