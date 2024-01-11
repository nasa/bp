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

#include "bp_cmd.h"
#include "bp_storecfg.h"
#include "bp_tlmcfg.h"
#include "bp_global.h"
#include "bp_app.h"
#include "bp_platform_cfg.h"
#include "bp_msg.h"
#include "bp_eventids.h"
#include "bp_version.h"
#include "bp_flow.h"
#include "bp_cla_bundle_io.h"
#include "bplib_routing.h"

#include "cfe.h"

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

/************************************************
 * Exported Functions
 ************************************************/

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
    BP_GlobalData.HkPkt.Payload.ValidCmdCnt   = 0;
    BP_GlobalData.HkPkt.Payload.InvalidCmdCnt = 0;

    /* Clear Custom Telemetry */
    BP_ClearCustomTlm(&BP_GlobalData.HkPkt.Payload.CustomTlm);

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

    /* Disable All Flows */
    BP_ForEachFlow(BP_DoPerFlowDisable, NULL);

    /* Reopen All Flows and Issue Event */
    status = BP_FlowLoad(NULL);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Tables: Reloaded");
    }

    /* Initialize the "EnableMask" in TLM from reloaded config */
    BP_DoRebuildFlowBitmask();

    return status;
}

/*-----------------------------------------------
 * EnableFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_EnableFlowCmd(const BP_EnableFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Enable Flow and Issue Event */
    status = BP_FlowEnable(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Enabled", BP_FlowGetName(flow));
    }

    /* Rebuild the "EnableMask" in HkPkt */
    BP_DoRebuildFlowBitmask();

    return status;
}

/*-----------------------------------------------
 * DisableFlowCmd
 *-----------------------------------------------*/
CFE_Status_t BP_DisableFlowCmd(const BP_DisableFlowCmd_t *cmd)
{
    BP_FlowHandle_t flow;
    CFE_Status_t    status;

    /* Get Flow */
    flow = BP_FlowGetHandle(cmd->Payload.Name);

    /* Disable Flow and Issue Event */
    status = BP_FlowDisable(flow);
    if (status == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP Flow %s: Disabled", BP_FlowGetName(flow));
    }

    /* Rebuild the "EnableMask" in HkPkt */
    BP_DoRebuildFlowBitmask();

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
        BP_GlobalData.HkPkt.Payload.MemInUse = mem_use;
    }
    else
    {
        BP_GlobalData.HkPkt.Payload.MemInUse = 0;
    }

    status = bplib_query_integer(BP_GlobalData.RouteTbl, BP_INVALID_HANDLE, bplib_variable_mem_high_use, &mem_use);
    if (status == BP_SUCCESS)
    {
        BP_GlobalData.HkPkt.Payload.MemHighWater = mem_use;
    }
    else
    {
        BP_GlobalData.HkPkt.Payload.MemHighWater = 0;
    }

    /* Populate Custom Flash Statistics */
    BP_PopulateCustomTlm(&BP_GlobalData.HkPkt.Payload.CustomTlm);

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
CFE_Status_t BP_ProcessWakeupCmd(const BP_ProcessWakeupCmd_t *cmd)
{
    /* no options, just process the flows */
    BP_FlowProcess();
    bplib_route_periodic_maintenance(BP_GlobalData.RouteTbl);

    return CFE_SUCCESS;
}
