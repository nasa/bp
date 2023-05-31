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
#include "bp_global.h"
#include "bp_app.h"
#include "bp_platform_cfg.h"
#include "bp_msg.h"
#include "bp_eventids.h"
#include "bp_version.h"
#include "bp_flow.h"
#include "bp_cla_bundle_io.h"
#include "bplib_routing.h"

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

/*-----------------------------------------------
 * A helper/wrapper that handles stats commands that operate per-flow
 * This is used in conjunction with BP_ForEachFlow() which invokes it for each flow handle
 *-----------------------------------------------*/
static void BP_SendStatsMsgPerFlow(BP_FlowHandle_t fh, void *Arg)
{
    bool want_enabled_flows = *((bool *)Arg);

    /*
     * this may send either the enabled flows or the disabled flows,
     * depending on which the caller asked for via the arg
     */
    if (want_enabled_flows == BP_FlowIsEnabled(fh))
    {
        BP_FlowGetStats(fh, &BP_GlobalData.FlowHkPkt.FlowStats);
        CFE_SB_TimeStampMsg(CFE_MSG_PTR(BP_GlobalData.FlowHkPkt.TelemetryHeader));

        /* transmit msg is always "best effort", it does not fail unless misconfigured */
        CFE_SB_TransmitMsg(CFE_MSG_PTR(BP_GlobalData.FlowHkPkt.TelemetryHeader), true);
    }
}

/*-----------------------------------------------
 * A helper/wrapper that handles the flow stats reset command
 * This is used in conjunction with BP_ForEachFlow() which invokes it for each flow handle
 *-----------------------------------------------*/
static void BP_DoPerFlowStatReset(BP_FlowHandle_t fh, void *Arg)
{
    BP_FlowClearStats(fh);
}

/*-----------------------------------------------
 * A helper/wrapper that disables all flows
 * This is used in conjunction with BP_ForEachFlow() which invokes it for each flow handle
 *-----------------------------------------------*/
static void BP_DoPerFlowDisable(BP_FlowHandle_t fh, void *Arg)
{
    BP_FlowDisable(fh);
}

/*-----------------------------------------------
 * A helper/wrapper that handles the flow enable/disable command
 * This is used in conjunction with BP_ForEachFlow() which invokes it for each flow handle
 * The objective is to recompute the 32-bit mask value for TLM.
 *-----------------------------------------------*/
static void BP_RebuildBitmaskPerFlow(BP_FlowHandle_t fh, void *Arg)
{
    uint32 *Mask = (uint32 *)Arg;
    uint32  Idx;

    if (BP_FlowIsEnabled(fh))
    {
        /* This is called only for handles which are known good, and BP_FlowIsEnabled()
         * confirmed that the flow handle is good, so "ToIndex" will never fail */
        BP_FlowHandle_ToIndex(fh, &Idx);
        *Mask |= 1 << Idx;
    }
}

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
    BP_ForEachFlow(BP_RebuildBitmaskPerFlow, &BP_GlobalData.HkPkt.EnableMask);

    /* Application startup event message */
    CFE_EVS_SendEvent(BP_INIT_APP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP App Version %d.%d.%d.%d: Initialized",
                      BP_MAJOR_VERSION, BP_MINOR_VERSION, BP_REVISION, BP_MISSION_REV);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * PktLenCheck
 *-----------------------------------------------*/
static CFE_Status_t BP_PktLenCheck(const CFE_SB_Buffer_t *MsgBuf, size_t ExpLen)
{
    size_t act_len;

    if (CFE_MSG_GetSize(&MsgBuf->Msg, &act_len) != CFE_SUCCESS || act_len != ExpLen)
    {
        CFE_EVS_SendEvent(BP_INVALID_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid length in packet, exp = %lu, act = %lu", (unsigned long)ExpLen,
                          (unsigned long)act_len);
        return CFE_STATUS_WRONG_MSG_LENGTH;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * NoopCmd
 *-----------------------------------------------*/
CFE_Status_t BP_NoopCmd(const BP_NoopCmd_t *cmd)
{
    /* Issue Event */
    CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION,
                      "No operation command received for BP version %d.%d.%d.%d", BP_MAJOR_VERSION, BP_MINOR_VERSION,
                      BP_REVISION, BP_MISSION_REV);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * ResetAppCmd
 *-----------------------------------------------*/
CFE_Status_t BP_ResetAppCmd(const BP_ResetAppCmd_t *cmd)
{
    /* Clear Command Counters */
    BP_GlobalData.HkPkt.ValidCmdCnt   = 0;
    BP_GlobalData.HkPkt.InvalidCmdCnt = 0;

    /* Clear Custom Telemetry */
    BP_ClearCustomTlm(&BP_GlobalData.HkPkt.CustomTlm);

    BP_ForEachFlow(BP_DoPerFlowStatReset, NULL);

    /* Issue Event */
    CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Statistics: Cleared");

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * ReloadFlowTableCmd
 *-----------------------------------------------*/
CFE_Status_t BP_ReloadFlowTableCmd(const BP_ReloadFlowTableCmd_t *cmd)
{
    CFE_Status_t status;
    uint32       EnableMask;

    /* Disable All Flows */
    BP_ForEachFlow(BP_DoPerFlowDisable, NULL);

    /* Reopen All Flows and Issue Event */
    status = BP_FlowLoad(NULL);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Tables: Reloaded");
    }

    /* Initialize the "EnableMask" in TLM from reloaded config */
    EnableMask = 0;
    BP_ForEachFlow(BP_RebuildBitmaskPerFlow, &EnableMask);
    BP_GlobalData.HkPkt.EnableMask = EnableMask;

    return status;
}

/*-----------------------------------------------
 * EnableFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_EnableFlowCmd(const BP_EnableFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;
    uint32          EnableMask;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Enable Flow and Issue Event */
    status = BP_FlowEnable(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Enabled", BP_FlowGetName(flow));
    }

    /* Rebuild the "EnableMask" in HkPkt */
    EnableMask = 0;
    BP_ForEachFlow(BP_RebuildBitmaskPerFlow, &EnableMask);
    BP_GlobalData.HkPkt.EnableMask = EnableMask;

    return status;
}

/*-----------------------------------------------
 * DisableFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_DisableFlowCmd(const BP_DisableFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;
    uint32          EnableMask;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Disable Flow and Issue Event */
    status = BP_FlowDisable(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Disabled", BP_FlowGetName(flow));
    }

    /* Rebuild the "EnableMask" in HkPkt */
    EnableMask = 0;
    BP_ForEachFlow(BP_RebuildBitmaskPerFlow, &EnableMask);
    BP_GlobalData.HkPkt.EnableMask = EnableMask;

    return status;
}

/*-----------------------------------------------
 * FlushFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_FlushFlowCmd(const BP_FlushFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Flush Flow and Issue Event */
    status = BP_FlowFlush(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Flushed", BP_FlowGetName(flow));
    }

    return status;
}

/*-----------------------------------------------
 * PauseFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_PauseFlowCmd(const BP_PauseFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Pause Flow and Issue Event */
    status = BP_FlowPause(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Paused", BP_FlowGetName(flow));
    }

    return status;
}

/*-----------------------------------------------
 * ResumeFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_ResumeFlowCmd(const BP_ResumeFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Resume Flow and Issue Event */
    status = BP_FlowResume(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Resumed", BP_FlowGetName(flow));
    }

    return status;
}

/*-----------------------------------------------
 * OverrideTimeoutCmd
 *-----------------------------------------------*/
CFE_Status_t BP_OverrideTimeoutCmd(const BP_OverrideTimeoutCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Override Timeout and Issue Event */
    status = BP_FlowSetTimeout(flow, (int)cmd->Payload.Timeout);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Timeout Overridden %d",
                          BP_FlowGetName(flow), (int)cmd->Payload.Timeout);
    }

    return status;
}

/*-----------------------------------------------
 * DisableOverrideTimeoutCmd
 *-----------------------------------------------*/
CFE_Status_t BP_DisableOverrideTimeoutCmd(const BP_DisableOverrideTimeoutCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Disable Override and Issue Event */
    status = BP_FlowRevertTimeout(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Timeout Override Disabled",
                          BP_FlowGetName(flow));
    }

    return status;
}

/*-----------------------------------------------
 * OverridePriorityCmd
 *-----------------------------------------------*/
CFE_Status_t BP_OverridePriorityCmd(const BP_OverridePriorityCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Override Priority and Issue Event */
    status = BP_FlowSetPriority(flow, (int)cmd->Payload.Priority);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Priority Overridden %d",
                          BP_FlowGetName(flow), (int)cmd->Payload.Priority);
    }

    return status;
}

/*-----------------------------------------------
 * DisableOverridePriorityCmd
 *-----------------------------------------------*/
CFE_Status_t BP_DisableOverridePriorityCmd(const BP_DisableOverridePriorityCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Disable Override and Issue Event */
    status = BP_FlowRevertPriority(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Priority Override Disabled",
                          BP_FlowGetName(flow));
    }

    return status;
}

/*-----------------------------------------------
 * BP_SendAppTlmCmd
 *-----------------------------------------------*/
CFE_Status_t BP_SendAppTlmCmd(const BP_SendAppTlmCmd_t *cmd)
{
    bp_sval_t mem_use;
    int       status;

    /* Populate Memory Usage Statistics */
    status = bplib_query_integer(BP_GlobalData.RouteTbl, BP_INVALID_HANDLE, bplib_variable_mem_current_use, &mem_use);
    if (status == BP_SUCCESS)
    {
        BP_GlobalData.HkPkt.MemInUse = mem_use;
    }
    else
    {
        BP_GlobalData.HkPkt.MemInUse = 0;
    }

    status = bplib_query_integer(BP_GlobalData.RouteTbl, BP_INVALID_HANDLE, bplib_variable_mem_high_use, &mem_use);
    if (status == BP_SUCCESS)
    {
        BP_GlobalData.HkPkt.MemHighWater = mem_use;
    }
    else
    {
        BP_GlobalData.HkPkt.MemHighWater = 0;
    }

    /* Populate Custom Flash Statistics */
    BP_PopulateCustomTlm(&BP_GlobalData.HkPkt.CustomTlm);

    /* Send Application Housekeeping Packet */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BP_GlobalData.HkPkt.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BP_GlobalData.HkPkt.TelemetryHeader), true);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_SendEnabledFlowTlmCmd
 *-----------------------------------------------*/
CFE_Status_t BP_SendEnabledFlowTlmCmd(const BP_SendEnabledFlowTlmCmd_t *cmd)
{
    bool want_enabled_flows = true;

    /* Populate and Send Flow Housekeeping Packets */
    BP_ForEachFlow(BP_SendStatsMsgPerFlow, &want_enabled_flows);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_SendDisabledFlowTlmCmd
 *-----------------------------------------------*/
CFE_Status_t BP_SendDisabledFlowTlmCmd(const BP_SendDisabledFlowTlmCmd_t *cmd)
{
    bool want_enabled_flows = false;

    /* Populate and Send Flow Housekeeping Packets */
    BP_ForEachFlow(BP_SendStatsMsgPerFlow, &want_enabled_flows);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_ProcessWakeupCmd
 *-----------------------------------------------*/
CFE_Status_t BP_ProcessWakeupCmd(const CFE_MSG_CommandHeader_t *cmd)
{
    /* no options, just process the flows */
    BP_FlowProcess();
    bplib_route_periodic_maintenance(BP_GlobalData.RouteTbl);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * ProcessCmd
 *-----------------------------------------------*/
static CFE_Status_t ProcessCmd(const CFE_SB_Buffer_t *MsgBufPtr)
{
    CFE_MSG_FcnCode_t cmd_code;
    CFE_Status_t      cmd_result;

    cmd_result = CFE_MSG_GetFcnCode(&MsgBufPtr->Msg, &cmd_code);
    if (cmd_result != CFE_SUCCESS)
    {
        return cmd_result;
    }

    /* Invoke specific command handler */
    switch (cmd_code)
    {
        case BP_NOOP_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_NoopCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_NoopCmd((const BP_NoopCmd_t *)MsgBufPtr);
            }
            break;
        case BP_RESET_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_ResetAppCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_ResetAppCmd((const BP_ResetAppCmd_t *)MsgBufPtr);
            }
            break;
        case BP_RELOAD_FLOW_TABLE_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_ReloadFlowTableCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_ReloadFlowTableCmd((const BP_ReloadFlowTableCmd_t *)MsgBufPtr);
            }
            break;
        case BP_ENABLE_FLOW_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_EnableFlowCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_EnableFlowCmd((const BP_EnableFlowCmd_t *)MsgBufPtr);
            }
            break;
        case BP_DISABLE_FLOW_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_DisableFlowCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_DisableFlowCmd((const BP_DisableFlowCmd_t *)MsgBufPtr);
            }
            break;
        case BP_FLUSH_FLOW_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_FlushFlowCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_FlushFlowCmd((const BP_FlushFlowCmd_t *)MsgBufPtr);
            }
            break;
        case BP_PAUSE_FLOW_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_PauseFlowCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_PauseFlowCmd((const BP_PauseFlowCmd_t *)MsgBufPtr);
            }
            break;
        case BP_RESUME_FLOW_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_ResumeFlowCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_ResumeFlowCmd((const BP_ResumeFlowCmd_t *)MsgBufPtr);
            }
            break;
        case BP_OVERRIDE_TIMEOUT_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_OverrideTimeoutCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_OverrideTimeoutCmd((const BP_OverrideTimeoutCmd_t *)MsgBufPtr);
            }
            break;
        case BP_DISABLE_OVERRIDE_TIMEOUT_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_DisableOverrideTimeoutCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_DisableOverrideTimeoutCmd((const BP_DisableOverrideTimeoutCmd_t *)MsgBufPtr);
            }
            break;
        case BP_OVERRIDE_PRIORITY_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_OverridePriorityCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_OverridePriorityCmd((const BP_OverridePriorityCmd_t *)MsgBufPtr);
            }
            break;
        case BP_DISABLE_OVERRIDE_PRIORITY_CC:
            cmd_result = BP_PktLenCheck(MsgBufPtr, sizeof(BP_DisableOverridePriorityCmd_t));
            if (cmd_result == CFE_SUCCESS)
            {
                cmd_result = BP_DisableOverridePriorityCmd((const BP_DisableOverridePriorityCmd_t *)MsgBufPtr);
            }
            break;
        default:
            cmd_result = CFE_STATUS_BAD_COMMAND_CODE;
            break;
    }

    /* Update Housekeeping Statistics */
    if (cmd_result == CFE_SUCCESS)
        BP_GlobalData.HkPkt.ValidCmdCnt++;
    else
        BP_GlobalData.HkPkt.InvalidCmdCnt++;

    return cmd_result;
}

/*-----------------------------------------------
 * ProcessHk
 *-----------------------------------------------*/
static CFE_Status_t ProcessHk(const CFE_SB_Buffer_t *MessagePtr)
{
    CFE_Status_t      status;
    CFE_MSG_FcnCode_t cmd_code;

    status = CFE_MSG_GetFcnCode(&MessagePtr->Msg, &cmd_code);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    if (cmd_code == BP_SEND_APP_CC)
    {
        status = BP_SendAppTlmCmd((const BP_SendAppTlmCmd_t *)MessagePtr);
    }
    else if (cmd_code == BP_SEND_ENABLED_CC)
    {
        status = BP_SendEnabledFlowTlmCmd((const BP_SendEnabledFlowTlmCmd_t *)MessagePtr);
    }
    else if (cmd_code == BP_SEND_DISABLED_CC)
    {
        status = BP_SendDisabledFlowTlmCmd((const BP_SendDisabledFlowTlmCmd_t *)MessagePtr);
    }
    else
    {
        status = CFE_STATUS_BAD_COMMAND_CODE;
    }

    return status;
}

/*-----------------------------------------------
 * ProcessFlows
 *-----------------------------------------------*/
static CFE_Status_t ProcessFlows(const CFE_SB_Buffer_t *MessagePtr)
{
    CFE_Status_t      status;
    CFE_MSG_FcnCode_t cmd_code;

    status = CFE_MSG_GetFcnCode(&MessagePtr->Msg, &cmd_code);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    if (cmd_code == BP_WAKEUP_PROCESS_CC)
    {
        status = BP_ProcessWakeupCmd((const CFE_MSG_CommandHeader_t *)MessagePtr);
    }
    else
    {
        status = CFE_STATUS_BAD_COMMAND_CODE;
    }

    return status;
}

/*-----------------------------------------------
 * ProcessPkt
 *-----------------------------------------------*/
CFE_Status_t BP_ProcessPkt(const CFE_SB_Buffer_t *MessagePtr)
{
    CFE_SB_MsgId_t MessageID;
    CFE_Status_t   Status;

    Status = CFE_MSG_GetMsgId(&MessagePtr->Msg, &MessageID);
    if (Status == CFE_SUCCESS)
    {
        switch (CFE_SB_MsgIdToValue(MessageID))
        {
            case BP_CMD_MID:
                Status = ProcessCmd(MessagePtr);
                break;
            case BP_SEND_HK_MID:
                Status = ProcessHk(MessagePtr);
                break;
            case BP_WAKEUP_MID:
                Status = ProcessFlows(MessagePtr);
                break;
            default:
                Status = CFE_STATUS_UNKNOWN_MSG_ID;
                break;
        }
    }

    return Status;
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
            BP_ProcessPkt(MsgBuf);
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
