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

/************************************************************************
** Includes
*************************************************************************/

#include <stdint.h>

#include "cfe.h"
#include "bp_cfg.h"
#include "bp_flow.h"
#include "bplib.h"
#include "cfe_tbl_filedef.h"

/************************************************************************
** Data
*************************************************************************/

/*
** Table file header
*/
CFE_TBL_FileDef_t CFE_TBL_FileDef = {"BP_FlowTable", "BP.FlowTable", "Configuration of bundle flows",
                                     "bp_flowtable.tbl", (sizeof(BP_FlowTbl_t))};

/*
** Table contents
*/
BP_FlowTbl_t BP_FlowTable = {
    .LocalNodeIpn = 12,
    .Flows = {{/* Flow 0 */
               .Name      = "HKT",
               .Enabled   = true,
               .PipeDepth = BP_APP_READ_LIMIT,
               .SrcServ   = 1,
               .DstNode   = 23,
               .DstServ   = 1,
               .Timeout   = 20,
               .Lifetime  = 86400,
               .Priority  = BP_COS_NORMAL,
               .MaxActive = 250,
               .Store     = BP_FLASH_STORE,
               .PktTbl    = {{CF_SPACE_TO_GND_PDU_MID0, 1, 1, BP_APP_READ_LIMIT}},
               .RecvStreamId = CFE_SB_MSGID_RESERVED
              {/* Flow 1 */
               .Name      = "EVT",
               .Enabled   = false,
               .PipeDepth = BP_APP_READ_LIMIT,
               .SrcServ   = 2,
               .DstNode   = 23,
               .DstServ   = 2,
               .Timeout   = 0,
               .Lifetime  = 86400,
               .Priority  = BP_COS_BULK,
               .MaxActive = 0,
               .Store     = BP_FLASH_STORE,
               .PktTbl    = {{CFE_SB_MSGID_WRAP_VALUE(CFE_EVS_EVENT_MSG_MID), 1, 1, BP_APP_READ_LIMIT}},
               .RecvStreamId = CFE_SB_MSGID_RESERVED}}};

/************************/
/*  End of File Comment */
/************************/
