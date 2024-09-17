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

#ifndef FWP_EVP_H
#define FWP_EVP_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "bplib.h"

/* ================== */
/* Exported Functions */
/* ================== */

/**
 * \brief  FWP Event Management Proxy's Initialization
 * \par    Description
 *           Initialize event mangament
 * \par    Assumptions, External Events, and Notes:
 *           None
 * \return Execution status
 * \retval BPLIB_SUCCESS: Execution was successful
 * \retval BPLIB_EM_ILLEGAL_APP_ID: Unable to find caller's AppID
 * \retval BPLIB_EM_UNKNOWN_FILTER: Given filter scheme is not binary
 * \retval BPLIB_EM_BAD_ARGUMENT: Number of filters do not match filters given
 * \retval BPLIB_UNKNOWN: Unknown status
 */
BPLib_Status_t BPA_EVP_Init(void);

/**
 * \brief  FWP Event Management Proxy's Event Generation
 * \par    Description
 *           Generate a software event
 * \par    Assumptions, External Events, and Notes:
 *           None
 * \return Execution status
 * \retval BPLIB_SUCCESS: Execution was successful
 * \retval BPLIB_EM_INVALID_PARAMETER: Text to output is NULL
 * \retval BPLIB_EM_ILLEGAL_APP_ID: Unable to find caller's AppID
 * \retval BPLIB_EM_APP_NOT_REGISTERED: Application associated with AppID was not registered
 * \retval BPLIB_EM_APP_SQUELCHED: App is not filtered but squelch token doesn't allow event generation
 * \retval BPLIB_UNKNOWN: Unknown status
 */
BPLib_Status_t BPA_EVP_SendEvent(uint16_t EventID, BPLib_EM_EventType_t EventType, char const* EventText);

#endif /* FWP_EVP_H */
