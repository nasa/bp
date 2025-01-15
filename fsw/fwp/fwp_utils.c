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

#include "fwp_utils.h"

inline BPLib_Status_t BPA_CFE_Status_Translate(CFE_Status_t CFE_Status)     { return (CFE_Status & CFE_SEVERITY_SUCCESS) ? BPLIB_SUCCESS : BPLIB_ERROR; }
inline CFE_Status_t BPA_BPLib_Status_Translate(BPLib_Status_t BPLib_Status) { return (BPLib_Status >= BPLIB_SUCCESS) ? CFE_SUCCESS : CFE_STATUS_NOT_IMPLEMENTED; }