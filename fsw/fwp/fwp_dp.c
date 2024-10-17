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

    Status = BPLib_NC_StartAllApplicationsCmd();

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

    Status = BPLib_NC_VerifyBundleStorageCmd();

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

    Status = BPLib_NC_InitBundleStorageCmd();

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

    Status = BPLib_NC_VerifyBundleMetadataCmd();

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

    Status = BPLib_NC_RebuildBundleMetadataCmd();

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

    Status = BPLib_NC_ClearVolatileCmd();

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

    Status = BPLib_NC_ReloadSavedDataCmd();

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

    BPLib_NC_ResetAllCountersCmd();
}

/* Reset counter command */
void BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    BPLib_Status_t Status;

    Status = BPLib_NC_ResetCounterCmd(Msg->Payload);

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

    Status = BPLib_NC_ResetSourceCountersCmd(Msg->Payload);

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

    Status = BPLib_NC_ResetBundleCountersCmd();

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

    Status = BPLib_NC_ResetErrorCountersCmd();

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

    Status = BPLib_NC_AddApplicationCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveApplicationCmd(Msg->Payload);

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

    Status = BPLib_NC_SetRegistrationStateCmd(Msg->Payload);

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

    Status = BPLib_NC_StartApplicationCmd(Msg->Payload);

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

    Status = BPLib_NC_StopApplicationCmd(Msg->Payload);

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

    Status = BPLib_NC_AddAuthSourcesCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveAuthSourcesCmd(Msg->Payload);

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

    Status = BPLib_NC_AddAuthCustodySourcesCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveAuthCustodySourcesCmd(Msg->Payload);

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

    Status = BPLib_NC_AddAuthCustodiansCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveAuthCustodiansCmd(Msg->Payload);

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

    Status = BPLib_NC_AddAuthReportToEidCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveAuthReportToEidCmd(Msg->Payload);

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

    Status = BPLib_NC_AddLatencyCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveLatencyCmd(Msg->Payload);

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

    Status = BPLib_NC_ContactSetupCmd(Msg->Payload);

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

    Status = BPLib_NC_ContactStartCmd(Msg->Payload);

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

    Status = BPLib_NC_ContactStopCmd(Msg->Payload);

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

    Status = BPLib_NC_ContactTeardownCmd(Msg->Payload);

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

    Status = BPLib_NC_AddMibArrayKeyCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveMibArrayKeyCmd(Msg->Payload);

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

    Status = BPLib_NC_SetMibItemCmd(Msg->Payload);

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

    Status = BPLib_NC_AddStorageAllocationCmd(Msg->Payload);

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

    Status = BPLib_NC_RemoveStorageAllocationCmd(Msg->Payload);

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

    Status = BPLib_NC_PerformSelfTestCmd();

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

    Status = BPLib_NC_SendNodeMibConfigHkCmd();

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

    Status = BPLib_NC_SendSourceMibConfigHkCmd();

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
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
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

    Status = BPLib_NC_SendStorageHkCmd();

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
    }
    else
    {
        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }
}
