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

/************************************************************************
** Includes
*************************************************************************/

#include "bplib.h"
#include "bplib_store_ram.h"
#include "bplib_store_file.h"
#include "bplib_store_flash.h"
#include "bplib_flash_sim.h"

#include "cfe.h"
#include "bp_cfg.h"
#include "bp_flow.h"
#include "bp_events.h"

/************************************************************************
** Local Functions
*************************************************************************/

/*
 * File Storage Service - Init
 */
void local_store_file_init(void)
{
    bplib_store_file_init(NULL);
}

/*
 * File Storage Service - Create
 */
bp_handle_t local_store_file_create(int type, bp_ipn_t node, bp_ipn_t service, bool recover, void *parm)
{
    (void)parm;

    bp_file_attr_t attributes = {.root_path = BP_FILE_STORE_ROOT, .cache_size = BP_FILE_CACHE_SIZE};

    return bplib_store_file_create(type, node, service, recover, &attributes);
}

/*
 * Flash Storage Service - Init
 */
void local_store_flash_init(void)
{
    /* flash driver structure */
    static bp_flash_driver_t flash_driver = {
        .num_blocks      = FLASH_SIM_NUM_BLOCKS,
        .pages_per_block = FLASH_SIM_PAGES_PER_BLOCK,
        .page_size       = FLASH_SIM_PAGE_SIZE,
        .read            = bplib_flash_sim_page_read,
        .write           = bplib_flash_sim_page_write,
        .erase           = bplib_flash_sim_block_erase,
        .isbad           = bplib_flash_sim_block_is_bad,
        .phyblk          = bplib_flash_sim_physical_block,
    };

    /* initialize simulated flash device */
    bplib_flash_sim_initialize();

    /* initialize bplib flash store */
    bplib_store_flash_init(flash_driver, true);
}

/*
 * Flash Storage Service - Init
 */
bp_handle_t local_store_flash_create(int type, bp_ipn_t node, bp_ipn_t service, bool recover, void *parm)
{
    bp_flash_attr_t attr;

    if (type == BP_STORE_DATA_TYPE)
        attr.max_data_size = BP_MAX_BUNDLE_SIZE;
    else if (type == BP_STORE_DACS_TYPE)
        attr.max_data_size = FLASH_SIM_PAGE_SIZE;
    else if (type == BP_STORE_PAYLOAD_TYPE)
        attr.max_data_size = FLASH_SIM_PAGE_SIZE;

    return bplib_store_flash_create(type, node, service, recover, &attr);
}

/************************************************************************
** Init Function
*************************************************************************/

void BP_StorageService_Configure(void)
{
    bp_store_t StoreCfg;

    /* RAM */
    StoreCfg = (bp_store_t) {
        .create     = bplib_store_ram_create,
        .destroy    = bplib_store_ram_destroy,
        .enqueue    = bplib_store_ram_enqueue,
        .dequeue    = bplib_store_ram_dequeue,
        .retrieve   = bplib_store_ram_retrieve,
        .release    = bplib_store_ram_release,
        .relinquish = bplib_store_ram_relinquish,
        .getcount   = bplib_store_ram_getcount,
    };
    BP_StorageService_Register(BP_RAM_STORE, bplib_store_ram_init, &StoreCfg);

    /* File System */
    StoreCfg = (bp_store_t) {
        .create     = local_store_file_create,
        .destroy    = bplib_store_file_destroy,
        .enqueue    = bplib_store_file_enqueue,
        .dequeue    = bplib_store_file_dequeue,
        .retrieve   = bplib_store_file_retrieve,
        .release    = bplib_store_file_release,
        .relinquish = bplib_store_file_relinquish,
        .getcount   = bplib_store_file_getcount,
    };
    BP_StorageService_Register(BP_FILE_STORE, local_store_file_init, &StoreCfg);

    /* Flash Raw Block */
    StoreCfg = (bp_store_t) {
        .create     = local_store_flash_create,
        .destroy    = bplib_store_flash_destroy,
        .enqueue    = bplib_store_flash_enqueue,
        .dequeue    = bplib_store_flash_dequeue,
        .retrieve   = bplib_store_flash_retrieve,
        .release    = bplib_store_flash_release,
        .relinquish = bplib_store_flash_relinquish,
        .getcount   = bplib_store_flash_getcount,
    };
    BP_StorageService_Register(BP_FLASH_STORE, local_store_flash_init, &StoreCfg);
}
