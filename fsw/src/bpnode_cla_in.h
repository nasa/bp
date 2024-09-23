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
 * \file
 *   Header file for CLA Input.
 */

#ifndef BPNODE_CLA_IN_H
#define BPNODE_CLA_IN_H

/*
** Include Files
*/

#include "cfe.h"
#include "iodriver_base.h"
#include "iodriver_packet_io.h"
#include "bplib.h"


/*
** Macro Definitions
*/

#define BPNODE_CLA_IN_INIT_SEM_BASE_NAME    "BPN_CLA_IN_INIT"   /**< \brief Initialization semaphore base name */
#define BPNODE_CLA_IN_BASE_NAME             "BPNODE.CLA_IN"     /**< \brief Task base name */
#define BPNODE_CLA_IN_SLEEP_MSEC            (1000u)             /**< \brief Sleep time */
#define BPNODE_CLA_PIPE_DEPTH               (32u)               /**< \brief CLA pipe depth */
#define BPNODE_CLA_IN_PIPE_BASE_NAME        "BPNODE_CLA_PIPE"   /**< \brief CLA pipe base name */
#define BPNODE_CLA_PSP_INPUT_SUBCHANNEL     (1u)                /**< \brief IODriver unsock_intf input subchannel*/
#define BPNODE_CLA_PSP_INPUT_BUFFER_SIZE    (3072u)             /**< \brief IODriver buffer size*/

/*
** Type Definitions
*/

/** 
** \brief CLA In Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t                 TaskId;
    osal_id_t                       InitSemId;
    uint32                          PerfId;
    uint32                          RunStatus;
    bool                            IngressServiceEnabled;
    /* IODriver usock_intf related*/
    CFE_PSP_IODriver_Direction_t    Dir;
    CFE_PSP_IODriver_Location_t     PspLocation;
    size_t                          CurrentBufferSize;
    uint8_t                         BundleBuffer[BPNODE_CLA_PSP_INPUT_BUFFER_SIZE];
    
    BPLib_ContactsTable_t           ContactsTbl;
    
} BPNode_ClaInData_t;


/*
** Exported Functions
*/

/**
 * \brief Create CLA In Task(s)
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
int32 BPNode_ClaInCreateTasks(void);

/**
 * \brief Initialize provided CLA In task
 *
 *  \par Description
 *       Initialize provided CLA In task
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
int32 BPNode_ClaIn_TaskInit(uint8 *ChanId);

/**
 * \brief CLA In Main Task
 *
 *  \par Description
 *       CLA In task main loop. Receive bundles from CLs and pass them to Bundle Interface.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_ClaIn_AppMain(void);

/**
 * \brief Exit provided CLA In task
 *
 *  \par Description
 *       Exit CLA In task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ContId Contacts ID for this task
 */
void BPNode_ClaIn_TaskExit(uint8 ContId);

/**
 * \brief Process Bundle Input from CLA
 *
 *  \par Description
 *       Receive and process candidate bundle (bundle or control message) from CLA
 *       and pass the bundle to Bundle Interface. 
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] CLAIngress CLA input data
 *  \param[in] ContId Contact ID
 * 
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_CLA_ProcessBundleInput(BPNode_ClaInData_t *CLAIngress, uint8 ContId);

#endif /* BPNODE_CLA_IN_H */

