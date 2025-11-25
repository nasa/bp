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

/*
** \file
**   This file contains the source code for the CRS Config Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"


BPLib_ARP_CRSTable_t CRSTable = {
    .CRS_Set = {
        {.DestEID            =    // "100.1"
            {
                .Scheme       = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .Allocator    = 0,
                .Node         = 100,
                .Service      = 1
            },
         .TimeTrigger = 2,
         .SizeTrigger = 10
        },
        {.DestEID            =    // "100.2"
            {
                .Scheme       = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .Allocator    = 0,
                .Node         = 100,
                .Service      = 2
            },
         .TimeTrigger = 2,
         .SizeTrigger = 10
        },
        {.DestEID            =    // "100.3"
            {
                .Scheme       = BPLIB_EID_SCHEME_IPN,
                .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                .Allocator    = 0,
                .Node         = 100,
                .Service      = 3
            },
         .TimeTrigger = 2,
         .SizeTrigger = 10
        },
    }
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(CRSTable, BPNODE.CRSTable, Compressed Reporting Table, bpnode_crs.tbl)

