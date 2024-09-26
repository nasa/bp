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

#include "bpnode_msgids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"
#include "bpnode_msg.h"

#include "bplib.h"
#include "bplib_eventids.h"
#include "fwp_dp.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Noop command */
BPLib_Status_t BPA_DP_NoopCmd()
{
    char VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];
    char LastOfficialRelease[BPNODE_CFG_MAX_VERSION_STR_LEN];
    BPLib_Status_t Status;

    Status = BPLib_NC_NoopCmd();

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    (void) snprintf(LastOfficialRelease, BPNODE_CFG_MAX_VERSION_STR_LEN, "v%u.%u.%u",
        BPNODE_MAJOR_VERSION,
        BPNODE_MINOR_VERSION,
        BPNODE_REVISION);

    CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode",
                                BPNODE_VERSION, BPNODE_BUILD_CODENAME, LastOfficialRelease);

    BPLib_EM_SendEvent(BPLIB_NOOP_EID, BPLib_EM_EventType_INFORMATION,
                        "No-op command. %s", VersionString);

    return Status;
}

/* Add all applications command */
BPLib_Status_t BPA_DP_AddAllApplicationsCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAllApplicationsCmd();

    return Status;
}

/* Start all applications command */
BPLib_Status_t BPA_DP_StartAllApplicationsCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartAllApplicationsCmd();

    return Status;
}

/* Verify bundle storage command */
BPLib_Status_t BPA_DP_VerifyBundleStorageCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleStorageCmd();

    return Status;
}

/* Initialize bundle storage command */
BPLib_Status_t BPA_DP_InitBundleStorageCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_InitBundleStorageCmd();

    return Status;
}

/* Verify bundle metadata (7.2) command */
BPLib_Status_t BPA_DP_VerifyBundleMetadataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleMetadataCmd();

    return Status;
}

/* Rebuild bundle metadata command */
BPLib_Status_t BPA_DP_RebuildBundleMetadataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RebuildBundleMetadataCmd();

    return Status;
}

/* Clear volatile command */
BPLib_Status_t BPA_DP_ClearVolatileCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ClearVolatileCmd();

    return Status;
}

/* Reload saved data (7.2) command */
BPLib_Status_t BPA_DP_ReloadSavedDataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ReloadSavedDataCmd();

    return Status;
}

/* Reset all counters command */
BPLib_Status_t BPA_DP_ResetAllCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetAllCountersCmd();

    return Status;
}

/* Reset counter command */
BPLib_Status_t BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetCounterCmd(Msg->Payload);

    return Status;
}

/* Reset source counters command */
BPLib_Status_t BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetSourceCountersCmd(Msg->Payload);

    return Status;
}

/* Reset bundle counters command */
BPLib_Status_t BPA_DP_ResetBundleCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetBundleCountersCmd();

    return Status;
}

/* Reset error counters command */
BPLib_Status_t BPA_DP_ResetErrorCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetErrorCountersCmd();

    return Status;
}

/* Add application command */
BPLib_Status_t BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddApplicationCmd(Msg->Payload);

    return Status;
}

/* Remove application command */
BPLib_Status_t BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveApplicationCmd(Msg->Payload);

    return Status;
}

/* Set registration state command */
BPLib_Status_t BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetRegistrationStateCmd(Msg->Payload);

    return Status;
}

/* Start application command */
BPLib_Status_t BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartApplicationCmd(Msg->Payload);

    return Status;
}

/* Stop application command */
BPLib_Status_t BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StopApplicationCmd(Msg->Payload);

    return Status;
}

/* Add authorized sources command */
BPLib_Status_t BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthSourcesCmd(Msg->Payload);

    return Status;
}

/* Remove authorized sources command */
BPLib_Status_t BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthSourcesCmd(Msg->Payload);

    return Status;
}

/* Add authorized custody sources command */
BPLib_Status_t BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodySourcesCmd(Msg->Payload);

    return Status;
}

/* Remove authorized custody sources command */
BPLib_Status_t BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodySourcesCmd(Msg->Payload);

    return Status;
}

/* Add authorized custodians command */
BPLib_Status_t BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodiansCmd(Msg->Payload);

    return Status;
}

/* Remove authorized custodians command */
BPLib_Status_t BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodiansCmd(Msg->Payload);

    return Status;
}

/* Add authorized report-to eid command */
BPLib_Status_t BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthReportToEidCmd(Msg->Payload);

    return Status;
}

/* Remove authorized report-to eid command */
BPLib_Status_t BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthReportToEidCmd(Msg->Payload);

    return Status;
}

/* Add latency command */
BPLib_Status_t BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddLatencyCmd(Msg->Payload);

    return Status;
}

/* Remove latency command */
BPLib_Status_t BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveLatencyCmd(Msg->Payload);

    return Status;
}

/* Contact setup command */
BPLib_Status_t BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactSetupCmd(Msg->Payload);

    return Status;
}

/* Contact start command */
BPLib_Status_t BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStartCmd(Msg->Payload);

    return Status;
}

/* Contact stop command */
BPLib_Status_t BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStopCmd(Msg->Payload);

    return Status;
}

/* Contact teardown command */
BPLib_Status_t BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactTeardownCmd(Msg->Payload);

    return Status;
}

/* Add mib array key command */
BPLib_Status_t BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddMibArrayKeyCmd(Msg->Payload);

    return Status;
}

/* Remove mib array key command */
BPLib_Status_t BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveMibArrayKeyCmd(Msg->Payload);

    return Status;
}

/* Set mib item command */
BPLib_Status_t BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetMibItemCmd(Msg->Payload);

    return Status;
}

/* Add storage allocation command */
BPLib_Status_t BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddStorageAllocationCmd(Msg->Payload);

    return Status;
}

/* Remove storage allocation command */
BPLib_Status_t BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveStorageAllocationCmd(Msg->Payload);

    return Status;
}

/* Perform self test (7.2) command */
BPLib_Status_t BPA_DP_PerformSelfTestCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_PerformSelfTestCmd();

    return Status;
}

/* Send node mib config hk command */
BPLib_Status_t BPA_DP_SendNodeMibConfigHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendNodeMibConfigHkCmd();

    return Status;
}

/* Send per-source mib config hk command */
BPLib_Status_t BPA_DP_SendSourceMibConfigHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibConfigHkCmd();

    return Status;
}

/* Send Node MIB Counters housekeeping command */
BPLib_Status_t BPA_DP_SendNodeMibCountersHkCmd()
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.NodeMibCountersHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader), true);

    Status = BPLib_NC_SendNodeMibCountersHkCmd();

    return Status;
}

/* Send per-source mib counter hk command */
BPLib_Status_t BPA_DP_SendSourceMibCountersHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibCountersHkCmd();

    return Status;
}

/* Send storage hk command */
BPLib_Status_t BPA_DP_SendStorageHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendStorageHkCmd();

    return Status;
}

/* Send channel/contact status hk command */
BPLib_Status_t BPA_DP_SendChannelContactStatHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendChannelContactStatHkCmd();

    return Status;
}
