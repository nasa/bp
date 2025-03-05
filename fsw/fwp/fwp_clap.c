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

BPLib_Status_t BPA_CLAP_ContactStart(BPLib_CLA_ContactsSet_t ContactInfo)
{
    int32_t OtherStatus;
    BPLib_Status_t Status;
    BPLib_CLA_ContactRunState_t RunState;

    /* Default to success */
    Status = BPLIB_SUCCESS;

    /* Verify that the contact is in the correct state before starting it */
    RunState = BPLib_CLA_GetContactRunState(ContactInfo.ContactId);
    if (RunState != BPLIB_CLA_TORNDOWN)
    {


        if (Status == BPLIB_SUCCESS)
        {
            /* Set the run state to started */
            Status = BPLib_CLA_SetContactRunState(ContactInfo.ContactId, BPLIB_CLA_STARTED);
        }
    }
    else
    {
        Status = BPLIB_CLA_INCORRECT_STATE;

        BPLib_EM_SendEvent(BPNODE_CLAP_INIT_STATE_DBG_EID, BPLib_EM_EventType_ERROR,
                            "[Contact ID #%d]: Task must be setup before starting",
                            ContactInfo.ContactId);
    }

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactStop(void)
{
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    return Status;
}

BPLib_Status_t BPA_CLAP_ContactTeardown(void)
{
    BPLib_Status_t Status;

    Status = BPLIB_SUCCESS;

    return Status;
}