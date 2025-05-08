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
        /* Channel 0 */
        {
            .AddAutomatically       = true,
            .RequestCustody         = false,
            .AduWrapping            = false,
            .AduUnwrapping          = false,
            .RegState               = BPLIB_PI_ACTIVE,
            .HopLimit               = 10,
            .LocalServiceNumber     = BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_0,
            .MaxBundlePayloadSize   = BPLIB_MAX_PAYLOAD_SIZE,
            .CrcType                = BPLib_CRC_Type_CRC16,
            .BundleProcFlags        = BPLIB_BUNDLE_PROC_NO_FRAG_FLAG,
            .DestEID                =
                {
                    .Scheme         = BPLIB_EID_SCHEME_IPN,
                    .IpnSspFormat   = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                    .Allocator      = 0,
                    .Node           = BPNODE_EID_NODE_NUM_FOR_CONTACT_0,
                    .Service        = BPNODE_EID_SERVICE_NUM_FOR_CONTACT_0
                },
            .ReportToEID            =
                {
                    .Scheme         = BPLIB_EID_SCHEME_DTN,
                    .IpnSspFormat   = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                    .Allocator      = 0,
                    .Node           = 0,
                    .Service        = 0
                },
            .Lifetime               = 3600000,
            .PrevNodeBlkConfig =
                {
                    .IncludeBlock       = true,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BlockNum           = 2,
                    .BlockProcFlags     = 0
                },
            .AgeBlkConfig =
                {
                    .IncludeBlock       = true,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BlockNum           = 3,
                    .BlockProcFlags     = 0
                },
            .HopCountBlkConfig =
                {
                    .IncludeBlock       = true,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BlockNum           = 4,
                    .BlockProcFlags     = 0
                },
            .PayloadBlkConfig =
                {
                    .IncludeBlock       = true,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BlockNum           = 1,
                    .BlockProcFlags     = 0
                }
        },

        /* Channel 1 */
        {
            .AddAutomatically       = false,
            .RequestCustody         = false,
            .AduWrapping            = true,
            .AduUnwrapping          = false,
            .RegState               = BPLIB_PI_PASSIVE_DEFER,
            .HopLimit               = 10,
            .LocalServiceNumber     = BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_1,
            .MaxBundlePayloadSize   = BPLIB_MAX_PAYLOAD_SIZE,
            .CrcType                = BPLib_CRC_Type_CRC16,
            .BundleProcFlags        = BPLIB_BUNDLE_PROC_NO_FRAG_FLAG,
            .DestEID                =
                {
                    .Scheme         = BPLIB_EID_SCHEME_IPN,
                    .IpnSspFormat   = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                    .Allocator      = 0,
                    .Node           = BPLIB_LOCAL_EID_NODE_NUM,
                    .Service        = BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_1
                },
            .ReportToEID            =
                {
                    .Scheme         = BPLIB_EID_SCHEME_IPN,
                    .IpnSspFormat   = BPLIB_EID_IPN_SSP_FORMAT_TWO_DIGIT,
                    .Allocator      = 0,
                    .Node           = 100,
                    .Service        = 1
                },
            .Lifetime               = 3600000,
            .PrevNodeBlkConfig =
                {
                    .IncludeBlock       = false,
                    .CrcType            = BPLib_CRC_Type_None,
                    .BlockNum           = 1,
                    .BlockProcFlags     = 0
                },
            .AgeBlkConfig =
                {
                    .IncludeBlock       = false,
                    .CrcType            = BPLib_CRC_Type_None,
                    .BlockNum           = 0,
                    .BlockProcFlags     = 0
                },
            .HopCountBlkConfig =
                {
                    .IncludeBlock       = false,
                    .CrcType            = BPLib_CRC_Type_None,
                    .BlockNum           = 0,
                    .BlockProcFlags     = 0
                },
            .PayloadBlkConfig =
                {
                    .IncludeBlock       = true,
                    .CrcType            = BPLib_CRC_Type_CRC16,
                    .BlockNum           = 1,
                    .BlockProcFlags     = 0
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
