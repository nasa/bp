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

#include <string.h>
#include <assert.h>

#include "cfe.h"
#include "bp_msgids.h"
#include "bp_storecfg.h"
#include "bp_tlmcfg.h"
#include "bp_global.h"
#include "bp_app.h"
#include "bp_platform_cfg.h"
#include "bp_msg.h"
#include "bp_eventids.h"
#include "bp_version.h"
#include "bp_flow.h"
#include "bp_dispatch.h"
#include "bp_cla_bundle_io.h"
#include "bplib_routing.h"
#include "bpl_evm_api.h"
#include "bpnode_evt_cfs.h"

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

static CFE_Status_t BP_SetupLibrary(void)
{
    /* Initialize BP Library */
    if (bplib_init() != BP_SUCCESS)
    {
        fprintf(stderr, "%s(): bplib_init failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Test route table with 1MB of cache */
    BP_GlobalData.RouteTbl = bplib_route_alloc_table(10, 16 << 20);
    if (BP_GlobalData.RouteTbl == NULL)
    {
        fprintf(stderr, "%s(): bplib_route_alloc_table failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    BPL_EVM_ProxyCallbacks_t EventProxyCallbacks = {
        .Initialize_Impl = BPNODE_EVT_Initialize_Impl,
        .SendEvent_Impl = BPNODE_EVT_SendEvent_Impl,
    };

    (void) BPL_EVM_Initialize(EventProxyCallbacks);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * AppInit
 *-----------------------------------------------*/
static CFE_Status_t AppInit(void)
{
    static const CFE_EVS_BinFilter_t BP_EVENT_FILTER_INIT[] = {
        {BP_IO_SEND_ERR_EID, CFE_EVS_FIRST_8_STOP},   {BP_IO_RECEIVE_ERR_EID, CFE_EVS_FIRST_8_STOP},
        {BP_LIB_PROC_ERR_EID, CFE_EVS_FIRST_8_STOP},  {BP_LIB_LOAD_ERR_EID, CFE_EVS_FIRST_8_STOP},
        {BP_LIB_STORE_ERR_EID, CFE_EVS_FIRST_8_STOP}, {BP_LIB_ACCEPT_ERR_EID, CFE_EVS_FIRST_8_STOP},
        {BP_FILE_ERR_EID, CFE_EVS_FIRST_8_STOP},      {BP_STORE_INFO_EID, CFE_EVS_FIRST_8_STOP},
        {BP_BPLIB_INFO_EID, CFE_EVS_FIRST_8_STOP}};

    const size_t   num_event_filters = sizeof(BP_EVENT_FILTER_INIT) / sizeof(BP_EVENT_FILTER_INIT[0]);
    CFE_Status_t   status            = CFE_SUCCESS;
    CFE_ES_AppId_t app_id;

    assert(num_event_filters <= BP_MAX_EVENT_FILTERS);
    memset(&BP_GlobalData, 0, sizeof(BP_GlobalData));
    BP_GlobalData.Throttles = BP_Throttles;

    memcpy(BP_GlobalData.EventFilters, BP_EVENT_FILTER_INIT, sizeof(BP_EVENT_FILTER_INIT));

    /* Register Application with Event Services */
    status = CFE_EVS_Register(BP_GlobalData.EventFilters, num_event_filters, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
        return status;

    /* Get Application ID */
    status = CFE_ES_GetAppID(&app_id);
    if (status != CFE_SUCCESS)
        return status;

    /* Get Application Name */
    status = CFE_ES_GetAppName(BP_GlobalData.AppName, app_id, sizeof(BP_GlobalData.AppName));
    if (status != CFE_SUCCESS)
        return status;

    /* Initialize Software Bus Application Pipe */
    status = CFE_SB_CreatePipe(&BP_GlobalData.AppPipe, BP_APP_PIPE_DEPTH, BP_APP_PIPE_NAME);
    if (status != CFE_SUCCESS)
        return status;

    /* Subscribe to BP Commands */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BP_CMD_MID), BP_GlobalData.AppPipe);
    if (status != CFE_SUCCESS)
        return status;

    /* Subscribe to BP Send Housekeeping Requests */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BP_SEND_HK_MID), BP_GlobalData.AppPipe);
    if (status != CFE_SUCCESS)
        return status;

    /* Subscribe to BP Wakeup Requests */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BP_WAKEUP_MID), BP_GlobalData.AppPipe);
    if (status != CFE_SUCCESS)
        return status;

    /* Initialize Messages */
    CFE_MSG_Init(CFE_MSG_PTR(BP_GlobalData.HkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BP_HK_TLM_MID),
                 sizeof(BP_HkPkt_t));
    CFE_MSG_Init(CFE_MSG_PTR(BP_GlobalData.FlowHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BP_FLOW_HK_TLM_MID),
                 sizeof(BP_FlowHkPkt_t));

    /* Initialize Storage Service Module */
    BP_StorageService_Init();

    /* Initialize BPLib */
    status = BP_SetupLibrary();
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    /* Initialize Flow Module */
    status = BP_FlowInit(BP_GlobalData.AppName);
    if (status != CFE_SUCCESS)
        return status;

    /* Create the CLA task(s) */
    status = BP_CLA_Init();
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    /* Register known storage services */
    BP_StorageService_Configure();

    /* Initialize Default Bundle Flows */
    status = BP_FlowLoad(BP_DEFAULT_FLOW_TBL);
    if (status != CFE_SUCCESS)
        return status;

    /* Initialize the "EnableMask" in TLM from initial config */
    BP_DoRebuildFlowBitmask();

    /* Application startup event message */
    CFE_EVS_SendEvent(BP_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "BP App Version %d.%d.%d.%d: Initialized (KRM)",
                      BP_MAJOR_VERSION, BP_MINOR_VERSION, BP_REVISION, BP_MISSION_REV);

    BPL_EVM_EventInfo_t const EventInfo = {
        .Type = BPL_EVM_EventType_INFO,
        .ID = 0
    };

    (void) BPL_EVM_SendEvent(&EventInfo, "Hello, work!\n");

    return CFE_SUCCESS;
}

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BP_AppMain
 *-----------------------------------------------*/
void BP_AppMain(void)
{
    CFE_SB_Buffer_t *MsgBuf;
    CFE_Status_t     sb_rcv_result;
    uint32           run_status = CFE_ES_RunStatus_APP_ERROR;

    /* Initialize Application */
    CFE_ES_PerfLogEntry(BP_APPMAIN_PERF_ID);
    if (AppInit() == CFE_SUCCESS)
    {
        run_status = CFE_ES_RunStatus_APP_RUN;
    }

    /* Main Process Loop */
    while (CFE_ES_RunLoop(&run_status) == true)
    {
        /* Block on Software Bus */
        CFE_ES_PerfLogExit(BP_APPMAIN_PERF_ID);
        sb_rcv_result = CFE_SB_ReceiveBuffer(&MsgBuf, BP_GlobalData.AppPipe, BP_SB_TIMEOUT_MS);
        CFE_ES_PerfLogEntry(BP_APPMAIN_PERF_ID);

        /* Handle Result */
        if (sb_rcv_result == CFE_SUCCESS)
        {
            BP_AppPipe(MsgBuf);
        }
        else if (sb_rcv_result != CFE_SB_TIME_OUT)
        {
            CFE_EVS_SendEvent(BP_SB_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Main loop error: SB receive: result = 0x%08X", (unsigned int)sb_rcv_result);
            run_status = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* Exit Application */
    CFE_EVS_SendEvent(BP_EXIT_ERR_EID, CFE_EVS_EventType_ERROR, "BP application terminating: result = 0x%08X",
                      (unsigned int)run_status);
    CFE_ES_WriteToSysLog("BP application terminating: result = 0x%08X\n",
                         (unsigned int)run_status); /* in case event services not working */
    CFE_ES_PerfLogExit(BP_APPMAIN_PERF_ID);
    CFE_ES_ExitApp(run_status); /* let cFE kill the task and any child tasks */
}
