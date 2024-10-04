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
#include "bpnode_platform_cfg.h"

#include "bplib.h"

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
    uint16 AcceptedDirectiveCount;  /**< \brief Accepted Directive Count */
    uint16 RejectedDirectiveCount;  /**< \brief Rejected Directive Count */
    uint16 AduCountDelivered;       /**< \brief ADU Delivered Count */
    uint16 AduCountReceived;        /**< \brief ADU Received Count */

    uint32 Spare;                   /* Temporary padding */

    uint32 TimeBootEra;             /**< \brief Boot Era for Monotonic Time */
    int64  MonotonicTime;           /**< \brief Monotonic Time Counter */
    int64  CorrelationFactor;       /**< \brief Time Correlation Factor */
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
 * \brief Channel status data
 */
typedef struct BPNode_ChannelStats
{
    uint32 LocalServiceNum;         /**< \brief Service number for local application */
    uint8  State;                   /**< \brief Channel state (ADDED, STARTED, or STOPPED) */
    uint8  RegistrationState;       /**< \brief Active, PassiveDeferred, or PassiveAbandon */
    uint16 Spare;                   /**< \brief Padding */
    uint32 OutputQueueId;           /**< \brief PI output queue ID */

} BPNode_ChannelStats_t;

/**
 * \brief Contact status data
 */
typedef struct BPNode_ContactStats
{
    uint32 Spare;       /* TODO */
} BPNode_ContactStats_t;

/**
 * \brief Channel/contact status housekeeping payload
 */
typedef struct BPNode_ChannelContactStatHkTlm_Payload
{
    BPNode_ChannelStats_t ChannelStats[BPLIB_MAX_NUM_CHANNELS];
    BPNode_ContactStats_t ContactStats[BPNODE_MAX_NUM_CONTACTS];

    uint32 TimeBootEra;             /**< \brief Boot Era for Monotonic Time */
    int64  MonotonicTime;           /**< \brief Monotonic Time Counter */
    int64  CorrelationFactor;       /**< \brief Time Correlation Factor */
} BPNode_ChannelContactStatHkTlm_Payload_t;


#endif /* BPNODE_MSGDEFS_H */
