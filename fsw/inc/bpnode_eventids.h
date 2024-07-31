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

#define BPNODE_RESERVED_EID         0
#define BPNODE_INIT_INF_EID         1
#define BPNODE_CC_ERR_EID           2
#define BPNODE_NOOP_INF_EID         3
#define BPNODE_RESET_INF_EID        4
#define BPNODE_MID_ERR_EID          5
#define BPNODE_CMD_LEN_ERR_EID      6
#define BPNODE_PIPE_ERR_EID         7
#define BPNODE_VALUE_INF_EID        8
#define BPNODE_CR_CMD_PIPE_ERR_EID  9
#define BPNODE_CR_WKP_PIPE_ERR_EID 10
#define BPNODE_SUB_HK_ERR_EID      11
#define BPNODE_SUB_CMD_ERR_EID     12
#define BPNODE_SUB_WKP_ERR_EID     13
#define BPNODE_TBL_REG_ERR_EID     14
#define BPNODE_TBL_LD_ERR_EID      15
#define BPNODE_TBL_ADDR_ERR_EID    16
#define BPNODE_TBL_MNG_ERR_EID     17

#endif /* BPNODE_EVENTS_H */
