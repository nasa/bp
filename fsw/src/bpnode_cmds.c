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

#include "bpnode_app.h"
#include "bpnode_cmds.h"
#include "bpnode_msgids.h"
#include "bpnode_eventids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"
#include "bpnode_msg.h"

#include "bplib.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

/* BPLib to cFE translate function */
CFE_Status_t BPLib_to_cFE(BPLib_Status_t BPLib_Status)
{
    CFE_Status_t cFE_Status;

    switch(BPLib_Status)
    {
        case BPLIB_SUCCESS:
            cFE_Status = CFE_SUCCESS;
            break;
        default:
            cFE_Status = CFE_STATUS_NOT_IMPLEMENTED;
            break;
    }

    return cFE_Status;
}

/* Noop command */
CFE_Status_t BPNode_NoopCmd()
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

    BPLib_EM_SendEvent(BPNODE_NOOP_INF_EID, BPLib_EM_EventType_INFORMATION, 
                        "No-op command. %s", VersionString);

    return BPLib_to_cFE(Status);;
}

/* Add all applications command */
CFE_Status_t BPNode_AddAllApplicationsCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAllApplicationsCmd();

    return BPLib_to_cFE(Status);
}

/* Start all applications command */
CFE_Status_t BPNode_StartAllApplicationsCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartAllApplicationsCmd();

    return BPLib_to_cFE(Status);
}

/* Verify bundle storage command */
CFE_Status_t BPNode_VerifyBundleStorageCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleStorageCmd();

    return BPLib_to_cFE(Status);
}

/* Initialize bundle storage command */
CFE_Status_t BPNode_InitBundleStorageCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_InitBundleStorageCmd();

    return BPLib_to_cFE(Status);
}

/* Verify bundle metadata (7.2) command */
CFE_Status_t BPNode_VerifyBundleMetadataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_VerifyBundleMetadataCmd();

    return BPLib_to_cFE(Status);
}

/* Rebuild bundle metadata command */
CFE_Status_t BPNode_RebuildBundleMetadataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RebuildBundleMetadataCmd();

    return BPLib_to_cFE(Status);
}

/* Clear volatile command */
CFE_Status_t BPNode_ClearVolatileCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ClearVolatileCmd();

    return BPLib_to_cFE(Status);
}

/* Reload saved data (7.2) command */
CFE_Status_t BPNode_ReloadSavedDataCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ReloadSavedDataCmd();

    return BPLib_to_cFE(Status);
}

/* Reset all counters command */
CFE_Status_t BPNode_ResetAllCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetAllCountersCmd();

    return BPLib_to_cFE(Status);
}

/* Reset counter command */
CFE_Status_t BPNode_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetCounterCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Reset source counters command */
CFE_Status_t BPNode_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetSourceCountersCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Reset bundle counters command */
CFE_Status_t BPNode_ResetBundleCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetBundleCountersCmd();

    return BPLib_to_cFE(Status);
}

/* Reset error counters command */
CFE_Status_t BPNode_ResetErrorCountersCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ResetErrorCountersCmd();

    return BPLib_to_cFE(Status);
}

/* Add application command */
CFE_Status_t BPNode_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddApplicationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove application command */
CFE_Status_t BPNode_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveApplicationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Set registration state command */
CFE_Status_t BPNode_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetRegistrationStateCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Start application command */
CFE_Status_t BPNode_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StartApplicationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Stop application command */
CFE_Status_t BPNode_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_StopApplicationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add authorized sources command */
CFE_Status_t BPNode_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthSourcesCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove authorized sources command */
CFE_Status_t BPNode_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthSourcesCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add authorized custody sources command */
CFE_Status_t BPNode_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodySourcesCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove authorized custody sources command */
CFE_Status_t BPNode_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodySourcesCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add authorized custodians command */
CFE_Status_t BPNode_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthCustodiansCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove authorized custodians command */
CFE_Status_t BPNode_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthCustodiansCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add authorized report-to eid command */
CFE_Status_t BPNode_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddAuthReportToEidCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove authorized report-to eid command */
CFE_Status_t BPNode_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveAuthReportToEidCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add latency command */
CFE_Status_t BPNode_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddLatencyCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove latency command */
CFE_Status_t BPNode_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveLatencyCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Contact setup command */
CFE_Status_t BPNode_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactSetupCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Contact start command */
CFE_Status_t BPNode_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStartCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Contact stop command */
CFE_Status_t BPNode_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactStopCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Contact teardown command */
CFE_Status_t BPNode_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_ContactTeardownCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add mib array key command */
CFE_Status_t BPNode_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddMibArrayKeyCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove mib array key command */
CFE_Status_t BPNode_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveMibArrayKeyCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Set mib item command */
CFE_Status_t BPNode_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SetMibItemCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Add storage allocation command */
CFE_Status_t BPNode_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_AddStorageAllocationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Remove storage allocation command */
CFE_Status_t BPNode_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_RemoveStorageAllocationCmd(Msg->Payload);

    return BPLib_to_cFE(Status);
}

/* Perform self test (7.2) command */
CFE_Status_t BPNode_PerformSelfTestCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_PerformSelfTestCmd();

    return BPLib_to_cFE(Status);
}

/* Send node mib config hk command */
CFE_Status_t BPNode_SendNodeMibConfigHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendNodeMibConfigHkCmd();

    return BPLib_to_cFE(Status);
}

/* Send per-source mib config hk command */
CFE_Status_t BPNode_SendSourceMibConfigHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibConfigHkCmd();

    return BPLib_to_cFE(Status);
}

/* Send Node MIB Counters housekeeping command */
CFE_Status_t BPNode_SendNodeMibCountersHkCmd()
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

    return BPLib_to_cFE(Status);
}

/* Send per-source mib counter hk command */
CFE_Status_t BPNode_SendSourceMibCountersHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendSourceMibCountersHkCmd();

    return BPLib_to_cFE(Status);
}

/* Send storage hk command */
CFE_Status_t BPNode_SendStorageHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendStorageHkCmd();

    return BPLib_to_cFE(Status);
}

/* Send channel/contact status hk command */
CFE_Status_t BPNode_SendChannelContactStatHkCmd()
{
    BPLib_Status_t Status;

    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    Status = BPLib_NC_SendChannelContactStatHkCmd();

    return BPLib_to_cFE(Status);
}
