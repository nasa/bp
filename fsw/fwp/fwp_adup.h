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


/*
** Type Definitions
*/

/** 
** \brief Application state
*/
typedef enum 
{
    BPA_ADUP_APP_STOPPED = 0,
    BPA_ADUP_APP_ADDED   = 1,
    BPA_ADUP_APP_STARTED = 2
} BPA_ADUP_ApplicationState_t;

/** 
** \brief State configurations needed for ADU ingest/output
*/
typedef struct 
{
    bool   AddAutomatically;
    BPA_ADUP_ApplicationState_t AppState;
} BPA_ADUP_State_t;

/** 
** \brief ADU Proxy Config Table Entry
*/
typedef struct
{
    CFE_SB_MsgId_t SendToMsgId;
    uint32         NumRecvFrmMsgIds;
    CFE_SB_MsgId_t RecvFrmMsgIds[BPNODE_MAX_CHAN_SUBSCRIPTION];
} BPA_ADUP_Config_t ;

/** 
** \brief ADU Proxy Config Table
*/
typedef struct
{
    BPA_ADUP_Config_t Entries[BPNODE_MAX_NUM_CHANNELS];
} BPA_ADUP_Table_t;


/*
** Exported Functions
*/

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
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Ingest was successful
 */
BPLib_Status_t BPA_ADUP_In(void *AduPtr, uint8_t ChanId);

/**
 * \brief FWP ADU Proxy Out
 *
 *  \par Description
 *       Sends an ADU out to the receiving application
 *
 *  \par Assumptions, External Events, and Notes:
 *       - The pointer type is void to allow implementations with different pointer types
 * 
 *  \param[in] AduPtr Pointer to the ADU
 *  \param[in] ChanId Channel ID
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_ADUP_Out(void *AduPtr, uint8_t ChanId);

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
BPLib_Status_t BPA_ADUP_AddApplication(uint8_t ChanId);

/**
 * \brief FWP ADU Proxy Start Application
 *
 *  \par Description
 *       Sets application state to started
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_StartApplication(uint8_t ChanId);

/**
 * \brief FWP ADU Proxy Stop Application
 *
 *  \par Description
 *       Sets application state to stopped
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ChanId Channel ID corresponding to an ADU Task ID
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Operation was successful
 */
BPLib_Status_t BPA_ADUP_StopApplication(uint8_t ChanId);

#endif /* FWP_ADUP_H */
