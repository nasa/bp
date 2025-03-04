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

#define BPNODE_CLA_IN_SEM_BASE_NAME          "BPN_CLA_IN"    /** \brief Initialization semaphore base name */
#define BPNODE_CLA_IN_BASE_NAME              "BPNODE.CLA_IN" /** \brief Task base name */
#define BPNODE_CLA_IN_SLEEP_MSEC             (1000u)         /** \brief Sleep time */
#define BPNODE_CLA_IN_BUNDLE_PROC_SLEEP_MSEC (250u)          /** \brief Bundle processing Sleep time */
#define BPNODE_CLA_PSP_INPUT_SUBCHANNEL      (1u)            /** \brief IODriver unsock_intf input subchannel*/
#define BPNODE_CLA_PSP_INPUT_BUFFER_SIZE     (3072u)         /** \brief IODriver buffer size*/
#define BPNODE_CLA_IN_SEM_INIT_WAIT_MSEC     (2000u)         /** \brief Wait time for init semaphore take, in milliseconds */
#define BPNODE_CLA_IN_SEM_WAKEUP_WAIT_MSEC   (1100u)         /** \brief Wait time for wakeup semaphore take, in milliseconds */

#define BPNODE_CLA_IN_MAX_BUNDLES_PER_CYCLE  (10u)           /** \brief Maximum number of bundles to receive per wakeup */

/*
** Type Definitions
*/

/** 
** \brief CLA In Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t              TaskId;
    osal_id_t                    InitSemId;
    osal_id_t                    WakeupSemId;
    osal_id_t                    ExitSemId;
    uint32                       PerfId;
    bool                         IngressServiceEnabled;

    /* IODriver usock_intf related*/
    CFE_PSP_IODriver_Direction_t Dir;
    CFE_PSP_IODriver_Location_t  PspLocation;
    size_t                       CurrentBufferSize;
    uint8_t                      BundleBuffer[BPNODE_CLA_PSP_INPUT_BUFFER_SIZE];
        
} BPNode_ClaInData_t;


/*
** Exported Functions
*/

/**
 * \brief Create CLA In Task(s)
 *
 *  \par Description
 *       Initialize semaphores, then create the child task(s)
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
 *  \param[in] ContId Contact ID
 * 
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 BPNode_ClaIn_ProcessBundleInput(uint8 ContId);

#endif /* BPNODE_CLA_IN_H */

