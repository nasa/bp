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

#ifndef FWP_EVP_H
#define FWP_EVP_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "bplib.h"

/* ====== */
/* Macros */
/* ====== */

/* This character is used as an indication
   that the input string was truncated due
   to reaching the host's size limit */
#define BPLIB_EM_MSG_TRUNCATED ('$')

/* Defines the max length of the expanded event string */
#define BPLIB_EM_MAX_MESSAGE_LENGTH (CFE_MISSION_EVS_MAX_MESSAGE_LENGTH)

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
 * \retval BPLIB_SUCCESS (0): Execution was successful
 * \retval BPLIB_EM_MSG_TRUNCATED (0): Event message string was truncated due to host limitations
 * \retval BPLIB_EM_INVALID_PARAMETER (-11u): Text to output is NULL
 * \retval BPLIB_EM_ILLEGAL_APP_ID (-8u): Unable to find caller's AppID
 * \retval BPLIB_EM_APP_NOT_REGISTERED (-12u): Application associated with AppID was not registered
 * \retval BPLIB_EM_APP_SQUELCHED (-13u): App is not filtered but squelch token doesn't allow event generation
 * \retval BPLIB_UNKNOWN (-2u): Unknown status
 */
BPLib_Status_t BPA_EVP_SendEvent(uint16_t EventID, BPLib_EM_EventType_t EventType, char const* EventText);

#endif /* FWP_EVP_H */
