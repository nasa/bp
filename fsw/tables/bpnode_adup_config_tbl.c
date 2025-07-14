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
**   This file contains the source code for the ADU Proxy Config Table
*/

#include "cfe.h"
#include "fwp_adup.h"
#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_msgids.h"

BPA_ADUP_Config_t ADUProxyTable[BPLIB_MAX_NUM_CHANNELS] = {
    {
        .SendToMsgId = CFE_SB_MSGID_WRAP_VALUE(BPNODE_ADU_OUT_SEND_TO_MID),
        .NumRecvFrmMsgIds = 4,
        .RecvFrmMsgIds = {
            CFE_SB_MSGID_WRAP_VALUE(0x080A),                /* CFE_SB_STATS */
            CFE_SB_MSGID_WRAP_VALUE(0x080D),                /* CFE_SB_ALLSUBS */
            CFE_SB_MSGID_WRAP_VALUE(0x080E),                /* CFE_SB_ONESUB */
        },
        .MsgLims = {
            10,
            10,
            10,
            10
        }
    },
    {
        .SendToMsgId = CFE_SB_MSGID_WRAP_VALUE(BPNODE_ADU_OUT_SEND_TO_MID),
        .NumRecvFrmMsgIds = 1,
        .RecvFrmMsgIds = {
            CFE_SB_MSGID_WRAP_VALUE(0x0806),                /* CFE_TIME_DIAG_HK */
        },
        .MsgLims = {
            10
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
CFE_TBL_FILEDEF(ADUProxyTable, BPNODE.ADUProxyTable, ADU Proxy Config Table, bpnode_adup.tbl)
