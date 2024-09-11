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
#include "bplib.h"


/*
** Type Definitions
*/

/** 
** \brief Application state
*/
typedef enum 
{
    BPA_ADUP_APP_STOPPED = 0,
    BPA_ADUP_APP_STARTED = 1
} BPA_ADUP_ApplicationState_t;

/** 
** \brief Configurations needed to ADU ingest/output
*/
typedef struct 
{
    bool     Packetize;
    uint32_t InPendTimeout;
    uint32_t OutPendTimeout;
    BPA_ADUP_ApplicationState_t AppState;
} BPA_ADUP_Configs_t;

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
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Ingest was successful
 */
BPLib_Status_t BPA_ADUP_In(void *AduPtr);

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
 * 
 *  \return Execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_ADUP_Out(void *AduPtr);

/**
 * \brief FWP ADU Proxy Add Application
 *
 *  \par Description
 *       Adds new application configurations
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] AppConfigs Pointer to the new application configurations
 */
void BPA_ADUP_AddApplication(BPA_ADUP_Configs_t *AppConfigs);

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
 */
void BPA_ADUP_StartApplication(uint8_t ChanId);

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
 */
void BPA_ADUP_StopApplication(uint8_t ChanId);

#endif /* FWP_ADUP_H */
