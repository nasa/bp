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
 *   BPNode Application Topic IDs
 */
#ifndef BPNODE_TOPICIDS_H
#define BPNODE_TOPICIDS_H

/* Command topic IDs */
#define CFE_MISSION_BPNODE_CMD_TOPICID                          0xB0
#define CFE_MISSION_BPNODE_WAKEUP_TOPICID                       0xB1

/* Telemetry topic IDs */
#define CFE_MISSION_BPNODE_NODE_MIB_CONFIG_HK_TLM_TOPICID       0xB0
#define CFE_MISSION_BPNODE_SOURCE_MIB_CONFIG_HK_TLM_TOPICID     0xB1
#define CFE_MISSION_BPNODE_NODE_MIB_COUNTERS_HK_TLM_TOPICID     0xB2
#define CFE_MISSION_BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_TOPICID   0xB3
#define CFE_MISSION_BPNODE_STORAGE_HK_TLM_TOPICID               0xB4
#define CFE_MISSION_BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_TOPICIC  0xB5

#endif
