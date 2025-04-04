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
 *   BPNode Application Message IDs
 */

#ifndef BPNODE_MSGIDS_H
#define BPNODE_MSGIDS_H

#include "cfe_core_api_base_msgids.h"
#include "bpnode_topicids.h"

/*
** Macro Definitions
*/

/* Command message IDs */
#define BPNODE_CMD_MID                          CFE_PLATFORM_CMD_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_CMD_TOPICID)
#define BPNODE_WAKEUP_MID                       CFE_PLATFORM_CMD_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_WAKEUP_TOPICID)

/* Telemetry message IDs */
#define BPNODE_NODE_MIB_CONFIG_HK_TLM_MID       CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_NODE_MIB_CONFIG_HK_TLM_TOPICID)
#define BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID     CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_SOURCE_MIB_CONFIG_HK_TLM_TOPICID)
#define BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID     CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_NODE_MIB_COUNTERS_HK_TLM_TOPICID)
#define BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID   CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_TOPICID)
#define BPNODE_STORAGE_HK_TLM_MID               CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_STORAGE_HK_TLM_TOPICID)
#define BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID  CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_TOPICID)

/* ADU Egress IDs */
#define BPNODE_ADU_OUT_SEND_TO_MID              CFE_PLATFORM_TLM_TOPICID_TO_MIDV(CFE_MISSION_BPNODE_ADU_OUT_SEND_TO_TLM_TOPICID)

#endif /* BPNODE_MSGIDS_H */
