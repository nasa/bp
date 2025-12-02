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

#include "fwp_utils.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

BPLib_Status_t BPA_CFE_Status_Translate(CFE_Status_t CFE_Status)
{ 
    BPLib_Status_t Status;
    uint32_t ErrType = (CFE_Status & CFE_SEVERITY_BITMASK);

    Status = BPLIB_UNKNOWN;

    if (CFE_Status == CFE_TBL_INFO_UPDATED)
    {
        Status = BPLIB_TBL_UPDATED;
    }
    else if (ErrType == CFE_SEVERITY_ERROR)
    {
        Status = BPLIB_ERROR;
    }
    else if (ErrType == CFE_SEVERITY_INFO || ErrType == CFE_SEVERITY_SUCCESS)
    {
        Status = BPLIB_SUCCESS;
    }

    return Status;
}

CFE_Status_t BPA_BPLib_Status_Translate(BPLib_Status_t BPLib_Status)
{ 
    if (BPLib_Status >= BPLIB_SUCCESS)
    {
        return CFE_SUCCESS;
    }
    else
    {
        return CFE_STATUS_NOT_IMPLEMENTED;
    }
}