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
**   This file contains the source code for the MIB Per Node Config Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"


BPLib_NC_MibPerNodeConfig_t MIBConfigPNTable = {
    .InstanceEID = {
        .Scheme       = BPLIB_LOCAL_EID_SCHEME,
        .IpnSspFormat = BPLIB_LOCAL_EID_IPN_SSP_FORMAT,
        .Allocator    = BPLIB_LOCAL_EID_ALLOCATOR,
        .Node         = BPLIB_LOCAL_EID_NODE_NUM,
        .Service      = BPLIB_LOCAL_EID_SERVICE_NUM
                   },

    /* Per Node values */
    .SystemNodeName                         = "Name",
    .SystemSoftwareExec                     = "SysExec",
    .SystemSoftwareExecVersion              = "Version",
    .BundleAgentSoftwareVersion             = "BA Version",
    .BundleAgentOperationalState            = "Active",
    .BundleAgentConfiguration               = "BA Config",
    .ParamSupportedCLAs                     = "",
    .NodeActiveEndpoints                    = "",
    .SystemNodeUpTime                       = 0,
    .NodeStartupCounter                     = 0,
    .BundleAgentAvailableStorage            = 1000,
    .BundleSizeNoFragment                   = 1000,
    .BundleIngressRejectedRateBytesPerSec   = 0,
    .BundleIngressRejectedRateBundlesPerSec = 0,
    .ParamSetMaxSequenceNum                 = 100000,
    .ParamSetMaxPayloadLength               = 100000,
    .ParamSetMaxBundleLength                = 100000,
    .ParamSetNodeDTNTime                    = 0,
    .ParamSetBehaviorEventReporting         = 10,

    /* Per Node and per Source values */
    .ParamSetMaxLifetime             = 0,
    .KbytesCountStorageAvailable     = 0,
    .BundleIngressRateBytesPerSec    = 0,
    .BundleIngressRateBundlesPerSec  = 0,
    .BundleEgressRateBytesPerSec     = 0,
    .BundleIngestedRateBundlesPerSec = 0,
    .BundleIngestedRateBytesPerSec   = 0,
    .BundleDeliveryRateBundlesPerSec = 0,
    .BundleDeliveryRateBytesPerSec   = 0,
    .BundleEgressRateBundlesPerSec   = 0,
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the a image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(MIBConfigPNTable, BPNODE.MIBConfigPNTable, MIB per node Table, bpnode_mib_pn.tbl)
