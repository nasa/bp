#include "bp_app.h"
#include "bp_eventids.h"
#include "bp_dispatch.h"
#include "bp_cmd.h"
#include "bp_global.h"
#include "bp_eventids.h"

#include "bp_eds_dictionary.h"
#include "bp_eds_dispatcher.h"

#include "cfe_msg.h"

/* clang-format off */
static const EdsDispatchTable_BP_Application_CFE_SB_Telecommand_t BP_TC_DISPATCH_TABLE =
{
    .CMD =
    {
        .DisableFlowCmd_indication             = BP_DisableFlowCmd,
        .DisableOverridePriorityCmd_indication = BP_DisableOverridePriorityCmd,
        .DisableOverrideTimeoutCmd_indication  = BP_DisableOverrideTimeoutCmd,
        .EnableFlowCmd_indication              = BP_EnableFlowCmd,
        .FlushFlowCmd_indication               = BP_FlushFlowCmd,
        .NoopCmd_indication                    = BP_NoopCmd,
        .OverridePriorityCmd_indication        = BP_OverridePriorityCmd,
        .OverrideTimeoutCmd_indication         = BP_OverrideTimeoutCmd,
        .PauseFlowCmd_indication               = BP_PauseFlowCmd,
        .ReloadFlowTableCmd_indication         = BP_ReloadFlowTableCmd,
        .ResetAppCmd_indication                = BP_ResetAppCmd,
        .ResumeFlowCmd_indication              = BP_ResumeFlowCmd
    },

    .SEND_APP_TLM =
    {
        .SendAppTlmCmd_indication          = BP_SendAppTlmCmd,
        .SendDisabledFlowTlmCmd_indication = BP_SendDisabledFlowTlmCmd,
        .SendEnabledFlowTlmCmd_indication  = BP_SendEnabledFlowTlmCmd
    },

    .WAKEUP =
    {
        .ProcessWakeupCmd_indication = BP_ProcessWakeupCmd
    }
};
/* clang-format on */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* BP_TaskPipe() -- Process command pipe message           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BP_AppPipe(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_Status_t      status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    status = EdsDispatch_BP_Application_Telecommand(CFE_SB_Telecommand_indication_Command_ID_Telecommand, BufPtr,
                                                    &BP_TC_DISPATCH_TABLE);

    if (status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&BufPtr->Msg, &MsgId);
        ++BP_GlobalData.HkPkt.Payload.InvalidCmdCnt;

        if (status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            CFE_EVS_SendEvent(BP_MID_ERR_EID, CFE_EVS_EventType_ERROR, "L%d TO: Invalid Msg ID Rcvd 0x%x status=0x%08x",
                              __LINE__, (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)status);
        }
        else if (status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_MSG_GetSize(&BufPtr->Msg, &MsgSize);
            CFE_MSG_GetFcnCode(&BufPtr->Msg, &MsgFc);
            CFE_EVS_SendEvent(BP_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid length for command: ID = 0x%X, CC = %d, length = %u",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId), (int)MsgFc, (unsigned int)MsgSize);
        }
        else
        {
            CFE_MSG_GetFcnCode(&BufPtr->Msg, &MsgFc);
            CFE_EVS_SendEvent(BP_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                              "L%d TO: Invalid Function Code Rcvd In Ground Command 0x%x", __LINE__,
                              (unsigned int)MsgFc);
        }
    }
}
