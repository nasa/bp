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
 * BPNode Application Platform Configuration Header File
 *
 * This is a compatibility header for the "platform_cfg.h" file that has
 * traditionally provided both public and private config definitions
 * for each CFS app.
 *
 * These definitions are now provided in two separate files, one for
 * the public/mission scope and one for internal scope.
 *
 * @note This file may be overridden/superceded by mission-provided definitions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */

#ifndef BPNODE_PLATFORM_CFG_H
#define BPNODE_PLATFORM_CFG_H

#include "bpnode_mission_cfg.h"
#include "bpnode_internal_cfg.h"

/** 
 * \brief Maximum number of channels that can be running at once
 *          This drives the number of entries in the channel and ADU proxy configuration
 *          tables, as well as the number of ADU In/Out tasks 
 */
#define BPNODE_MAX_NUM_CHANNELS     (1u)

/**
 * \brief ADU In Task stack size 
 */
#define BPNODE_ADU_IN_STACK_SIZE  (8192u)   

/**
 * \brief ADU Out Task stack size 
 */
#define BPNODE_ADU_OUT_STACK_SIZE (8192u)

/**
 * \brief ADU In Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_IN_PRIORITY_BASE  (100u)

/**
 * \brief ADU Out Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_OUT_PRIORITY_BASE (BPNODE_ADU_IN_PRIORITY_BASE + BPNODE_MAX_NUM_CHANNELS)

/**
 * \brief Number of milliseconds to wait when trying to take a semaphore
 */
#define BPNODE_SEM_WAIT_MSEC    (5000u)

#endif /* BPNODE_PLATFORM_CFG_H */
