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

#ifndef BP_EVENTS_H
#define BP_EVENTS_H

/*************************************************************************
 * Macros
 *************************************************************************/

#define BP_INFO_EID                      1
#define BP_INIT_APP_INFO_EID             2
#define BP_INVALID_CC_ERR_EID            3
#define BP_INVALID_MID_ERR_EID           4
#define BP_INVALID_LEN_ERR_EID           5
#define BP_SB_RECEIVE_ERR_EID            6
#define BP_EXIT_ERR_EID                  7
#define BP_TBL_LOAD_ERR_EID              8
#define BP_TBL_LOAD_INFO_EID             9
#define BP_INVALID_FLOW_ERR_EID          10
#define BP_PARM_ERR_EID                  11
#define BP_PIPE_ERR_EID                  12
#define BP_PIPE_INFO_EID                 13
#define BP_SEM_ERR_EID                   14
#define BP_SEM_INFO_EID                  15
#define BP_FLOW_DISABLED_ERR_EID         16
#define BP_IO_OPEN_ERR_EID               17
#define BP_IO_OPEN_INFO_EID              18
#define BP_IO_RATES_NOT_UPDATED_INFO_EID 19
#define BP_LIB_OPEN_ERR_EID              20
#define BP_LIB_INVALID_STORE_ERR_EID     21
#define BP_FILE_INFO_EID                 22
#define BP_STORE_ERR_EID                 23
#define BP_LIB_INIT_ERR_EID              24

#define BP_IO_SEND_ERR_EID                 30
#define BP_IO_RECEIVE_ERR_EID              31
#define BP_LIB_PROC_ERR_EID                32
#define BP_LIB_LOAD_ERR_EID                33
#define BP_LIB_STORE_ERR_EID               34
#define BP_LIB_ACCEPT_ERR_EID              35
#define BP_LIB_CONFIG_ERR_EID              36
#define BP_FILE_ERR_EID                    37
#define BP_FILE_POLLING_NOT_ACTIVE_ERR_EID 38
#define BP_STORE_INFO_EID                  39

#define BP_BPLIB_INFO_EID 40

/* placeholder for CLA/Bundle flow events */
#define BP_CLA_INIT_INF_EID 50
#define BP_CLA_ERR_EID      51
#define BP_CLA_INFO_EID     52

#endif /* BP_EVENTS_H */
