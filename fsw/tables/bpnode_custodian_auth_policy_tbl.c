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
**   This file contains the source code for the Custodian Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"

BPLib_PDB_CustodianTable_t CustodianTable = {
    // .AuthCustodians = {"100.1", "100.1","100.1","100.1","100.1","100.1","100.1","100.1","100.1","100.1"}
    .AuthCustodians =
        {   // 0
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 1
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 2
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 3
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 4
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 5
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 6
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 7
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 8
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,},
            // 9
            {.Scheme       = BPLIB_EID_SCHEME_IPN,
             .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
             .MaxAllocator = 0,
             .MinAllocator = 0,
             .MaxNode      = 100, /* EID 100.1 */
             .MinNode      = 100,
             .MaxService   = 1,
             .MinService   = 1,}
        }
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(CustodianTable, BPNODE.CustodianTable, Custodian policy Table, bpnode_custodia.tbl)
