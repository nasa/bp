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

/* TODO: Documentation */
BPLib_Status_t BPNode_CFE_Status_To_BPLib_Status(CFE_Status_t CFE_Status);

/* TODO: Documentation */
CFE_Status_t BPNode_BPLib_Status_To_CFE_Status(BPLib_Status_t BPLib_Status);

#endif /* FWP_UTILS_H */