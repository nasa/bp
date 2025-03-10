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

#include "fwp_clap.h"

/* =================== */
/* Function Prototypes */
/* =================== */

BPLib_Status_t BPA_CLAP_ContactSetup(BPLib_CLA_ContactsSet_t ContactInfo, uint32_t ContactId)
{
    BPLib_Status_t Status;

    /* Initialize CLA in */
    Status = BPNode_ClaIn_Setup(ContactId, ContactInfo.PortNum, ContactInfo.CLAddr);

    if (Status == BPLIB_SUCCESS)
    {
        /* Initialize CLA out */
        Status = BPNode_ClaOut_Setup(ContactId, ContactInfo.PortNum, ContactInfo.CLAddr);
    }

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactStart(uint32_t ContactId)
{
    BPLib_Status_t Status;

    /* Start CLA In */
    Status = BPNode_ClaIn_Start(ContactId);

    if (Status == BPLIB_SUCCESS)
    {
        /* Start CLA Out */
        Status = BPNode_ClaOut_Start(ContactId);
    }

    return Status;
}

void BPA_CLAP_ContactStop(uint32_t ContactId)
{
    BPNode_ClaIn_Stop(ContactId);
    BPNode_ClaOut_Stop(ContactId);

    return;
}

BPLib_Status_t BPA_CLAP_ContactTeardown(void)
{
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    return Status;
}