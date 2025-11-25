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
 *   Specification for the BPNode table structures
 *
 * @note
 *   Constants and enumerated types related to these table structures
 *   are defined in bpnode_tbldefs.h.
 */

#ifndef BPNODE_TBL_H
#define BPNODE_TBL_H

#include "bpnode_tbldefs.h"
#include "bpnode_tblstruct.h"

/*
** Macro Definitions
*/

/* Define filenames of default data images for tables */
#define CRS_TABLE_FILE "/cf/bpnode_crs.tbl"
#define ADUP_CONFIG_TABLE_FILE "/cf/bpnode_adup.tbl"
#define CONTACTS_TABLE_FILE "/cf/bpnode_contacts.tbl"
#define CUSTODIAN_TABLE_FILE "/cf/bpnode_custodia.tbl"
#define CUSTODY_TABLE_FILE "/cf/bpnode_custody.tbl"
#define CHANNEL_TABLE_FILE "/cf/bpnode_channel.tbl"
#define REPORTTO_TABLE_FILE "/cf/bpnode_reportto.tbl"
#define SRC_AUTH_TABLE_FILE "/cf/bpnode_src_auth.tbl"
#define SRC_LATENCY_TABLE_FILE "/cf/bpnode_latency.tbl"
#define STORAGE_TABLE_FILE "/cf/bpnode_storage.tbl"
#define MIB_CONFIG_PN_TABLE_FILE "/cf/bpnode_mib_pn.tbl"
#define MIB_CONFIG_PS_TABLE_FILE "/cf/bpnode_mib_ps.tbl"

#endif /* BPNODE_TBL_H */
