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
 * \brief Number of generic worker jobs to process per cycle
 */
#define BPNODE_NUM_JOBS_PER_CYCLE           (1000)

/**
 * \brief Number of bundles to enqueue for egress (from cache) during our wakeup
 *        To prevent the generic worker job list from completely filling
 *        (which allows us to receive new bundles/adus)
 *        this number should be less than BPNODE_NUM_JOBS_PER_CYCLE
 */
#define BPNODE_MAX_BUNDLES_TO_ENQUEUE_DURING_CACHE_SCAN (BPNODE_NUM_JOBS_PER_CYCLE / 2)

/**
 * \brief Size of BPLib's Memory Pool, in bytes
 */
#define BPNODE_MEM_POOL_LEN               (8000000u)

/**
 * \brief Maximum number of simultaneous unsorted BPLib generic worker jobs
 */
#define BPNODE_MAX_UNSORTED_JOBS          (1024u)

/**
 *  \brief Maximum number of bundles to forward per wakeup 
 */
#define BPNODE_CLA_OUT_MAX_BUNDLES_PER_CYCLE  (200u)

/** 
 * \brief Maximum number of bundles to receive per wakeup 
 */
#define BPNODE_CLA_IN_MAX_BUNDLES_PER_CYCLE  (200u)           

/** @} */


/** 
 * @defgroup Child task priorities
 * \brief    These are listed in the order of their relative priorities. Generic workers
 *           have first priority since moving the bundles through BPNode is top priority.
 *           Out tasks have a higher priority than In tasks so that memory can be freed up
 *           for new incoming bundles. ADU tasks have higher priority than CLA tasks to
 *           prioritize delivering ADUs to their final destination.
 * @{
 */

 /**
 * \brief Generic Worker Task priority base 
 *          The worker ID is added to the base value to get the final task priority
 */
#define BPNODE_GEN_WRKR_PRIORITY_BASE  (100u)

/**
 * \brief ADU Out Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_OUT_PRIORITY_BASE (BPNODE_GEN_WRKR_PRIORITY_BASE + BPNODE_NUM_GEN_WRKR_TASKS)

/**
 * \brief CLA Out Task priority base 
 *          The Contact ID is added to the base value to get the final task priority
 */
#define BPNODE_CLA_OUT_PRIORITY_BASE (BPNODE_ADU_OUT_PRIORITY_BASE + BPLIB_MAX_NUM_CHANNELS)

 /**
 * \brief ADU In Task priority base 
 *          The channel ID is added to the base value to get the final task priority
 */
#define BPNODE_ADU_IN_PRIORITY_BASE  (BPNODE_CLA_OUT_PRIORITY_BASE + BPLIB_MAX_NUM_CONTACTS)

/**
 * \brief CLA In Task priority base 
 *          The Contact ID is added to the base value to get the final task priority
 */
#define BPNODE_CLA_IN_PRIORITY_BASE  (BPNODE_ADU_IN_PRIORITY_BASE + BPLIB_MAX_NUM_CHANNELS)

/** @} */


#endif /* BPNODE_PLATFORM_CFG_H */
