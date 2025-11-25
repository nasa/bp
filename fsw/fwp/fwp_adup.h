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
 * \file
 *   This file contains the header definitions for the FWP ADU Proxy.
 */


#ifndef FWP_ADUP_H
#define FWP_ADUP_H

/*
** Include Files
*/

#include <stdint.h>
#include <stdbool.h>
#include "cfe.h"
#include "bplib.h"
#include "bpnode_platform_cfg.h"


/*
** Macro Definitions
*/

#define BPNODE_MAX_CHAN_SUBSCRIPTION     10     /**< \brief Max number of message IDs one channel can subscribe to */

/**
** \brief State configurations needed for ADU ingest/output
*/
typedef struct
{
    bool AddAutomatically;
} BPA_ADUP_State_t;

/**
** \brief ADU Proxy Config Table Entry
*/
typedef struct
{
    CFE_SB_MsgId_t SendToMsgId;
    uint32         NumRecvFrmMsgIds;
    uint32         MsgLims[BPNODE_MAX_CHAN_SUBSCRIPTION];
    CFE_SB_MsgId_t RecvFrmMsgIds[BPNODE_MAX_CHAN_SUBSCRIPTION];
} BPA_ADUP_Config_t ;

/**
** \brief ADU Proxy Config Table
*/
typedef struct
{
    BPA_ADUP_Config_t Entries[BPLIB_MAX_NUM_CHANNELS];
} BPA_ADUP_Table_t;


/*
** Exported Functions
*/

/**
 * \brief FWP ADU Proxy Validate Config Table
 *
 *  \par Description
 *       Validates the ADU Proxy Configuration Table
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] TblData ADU Proxy Configuration Table pointer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE Invalid table parameter
 */
CFE_Status_t BPA_ADUP_ValidateConfigTbl(void *TblData);

/**
 * \brief FWP ADU Proxy In
 *
 *  \par Description
 *       Ingests an ADU and sends it to PI
 *
 *  \par Assumptions, External Events, and Notes:
 *       - The pointer type is void to allow implementations with different pointer types
 *
 *  \param[in] AduPtr Pointer to the ADU
 *  \param[in] ChanId Channel ID
 *  \param[out] AduSize Size of ADU ingested
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Ingest was successful
 */
BPLib_Status_t BPA_ADUP_In(void *AduPtr, uint32_t ChanId, size_t *AduSize);

/**
 * \brief FWP ADU Proxy Out
 *
 *  \par Description
 *       Receives an ADU from PI and sends it onto the software bus, with optional header
 *       wrapping.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ChanId Channel ID
 *  \param[in] Timeout Total time to pend on channel out queue (in milliseconds)
 *  \param[out] AduSize Size of ADU egressed
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_ADUP_Out(uint32_t ChanId, uint32_t Timeout, size_t *AduSize);

/**
 * \brief FWP ADU Proxy Add Application
 *
 *  \par Description
 *       Adds new application configurations from ADU Proxy and Channel Config Tables
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_AddApplication(uint32_t ChanId);

/**
 * \brief FWP ADU Proxy Start Application
 *
 *  \par Description
 *       Subscribes to configured msgids and sets app state to started
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_StartApplication(uint32_t ChanId);

/**
 * \brief FWP ADU Proxy Stop Application
 *
 *  \par Description
 *       Unsubscribes from configured msgids and sets app state to stopped
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_StopApplication(uint32_t ChanId);

/**
 * \brief FWP ADU Proxy Remove Application
 *
 *  \par Description
 *       Sets application state to removed
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 *
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_RemoveApplication(uint32_t ChanId);

#endif /* FWP_ADUP_H */
