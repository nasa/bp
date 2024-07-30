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
 * Main header file for the BPNode app
 */

#ifndef BPNODE_APP_H
#define BPNODE_APP_H


/*
** Include Files
*/

#include "cfe.h"
#include "cfe_config.h"

#include "bpnode_mission_cfg.h"
#include "bpnode_platform_cfg.h"

#include "bpnode_perfids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

/*
** Type Definitions
*/

/** 
** \brief Global Data
*/
typedef struct
{
    uint32 RunStatus;                       /**< \brief Run status for main processing loop */

    CFE_SB_PipeId_t CommandPipe;            /**< \brief Pipe Id for command pipe */
    CFE_SB_PipeId_t WakeupPipe;             /**< \brief Pipe Id for wakeup pipe */

    BPNode_ExampleTable_t *ExampleTblPtr;   /**< \brief Pointer to example table */
    CFE_TBL_Handle_t TblHandles[BPNODE_NUMBER_OF_TABLES];   /**< \brief Table handles */

    BPNode_NodeMibCountersHkTlm_t NodeMibCountersHkTlm;     /**< \brief Node MIB Counters housekeeping packet */

} BPNode_AppData_t;


/*
** Global Data
*/

extern BPNode_AppData_t BPNode_AppData;


/*
** Exported Functions
*/

/**
 * \brief cFS Bundle Protocol Node (BPNode) application entry point
 *
 *  \par Description
 *       BPNode application entry point and main process loop
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_AppMain(void);

/**
 * \brief Initializes the BPNode application
 *
 *  \par Description
 *       BPNode application initialization routine. This
 *       function performs all the required startup steps to
 *       initialize BPNode data structures and get the application
 *       registered with the cFE services so it can begin to receive
 *       command messages.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AppInit(void);

/**
 * \brief Perform regular wakeup processing
 *
 *  \par Description
 *       This function performs the regular BPNode wakeup routine, including
 *       managing tables and checking the command pipe for new commands.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_WakeupProcess(void);

#endif /* BPNODE_APP_H */
