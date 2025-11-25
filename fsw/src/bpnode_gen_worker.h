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
 * National Aeronautics and Space Administration. All Rights Reserved. */

/**
 * @file
 *   This file contains the function definitions for the Generic Worker Task(s)
 */

#ifndef BPNODE_GEN_WORKER_H
#define BPNODE_GEN_WORKER_H

/*
** Include Files
*/

#include "cfe.h"


/*
** Macro Definitions
*/

#define BPNODE_GEN_WRKR_SEM_BASE_NAME        "BPN_GEN_WRKR_SEM" /** \brief Semaphore base name */
#define BPNODE_GEN_WRKR_BASE_NAME            "BPNODE.GEN_WRKR"  /** \brief Task base name */
#define BPNODE_GEN_WRKR_SEM_INIT_WAIT_MSEC   (2000u)            /** \brief Wait time for init semaphore take, in milliseconds */

/*
** Type Definitions
*/

/**
** \brief Generic Worker Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t TaskId;
    osal_id_t       InitSemId;
    osal_id_t       ExitSemId;
    uint32          PerfId;
    uint32          RunStatus;
    int32           BPLibWorkerId;
} BPNode_GenWorkerData_t;


/*
** Exported Functions
*/

/**
 * \brief Create Generic Worker Task(s)
 *
 *  \par Description
 *       Initialize init semaphore, then create the child task(s)
 *
 *  \par Assumptions, External Events, and Notes:
 *       - Note: This is the only function in this file called by the main task, all other
 *         functions are called by the child task(s)
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval OSAL or cFE error code
 */
int32 BPNode_GenWorkerCreateTasks(void);

/**
 * \brief Initialize provided Generic Worker task
 *
 *  \par Description
 *       Initialize provided Generic Worker task
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] WorkerId Pointer to worker ID to set
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval OSAL or cFE error code
 */
int32 BPNode_GenWorker_TaskInit(uint8 *WorkerId);

/**
 * \brief Generic Worker Main Task
 *
 *  \par Description
 *       Generic Worker task main loop.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_GenWorker_AppMain(void);

/**
 * \brief Exit provided Generic Worker task
 *
 *  \par Description
 *       Exit Generic Worker task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] WorkerId Worker ID for this task
 */
void BPNode_GenWorker_TaskExit(uint8 WorkerId);


#endif /* BPNODE_GEN_WORKER_H */
