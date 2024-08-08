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

/**
 * \file
 *   This file contains the source code for the BPNode EDS bindings
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"
#include "bpnode_eventids.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"

#include "bpnode_eds_dispatcher.h"
#include "bpnode_eds_dictionary.h"

/* Command code lookup table */
static const EdsDispatchTable_BPNode_Application_CFE_SB_Telecommand_t BPNODE_TC_DISPATCH_TABLE = {
    .CMD     = {.NoopCmd_indication          = BPNode_NoopCmd,
                .ResetAllCountersCmd_indication = BPNode_ResetAllCountersCmd,
                .SendNodeMibCountersHkCmd_indication = BPNode_SendNodeMibCountersHkCmd
               }
};


/*
** Function Definitions
*/

/*
 * Temporary stack dump
 */
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

void print_stack_trace(uint16 num_frames) {
    void *callstack[128];
    if (num_frames > 128) num_frames = 128;
    int frames = backtrace(callstack, num_frames);
    char **strs = backtrace_symbols(callstack, frames);
    if (strs == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    printf("Stack trace:\n");
    for (int i = 0; i < frames; ++i) {
        printf("%s\n", strs[i]);
    }

    free(strs);
}

/* Process packets received on command pipe */
void BPNode_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_Status_t      Status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    Status = EdsDispatch_BPNode_Application_Telecommand(SBBufPtr, &BPNODE_TC_DISPATCH_TABLE);

    /* Invalid packet */
    if (Status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);
        CFE_MSG_GetSize(&SBBufPtr->Msg, &MsgSize);
        CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &MsgFc);

        BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount++;

        /* Invalid message ID */
        if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            CFE_EVS_SendEvent(BPNODE_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid command packet,MID = 0x%x", 
                              (uint16) CFE_SB_MsgIdToValue(MsgId));
        }
        /* Invalid message length */
        else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_EVS_SendEvent(BPNODE_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u",
                              (uint16) CFE_SB_MsgIdToValue(MsgId), 
                              (uint8) MsgFc, (uint16) MsgSize);
        }
        /* Invalid command code */
        else
        {
            CFE_EVS_SendEvent(BPNODE_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                              "BPNode_TaskPipe: Invalid ground command code: CC = %d", (uint8) MsgFc);
        }

        printf ("Stack trace should be here.\n");
        print_stack_trace(8);

    }
}
