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
 *   Specification for the BPNode command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */

#ifndef BPNODE_FCNCODES_H
#define BPNODE_FCNCODES_H

/*
** Macro Definitions
*/

/*
** BPNode command codes
*/

/* Noop */
#define BPNODE_NOOP_CC                              (0)

/* Startup directives */
#define BPNODE_ADD_ALL_APPLICATIONS_CC              (1)
#define BPNODE_START_ALL_APPLICATIONS_CC            (2)
#define BPNODE_VERIFY_BUNDLE_STORAGE_CC             (3)
#define BPNODE_INIT_BUNDLE_STORAGE_CC               (4)
#define BPNODE_VERIFY_BUNDLE_METADATA_CC            (5) /* Deferred */
#define BPNODE_REBUILD_BUNDLE_METADATA_CC           (6)
#define BPNODE_CLEAR_VOLATILE_CC                    (7)
#define BPNODE_RELOAD_SAVED_DATA_CC                 (8) /* Deferred */

/* Counter directives */
#define BPNODE_RESET_ALL_COUNTERS_CC                (9)
#define BPNODE_RESET_COUNTER_CC                     (10)
#define BPNODE_RESET_SOURCE_COUNTERS_CC             (11)
#define BPNODE_RESET_BUNDLE_COUNTERS_CC             (12)
#define BPNODE_RESET_ERROR_COUNTERS_CC              (13)

/* Application directives */
#define BPNODE_ADD_APPLICATION_CC                   (14)
#define BPNODE_REMOVE_APPLICATION_CC                (15)
#define BPNODE_SET_REGISTRATION_STATE_CC            (16)
#define BPNODE_START_APPLICATION_CC                 (17)
#define BPNODE_STOP_APPLICATION_CC                  (18)

/* Policy directives */
#define BPNODE_ADD_AUTH_SOURCES_CC                  (19)
#define BPNODE_REMOVE_AUTH_SOURCES_CC               (20)
#define BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC          (21)
#define BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC       (22)
#define BPNODE_ADD_AUTH_CUSTODIANS_CC               (23)
#define BPNODE_REMOVE_AUTH_CUSTODIANS_CC            (24)
#define BPNODE_ADD_AUTH_REPORT_TO_EID_CC            (25)
#define BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC         (26)
#define BPNODE_ADD_LATENCY_CC                       (27)
#define BPNODE_REMOVE_LATENCY_CC                    (28)

/* Contact directives */
#define BPNODE_CONTACT_SETUP_CC                     (29)
#define BPNODE_CONTACT_START_CC                     (30)
#define BPNODE_CONTACT_STOP_CC                      (31)
#define BPNODE_CONTACT_TEARDOWN_CC                  (32)

/* MIB directives */
#define BPNODE_ADD_MIB_ARRAY_KEY_CC                 (33)
#define BPNODE_REMOVE_MIB_ARRAY_KEY_CC              (34)
#define BPNODE_SET_MIB_ITEM_CC                      (35)

/* Storage directives */
#define BPNODE_ADD_STORAGE_ALLOCATION_CC            (36)
#define BPNODE_REMOVE_STORAGE_ALLOCATION_CC         (37)
#define BPNODE_PERFORM_SELF_TEST_CC                 (38) /* Deferred */

/* Telemetry directives */
#define BPNODE_SEND_NODE_MIB_CONFIG_HK_CC           (40)
#define BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC         (41)
#define BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC         (42)
#define BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC       (43)
#define BPNODE_SEND_STORAGE_HK_CC                   (44)
#define BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC      (45)
#define BPNODE_SEND_NODE_MIB_REPORTS_HK_CC          (46)

#endif /* BPNODE_FCNCODES_H */
