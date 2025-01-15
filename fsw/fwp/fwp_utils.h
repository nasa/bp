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

#ifndef FWP_UTILS_H
#define FWP_UTILS_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "bplib_api_types.h"

/* ====== */
/* Macros */
/* ====== */

#define BPA_MAX_CFE_SUCCESS (0x0FFFFFFFu)

/* =================== */
/* Function Prototypes */
/* =================== */

/**
 * \brief     Convert a cFE status type return code to a BPLib status type
 * \details   The passed in cFE status is compared to BPA_MAX_CFE_SUCCESS to determine success
 * \note      This assume all success return codes from cFE have a MSB of 0
 * \param[in] CFE_Status (CFE_Status_t) cFE status to convert to BPLib_Status_t equivalent
 * \return    Execution status translated into a BPLib_Status_t type
 * \anchor    BPA_CFE_Status_Translate
 */
BPLib_Status_t BPA_CFE_Status_Translate(CFE_Status_t CFE_Status);

/**
 * \brief     Convert a BPLib status type return code to a cFE status type
 * \details   The passed in BPLib status is compared to BPLIB_SUCCESS to determine success
 * \note      This assume all success return codes from BPLib are positive
 * \param[in] BPLib_Status (BPLib_Status_t) BPLib status to convert to CFE_Status_t equivalent
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_BPLib_Status_Translate
 */
CFE_Status_t BPA_BPLib_Status_Translate(BPLib_Status_t BPLib_Status);

#endif /* FWP_UTILS_H */