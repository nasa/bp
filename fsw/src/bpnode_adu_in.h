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
 *   This file contains the function definitions for the ADU In Task(s)
 */

#ifndef BPNODE_ADU_IN_H
#define BPNODE_ADU_IN_H

/*
** Include Files
*/

#include "cfe.h"
#include "fwp.h"


/*
** Macro Definitions
*/

#define BPNODE_ADU_IN_INIT_SEM_BASE_NAME "BPN_ADU_IN_INIT" /**< \brief Initialization semaphore base name */

#define BPNODE_ADU_IN_BASE_NAME      "BPNODE.ADU_IN"    /**< \brief Task base name */

#define BPNODE_ADU_IN_SLEEP_MSEC     (1000u)            /**< \brief Sleep time */

#define BPNODE_ADU_PIPE_DEPTH        (32u)              /**< \brief ADU pipe depth */

#define BPNODE_ADU_IN_PIPE_BASE_NAME "BPNODE_ADU_PIPE"  /**< \brief ADU pipe base name */

#define BPNODE_ADU_IN_SB_TIMEOUT     (1000u)            /**< \brief SB timeout */

#define BPNODE_ADU_IN_MAX_ADUS_PER_CYCLE (10u)          /**< \brief Maximum number of ADUs to ingest per wakeup cycle */

/*
** Type Definitions
*/

/** 
** \brief ADU In Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t TaskId;
    osal_id_t       InitSemId;
    uint32          PerfId;
    uint32          RunStatus;
    CFE_SB_PipeId_t AduPipe;
    bool            ClearPipe;
    bool            AduUnwrapping;
    uint32          MaxBundlePayloadSize;
    uint32          NumRecvFromMsgIds;
    CFE_SB_MsgId_t  RecvFromMsgIds[BPNODE_MAX_CHAN_SUBSCRIPTION];
} BPNode_AduInData_t;


/*
** Exported Functions
*/

/**
 * \brief Create ADU In Task(s)
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
int32 BPNode_AduInCreateTasks(void);

/**
 * \brief Initialize provided ADU In task
 *
 *  \par Description
 *       Initialize provided ADU In task
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ChanId Pointer to channel ID to set
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval OSAL or cFE error code
 */
int32 BPNode_AduIn_TaskInit(uint8 *ChanId);

/**
 * \brief ADU In Main Task
 *
 *  \par Description
 *       ADU In task main loop. Extract ADUs from bundles and deliver to destination app.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_AduIn_AppMain(void);

/**
 * \brief Exit provided ADU In task
 *
 *  \par Description
 *       Exit ADU In task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ChanId Channel ID for this task
 */
void BPNode_AduIn_TaskExit(uint8 ChanId);


#endif /* BPNODE_ADU_IN_H */
