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
** File: coveragetest_bpnode_app.c
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
** Includes
*/
#include "bplib.h"
#include "bpnode_coveragetest_common.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

/*
** Test Case For:
** void BPNode_AppMain( void )
*/
void Test_BPNode_AppMain(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;
    UT_CheckEvent_t  EventTest;

    /*
     * BPNode_AppMain does not return a value,
     * but it has several internal decision points
     * that need to be exercised here.
     *
     * First call it in "nominal" mode where all
     * dependent calls should be successful by default.
     */
    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);

    /* Failure of BPNode_AppInit()*/
    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_Register), 1, CFE_EVS_INVALID_PARAMETER);

    BPNode_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);

    /* Receive wakeup message but no command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);

    /* Wakeup pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 3);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_PIPE_ERR_EID, "SB Pipe Read Error, App Will Exit");

    /* Command pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_PIPE_RD_ERR);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 5);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_PIPE_ERR_EID, "SB Pipe Read Error, App Will Exit");

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 3, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 8);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);
    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}


/*
** Test Case For:
** void BPNode_ProcessMain( void )
*/
void Test_BPNode_ProcessMain(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_PIPE_RD_ERR);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UtAssert_INT32_EQ(BPNode_ProcessMain(), CFE_SB_PIPE_RD_ERR);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);

    /* Receipt of a null buffer */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_PIPE_RD_ERR);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), NULL, sizeof(BufPtr), false);
    UtAssert_INT32_EQ(BPNode_ProcessMain(), CFE_SB_PIPE_RD_ERR);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 4);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);

    /* Command receive error */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);
    UtAssert_INT32_EQ(BPNode_ProcessMain(), CFE_SB_PIPE_RD_ERR);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 5);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);
}


/*
** Test Case For:
** CFE_Status_t BPNode_AppInit( void )
*/
void Test_BPNode_AppInit(void)
{
    UT_CheckEvent_t EventTest;

    /* Nominal case should return CFE_SUCCESS */
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_INIT_INF_EID, NULL);

    /*
    ** Trigger a failure for each of the sub-calls, and confirm a write to syslog for
    ** failure to register with EVS, and that an event is generated for subsequent error paths.
    ** Note that the stub counts accumulate, because the status is _not_ reset between test cases.
    */
    
    /* Failure to register with event services */
    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_Register), 1, CFE_EVS_INVALID_PARAMETER);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_EVS_INVALID_PARAMETER);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);

    /* Failure to create command pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2); /* 1 from previous nominal case, 1 from this error path */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CR_CMD_PIPE_ERR_EID, NULL);

    /* Failure to create wakeup pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 2, CFE_SB_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3); /* 1 from previous nominal case, 1 from this error path */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CR_WKP_PIPE_ERR_EID, NULL);

    /* Failure to subscribe to HK requests */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, CFE_SB_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4); /* 1 additional event sent from this error path */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_SUB_HK_ERR_EID, NULL);

    /* Failure to subscribe to commands */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, CFE_SB_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 5); /* 1 additional event sent from this error path */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_SUB_CMD_ERR_EID, NULL);

    /* Failure to subscribe to wakeups */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 3, CFE_SB_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 6); /* 1 additional event sent from this error path */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_SUB_WKP_ERR_EID, NULL);

    /* Failure to register table */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_Register), 1, CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 7); /* 1 from table registration error */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TBL_REG_ERR_EID, NULL);

    /* Failure to load table */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_Load), 1, CFE_TBL_BAD_ARGUMENT);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_TBL_BAD_ARGUMENT);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 8); /* 1 from table load error */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TBL_LD_ERR_EID, NULL);

    /* Failure to get table address */
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_ERR_UNREGISTERED);
    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_TBL_ERR_UNREGISTERED);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 9); /* 1 from table load error */
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TBL_ADDR_ERR_EID, NULL);

}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(BPNode_AppMain);
    ADD_TEST(BPNode_ProcessMain);
//    ADD_TEST(BPNode_AppInit);
}
