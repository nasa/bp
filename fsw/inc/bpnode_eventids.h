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
#define BPNODE_ADU_START_SUB_DBG_EID            23
#define BPNODE_ADU_STOP_UNSUB_DBG_EID           26
#define BPNODE_ADU_IN_TOO_BIG_ERR_EID           27
#define BPNODE_ADU_IN_INIT_SEM_ERR_EID          28
#define BPNODE_ADU_IN_NOTIF_ERR_EID             29
#define BPNODE_ADU_IN_EXIT_SEM_ERR_EID          30
#define BPNODE_ADU_IN_CREATE_ERR_EID            31
#define BPNODE_ADU_IN_RUN_ERR_EID               32
#define BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID       33
#define BPNODE_ADU_IN_INIT_INF_EID              34
#define BPNODE_ADU_IN_EXIT_CRIT_EID             35
#define BPNODE_ADU_IN_NO_ID_ERR_EID             36
#define BPNODE_ADU_IN_INV_ID_ERR_EID            37
#define BPNODE_ADU_IN_CR_PIPE_ERR_EID           38
#define BPNODE_ADU_IN_UNK_EXIT_CRIT_EID         39
#define BPNODE_ADU_OUT_INIT_SEM_ERR_EID         40
#define BPNODE_ADU_OUT_NOTIF_ERR_EID            41
#define BPNODE_ADU_OUT_EXIT_SEM_ERR_EID         42
#define BPNODE_ADU_OUT_CREATE_ERR_EID           43
#define BPNODE_ADU_OUT_RUN_ERR_EID              44
#define BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID      45
#define BPNODE_ADU_OUT_INIT_INF_EID             46
#define BPNODE_ADU_OUT_EXIT_CRIT_EID            47
#define BPNODE_ADU_OUT_NO_ID_ERR_EID            48
#define BPNODE_ADU_OUT_INV_ID_ERR_EID           49
#define BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID        50
#define BPNODE_TIME_INIT_ERR_EID                51
#define BPNODE_TIME_WKP_ERR_EID                 52
#define BPNODE_CLAP_IO_FIND_NAME_DBG_EID        53
#define BPNODE_CLAP_IO_PORT_DBG_EID             54
#define BPNODE_CLAP_IO_IP_DBG_EID               55
#define BPNODE_CLAP_IO_DIR_DBG_EID              56
#define BPNODE_CLAP_IO_SET_RUN_DBG_EID          57
#define BPNODE_CLAP_INIT_SEM_DBG_EID            58
#define BPNODE_CLAP_INIT_STATE_DBG_EID          59

#define BPNODE_CLA_IN_FIND_NAME_ERR_EID         61
#define BPNODE_CLA_IN_CFG_PORT_ERR_EID          62
#define BPNODE_CLA_IN_CFG_IP_ERR_EID            63
#define BPNODE_CLA_IN_CFG_DIR_ERR_EID           64
#define BPNODE_CLA_IN_CFG_SET_RUN_ERR_EID       65
#define BPNODE_CLA_IN_INIT_SEM_ERR_EID          66
#define BPNODE_CLA_IN_INIT_INF_EID              67
#define BPNODE_CLA_IN_NOTIF_ERR_EID             68
#define BPNODE_CLA_IN_EXIT_SEM_ERR_EID          69
#define BPNODE_CLA_IN_CREATE_ERR_EID            70
#define BPNODE_CLA_IN_RUN_ERR_EID               71
#define BPNODE_CLA_IN_EXIT_CRIT_EID             72
#define BPNODE_CLA_IN_NO_ID_ERR_EID             73
#define BPNODE_CLA_IN_INV_ID_ERR_EID            74
#define BPNODE_CLA_IN_SETUP_INF_EID             76
#define BPNODE_CLA_IN_UNK_EXIT_CRIT_EID         77
#define BPNODE_CLA_IN_IO_READ_ERR_EID           81

#define BPNODE_CLA_OUT_LIB_LOAD_ERR_EID         82
#define BPNODE_CLA_OUT_FIND_NAME_ERR_EID        83
#define BPNODE_CLA_OUT_CFG_PORT_ERR_EID         84
#define BPNODE_CLA_OUT_CFG_IP_ERR_EID           85
#define BPNODE_CLA_OUT_CFG_DIR_ERR_EID          86
#define BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID      87
#define BPNODE_CLA_OUT_INIT_SEM_ERR_EID         88
#define BPNODE_CLA_OUT_INIT_INF_EID             89
#define BPNODE_CLA_OUT_NOTIF_ERR_EID            90
#define BPNODE_CLA_OUT_EXIT_SEM_ERR_EID         91
#define BPNODE_CLA_OUT_CREATE_ERR_EID           92
#define BPNODE_CLA_OUT_RUN_ERR_EID              93
#define BPNODE_CLA_OUT_EXIT_CRIT_EID            94
#define BPNODE_CLA_OUT_NO_ID_ERR_EID            95
#define BPNODE_CLA_OUT_DEL_EXIT_SEM_ERR_EID     96
#define BPNODE_CLA_OUT_DEL_INIT_SEM_ERR_EID     97
#define BPNODE_CLA_OUT_INV_ID_ERR_EID           98
#define BPNODE_CLA_OUT_SETUP_INF_EID            99
#define BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID        100

#define BPNODE_GEN_WRKR_SEM_CR_ERR_EID          101
#define BPNODE_GEN_WRKR_EXIT_SEM_ERR_EID        102
#define BPNODE_GEN_WRKR_CREATE_ERR_EID          103
#define BPNODE_GEN_WRKR_RUN_ERR_EID             104
#define BPNODE_GEN_WRKR_SEM_INIT_ERR_EID        105
#define BPNODE_GEN_WRKR_INIT_INF_EID            106
#define BPNODE_GEN_WRKR_EXIT_CRIT_EID           107
#define BPNODE_GEN_WRKR_NO_ID_ERR_EID           108
#define BPNODE_GEN_WRKR_INV_ID_ERR_EID          109
#define BPNODE_GEN_WRKR_UNK_EXIT_CRIT_EID       110
#define BPNODE_GEN_WRKR_NOTIF_ERR_EID           111
#define BPNODE_GEN_WRKR_REGISTER_ERR_EID        112
#define BPNODE_GEN_WRKR_TASKRUN_ERR_EID         113

#define BPNODE_NC_AS_INIT_ERR_EID               114
#define BPNODE_DP_SEND_NODE_CNTRS_ERR_EID       115
#define BPNODE_ADU_REM_CHAN_ERR_EID             116
#define BPNODE_ADU_REM_STAT_ERR_EID             117
#define BPNODE_DEL_HANDLER_ERR_EID              118
#define BPNODE_QM_INIT_ERR_EID                  122
#define BPNODE_MEM_INIT_ERR_EID                 123
#define BPNODE_ADU_OUT_PI_OUT_ERR_EID           124

#define BPNODE_TBL_UPDATE_INF_EID               125
#define BPNODE_NC_CFG_UPDATE_ERR_EID            126

#define BPNODE_INIT_SB_CONTACT_ERR_EID          127
#define BPNODE_INIT_NOTIF_CR_ERR_EID            128

#endif /* BPNODE_EVENTS_H */
