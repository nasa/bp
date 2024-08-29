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
 *  Unit tests for bpnode_eds_dispatch.c
 */

/*
** Include Files
*/

#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

#include "cfe_msg_dispatcher.h"


/*
** Function Definitions
*/

/* Test nominal task pipe processing */
void Test_BPNode_TaskPipe_Nominal(void)
{
    CFE_SB_Buffer_t Buf;

    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_SUCCESS);

    UtAssert_VOIDCALL(BPNode_TaskPipe(&Buf));

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test task pipe processing with an invalid message ID */
void Test_BPNode_TaskPipe_InvalidMsgId(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    MsgId = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_Size_t    MsgSize = 0;
    CFE_MSG_FcnCode_t FcnCode = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_UNKNOWN_MSG_ID);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_MID_ERR_EID, NULL);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test task pipe processing with an invalid message length */
void Test_BPNode_TaskPipe_InvalidLength(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    MsgId = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_Size_t    MsgSize = 0;
    CFE_MSG_FcnCode_t FcnCode = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_WRONG_MSG_LENGTH);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID, NULL);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test task pipe processing with an invalid command code */
void Test_BPNode_TaskPipe_InvalidCmdCode(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    MsgId = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_Size_t    MsgSize = 0;
    CFE_MSG_FcnCode_t FcnCode = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_BAD_COMMAND_CODE);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CC_ERR_EID, NULL);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}


/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_TaskPipe_Nominal);
    ADD_TEST(Test_BPNode_TaskPipe_InvalidMsgId);
    ADD_TEST(Test_BPNode_TaskPipe_InvalidLength);
    ADD_TEST(Test_BPNode_TaskPipe_InvalidCmdCode);
}
