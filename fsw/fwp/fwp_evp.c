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
 * National Aeronautics and Space Administration. All Rights Reserved.
 *
 */

/* ======== */
/* Includes */
/* ======== */

#include "fwp_evp.h"

/* ==================== */
/* Function definitions */
/* ==================== */

/* Proxy for event management initialization */
BPLib_Status_t BPA_EVP_Init(void)
{
    BPLib_Status_t Status;

    /* Call EVS Register with legacy default values */
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);

    switch(Status)
    {
        case CFE_SUCCESS:
            Status = BPLIB_SUCCESS;
            break;
        case CFE_EVS_APP_ILLEGAL_APP_ID:
            /* Returned by EVS_GetCurrentContext() from cfe_evs_utils.c */
            Status = BPLIB_EM_ILLEGAL_APP_ID;
            break;
        case CFE_EVS_UNKNOWN_FILTER:
            /* Returned by CFE_EVS_Register() from cfe_evs.c */
            Status = BPLIB_EM_UNKNOWN_FILTER;
            break;
        case CFE_ES_BAD_ARGUMENT:
            /* Returned by CFE_EVS_Register() from cfe_evs.c */
            Status = BPLIB_EM_BAD_ARGUMENT;
            break;
        default:
            Status = BPLIB_UNKNOWN;
            break;
    }

    return Status;
}

/* Proxy for software event generation */
BPLib_Status_t BPA_EVP_SendEvent(uint16_t EventID, BPLib_EM_EventType_t EventType, char const* Spec)
{
    BPLib_Status_t Status;
    uint16 CfeEventType;

    /* Translate BPLib event type to cFE event type */
    switch(EventType)
    {
        case BPLib_EM_EventType_DEBUG:
            CfeEventType = CFE_EVS_EventType_DEBUG;
            break;

        case BPLib_EM_EventType_INFORMATION:
            CfeEventType = CFE_EVS_EventType_INFORMATION;
            break;

        case BPLib_EM_EventType_WARNING:
            CfeEventType = CFE_EVS_EventType_INFORMATION;
            break;

        case BPLib_EM_EventType_CRITICAL:
            CfeEventType = CFE_EVS_EventType_CRITICAL;
            break;

        default:
            CfeEventType = CFE_EVS_EventType_ERROR;
            break;

    }

    // Use host-specific event generator
    Status = CFE_EVS_SendEvent(EventID, CfeEventType, "%s", Spec);

    // Translate host specific return codes into BPLib return codes
    switch(Status)
    {
        case CFE_SUCCESS:
            Status = BPLIB_SUCCESS;
            break;
        case CFE_EVS_INVALID_PARAMETER:
            /* Returned by CFE_EVS_SendEvent() from cfe_evs.c */
            Status = BPLIB_EM_INVALID_PARAMETER;
            break;
        case CFE_EVS_APP_ILLEGAL_APP_ID:
            /* Returned by EVS_GetCurrentContext() from cfe_evs_utils.c */
            Status = BPLIB_EM_ILLEGAL_APP_ID;
            break;
        case CFE_EVS_APP_NOT_REGISTERED:
            /* Returned by EVS_NotRegistered() from cfe_evs_utils.c */
            Status = BPLIB_EM_APP_NOT_REGISTERED;
            break;
        case CFE_EVS_APP_SQUELCHED:
            /* Returned by CFE_EVS_SendEvent() from cfe_evs.c */
            Status = BPLIB_EM_APP_SQUELCHED;
            break;
        default:
            Status = BPLIB_UNKNOWN;
            break;
    }

    return Status;
}
