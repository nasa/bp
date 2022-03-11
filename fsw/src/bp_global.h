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

#ifndef BP_GLOBAL_H
#define BP_GLOBAL_H

#include "cfe_mission_cfg.h"
#include "bp_platform_cfg.h"

#include "cfe_sb_api_typedefs.h"
#include "cfe_evs_api_typedefs.h"
#include "cfe_tbl_api_typedefs.h"

#include "bp_msg.h"
#include "bp_flow.h"
#include "bp_io.h"
#include "bp_storage.h"

/************************************************
 * BP application global data
 ************************************************/

#define BP_MAX_EVENT_FILTERS 9

typedef struct
{
    CFE_SB_PipeId_t     AppPipe;
    BP_HkPkt_t          HkPkt;
    BP_FlowHkPkt_t      FlowHkPkt;
    CFE_EVS_BinFilter_t EventFilters[BP_MAX_EVENT_FILTERS];
    char                AppName[CFE_MISSION_MAX_API_LEN];
    char                ConfigTableName[CFE_TBL_MAX_FULL_NAME_LEN];

    CFE_ResourceId_t LastFlowHandle;
    CFE_ResourceId_t LastIoHandle;
    CFE_ResourceId_t LastStorageHandle;

    /* Flow Table */
    CFE_TBL_Handle_t   FlowTableHandle;
    BP_FlowCtrlEntry_t FlowControl[BP_MAX_FLOWS];
    BP_FlowHandle_t    FlowPriorityHead;

    /* Storage Services */
    BP_StorageService_t StorageServices[BP_NUM_STORAGE_SERVICES];

    /* Throttling */
    BP_Throttle_t Throttles[BP_NUM_SEM_THROTTLES];

    /* I/O Control */
    BP_IOCtrl_t IOControl[BP_MAX_IO_DESC];

} BP_GlobalData_t;

extern BP_GlobalData_t BP_GlobalData;

#endif /* BP_GLOBAL_H */
