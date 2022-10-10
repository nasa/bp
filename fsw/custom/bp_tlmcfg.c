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

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bp_cfg.h"
#include "bp_tlmcfg.h"
#include "bplib_store_flash.h"

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BP_PopulateCustomTlm
 *-----------------------------------------------*/
void BP_PopulateCustomTlm(BP_CustomTlm_t *custom_tlm)
{
    bp_flash_stats_t flash_stats;
    bplib_store_flash_stats(&flash_stats, false, false);
    custom_tlm->NumFreeFlashBlocks = flash_stats.num_free_blocks;
    custom_tlm->NumUsedFlashBlocks = flash_stats.num_used_blocks;
    custom_tlm->NumFailFlashBlocks = flash_stats.num_fail_blocks;
    custom_tlm->FlashErrorCount    = flash_stats.error_count;
}

/*-----------------------------------------------
 * BP_ClearCustomTlm
 *-----------------------------------------------*/
void BP_ClearCustomTlm(BP_CustomTlm_t *custom_tlm)
{
    (void)custom_tlm;
    bplib_store_flash_stats(NULL, false, true);
}
