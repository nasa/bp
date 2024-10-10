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
#define BPNODE_VALUE_INF_EID                    8
#define BPNODE_CR_CMD_PIPE_ERR_EID              9
#define BPNODE_CR_WKP_PIPE_ERR_EID              10
#define BPNODE_SUB_HK_ERR_EID                   11
#define BPNODE_SUB_CMD_ERR_EID                  12
#define BPNODE_SUB_WKP_ERR_EID                  13
#define BPNODE_TBL_REG_ERR_EID                  14
#define BPNODE_TBL_LD_ERR_EID                   15
#define BPNODE_TBL_ADDR_ERR_EID                 16
#define BPNODE_TBL_MNG_ERR_EID                  17
#define BPNODE_FWP_INIT_ERR_EID                 18
#define BPNODE_EXIT_CRIT_EID                    19
#define BPNODE_DELETE_HNDLR_ERR_EID             20

#define BPNODE_ADU_IN_INIT_SEM_ERR_EID          21
#define BPNODE_ADU_IN_EXIT_SEM_ERR_EID          22
#define BPNODE_ADU_IN_CREATE_ERR_EID            23
#define BPNODE_ADU_IN_RUN_ERR_EID               24
#define BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID       25
#define BPNODE_ADU_IN_INIT_INF_EID              26
#define BPNODE_ADU_IN_EXIT_CRIT_EID             27
#define BPNODE_ADU_IN_NO_ID_ERR_EID             28
#define BPNODE_ADU_IN_INV_ID_ERR_EID            29
#define BPNODE_ADU_IN_CR_PIPE_ERR_EID           30
#define BPNODE_ADU_IN_UNK_EXIT_CRIT_EID         31

#define BPNODE_ADU_OUT_INIT_SEM_ERR_EID         40
#define BPNODE_ADU_OUT_EXIT_SEM_ERR_EID         41
#define BPNODE_ADU_OUT_CREATE_ERR_EID           42
#define BPNODE_ADU_OUT_RUN_ERR_EID              43
#define BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID      44
#define BPNODE_ADU_OUT_INIT_INF_EID             45
#define BPNODE_ADU_OUT_EXIT_CRIT_EID            46
#define BPNODE_ADU_OUT_NO_ID_ERR_EID            47
#define BPNODE_ADU_OUT_INV_ID_ERR_EID           48
#define BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID        49

#define BPNODE_TIME_INIT_ERR_EID                50
#define BPNODE_TIME_WKP_ERR_EID                 51

#define BPNODE_ADU_SUB_ERR_EID                  52
#define BPNODE_ADU_UNSUB_ERR_EID                53
#define BPNODE_ADU_ADD_CHAN_ERR_EID             54
#define BPNODE_ADU_ADD_STAT_ERR_EID             55
#define BPNODE_ADU_STRT_CHAN_ERR_EID            56
#define BPNODE_ADU_STRT_STAT_ERR_EID            57
#define BPNODE_ADU_STOP_CHAN_ERR_EID            58
#define BPNODE_ADU_STOP_STAT_ERR_EID            58
#define BPNODE_ADU_TOO_BIG_ERR_EID              59

#define BPNODE_ADD_APP_INF_EID                  60
#define BPNODE_STRT_APP_INF_EID                 61
#define BPNODE_STOP_APP_INF_EID                 62
#define BPNODE_AUTO_ADD_APP_INF_EID             63

#define BPNODE_CLA_IN_INIT_SEM_ERR_EID          121
#define BPNODE_CLA_IN_EXIT_SEM_ERR_EID          122
#define BPNODE_CLA_IN_CREATE_ERR_EID            123
#define BPNODE_CLA_IN_RUN_ERR_EID               124
#define BPNODE_CLA_IN_INIT_SEM_TK_ERR_EID       125
#define BPNODE_CLA_IN_INIT_INF_EID              126
#define BPNODE_CLA_IN_EXIT_CRIT_EID             127
#define BPNODE_CLA_IN_NO_ID_ERR_EID             128
#define BPNODE_CLA_IN_INV_ID_ERR_EID            129
#define BPNODE_CLA_IN_CR_PIPE_ERR_EID           130
#define BPNODE_CLA_IN_SUB_PIPE_ERR_EID          131
#define BPNODE_CLA_IN_UNK_EXIT_CRIT_EID         132
#define BPNODE_CLA_IN_LIB_PROC_ERR_EID          133

#define BPNODE_CLA_OUT_INIT_SEM_ERR_EID         140
#define BPNODE_CLA_OUT_EXIT_SEM_ERR_EID         141
#define BPNODE_CLA_OUT_CREATE_ERR_EID           142
#define BPNODE_CLA_OUT_RUN_ERR_EID              143
#define BPNODE_CLA_OUT_INIT_SEM_TK_ERR_EID      144
#define BPNODE_CLA_OUT_INIT_INF_EID             145
#define BPNODE_CLA_OUT_EXIT_CRIT_EID            146
#define BPNODE_CLA_OUT_NO_ID_ERR_EID            147
#define BPNODE_CLA_OUT_INV_ID_ERR_EID           148
#define BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID        149
#define BPNODE_CLA_OUT_LIB_LOAD_ERR_EID         150

#define BPNODE_CLA_IOD_FINDNAME_IN_EID          180
#define BPNODE_CLA_IOD_COMMAND_DIR_IN_EID       181
#define BPNODE_CLA_IOD_COMMAND_RUN_IN_EID       182
#define BPNODE_CLA_IOD_FINDNAME_OUT_EID         185
#define BPNODE_CLA_IOD_COMMAND_DIR_OUT_EID      186
#define BPNODE_CLA_IOD_COMMAND_RUN_OUT_EID      187

#endif /* BPNODE_EVENTS_H */
