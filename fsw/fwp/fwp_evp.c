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
    size_t SpecLen;
    char EventStr[BPLIB_EM_MAX_MESSAGE_LENGTH];
    bool Truncated;

    // Default to indicating that the string is nottruncated
    Truncated = false;

    // Copy the max message length amount of characters to the final event string
    strncpy(EventStr, Spec, BPLIB_EM_MAX_MESSAGE_LENGTH);

    /* Verify that the max message length for EM is more than a truncation character and >= the pre-defined
       max length of an EVS message according to CFE */
    SpecLen = strlen(Spec);
    if (SpecLen >= BPLIB_EM_MAX_MESSAGE_LENGTH)
    {
        /* Put the truncation character at the end of the string,
           leaving a space for the null character */
        EventStr[BPLIB_EM_MAX_MESSAGE_LENGTH - 2u] = BPLIB_EM_MSG_TRUNCATED;

        // Indicate that truncation occured
        Truncated = true;
    }

    // Use host-specific event generator
    Status = CFE_EVS_SendEvent(EventID, EventType, "%s", EventStr);
    
    // Translate host specific return codes into BPLib return codes
    switch(Status)
    {
        case CFE_SUCCESS:
            /* BPLIB_EM_MSG_TRUNCATED == BPLIB_SUCCESS but for those
               in the meatspace this distinction is meaningful */
            Status = Truncated ? BPLIB_EM_MSG_TRUNCATED : BPLIB_SUCCESS;
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
