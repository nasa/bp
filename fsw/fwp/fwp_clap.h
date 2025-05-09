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

/* ====== */
/* Macros */
/* ====== */

#define BPNODE_CLA_SB_CONTACT_ID (BPLIB_MAX_NUM_CONTACTS - 1) /** \brief Make the last contact an SB contact */

/* =================== */
/* Function Prototypes */
/* =================== */

/**
  * \brief     Configure the given convergence layer, BPA Storage output queues, rate limit for
  *            sending and receiving bundles, the destination LTP engine ID (if LTP CL), and TCPCLP
  *            session (if TCPCLP for the given contact)
  * \note      As of right now, this function only sets up the port, IP address, and flow direction
  * \param[in] ContactId (uint32) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \param[in] ContactInfo (BPLib_CLA_ContactsSet_t) Entry from the Contacts Configuration. This variable
  *                                                  holds all the information needed to configure the
  *                                                  CLA tasks
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: A I/O driver API call failed operation
  */
BPLib_Status_t BPA_CLAP_ContactSetup(uint32 ContactId, BPLib_CLA_ContactsSet_t ContactInfo);

/**
  * \brief     Start transferring bundles between underlying network and BI
  * \param[in] ContactId (uint32) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: A UDP conntection couldn't be set to running
  */
BPLib_Status_t BPA_CLAP_ContactStart(uint32 ContactId);

/**
  * \brief     Send any CRS and custody signals under construction, stop transferring bundles to and from CL,
  *            request CL cancel transfers in progress, stop requesting BI for output queue bundles, and if
  *            LTP, request LTP cancel each active session and notify BI of complete and incomplete bundle
  *            transmissions
  * \param[in] ContactId (uint32) Index into the various contact info tracking arrays that corresponds
  *                               to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: Something went wrong while running CFE_PSP_IODriver_Command
  */
BPLib_Status_t BPA_CLAP_ContactStop(uint32 ContactId);

/**
  * \brief     Disestablish CLA, free all CLA resources, discard output queue, and delete custody timers
  * \param[in] ContactId (uint32) Index into the various contact info tracking arrays that corresponds
  *                                 to that contact's info
  * \return    void
  */
void BPA_CLAP_ContactTeardown(uint32 ContactId);

#endif /* FWP_CLAP_H */