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

/*
** File: bpnode_cmds_tests.c
**
** Purpose:
** Coverage Unit Test cases for the BPNode Application
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode application.
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_BPNode_SendNodeMibCountersHkCmd(void)
{
    /*
     * Test Case For:
     * void BPNode_SendNodeMibCountersHkCmd( const BPNode_SendNodeMibCountersHkCmd_t *Msg )
     */
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

void Test_BPNode_NoopCmd(void)
{
    /*
     * Test Case For:
     * void BPNode_NoopCmd( const BPNode_Noop_t *Msg )
     */
    BPNode_NoopCmd_t TestMsg;
    UT_CheckEvent_t      EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* test dispatch of NOOP */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_NOOP_INF_EID, NULL);

    UtAssert_INT32_EQ(BPNode_NoopCmd(&TestMsg), CFE_SUCCESS);

    /*
     * Confirm that the event was generated
     */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

void Test_BPNode_ResetAllCountersCmd(void)
{
    /*
     * Test Case For:
     * void BPNode_ResetAllCounters( const BPNode_ResetAllCounters_t *Msg )
     */
    BPNode_ResetAllCountersCmd_t TestMsg;
    UT_CheckEvent_t           EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset all counters command");

    UtAssert_INT32_EQ(BPNode_ResetAllCountersCmd(&TestMsg), CFE_SUCCESS);

    /*
     * Confirm that the event was generated
     */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(BPNode_SendNodeMibCountersHkCmd);
    ADD_TEST(BPNode_NoopCmd);
    ADD_TEST(BPNode_ResetAllCountersCmd);
}
