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
 *  Unit tests for bpnode_dispatch.c
 */

/*
** Include Files
*/

#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

/*
** Function Definitions
*/

/* Test task pipe processing in nominal case */
void Test_BPNode_TaskPipe_Nominal(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            MsgSize = sizeof(BPNode_NoopCmd_t);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);

    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(BPNode_NoopCmd, 1);
}

/* Test task pipe processing after receiving an invalid message ID */
void Test_BPNode_TaskPipe_InvalidMsgId(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_INVALID_MSG_ID;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_MID_ERR_EID, 
                                "Invalid command packet,MID = 0x%x");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);

    BPNode_TaskPipe(&Buf);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid noop */
void Test_BPNode_ProcessGroundCommand_ValidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_NoopCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid noop */
void Test_BPNode_ProcessGroundCommand_InvalidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_NoopCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}

/* Test ground command processing after receiving a valid reset-all-counters */
void Test_BPNode_ProcessGroundCommand_ValidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetAllCountersCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-all-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetAllCountersCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}


/* Test ground command processing after receiving a valid send-node-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_ValidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendNodeMibCountersHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendNodeMibCountersHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}

/* Test ground command processing after receiving an invalid command code */
void Test_BPNode_ProcessGroundCommand_InvalidCmdCode(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_MSG_FcnCode_t FcnCode = 1000;   /* Invalid CC */
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPNode_ProcessGroundCommand(&Buf);

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CC_ERR_EID, NULL);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}

/* Test command length verification in nominal case */
void Test_BPNode_VerifyCmdLength_Nominal(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = ActualLength;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);

    BPNode_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 0);
}

/* Test command length verification for an invalid length */
void Test_BPNode_VerifyCmdLength_InvalidLength(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = 2;
    CFE_SB_MsgId_t    MsgId          = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode        = BPNODE_NOOP_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPNode_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_TaskPipe_Nominal);
    ADD_TEST(Test_BPNode_TaskPipe_InvalidMsgId);

    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidNoop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidNoop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetAllCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetAllCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendNodeMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidCmdCode);

    ADD_TEST(Test_BPNode_VerifyCmdLength_Nominal);
    ADD_TEST(Test_BPNode_VerifyCmdLength_InvalidLength);
}
