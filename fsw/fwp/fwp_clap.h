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

#ifndef FWP_CLAP_H
#define FWP_CLAP_H

/* ======== */
/* Includes */
/* ======== */

#include "bplib_cla.h"
#include "cfe.h"
#include "bpnode_eventids.h"
#include "bpnode_app.h"

/* =================== */
/* Function Prototypes */
/* =================== */

/**
  * \brief     Configure the given convergence layer, BPA Storage output queues, rate limit for
  *            sending and receiving bundles, the destination LTP engine ID (if LTP CL), and TCPCLP
  *            session (if TCPCLP for the given contact)
  * \note      As of right now, this function only sets up the port, IP address, and flow direction
  * \param[in] ContactInfo (BPLib_CLA_ContactsSet_t) Information about the contact to initialize
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: A I/O driver API call failed operation
  */
BPLib_Status_t BPA_CLAP_ContactSetup(BPLib_CLA_ContactsSet_t ContactInfo, uint32_t ContactId);

/**
  * \brief     Start transferring bundles between underlying network and BI
  * \note      TODO
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_UNKNOWN_CONTACT: Provided contact ID does not match a contact ID in the Contacts Configuration
  * \retval    BPLIB_CLA_INCORRECT_STATE: The intended run state for the contact with the provided contact ID was incompatible with the
  *                                       current run state of the contact
  * \retval    BPLIB_IO_ERROR: A I/O driver API call failed operation
  */
BPLib_Status_t BPA_CLAP_ContactStart(uint32_t ContactId);

/**
  * \brief     Send any CRS and custody signals under construction, stop transferring bundles to and from CL,
  *            request CL cancel transfers in progress, stop requesting BI for output queue bundles, and if
  *            LTP, request LTP cancel each active session and notify BI of complete and incomplete bundle
  *            transmissions
  * \note      TODO
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_ERROR: TODO
  */
BPLib_Status_t BPA_CLAP_ContactStop(uint32_t ContactId);

/**
  * \brief     Disestablish CLA, free all CLA resources, discard output queue, and delete custody timers
  * \note      TODO
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_ERROR: TODO
  */
BPLib_Status_t BPA_CLAP_ContactTeardown(void);

#endif /* FWP_CLAP_H */