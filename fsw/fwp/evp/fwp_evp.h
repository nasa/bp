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

#ifndef BPNODE_EVP_CFS_H
#define BPNODE_EVP_CFS_H

/* ======== */
/* Includes */
/* ======== */
#include "cfe.h"
#include "bplib.h"

/* ======== */
/* Typedefs */
/* ======== */
#define BPNODE_EVP_MSG_TRUNCATED ('$')

// Defines the max length of the expanded event string
// Limits:
// - must be greater than or equal to 2
// - must be less than or equal to host limit (CFE_MISSION_EVS_MAX_MESSAGE_LENGTH)
#define BPNODE_EVP_MAX_MESSAGE_LENGTH (CFE_MISSION_EVS_MAX_MESSAGE_LENGTH)

/* ================== */
/* Exported Functions */
/* ================== */
BPL_Status_t BPA_EVP_Register(void);
BPL_Status_t BPA_EVP_SendEvent(uint16_t EventID, BPL_EM_EventType_t EventType,
                               char const * EventText, va_list EventTextArgPtr);

#endif /* BPNODE_EVP_CFS_H */
