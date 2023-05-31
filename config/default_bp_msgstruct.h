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

#ifndef BP_MSGSTRUCT_H
#define BP_MSGSTRUCT_H

/*************************************************************************
 * Includes
 **************************************************************************/

#include "bp_interface_cfg.h"
#include "bp_tlmcfg.h"

#include "cfe_msg_api_typedefs.h"
#include "cfe_msg_hdr.h"

/*************************************************************************
 * Typedefs
 **************************************************************************/

/*
 * Housekeeping Packet Structure
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader;
    uint16                    ValidCmdCnt;
    uint16                    InvalidCmdCnt;
    uint32                    EnableMask;
    uint32                    MemInUse;
    uint32                    MemHighWater;
    BP_CustomTlm_t            CustomTlm;
} BP_HkPkt_t;

typedef struct
{
    int8   SendHealthy;
    int8   ReceiveHealthy;
    uint16 SendNotReady;
    uint32 BytesSentPerSecond;
    uint32 BytesReceivedPerSecond;
} BP_IOStats_t;

typedef struct
{
    char         Name[BP_FLOW_NAME_SIZE];
    int8         Enabled;
    int8         Active;
    int8         Healthy;
    uint16       DataInDropped;
    uint16       DataOutDropped;
    uint32       LibFlags;
    uint32       Timeout;
    int32        Priority;
    BP_IOStats_t IOStats;
} BP_FlowStats_t;

/*
 * Flow Housekeeping Packet Structure
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader;
    BP_FlowStats_t            FlowStats;
} BP_FlowHkPkt_t;

/*
 * Noop command (no extra parameters)
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_NoopCmd_t;

/*
 * Reset command (no extra parameters)
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_ResetAppCmd_t;

/*
 * Reload Flow command (no extra parameters)
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_ReloadFlowTableCmd_t;

typedef struct
{
    char Name[BP_FLOW_NAME_SIZE];
} BP_FlowNameCmd_Payload_t;

typedef struct
{
    char   Name[BP_FLOW_NAME_SIZE];
    uint16 Priority;
} BP_FlowNameWithPriorityCmd_Payload_t;

typedef struct
{
    char   Name[BP_FLOW_NAME_SIZE];
    uint32 Timeout;
} BP_FlowNameWithTimeoutCmd_Payload_t;

/*
 * Enable Flow Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_EnableFlowCmd_t;

/*
 * Disable Flow Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_DisableFlowCmd_t;

/*
 * Flush Flow Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_FlushFlowCmd_t;

/*
 * Pause Flow Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_PauseFlowCmd_t;

/*
 * Resume Flow Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_ResumeFlowCmd_t;

/*
 * Override Timeout Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t             CommandHeader;
    BP_FlowNameWithTimeoutCmd_Payload_t Payload;
} BP_OverrideTimeoutCmd_t;

/*
 * Disable Override Timeout Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_DisableOverrideTimeoutCmd_t;

/*
 * Override Priority Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t              CommandHeader;
    BP_FlowNameWithPriorityCmd_Payload_t Payload;
} BP_OverridePriorityCmd_t;

/*
 * Disable Override Priority Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t  CommandHeader;
    BP_FlowNameCmd_Payload_t Payload;
} BP_DisableOverridePriorityCmd_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_SendAppTlmCmd_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_SendEnabledFlowTlmCmd_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
} BP_SendDisabledFlowTlmCmd_t;

#endif /* BP_MSG_H */
