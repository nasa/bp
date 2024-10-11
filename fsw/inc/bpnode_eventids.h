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

#endif /* BPNODE_EVENTS_H */
