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

/**
 * @file
 *
 * Define  BPNode Event IDs
 */

#ifndef BPNODE_EVENTS_H
#define BPNODE_EVENTS_H

#define BPNODE_RESERVED_EID                     0
#define BPNODE_INIT_INF_EID                     1
#define BPNODE_CC_ERR_EID                       2
#define BPNODE_NOOP_INF_EID                     3
#define BPNODE_RESET_INF_EID                    4
#define BPNODE_MID_ERR_EID                      5
#define BPNODE_CMD_LEN_ERR_EID                  6
#define BPNODE_PIPE_ERR_EID                     7
#define BPNODE_CR_CMD_PIPE_ERR_EID              8
#define BPNODE_CR_WKP_PIPE_ERR_EID              9
#define BPNODE_SUB_CMD_ERR_EID                  10
#define BPNODE_SUB_WKP_ERR_EID                  11
#define BPNODE_TBL_REG_ERR_EID                  12
#define BPNODE_TBL_LD_ERR_EID                   13
#define BPNODE_TBL_ADDR_ERR_EID                 14
#define BPNODE_TBL_MNG_ERR_EID                  15
#define BPNODE_FWP_INIT_ERR_EID                 16
#define BPNODE_EXIT_CRIT_EID                    17
#define BPNODE_AUTO_ADD_APP_INF_EID             18
#define BPNODE_ADU_ADD_CHAN_ERR_EID             19
#define BPNODE_ADU_ADD_STAT_ERR_EID             20
#define BPNODE_ADU_START_CHAN_ERR_EID           21
#define BPNODE_ADU_START_STAT_ERR_EID           22
#define BPNODE_ADU_START_SUB_ERR_EID            23
#define BPNODE_ADU_STOP_CHAN_ERR_EID            24
#define BPNODE_ADU_STOP_STAT_ERR_EID            25
#define BPNODE_ADU_STOP_UNSUB_ERR_EID           26
#define BPNODE_ADU_IN_TOO_BIG_ERR_EID           27
#define BPNODE_ADU_IN_INIT_SEM_ERR_EID          28
#define BPNODE_ADU_IN_CREATE_ERR_EID            29
#define BPNODE_ADU_IN_RUN_ERR_EID               30
#define BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID       31
#define BPNODE_ADU_IN_INIT_INF_EID              32
#define BPNODE_ADU_IN_EXIT_CRIT_EID             33
#define BPNODE_ADU_IN_NO_ID_ERR_EID             34
#define BPNODE_ADU_IN_INV_ID_ERR_EID            35
#define BPNODE_ADU_IN_CR_PIPE_ERR_EID           36
#define BPNODE_ADU_IN_UNK_EXIT_CRIT_EID         37
#define BPNODE_ADU_OUT_INIT_SEM_ERR_EID         38
#define BPNODE_ADU_OUT_CREATE_ERR_EID           39
#define BPNODE_ADU_OUT_RUN_ERR_EID              40
#define BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID      41
#define BPNODE_ADU_OUT_INIT_INF_EID             42
#define BPNODE_ADU_OUT_EXIT_CRIT_EID            43
#define BPNODE_ADU_OUT_NO_ID_ERR_EID            44
#define BPNODE_ADU_OUT_INV_ID_ERR_EID           45
#define BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID        46
#define BPNODE_TIME_INIT_ERR_EID                47
#define BPNODE_TIME_WKP_ERR_EID                 48
#define BPNODE_CLA_IN_INIT_SEM_ERR_EID          49
#define BPNODE_CLA_IN_EXIT_SEM_ERR_EID          50
#define BPNODE_CLA_IN_CREATE_ERR_EID            51
#define BPNODE_CLA_IN_RUN_ERR_EID               52
#define BPNODE_CLA_IN_INIT_SEM_TK_ERR_EID       53
#define BPNODE_CLA_IN_INIT_INF_EID              54
#define BPNODE_CLA_IN_EXIT_CRIT_EID             55
#define BPNODE_CLA_IN_NO_ID_ERR_EID             56
#define BPNODE_CLA_IN_INV_ID_ERR_EID            57
#define BPNODE_CLA_IN_CR_PIPE_ERR_EID           58
#define BPNODE_CLA_IN_SUB_PIPE_ERR_EID          59
#define BPNODE_CLA_IN_UNK_EXIT_CRIT_EID         60
#define BPNODE_CLA_IN_LIB_PROC_ERR_EID          61
#define BPNODE_CLA_OUT_INIT_SEM_ERR_EID         62
#define BPNODE_CLA_OUT_EXIT_SEM_ERR_EID         63
#define BPNODE_CLA_OUT_CREATE_ERR_EID           64
#define BPNODE_CLA_OUT_RUN_ERR_EID              65
#define BPNODE_CLA_OUT_INIT_SEM_TK_ERR_EID      66
#define BPNODE_CLA_OUT_INIT_INF_EID             67
#define BPNODE_CLA_OUT_EXIT_CRIT_EID            68
#define BPNODE_CLA_OUT_NO_ID_ERR_EID            69
#define BPNODE_CLA_OUT_INV_ID_ERR_EID           70
#define BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID        71
#define BPNODE_CLA_OUT_LIB_LOAD_ERR_EID         72
#define BPNODE_CLA_IOD_FINDNAME_IN_EID          73
#define BPNODE_CLA_IOD_COMMAND_DIR_IN_EID       74
#define BPNODE_CLA_IOD_COMMAND_RUN_IN_EID       75
#define BPNODE_CLA_IOD_FINDNAME_OUT_EID         76
#define BPNODE_CLA_IOD_COMMAND_DIR_OUT_EID      77
#define BPNODE_CLA_IOD_COMMAND_RUN_OUT_EID      78
#define BPNODE_GEN_WRKR_SEM_CR_ERR_EID          79
#define BPNODE_GEN_WRKR_CREATE_ERR_EID          80
#define BPNODE_GEN_WRKR_RUN_ERR_EID             81
#define BPNODE_GEN_WRKR_SEM_INIT_ERR_EID        82
#define BPNODE_GEN_WRKR_INIT_INF_EID            83
#define BPNODE_GEN_WRKR_EXIT_CRIT_EID           84
#define BPNODE_GEN_WRKR_NO_ID_ERR_EID           85
#define BPNODE_GEN_WRKR_INV_ID_ERR_EID          86
#define BPNODE_GEN_WRKR_UNK_EXIT_CRIT_EID       87
#define BPNODE_GEN_WRKR_SEM_TK_ERR_EID          88
#define BPNODE_WKP_SEM_ERR_EID                  89
#define BPNODE_ADU_REM_CHAN_ERR_EID             90
#define BPNODE_ADU_REM_STAT_ERR_EID             90

#endif /* BPNODE_EVENTS_H */
