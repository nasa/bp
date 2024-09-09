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


/*
** Macro Definitions
*/

#define BPNODE_ADU_IN_INIT_SEM_BASE_NAME "BPN_ADU_IN_INIT"

#define BPNODE_ADU_IN_EXIT_SEM_BASE_NAME "BPN_ADU_IN_EXIT"

#define BPNODE_ADU_IN_BASE_NAME "BPNODE.ADU_IN"


/*
** Type Definitions
*/

typedef struct
{
    CFE_ES_TaskId_t TaskId;
    osal_id_t       InitSemId;
    osal_id_t       ExitSemId;
    uint32          PerfId;
    uint32          RunStatus;
} BPNode_AduInData_t;


/*
** Exported Functions
*/

/**
 * \brief Create ADU In Task(s)
 *
 *  \par Description
 *       Initialize init and exit semaphores, then create the child task(s)
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
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
 *  \param[in] AduInId Pointer to ADU In ID to set
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval OSAL or cFE error code
 */
int32 BPNode_AduIn_TaskInit(uint32 *AduInId);

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
 *  \param[in] AduInId ADU In task ID
 */
void BPNode_AduIn_TaskExit(uint32 AduInId);


#endif /* BPNODE_ADU_IN_H */
