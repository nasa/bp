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
 *   Header file for CLA Bundle IO.
 */

#ifndef BPNODE_CLA_BUNDLE_IO_H
#define BPNODE_CLA_BUNDLE_IO_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bplib.h"
#include "iodriver_base.h"
#include "iodriver_packet_io.h"
#include "bpnode_eventids.h"

/************************************************
 * Defines
 ************************************************/
#define BPNODE_NUM_OF_CLA_THREAD_PAIR       2     
#define BPNODE_CLA_PSP_DRIVER_NAME          "unsock_intf"
#define BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL    0
#define BPNODE_CLA_PSP_INPUT_SUBCHANNEL     1
#define BPNODE_CLA_PSP_BUFFER_SIZE          3072


/************************************************
 * Typedefs
 ************************************************/
typedef struct
{
    CFE_PSP_IODriver_Direction_t Dir;
    CFE_ES_TaskId_t              ServiceTask;
    CFE_PSP_IODriver_Location_t  PspLocation;
    size_t                       CurrentBufferSize;
    uint8_t                      BundleBuffer[BPNODE_CLA_PSP_BUFFER_SIZE];

} BPNODE_CLA_BundleFlowEntry_t;

/************************************************
 * Exported Functions
 ************************************************/
/**
 * \brief Initializes CLA
 *
 *  \par Description
 *       Initialize CLA, called from BPNode_AppInit()
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNODE_CLA_Init(void);

/**
 * \brief Setup CLA
 *
 *  \par Description
 *       Setup CLA parameters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 BPNODE_CLA_SetupLibIntf(void);

/**
 * \brief Create CLA Output UNIX socket communication
 *
 *  \par Description
 *       Create CLA Output UNIX socket communication 
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNODE_CLA_InitOutput(BPNODE_CLA_BundleFlowEntry_t *Entry);

/**
 * \brief Create CLA Input UNIX socket communication
 *
 *  \par Description
 *       Create CLA UNIX socket input communication
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNODE_CLA_InitInput(BPNODE_CLA_BundleFlowEntry_t *Entry);

/**
 * \brief Create CLA Output child task
 *
 *  \par Description
 *       Create CLA Output child task
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 */
void BPNODE_CLA_OutputTask(void);

/**
 * \brief Create CLA Input child task
 *
 *  \par Description
 *       Create CLA Input child task
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 */
void BPNODE_CLA_InputTask(void);

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
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNODE_CLA_ProcessBundleInput(BPNODE_CLA_BundleFlowEntry_t *Entry);

/**
 * \brief Process Bundle Output to CLA
 *
 *  \par Description
 *       Receive the bundle from Bundle Interface and send the bundle to CL. 
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNODE_CLA_ProcessBundleOutput(BPNODE_CLA_BundleFlowEntry_t *Entry);


#endif /* BPNODE_CLA_BUNDLE_IO_H */

