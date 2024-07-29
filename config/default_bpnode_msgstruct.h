/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

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

/************************************************************************
 * Includes
 ************************************************************************/

#include "bpnode_mission_cfg.h"
#include "bpnode_msgdefs.h"
#include "cfe_msg_hdr.h"

/*************************************************************************/

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_NoopCmd_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_ResetAllCountersCmd_t;

/*************************************************************************/
/*
** Type definition (BPNode housekeeping)
*/

typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BPNode_SendNodeMibCountersHkCmd_t;


/* Node MIB config telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_NodeMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibConfigHkTlm_t;

/* Source MIB config telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_SourceMibConfigHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibConfigHkTlm_t;

/* Node MIB counters telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_NodeMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_NodeMibCountersHkTlm_t;

/* Source MIB counters telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_SourceMibCountersHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_SourceMibCountersHkTlm_t;

/* Storage telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_StorageHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_StorageHkTlm_t;

/* Channel/contact status telemetry */
typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader;     /**< \brief Telemetry header */
    BPNode_ChannelContactStatHkTlm_Payload_t Payload;  /**< \brief Telemetry payload */
} BPNode_ChannelContactStatHkTlm_t;

#endif /* BPNODE_MSGSTRUCT_H */
