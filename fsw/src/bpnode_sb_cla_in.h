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

#ifndef BPNODE_SB_CLA_IN_H
#define BPNODE_SB_CLA_IN_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "iodriver_base.h"
#include "iodriver_packet_io.h"
#include "bplib.h"
#include "bpnode_platform_cfg.h"


/* ====== */
/* Macros */
/* ====== */

#define BPNODE_SB_CLA_IN_SEM_BASE_NAME          "BPN_SB_CLA_IN"    /** \brief Initialization semaphore base name */
#define BPNODE_SB_CLA_IN_BASE_NAME              "BPNODE.SB_CLA_IN" /** \brief Task base name */
#define BPNODE_SB_CLA_IN_SLEEP_MSEC             (1000u)            /** \brief Sleep time */
#define BPNODE_SB_CLA_IN_BUNDLE_PROC_SLEEP_MSEC (250u)             /** \brief Bundle processing Sleep time */
#define BPNODE_SB_CLA_PSP_INPUT_BUFFER_SIZE     (4096)             /** \brief IODriver buffer size*/
#define BPNODE_SB_CLA_IN_SEM_INIT_WAIT_MSEC     (2000u)            /** \brief Wait time for init semaphore take, in milliseconds */
#define BPNODE_SB_CLA_IN_SEM_WAKEUP_WAIT_MSEC   (1100u)            /** \brief Wait time for wakeup semaphore take, in milliseconds */


/* =================== */
/* Function Prototypes */
/* =================== */

/**
  * \brief     Create an SB CLA In tasks
  * \return    Execution status
  * \retval    CFE_SUCCESS: Successful execution
  * \retval    OS errors from OS_BinSemCreate
  * \retval    OS errors from OS_BinSemTimedWait
  * \retval    CFE errors from CFE_ES_CreateChildTask
  */
CFE_Status_t BPNode_SB_ClaInCreateTasks(void);

/**
 * \brief CLA In Main Task
 *
 *  \par Description
 *       CLA In task main loop. Receive bundles from CLs and pass them to Bundle Interface.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_SB_ClaIn_AppMain(void);

/** \brief Exit provided CLA In task
 *
 *  \par Description
 *       Exit CLA In task gracefully
 */
void BPNode_SB_ClaIn_TaskExit(void);

/**
  * \brief     Delete semaphores associated with the task with the given contact ID
  * \return    void
  */
void BPNode_SB_ClaIn_DeleteSems(void);

#endif /* BPNODE_SB_CLA_IN_H */