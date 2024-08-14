/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"


BPNode_CRSTable_t CRSTable = {
    .DestEID        = "100.1", /*Destination EID, string (Node_Number.Service_Number)*/
    .TimeTrigger    = 2, /* CRS Time Trigger, timeout value that triggers ARP to send the CRS to PI, in seconds, uint32*/
    .SizeTrigger    = 10 /*Size trigger, CRS size trigger, maximum size for the CRS generated within ARP before it is encoded and sent out, size_t*/
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(CRSTable, BPNODE.CRSTable, Compressed Reporting Table, crs_tbl.tbl)

