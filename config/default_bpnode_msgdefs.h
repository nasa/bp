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
 *   message constant definitions.
 *
 *  For BPNode this is only the function/command code definitions
 */

#ifndef BPNODE_MSGDEFS_H
#define BPNODE_MSGDEFS_H

#include "common_types.h"
#include "bpnode_fcncodes.h"

/*
** Type Definitions
*/

/**
 * \brief Node MIB configuration housekeeping payload
 */
typedef struct BPNode_NodeMibConfigHkTlm_Payload
{
    /* TODO */
    uint32 Spare;
} BPNode_NodeMibConfigHkTlm_Payload_t;

/**
 * \brief Source MIB configuration housekeeping payload
 */
typedef struct BPNode_SourceMibConfigHkTlm_Payload
{
    /* TODO */
    uint32 Spare;
} BPNode_SourceMibConfigHkTlm_Payload_t;

/**
 * \brief Node MIB counters housekeeping payload
 */
typedef struct BPNode_NodeMibCountersHkTlm_Payload
{
    uint16 AcceptedDirectiveCount;
    uint16 RejectedDirectiveCount;
} BPNode_NodeMibCountersHkTlm_Payload_t;

/**
 * \brief Source MIB counters housekeeping payload
 */
typedef struct BPNode_SourceMibCountersHkTlm_Payload
{
    /* TODO */
    uint32 Spare;
} BPNode_SourceMibCountersHkTlm_Payload_t;

/**
 * \brief Storage housekeeping payload
 */
typedef struct BPNode_StorageHkTlm_Payload
{
    /* TODO */
    uint32 Spare;
} BPNode_StorageHkTlm_Payload_t;

/**
 * \brief Channel/contact status housekeeping payload
 */
typedef struct BPNode_ChannelContactStatHkTlm_Payload
{
    /* TODO */
    uint32 Spare;
} BPNode_ChannelContactStatHkTlm_Payload_t;


#endif /* BPNODE_MSGDEFS_H */
