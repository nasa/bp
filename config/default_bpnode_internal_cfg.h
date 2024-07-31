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
 *   BPNode Application Private Configuration Definitions
 *
 * This provides default values for configurable items that are internal
 * to this module and do NOT affect the interface(s) of this module.  Changes
 * to items in this file only affect the local module and will be transparent
 * to external entities that are using the public interface(s).
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */

#ifndef BPNODE_INTERNAL_CFG_H
#define BPNODE_INTERNAL_CFG_H

/*
** Macro Definitions
*/

#define BPNODE_CMD_PIPE_DEPTH               (32)    /* Depth of command pipe */

#define BPNODE_WAKEUP_PIPE_DEPTH            (1)     /* Depth of wakeup pipe */

#define BPNODE_WAKEUP_PIPE_TIMEOUT          (1200)  /* Timeout of wakeup pipe */

#define BPNODE_NUMBER_OF_TABLES             (1)     /* Number of table(s) */

#define BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE  (-1)    /* Table validation error code */

#define BPNODE_TBL_ELEMENT_1_MAX            (10)    /* Table entry maximum value */

#endif /* BPNODE_INTERNAL_CFG_H */
