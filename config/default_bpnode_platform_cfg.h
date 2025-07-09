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
#include "bplib.h"

/**
 * \brief Number of generic worker tasks to run
 *        Note: This should be set to the number of available CPU cores, having multiple
 *              generic worker tasks is only beneficial if there's more than one core.
 *              Until the cFE/OSAL supports spawning child tasks onto alternate CPU
 *              cores, it is recommended to keep this set to 1.
 */
#define BPNODE_NUM_GEN_WRKR_TASKS         (1)


/** 
 * @defgroup Child task stack sizes
 * @{
 */

/**
 * \brief ADU In Task stack size 
 */
#define BPNODE_ADU_IN_STACK_SIZE  (8192u)   

/**
 * \brief ADU Out Task stack size 
 */
#define BPNODE_ADU_OUT_STACK_SIZE (8192u)

/**
 * \brief CLA In Task stack size 
 */
#define BPNODE_CLA_IN_STACK_SIZE  (32768u)   

/**
 * \brief CLA Out Task stack size 
 */
#define BPNODE_CLA_OUT_STACK_SIZE (32768u)

/**
 * \brief Generic Worker Task stack size 
 */
#define BPNODE_GEN_WRKR_STACK_SIZE (8192u)

/** @} */

/** 
 * @defgroup Performance configurations
 * \brief    Note that adjusting these macros can radically change the performance of the
 *           system.
 * @{
 */

/**
 * \brief Maximum expected wakeups per second (10Hz by default). If this rate is
 *        expected to change in flight, this value should be set to the highest expected
 *        value, otherwise this should just be the default wakeup rate. Rates slower than
 *        once per second will require changing more than just this configuration and 
 *        are not recommended
 */
#define BPNODE_MAX_EXP_WAKEUP_RATE          (10)

/**
 * \brief Depth of wakeup pipe
 */
#define BPNODE_WAKEUP_PIPE_DEPTH            (BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief Message limit of wakeup pipe
 */
#define BPNODE_WAKEUP_PIPE_LIM              (BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief How many milliseconds to pend on wakeup pipe before timing out. Should be
 *        a bit longer than the wakeup period (so for a wakeup rate of every 100 msec,
 *        wait 120 msec for example)
 */
#define BPNODE_WAKEUP_PIPE_TIMEOUT          (1200 / BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief How many milliseconds child tasks wait for the start work notification before
 *        timing out. Should equal the length of the wakeup period.
 */
#define BPNODE_WAKEUP_WAIT_MSEC             (1000 / BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief How many milliseconds child tasks can wait for data to ingress/egress/work on
 *        before timing out. Should be shorter than a full wakeup period in order to keep
 *        the child task in sync with the start work notification.
 */
#define BPNODE_DATA_TIMEOUT_MSEC            (100 / BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief Number of generic worker jobs to process per cycle
 *        Note: Rule of thumb is 100,000 jobs per second for an ingress rate of 100Mbps.
 *              Current value is set for our maximum rate of 150Mbps
 */
#define BPNODE_NUM_JOBS_PER_CYCLE           (150000 / BPNODE_MAX_EXP_WAKEUP_RATE)

/**
 * \brief Size of BPLib's Memory Pool, in bytes
 */
#define BPNODE_MEM_POOL_LEN               (8000000u)

/**
 * \brief Maximum number of simultaneous unsorted BPLib generic worker jobs
 */
#define BPNODE_MAX_UNSORTED_JOBS          (1024u)


/** @} */


/** 
 * @defgroup Child task priorities
 * \brief    These are listed in the order of their relative priorities.
 *           CLA tasks have the highest priority since those rates are typically
 *           going to be higher than ADU rates. The Generic Worker task(s) also get
 *           high priority to ensure bundles get offloaded to storage quickly.
 * @{
 */

/**
 * \brief CLA In Task priority base 
 *          The Contact ID is added to the base value to get the final task priority
 */
#define BPNODE_CLA_IN_PRIORITY_BASE  (30u)

/**
 * \brief Generic Worker Task priority base 
 *          The worker ID is added to the base value to get the final task priority
 */
#define BPNODE_GEN_WRKR_PRIORITY_BASE  (BPNODE_CLA_IN_PRIORITY_BASE + BPLIB_MAX_NUM_CONTACTS)

/**
 * \brief CLA Out Task priority base 
 *          The Contact ID is added to the base value to get the final task priority
 */
#define BPNODE_CLA_OUT_PRIORITY_BASE (BPNODE_GEN_WRKR_PRIORITY_BASE + BPNODE_NUM_GEN_WRKR_TASKS)

/**
 * \brief ADU Out Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_OUT_PRIORITY_BASE (BPNODE_CLA_OUT_PRIORITY_BASE + BPLIB_MAX_NUM_CONTACTS)

/**
 * \brief ADU In Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_IN_PRIORITY_BASE  (BPNODE_ADU_OUT_PRIORITY_BASE + BPLIB_MAX_NUM_CHANNELS)


/** @} */


#endif /* BPNODE_PLATFORM_CFG_H */
