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

#include "bp_dispatch.h"
#include "bp_cmd.h"
#include "bp_global.h"
#include "bp_platform_cfg.h"
#include "bp_msg.h"
#include "bp_msgids.h"
#include "bp_eventids.h"

#include "cfe.h"

/*-----------------------------------------------
 * PktLenCheck
 *-----------------------------------------------*/
static CFE_Status_t BP_PktLenCheck(const CFE_SB_Buffer_t *MsgBuf, size_t ExpLen)
{
    size_t act_len;

    if (CFE_MSG_GetSize(&MsgBuf->Msg, &act_len) != CFE_SUCCESS || act_len != ExpLen)
    {
        CFE_EVS_SendEvent(BP_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid length in packet, exp = %lu, act = %lu", (unsigned long)ExpLen,
                          (unsigned long)act_len);
        return CFE_STATUS_WRONG_MSG_LENGTH;
    }

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
        case BP_RESET_APP_CC:
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
        BP_GlobalData.HkPkt.Payload.ValidCmdCnt++;
    else
        BP_GlobalData.HkPkt.Payload.InvalidCmdCnt++;

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

    if (cmd_code == BP_SEND_APP_TLM_CC)
    {
        status = BP_SendAppTlmCmd((const BP_SendAppTlmCmd_t *)MessagePtr);
    }
    else if (cmd_code == BP_SEND_ENABLED_FLOW_TLM_CC)
    {
        status = BP_SendEnabledFlowTlmCmd((const BP_SendEnabledFlowTlmCmd_t *)MessagePtr);
    }
    else if (cmd_code == BP_SEND_DISABLED_FLOW_TLM_CC)
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
        status = BP_ProcessWakeupCmd((const BP_ProcessWakeupCmd_t *)MessagePtr);
    }
    else
    {
        status = CFE_STATUS_BAD_COMMAND_CODE;
    }

    return status;
}

/*-----------------------------------------------
 * BP_AppPipe
 *-----------------------------------------------*/
void BP_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_SB_MsgId_t MsgId;
    CFE_Status_t   Status;

    Status = CFE_MSG_GetMsgId(&BufPtr->Msg, &MsgId);
    if (Status == CFE_SUCCESS)
    {
        switch (CFE_SB_MsgIdToValue(MsgId))
        {
            case BP_CMD_MID:
                ProcessCmd(BufPtr);
                break;
            case BP_SEND_HK_MID:
                ProcessHk(BufPtr);
                break;
            case BP_WAKEUP_MID:
                ProcessFlows(BufPtr);
                break;
            default:
                ++BP_GlobalData.HkPkt.Payload.InvalidCmdCnt;
                CFE_EVS_SendEvent(BP_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "BP: Invalid Msg ID Rcvd 0x%x",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId));
                break;
        }
    }
}
