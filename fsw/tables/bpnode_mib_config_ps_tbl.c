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
 * National Aeronautics and Space Administration. All Rights Reserved.
 *
 */

/*
** \file
**   This file contains the source code for the MIB Per Source Config Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"


BPLib_NC_MIBConfigPSTable_t MIBConfigPSTable = {
    .Sources = {
        {
            // .SrcEID                                 = "100.1",
            // .EIDPattern                             = "ipn",
            .SrcEIDs = {
                {.Scheme      = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .MaxAllocator = 0,
                .MinAllocator = 0,
                .MaxNode      = 100, /* EID 100.1 */
                .MinNode      = 100,
                .MaxService   = 1,
                .MinService   = 1,}
            },
            .Configs = {
                /* PARAM_SET_MAX_LIFETIME                */ 1000, 
                /* PARAM_SET_MAX_BSR_GENERATION_RATE     */ 20000,
                /* PARAM_SET_MAX_CBR_GENERATION_RATE     */ 20000,
                /* BUNDLE_SET_BEHAVIOR_RCVD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_RCVD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_CBR_GENERATE */ 0
            }
            
        },
        {
            // .SrcEID                                 = "101.1",
            // .EIDPattern                             = "ipn",
            .SrcEIDs = {
                {.Scheme       = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .MaxAllocator = 0,
                .MinAllocator = 0,
                .MaxNode      = 101, /* EID 101.1 */
                .MinNode      = 101,
                .MaxService   = 1,
                .MinService   = 1,}
            },
            .Configs = {
                /* PARAM_SET_MAX_LIFETIME                */ 1000, 
                /* PARAM_SET_MAX_BSR_GENERATION_RATE     */ 20000,
                /* PARAM_SET_MAX_CBR_GENERATION_RATE     */ 20000,
                /* BUNDLE_SET_BEHAVIOR_RCVD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_RCVD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_CBR_GENERATE */ 0
            }
        },
        {
            // .SrcEID                                 = "102.1",
            // .EIDPattern                             = "ipn",
            .SrcEIDs = {
                {.Scheme       = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .MaxAllocator = 0,
                .MinAllocator = 0,
                .MaxNode      = 102, /* EID 102.1 */
                .MinNode      = 102,
                .MaxService   = 1,
                .MinService   = 1,}
            },
            .Configs = {
                /* PARAM_SET_MAX_LIFETIME                */ 1000, 
                /* PARAM_SET_MAX_BSR_GENERATION_RATE     */ 20000,
                /* PARAM_SET_MAX_CBR_GENERATION_RATE     */ 20000,
                /* BUNDLE_SET_BEHAVIOR_RCVD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_BSR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_RCVD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_ACPT_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_FWRD_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLVR_CBR_GENERATE */ 0,
                /* BUNDLE_SET_BEHAVIOR_DLTD_CBR_GENERATE */ 0
            }
        },
    }
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the a image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(MIBConfigPSTable, BPNODE.MIBConfigPSTable, MIB per source Table, bpnode_mib_ps.tbl)
