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

#ifndef BP_TLMCFG_H
#define BP_TLMCFG_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"

/************************************************
 * Typedefs
 ************************************************/

#ifdef CFE_EDS_ENABLED_BUILD

#include "bp_eds_typedefs.h"

#else

/*
 * Supplemental Housekeeping Packet Structure
 */
typedef struct
{
    uint32 NumFreeFlashBlocks;
    uint32 NumUsedFlashBlocks;
    uint32 NumFailFlashBlocks;
    uint32 FlashErrorCount;
} BP_CustomTlm_t;

#endif

/************************************************
 * Exported Functions
 ************************************************/

void BP_PopulateCustomTlm(BP_CustomTlm_t *custom_tlm);
void BP_ClearCustomTlm(BP_CustomTlm_t *custom_tlm);

#endif /* BP_TLMCFG_H */
