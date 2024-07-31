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
 *  Unit tests for bpnode_cmds.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

/*
** Function Definitions
*/

/* Test node MIB counter housekeeping packet transmission in nominal case */
void Test_BPNode_SendNodeMibCountersHkCmd_Nominal(void)
{
    CFE_MSG_Message_t *MsgSend;
    CFE_MSG_Message_t *MsgTimestamp;

    /* Set up to capture send message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TransmitMsg), &MsgSend, sizeof(MsgSend), false);

    /* Set up to capture timestamp message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TimeStampMsg), &MsgTimestamp, sizeof(MsgTimestamp), false);

    /* Call unit under test, NULL pointer confirms command access is through APIs */
    BPNode_SendNodeMibCountersHkCmd(NULL);

    /* Confirm message sent*/
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_ADDRESS_EQ(MsgSend, &BPNode_AppData.NodeMibCountersHkTlm);

    /* Confirm timestamp msg address */
    UtAssert_STUB_COUNT(CFE_SB_TimeStampMsg, 1);
    UtAssert_ADDRESS_EQ(MsgTimestamp, &BPNode_AppData.NodeMibCountersHkTlm);
}

/* Test noop command in nominal case */
void Test_BPNode_NoopCmd_Nominal(void)
{
    BPNode_NoopCmd_t TestMsg;
    UT_CheckEvent_t  EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Test dispatch of noop */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_NOOP_INF_EID, NULL);

    UtAssert_INT32_EQ(BPNode_NoopCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test reset all counters command nominal case */
void Test_BPNode_ResetAllCountersCmd_Nominal(void)
{
    BPNode_ResetAllCountersCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset all counters command");

    UtAssert_INT32_EQ(BPNode_ResetAllCountersCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_SendNodeMibCountersHkCmd_Nominal);
    ADD_TEST(Test_BPNode_NoopCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetAllCountersCmd_Nominal);
}
