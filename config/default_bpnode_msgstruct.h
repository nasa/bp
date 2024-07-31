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
 * \brief Reset all counters command 
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_ResetAllCountersCmd_t;

/**
 * \brief Send node MIB counters housekeeping command 
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendNodeMibCountersHkCmd_t;

/*
** Telemetry type Definitions
*/

/**
 * \brief Node MIB configuration housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_NodeMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibConfigHkTlm_t;

/**
 * \brief Source MIB configuration housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_SourceMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibConfigHkTlm_t;

/**
 * \brief Node MIB counters housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_NodeMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibCountersHkTlm_t;

/**
 * \brief Source MIB counters housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_SourceMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibCountersHkTlm_t;

/**
 * \brief Storage housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_StorageHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_StorageHkTlm_t;

/**
 * \brief Channel/contact status housekeeping telemetry
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_ChannelContactStatHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_ChannelContactStatHkTlm_t;

#endif /* BPNODE_MSGSTRUCT_H */
