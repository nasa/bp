/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *   This file contains the source code for the BPNode software bus processing.
 */

/*
** Include Files:
*/
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"
#include "bpnode_eventids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Verify command packet length                                               */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool BPNode_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              Result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(BPNODE_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (uint16) CFE_SB_MsgIdToValue(MsgId), (uint8) FcnCode, 
                          (uint16) ActualLength, (uint16) ExpectedLength);

        Result = false;

        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;
    }

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* BPNode ground command processing                                           */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void BPNode_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process ground commands
    */
    switch (CommandCode)
    {
        case BPNODE_NOOP_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_NoopCmd_t)))
            {
                BPNode_NoopCmd((const BPNode_NoopCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_RESET_ALL_COUNTERS_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_ResetCountersCmd_t)))
            {
                BPNode_ResetAllCountersCmd((const BPNode_ResetCountersCmd_t *)SBBufPtr);
            }
            break;

        case BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC:
            if (BPNode_VerifyCmdLength(&SBBufPtr->Msg, sizeof(BPNode_SendHkCmd_t)))
            {
                BPNode_SendNodeMibCountersHkCmd((const BPNode_SendHkCmd_t *)SBBufPtr);
            }
            break;

        /* Default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(BPNODE_CC_ERR_EID, CFE_EVS_EventType_ERROR, 
                            "Invalid ground command code: CC = %d", CommandCode);
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the           */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void BPNode_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case BPNODE_CMD_MID:
            BPNode_ProcessGroundCommand(SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(BPNODE_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x", 
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}
