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

#ifndef BP_TBLSTRUCT_H
#define BP_TBLSTRUCT_H

#include <common_types.h>
#include <cfe_sb_extern_typedefs.h>

/* Flow Table */

typedef struct
{
    CFE_SB_MsgId_t StreamId;
    uint8          Priority;
    uint8          Reliability;
    uint16         BuffLim;
} BP_PktTblEntry_t;

typedef struct
{
    char             Name[BP_FLOW_NAME_SIZE];
    uint8            Enabled; /* bool */
    uint16           SrcServ;
    uint32           DstNode;
    uint16           DstServ;
    uint32           Timeout;   /* seconds */
    uint32           Lifetime;  /* seconds */
    uint16           Priority;  /* higher values mean higher priority */
    uint32           MaxActive; /* bundles */
    char             Store[BP_STORE_NAME_SIZE];
    BP_PktTblEntry_t PktTbl[BP_PKTTBL_MAX_ROWS];
    CFE_SB_MsgId_t   RecvStreamId;
    uint16           PipeDepth;
} BP_FlowTblEntry_t;

typedef struct
{
    uint32            LocalNodeIpn;
    BP_FlowTblEntry_t Flows[BP_MAX_FLOWS];
} BP_FlowTable_t;

#endif /* BP_TBLSTRUCT_H */
