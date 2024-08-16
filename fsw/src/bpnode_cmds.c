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


/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_cmds.h"
#include "bpnode_msgids.h"
#include "bpnode_eventids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"
#include "bpnode_msg.h"

#include "bplib.h"


/*
** Function Definitions
*/

/* Noop command */
CFE_Status_t BPNode_NoopCmd(const BPNode_NoopCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, 
                        "No-op command. Version %d.%d.%d.",
                        BPNODE_MAJOR_VERSION, BPNODE_MINOR_VERSION, BPNODE_REVISION);

    return CFE_SUCCESS;
}

/* Add all applications command */
CFE_Status_t BPNode_AddAllApplicationsCmd(const BPNode_AddAllApplicationsCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add all applications command not implemented");

    return CFE_SUCCESS;
}

/* Start all applications command */
CFE_Status_t BPNode_StartAllApplicationsCmd(const BPNode_StartAllApplicationsCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Start all applications command not implemented");

    return CFE_SUCCESS;
}

/* Verify bundle storage command */
CFE_Status_t BPNode_VerifyBundleStorageCmd(const BPNode_VerifyBundleStorageCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Verify bundle storage command not implemented");

    return CFE_SUCCESS;
}

/* Initialize bundle storage command */
CFE_Status_t BPNode_InitBundleStorageCmd(const BPNode_InitBundleStorageCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Initialize bundle storage command not implemented");

    return CFE_SUCCESS;
}

/* Verify bundle metadata (7.2) command */
CFE_Status_t BPNode_VerifyBundleMetadataCmd(const BPNode_VerifyBundleMetadataCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Verify bundle metadata (7.2) command not implemented");

    return CFE_SUCCESS;
}

/* Rebuild bundle metadata command */
CFE_Status_t BPNode_RebuildBundleMetadataCmd(const BPNode_RebuildBundleMetadataCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Rebuild bundle metadata command not implemented");

    return CFE_SUCCESS;
}

/* Clear volatile command */
CFE_Status_t BPNode_ClearVolatileCmd(const BPNode_ClearVolatileCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Clear volatile command not implemented");

    return CFE_SUCCESS;
}

/* Reload saved data (7.2) command */
CFE_Status_t BPNode_ReloadSavedDataCmd(const BPNode_ReloadSavedDataCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Reload saved data (7.2) command not implemented");

    return CFE_SUCCESS;
}

/* Reset all counters command */
CFE_Status_t BPNode_ResetAllCountersCmd(const BPNode_ResetAllCountersCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount = 0;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, 
                    "Reset all counters command");

    return CFE_SUCCESS;
}

/* Reset counter command */
CFE_Status_t BPNode_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Reset counter command not implemented");

    return CFE_SUCCESS;
}

/* Reset source counters command */
CFE_Status_t BPNode_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Reset source counters command not implemented");

    return CFE_SUCCESS;
}

/* Reset bundle counters command */
CFE_Status_t BPNode_ResetBundleCountersCmd(const BPNode_ResetBundleCountersCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Reset bundle counters command not implemented");

    return CFE_SUCCESS;
}

/* Reset error counters command */
CFE_Status_t BPNode_ResetErrorCountersCmd(const BPNode_ResetErrorCountersCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Reset error counters command not implemented");

    return CFE_SUCCESS;
}

/* Add application command */
CFE_Status_t BPNode_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add application command not implemented");

    return CFE_SUCCESS;
}

/* Remove application command */
CFE_Status_t BPNode_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove application command not implemented");

    return CFE_SUCCESS;
}

/* Set registration state command */
CFE_Status_t BPNode_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Set registration state command not implemented");

    return CFE_SUCCESS;
}

/* Start application command */
CFE_Status_t BPNode_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Start application command not implemented");

    return CFE_SUCCESS;
}

/* Stop application command */
CFE_Status_t BPNode_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Stop application command not implemented");

    return CFE_SUCCESS;
}

/* Add authorized sources command */
CFE_Status_t BPNode_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add authorized sources command not implemented");

    return CFE_SUCCESS;
}

/* Remove authorized sources command */
CFE_Status_t BPNode_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove authorized sources command not implemented");

    return CFE_SUCCESS;
}

/* Add authorized custody sources command */
CFE_Status_t BPNode_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add authorized custody sources command not implemented");

    return CFE_SUCCESS;
}

/* Remove authorized custody sources command */
CFE_Status_t BPNode_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove authorized custody sources command not implemented");

    return CFE_SUCCESS;
}

/* Add authorized custodians command */
CFE_Status_t BPNode_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add authorized custodians command not implemented");

    return CFE_SUCCESS;
}

/* Remove authorized custodians command */
CFE_Status_t BPNode_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove authorized custodians command not implemented");

    return CFE_SUCCESS;
}

/* Add authorized report-to eid command */
CFE_Status_t BPNode_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add authorized report-to eid command not implemented");

    return CFE_SUCCESS;
}

/* Remove authorized report-to eid command */
CFE_Status_t BPNode_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove authorized report-to eid command not implemented");

    return CFE_SUCCESS;
}

/* Add latency command */
CFE_Status_t BPNode_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add latency command not implemented");

    return CFE_SUCCESS;
}

/* Remove latency command */
CFE_Status_t BPNode_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove latency command not implemented");

    return CFE_SUCCESS;
}

/* Contact setup command */
CFE_Status_t BPNode_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Contact setup command not implemented");

    return CFE_SUCCESS;
}

/* Contact start command */
CFE_Status_t BPNode_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Contact start command not implemented");

    return CFE_SUCCESS;
}

/* Contact stop command */
CFE_Status_t BPNode_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Contact stop command not implemented");

    return CFE_SUCCESS;
}

/* Contact teardown command */
CFE_Status_t BPNode_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Contact teardown command not implemented");

    return CFE_SUCCESS;
}

/* Add mib array key command */
CFE_Status_t BPNode_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add mib array key command not implemented");

    return CFE_SUCCESS;
}

/* Remove mib array key command */
CFE_Status_t BPNode_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove mib array key command not implemented");

    return CFE_SUCCESS;
}

/* Set mib item command */
CFE_Status_t BPNode_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Set mib item command not implemented");

    return CFE_SUCCESS;
}

/* Add storage allocation command */
CFE_Status_t BPNode_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Add storage allocation command not implemented");

    return CFE_SUCCESS;
}

/* Remove storage allocation command */
CFE_Status_t BPNode_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Remove storage allocation command not implemented");

    return CFE_SUCCESS;
}

/* Perform self test (7.2) command */
CFE_Status_t BPNode_PerformSelfTestCmd(const BPNode_PerformSelfTestCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Perform self test (7.2) command not implemented");

    return CFE_SUCCESS;
}

/* Send node mib config hk command */
CFE_Status_t BPNode_SendNodeMibConfigHkCmd(const BPNode_SendNodeMibConfigHkCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Send node mib config hk command not implemented");

    return CFE_SUCCESS;
}

/* Send per-source mib config hk command */
CFE_Status_t BPNode_SendSourceMibConfigHkCmd(const BPNode_SendSourceMibConfigHkCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Send per-source mib config hk command not implemented");

    return CFE_SUCCESS;
}

/* Send Node MIB Counters housekeeping command */
CFE_Status_t BPNode_SendNodeMibCountersHkCmd(const BPNode_SendNodeMibCountersHkCmd_t *Msg)
{
    // Don't increment the counter, at least for testing.
    // BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader), true);

    return CFE_SUCCESS;
}

/* Send per-source mib counter hk command */
CFE_Status_t BPNode_SendSourceMibCountersHkCmd(const BPNode_SendSourceMibCountersHkCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Send per-source mib counter hk command not implemented");

    return CFE_SUCCESS;
}

/* Send storage hk command */
CFE_Status_t BPNode_SendStorageHkCmd(const BPNode_SendStorageHkCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Send storage hk command not implemented");

    return CFE_SUCCESS;
}

/* Send channel/contact status hk command */
CFE_Status_t BPNode_SendChannelContactStatusHk(const BPNode_SendChannelContactStatusHk_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION,
                    "Send channel/contact status hk command not implemented");

    return CFE_SUCCESS;
}
