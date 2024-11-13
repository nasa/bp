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
 * @file
 *   Specification for the BPNode command and telemetry
 *   message data types.
 *
 * @note
 *   Constants and enumerated types related to these message structures
 *   are defined in bpnode_msgdefs.h.
 */

#ifndef BPNODE_MSGSTRUCT_H
#define BPNODE_MSGSTRUCT_H

#include "bpnode_mission_cfg.h"
#include "bpnode_msgdefs.h"
#include "cfe_msg_hdr.h"
#include "bplib.h"

#include "bplib_nc_payloads.h"

/*
** Command type Definitions
*/

/**
 * \brief Noop command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_NoopCmd_t;

/**
 * \brief Add all applications command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_AddAllApplicationsCmd_t;

/**
 * \brief Start all applications command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_StartAllApplicationsCmd_t;

/**
 * \brief Verify bundle storage command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_VerifyBundleStorageCmd_t;

/**
 * \brief Initialize bundle storage command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_InitBundleStorageCmd_t;

/**
 * \brief Verify bundle metadata (7.2) command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_VerifyBundleMetadataCmd_t;

/**
 * \brief Rebuild bundle metadata command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_RebuildBundleMetadataCmd_t;

/**
 * \brief Clear volatile command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_ClearVolatileCmd_t;

/**
 * \brief Reload saved data (7.2) command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_ReloadSavedDataCmd_t;

/**
 * \brief Reset all counters command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_ResetAllCountersCmd_t;

/**
 * \brief Reset counter command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ResetCounter_Payload_t Payload;
} BPNode_ResetCounterCmd_t;

/**
 * \brief Reset source counters command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ResetSourceCounters_Payload_t Payload;
} BPNode_ResetSourceCountersCmd_t;

/**
 * \brief Reset bundle counters command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ResetBundleCounters_Payload_t Payload;
} BPNode_ResetBundleCountersCmd_t;

/**
 * \brief Reset error counters command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ResetErrorCounters_Payload_t Payload;
} BPNode_ResetErrorCountersCmd_t;

/**
 * \brief Add application command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddApplication_Payload_t Payload;
} BPNode_AddApplicationCmd_t;

/**
 * \brief Remove application command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveApplication_Payload_t Payload;
} BPNode_RemoveApplicationCmd_t;

/**
 * \brief Set registration state command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_SetRegistrationState_Payload_t Payload;
} BPNode_SetRegistrationStateCmd_t;

/**
 * \brief Start application command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_StartApplication_Payload_t Payload;
} BPNode_StartApplicationCmd_t;

/**
 * \brief Stop application command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_StopApplication_Payload_t Payload;
} BPNode_StopApplicationCmd_t;

/**
 * \brief Add authorized sources command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddAuthSources_Payload_t Payload;
} BPNode_AddAuthSourcesCmd_t;

/**
 * \brief Remove authorized sources command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveAuthSources_Payload_t Payload;
} BPNode_RemoveAuthSourcesCmd_t;

/**
 * \brief Add authorized custody sources command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddAuthCustodySources_Payload_t Payload;
} BPNode_AddAuthCustodySourcesCmd_t;

/**
 * \brief Remove authorized custody sources command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveAuthCustodySources_Payload_t Payload;
} BPNode_RemoveAuthCustodySourcesCmd_t;

/**
 * \brief Add authorized custodians command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddAuthCustodians_Payload_t Payload;
} BPNode_AddAuthCustodiansCmd_t;

/**
 * \brief Remove authorized custodians command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveAuthCustodians_Payload_t Payload;
} BPNode_RemoveAuthCustodiansCmd_t;

/**
 * \brief Add authorized report-to eid command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddAuthReportToEid_Payload_t Payload;
} BPNode_AddAuthReportToEidCmd_t;

/**
 * \brief Remove authorized report-to eid command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveAuthReportToEid_Payload_t Payload;
} BPNode_RemoveAuthReportToEidCmd_t;

/**
 * \brief Add latency command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddLatency_Payload_t Payload;
} BPNode_AddLatencyCmd_t;

/**
 * \brief Remove latency command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveLatency_Payload_t Payload;
} BPNode_RemoveLatencyCmd_t;

/**
 * \brief Contact setup command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ContactSetup_Payload_t Payload;
} BPNode_ContactSetupCmd_t;

/**
 * \brief Contact start command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ContactStart_Payload_t Payload;
} BPNode_ContactStartCmd_t;

/**
 * \brief Contact stop command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ContactStop_Payload_t Payload;
} BPNode_ContactStopCmd_t;

/**
 * \brief Contact teardown command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_ContactTeardown_Payload_t Payload;
} BPNode_ContactTeardownCmd_t;

/**
 * \brief Add mib array key command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddMibArrayKey_Payload_t Payload;
} BPNode_AddMibArrayKeyCmd_t;

/**
 * \brief Remove mib array key command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveMibArrayKey_Payload_t Payload;
} BPNode_RemoveMibArrayKeyCmd_t;

/**
 * \brief Set mib item command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_SetMibItem_Payload_t Payload;
} BPNode_SetMibItemCmd_t;

/**
 * \brief Add storage allocation command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_AddStorageAllocation_Payload_t Payload;
} BPNode_AddStorageAllocationCmd_t;

/**
 * \brief Remove storage allocation command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
    BPLib_RemoveStorageAllocation_Payload_t Payload;
} BPNode_RemoveStorageAllocationCmd_t;

/**
 * \brief Perform self test (7.2) command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_PerformSelfTestCmd_t;

/**
 * \brief Send node mib config hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendNodeMibConfigHkCmd_t;

/**
 * \brief Send per-source mib config hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendSourceMibConfigHkCmd_t;

/**
 * \brief Send node mib counter hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendNodeMibCountersHkCmd_t;

/**
 * \brief Send per-source mib counter hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendSourceMibCountersHkCmd_t;

/**
 * \brief Send storage hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendStorageHkCmd_t;

/**
 * \brief Send channel/contact status hk command
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendChannelContactStatHkCmd_t;


/*
** Telemetry type Definitions
*/

/**
 * \brief Node MIB configuration housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_NodeMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibConfigHkTlm_t;

/**
 * \brief Source MIB configuration housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_SourceMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibConfigHkTlm_t;

/**
 * \brief Node MIB counters housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_NodeMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibCountersHkTlm_t;

/**
 * \brief Source MIB counters housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_SourceMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibCountersHkTlm_t;

/**
 * \brief Storage housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_StorageHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_StorageHkTlm_t;

/**
 * \brief Channel/contact status housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPLib_ChannelContactStatHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_ChannelContactStatHkTlm_t;

#endif /* BPNODE_MSGSTRUCT_H */
