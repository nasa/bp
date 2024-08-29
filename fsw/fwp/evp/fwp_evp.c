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
#include "cfe.h"
#include "fwp_evp.h"

/* ==================== */
/* Function definitions */
/* ==================== */

/* CFE_EVS_Register() proxy */
int64_t BPA_EVP_Register(const void* Filters, uint16_t NumEventFilters, uint16_t FilterScheme)
{
    return (int64_t) CFE_EVS_Register(Filters, NumEventFilters, FilterScheme);
}

/* CFE_EVS_SendEvent() proxy */
int64_t BPA_EVP_SendEvent(uint16_t EventID, uint16_t EventType,
                             char const* EventText, va_list EventTextArgPtr)
{
    BPL_Status_t ReturnStatus;
    CFE_Status_t ProxyStatus;
    uint16_t HostEventType = BPNODE_EVP_TranslateTypeToHost(EventType);
    char ExpandedEventText[BPNODE_EVP_MAX_MESSAGE_LENGTH];
    int ExpandedLength;

    /*
    ** Due to how we truncate the message if its too long (as seen in code, below),
    ** we need to ensure that this buffer is at least 2 characters long.
    */
    assert(BPNODE_EVP_MAX_MESSAGE_LENGTH >= 2);
    assert(BPNODE_EVP_MAX_MESSAGE_LENGTH <= CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    memset(&ExpandedEventText, 0, sizeof(ExpandedEventText));
    ExpandedLength = vsnprintf((char *)ExpandedEventText, sizeof(ExpandedEventText),
                               EventText, EventTextArgPtr);

    if (ExpandedLength >= (int)sizeof(ExpandedEventText))
    {
        /* Mark character before zero terminator to indicate truncation */
        ExpandedEventText[sizeof(ExpandedEventText) - 2u] = BPNODE_EVP_MSG_TRUNCATED;
        /*
        ** TODO: should we return an error here?
        ** Note: In the cFE implementation, they don't treat message truncation as an error.
        */
    }

    /* TODO: We'll probably want to remove this, or wrap it behind an "if debug" compiler flag. */
    OS_printf("BPNODE_EVP_SendEvent_Impl(%u, %s, %s)\n",
              EventID, BPL_EM_EventTypeToString(EventType), ExpandedEventText);

    ProxyStatus = CFE_EVS_SendEvent(EventID, HostEventType, "%s", ExpandedEventText);

    if (ProxyStatus != CFE_SUCCESS)
    {
        OS_printf("BPNODE_EVP_SendEvent_Impl CFE_EVS_SendEvent returned error status: 0x%08X!\n",
                  ProxyStatus);

        ReturnStatus.ReturnValue = BPL_STATUS_ERROR_GENERAL;
    }
    else
    {
        ReturnStatus.ReturnValue = BPL_STATUS_SUCCESS;
    }

    return ReturnStatus;
}
