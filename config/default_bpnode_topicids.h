/*
 * NASA Docket No. GSC-19,559-1, and identified as "Delay/Disruption Tolerant Networking 
 * (DTN) Bundle Protocol (BP) v7 Core Flight System (cFS) Application Build 7.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software distributed under 
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF 
 * ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. The copyright notice to be 
 * included in the software is as follows: 
 *
 * Copyright 2025 United States Government as represented by the Administrator of the 
 * National Aeronautics and Space Administration. All Rights Reserved. */

/**
 * @file
 *   BPNode Application Topic IDs
 */

#ifndef BPNODE_TOPICIDS_H
#define BPNODE_TOPICIDS_H

/*
** Macro Definitions
*/

/* Command topic IDs */
#define CFE_MISSION_BPNODE_CMD_TOPICID                          0x38
#define CFE_MISSION_BPNODE_WAKEUP_TOPICID                       0x39

/* Telemetry topic IDs */
#define CFE_MISSION_BPNODE_NODE_MIB_CONFIG_HK_TLM_TOPICID       0x35
#define CFE_MISSION_BPNODE_SOURCE_MIB_CONFIG_HK_TLM_TOPICID     0x36
#define CFE_MISSION_BPNODE_NODE_MIB_REPORTS_HK_TLM_TOPICID      0x37
#define CFE_MISSION_BPNODE_NODE_MIB_COUNTERS_HK_TLM_TOPICID     0x38
#define CFE_MISSION_BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_TOPICID   0x39
#define CFE_MISSION_BPNODE_STORAGE_HK_TLM_TOPICID               0x3A
#define CFE_MISSION_BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_TOPICID  0x3B


/* ADU Egress IDs */

/* 
** The ADU configuration table can be configured to wrap egressing bundles in a cFE
** telemetry header with the following message ID. Other missions may choose to override
** this value or to not use it entirely by configuring the table differently
*/
#define CFE_MISSION_BPNODE_ADU_OUT_SEND_TO_TLM_TOPICID          0xA0

#endif /* BPNODE_TOPICIDS_H */
