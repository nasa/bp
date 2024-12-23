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
 *   This file contains the function definitions for the CLA Out Task(s)
 */


#ifndef BPNODE_CLA_OUT_H
#define BPNODE_CLA_OUT_H

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

#define BPNODE_CLA_OUT_SEM_BASE_NAME          "BPN_CLA_OUT"    /**< \brief Semaphore base name */
#define BPNODE_CLA_OUT_BASE_NAME              "BPNODE.CLA_OUT" /**< \brief Task base name */
#define BPNODE_CLA_OUT_SLEEP_MSEC             (1000u)          /**< \brief Sleep time */
#define BPNODE_CLA_OUT_PROC_BUNDLE_SLEEP_MSEC (250u)           /**< \brief Bundle processing Sleep time */
#define BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL      (0u)             /**< \briefIODriver Output subchannel*/
#define BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE     (3072u)          /**< \brief IODriver output buffer size*/
#define BPNODE_CLA_OUT_QUEUE_PEND_TIME        (100u)           /**< \brief Time to pend on bundle queue */

/*
** Type Definitions
*/

/** 
** \brief CLA Out Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t                 TaskId;
    osal_id_t                       InitSemId;
    osal_id_t                       WakeupSemId;
    osal_id_t                       ExitSemId;
    uint32                          PerfId;
    uint32                          RunStatus;
    bool                            EgressServiceEnabled;

    /* IODriver usock_intf related*/
    CFE_PSP_IODriver_Direction_t    Dir;
    CFE_PSP_IODriver_Location_t     PspLocation;
    size_t                          CurrentBufferSize;
    uint8_t                         BundleBuffer[BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE];
        
} BPNode_ClaOutData_t;


/*
** Exported Functions
*/

/**
 * \brief Create CLA Out Task(s)
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
int32 BPNode_ClaOutCreateTasks(void);

/**
 * \brief Initialize provided CLA Out task
 *
 *  \par Description
 *       Initialize provided CLA Out task
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ContId Pointer to contact ID to set
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval OSAL or cFE error code
 */
int32 BPNode_ClaOut_TaskInit(uint8 *ContId);

/**
 * \brief CLA Out Main Task
 *
 *  \par Description
 *       CLA Out task main loop. Pull bundle from BI queue and send to CL through UNIX socket.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_ClaOut_AppMain(void);

/**
 * \brief Exit provided CLA Out task
 *
 *  \par Description
 *       Exit CLA Out task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ContId Contact ID for this task
 */
void BPNode_ClaOut_TaskExit(uint8 ContId);

/**
 * \brief Process Bundle Output to CLA
 *
 *  \par Description
 *       Receive the bundle from Bundle Interface and send the bundle to CL. 
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ContId Contact ID
 * 
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 BPNode_ClaOut_ProcessBundleOutput(uint8 ContId);

#endif /* BPNODE_CLA_OUT_H */

