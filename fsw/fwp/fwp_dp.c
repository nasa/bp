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

    BPLib_NC_Noop();

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
        BPNode_AppData.ChannelContactStatHkTlm.Payload.ChannelStats[i].State = BPNode_AppData.AduState[i].AppState;
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
