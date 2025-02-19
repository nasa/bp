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

/*
** \file
**   This file contains the source code for the Channel Config Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"

BPLib_PI_ChannelTable_t ChannelTable = {
    .Configs = {
        {
            .AddAutomatically       = true,
            .RequestCustody         = false,
            .IncludePrevNodeBlk     = false,
            .IncludeHopCountBlk     = false,
            .AduWrapping            = true,
            .AduUnwrapping          = false,
            .RegState               = 0,
            .HopLimit               = 10,
            .LocalServiceNumber     = 2,
            .MaxBundlePayloadSize   = 15864,
            .PriBlkConfig           =
                {
                    .Version            = 7,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BundleProcFlags    = 4,
                    .DestEID            =    // "ipn:100.1"
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 100,
                            .Service      = 1
                        },
                    .SrcEID             =   // "ipn:101.1",
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 101,
                            .Service      = 1
                        },
                    .ReportToEID        =   // "ipn:100.1"
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 100,
                            .Service      = 1
                        },
                    .Lifetime           = 10000
                },
            .CanBlkConfig           =
                {
                    {
                        .BlockType          = 1,
                        .BlockNum           = 1,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 2,
                        .BlockNum           = 2,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 3,
                        .BlockNum           = 3,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 4,
                        .BlockNum           = 4,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 5,
                        .BlockNum           = 5,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                }
        },
        {
            .AddAutomatically       = false,
            .RequestCustody         = false,
            .IncludePrevNodeBlk     = false,
            .IncludeHopCountBlk     = false,
            .AduWrapping            = true,
            .AduUnwrapping          = false,
            .RegState               = 0,
            .HopLimit               = 10,
            .LocalServiceNumber     = 2,
            .MaxBundlePayloadSize   = 15864,
            .PriBlkConfig           =
                {
                    .Version            = 7,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BundleProcFlags    = 4,
                    .DestEID            =    // ipn:"100.1"
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 100,
                            .Service      = 1
                        },
                    .SrcEID             =   // "ipn:101.1",
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 101,
                            .Service      = 1
                        },
                    .ReportToEID        =   // "ipn:100.1"
                        {
                            .Scheme       = BPLIB_EID_SCHEME_IPN,
                            .IpnSspFormat = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                            .Allocator    = 0,
                            .Node         = 100,
                            .Service      = 1
                        },
                    .Lifetime           = 10000
                },
            .CanBlkConfig           =
                {
                    {
                        .BlockType          = 1,
                        .BlockNum           = 1,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 2,
                        .BlockNum           = 2,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 3,
                        .BlockNum           = 3,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 4,
                        .BlockNum           = 4,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                    {
                        .BlockType          = 5,
                        .BlockNum           = 5,
                        .BlockProcFlags     = 1,
                        .CrcType            = BPLib_CRC_Type_CRC16,
                        .Spare              = 0
                    },
                }
        }
    }
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(ChannelTable, BPNODE.ChannelTable, Channel Config Table, bpnode_channel.tbl)
