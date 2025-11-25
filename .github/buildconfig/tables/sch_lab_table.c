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

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "sch_lab_tbl.h"
#include "cfe_sb.h" /* Required to use the CFE_SB_MSGID_WRAP_VALUE macro */
#include "cfe_msgids.h"

/*
** Include headers for message IDs here
*/
#include "ci_lab_msgids.h"
#include "to_lab_msgids.h"
#ifdef HAVE_SAMPLE_APP
#include "sample_app_msgids.h"
#endif

#include "bpnode_msgids.h"
#include "cf_msgids.h"

/*
** SCH Lab schedule table
** When populating this table:
**  1. The entire table is processed (SCH_LAB_MAX_SCHEDULE_ENTRIES) but entries with a
**     packet rate of 0 are skipped
**  2. You can have commented out entries or entries with a packet rate of 0
**  3. If the table grows too big, increase SCH_LAB_MAX_SCHEDULE_ENTRIES
*/

SCH_LAB_ScheduleTable_t SCH_LAB_ScheduleTable = {.TickRate = 1,
            .Config   = {
                {CFE_SB_MSGID_WRAP_VALUE(CFE_ES_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CFE_EVS_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CFE_TIME_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CFE_SB_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CFE_TBL_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CI_LAB_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(TO_LAB_SEND_HK_MID), 4, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CF_SEND_HK_MID), 4, 0},
#ifdef HAVE_SAMPLE_APP
                {CFE_SB_MSGID_WRAP_VALUE(SAMPLE_APP_SEND_HK_MID), 4, 0},
#endif
                {CFE_SB_MSGID_WRAP_VALUE(BPNODE_CMD_MID),       4, 42}, /* BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC */
                {CFE_SB_MSGID_WRAP_VALUE(BPNODE_WAKEUP_MID),    1, 0},
                {CFE_SB_MSGID_WRAP_VALUE(CF_WAKE_UP_MID),       1, 1},
}};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(SCH_LAB_ScheduleTable, SCH_LAB_APP.ScheduleTable, Schedule Lab MsgID Table, sch_lab_table.tbl)
