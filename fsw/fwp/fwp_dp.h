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
 *   This file contains the prototypes for the BPNode command-handling functions
 */

#ifndef FWP_DP_H
#define FWP_DP_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe_error.h"
#include "bpnode_msg.h"
#include "bpnode_msgids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"

#include "bplib.h"
#include "bplib_eventids.h"

/* =================== */
/* Function Prototypes */
/* =================== */

/**
 * \brief Process a command pipe message
 *
 *  \par Description
 *       Processes a single software bus command pipe message. Checks
 *       the message ID and calls the appropriate routine
 *       to handle the message.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] SBBufPtr Pointer to Software Bus buffer
 */
void BPA_DP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr);

/**
 * \brief Process a command pipe message
 *
 *  \par Description
 *       Processes a single software bus command pipe message. Checks
 *       the command ID and calls the appropriate routine
 *       to handle the message.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] SBBufPtr Pointer to Software Bus buffer
 */
void BPA_DP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr);

/**
 * \brief Verify message length
 *
 *  \par Description
 *       Checks if the actual length of a software bus message matches
 *       the expected length and sends an error event if a mismatch
 *       occurs
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] MsgPtr         Message Pointer
 *  \param[in] ExpectedLength Expected length
 *
 *  \return Boolean message length matches response
 *  \retval true  Length matches expected
 *  \retval false Length does not match expected
 *
 */
bool BPA_DP_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

#endif /* FWP_DP_H */
