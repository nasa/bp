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
 *   This file contains the function definitions for the ADU Out Task(s)
 */

#ifndef BPNODE_ADU_OUT_H
#define BPNODE_ADU_OUT_H

/*
** Include Files
*/

#include "cfe.h"


/*
** Macro Definitions
*/

#define BPNODE_ADU_OUT_SEM_BASE_NAME        "BPN_ADU_OUT"    /** \brief Semaphore base name */
#define BPNODE_ADU_OUT_BASE_NAME            "BPNODE.ADU_OUT" /** \brief Task base name */
#define BPNODE_ADU_IN_PI_Q_TIMEOUT          (1000u)          /** \brief PI queue timeout */
#define BPNODE_ADU_OUT_SEM_INIT_WAIT_MSEC   (2000u)          /** \brief Wait time for init semaphore take, in milliseconds */
#define BPNODE_ADU_OUT_SEM_WAKEUP_WAIT_MSEC (1100u)          /** \brief Wait time for wakeup semaphore take, in milliseconds */
#define BPNODE_ADU_OUT_MAX_ADUS_PER_CYCLE   (10u)            /** \brief Maximum number of ADUs to egress per wakeup cycle */


/*
** Type Definitions
*/

/** 
** \brief ADU Out Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t TaskId;
    osal_id_t       InitSemId;
    osal_id_t       WakeupSemId;
    osal_id_t       ExitSemId;
    uint32          PerfId;
    uint32          RunStatus;
    bool            AduWrapping;
    CFE_SB_MsgId_t  SendToMsgId;
} BPNode_AduOutData_t;


/*
** Exported Functions
*/

/**
 * \brief Create ADU Out Task(s)
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
int32 BPNode_AduOutCreateTasks(void);

/**
 * \brief Initialize provided ADU Out task
 *
 *  \par Description
 *       Initialize provided ADU Out task
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
int32 BPNode_AduOut_TaskInit(uint8 *ChanId);

/**
 * \brief ADU Out Main Task
 *
 *  \par Description
 *       ADU Out task main loop. Extract ADUs from bundles and deliver to destination app.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_AduOut_AppMain(void);

/**
 * \brief Exit provided ADU Out task
 *
 *  \par Description
 *       Exit ADU Out task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ChanId Channel ID for this task
 */
void BPNode_AduOut_TaskExit(uint8 ChanId);


#endif /* BPNODE_ADU_OUT_H */
