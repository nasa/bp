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
 * National Aeronautics and Space Administration. All Rights Reserved.
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

#define BPNODE_CMD_MID_DEPTH                (8)    /* Default command pipe depth for a specific MID */

#define BPNODE_NUMBER_OF_TABLES             (12)     /* Number of table(s) */

#define BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE  (-1)    /* Table validation error code */

#endif /* BPNODE_INTERNAL_CFG_H */
