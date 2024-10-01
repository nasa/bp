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
 *  Unit tests for bpnode_app.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "fwp_tablep.h"


/*
** Function Definitions
*/

/* Test app main loop in nominal case */
void Test_BPNode_AppMain_Nominal(void)
{
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
}

/* Test app main loop after initialization failure */
void Test_BPNode_AppMain_FailedInit(void)
{
    /* Failure of BPNode_AppInit()*/
    UT_SetDeferredRetcode(UT_KEY(BPLib_EM_Init), 1, CFE_EVS_INVALID_PARAMETER);

    BPNode_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/* Test app main loop after receiving a wakeup (but no command) */
void Test_BPNode_AppMain_WakeupRecvd(void)
{
    /* Receive wakeup message but no command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
}

/* Test app main loop after wakeup pipe read error */
void Test_BPNode_AppMain_WakeupErr(void)
{
    UT_CheckEvent_t EventTest;

    /* Wakeup pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_PIPE_ERR_EID, "SB Pipe Read Error, App Will Exit");

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app main loop after command pipe read error */
void Test_BPNode_AppMain_CommandErr(void)
{
    UT_CheckEvent_t  EventTest;

    /* Command pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_TIME_OUT);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_PIPE_ERR_EID, "SB Pipe Read Error, App Will Exit");

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app main loop after receiving one command */
void Test_BPNode_AppMain_CommandRecvd(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 3);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);
    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}


/* Test wakeup process after receiving one command */
void Test_BPNode_WakeupProcess_CommandRecvd(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test wakeup process after failing Time maintenance activities */
void Test_BPNode_WakeupProcess_FailTimeMaint(void)
{
    UT_CheckEvent_t EventTest;

    /* Fail Time activities */
    UT_SetDeferredRetcode(UT_KEY(BPLib_TIME_MaintenanceActivities), 1, BPLIB_TIME_WRITE_ERROR);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TIME_WKP_ERR_EID, 
                                    "Error doing time maintenance activities, RC = %d");

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPA_TABLEP_TableUpdate, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}


/* Test wakeup process after getting an updated table */
void Test_BPNode_WakeupProcess_FailedTblUpdate(void)
{
    UT_CheckEvent_t  EventTest;
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TBL_ADDR_ERR_EID, NULL);

    UT_SetDefaultReturnValue(UT_KEY(BPA_TABLEP_TableUpdate), CFE_TBL_ERR_INVALID_HANDLE);
    UtAssert_INT32_NEQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(EventTest.MatchCount, 1);
}

/* Test wakeup process after receiving null buffer */
void Test_BPNode_WakeupProcess_NullBuf(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Receipt of a null buffer */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_PIPE_RD_ERR);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), NULL, sizeof(BufPtr), false);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SB_PIPE_RD_ERR);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test wakeup process after command receive error */
void Test_BPNode_WakeupProcess_RecvErr(void)
{
    /* Command receive error */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);
 
    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SB_PIPE_RD_ERR);
 
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(BPNode_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_INIT_INF_EID, "BPNode Initialized: %s");

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SUCCESS);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after event management initialization failure */
void Test_BPNode_AppInit_FailedEvs(void)
{
    /* Failure to register with event management */
    UT_SetDeferredRetcode(UT_KEY(BPLib_EM_Init), 1, CFE_EVS_INVALID_PARAMETER);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_EVS_INVALID_PARAMETER);

    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);

}

/* Test app initialization after command pipe creation failure */
void Test_BPNode_AppInit_FailedCmdPipeCreate(void)
{
    UT_CheckEvent_t EventTest;

    /* Failure to create command pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_BAD_ARGUMENT);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CR_CMD_PIPE_ERR_EID, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after wakeup pipe creation failure */
void Test_BPNode_AppInit_FailedWakeupPipeCreate(void)
{
    UT_CheckEvent_t EventTest;

    /* Failure to create wakeup pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 2, CFE_SB_BAD_ARGUMENT);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CR_WKP_PIPE_ERR_EID, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after failure to subscribe to commands */
void Test_BPNode_AppInit_FailedCommandSub(void)
{
    UT_CheckEvent_t EventTest;

    /* Failure to subscribe to commands */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, CFE_SB_BAD_ARGUMENT);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_SUB_CMD_ERR_EID, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after failure to subscribe to wakeups */
void Test_BPNode_AppInit_FailedWakeupSub(void)
{
    UT_CheckEvent_t EventTest;

    /* Failure to subscribe to wakeups */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, CFE_SB_BAD_ARGUMENT);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_SUB_WKP_ERR_EID, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after failure to register table */
void Test_BPNode_AppInit_FailedTblInit(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TBL_ADDR_ERR_EID, NULL);

    /* Failure to call BPA_TABLEP_TableInit() */    
    UT_SetDefaultReturnValue(UT_KEY(BPA_TABLEP_TableInit), CFE_TBL_ERR_INVALID_HANDLE);

    UtAssert_INT32_NEQ(BPNode_AppInit(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}


/* Test app initialization when FWP initialization fails */
void Test_BPNode_AppInit_FailedFwpInit(void)
{
    UT_CheckEvent_t EventTest;

    UT_SetDeferredRetcode(UT_KEY(BPLib_FWP_Init), 1, BPLIB_FWP_CALLBACK_INIT_ERROR);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_FWP_INIT_ERR_EID, "BPNode: Failure initializing function callbacks, RC = 0x%08lX");

    UtAssert_INT32_EQ(BPNode_AppInit(), BPLIB_FWP_CALLBACK_INIT_ERROR);
    
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization after failure to create ADU in child tasks */
void Test_BPNode_AppInit_FailedAduInTasks(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDeferredRetcode(UT_KEY(BPNode_AduInCreateTasks), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_CHILD_TASK_CREATE);
}

/* Test app initialization after failure to create ADU out child tasks */
void Test_BPNode_AppInit_FailedAduOutTasks(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDeferredRetcode(UT_KEY(BPNode_AduOutCreateTasks), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_CHILD_TASK_CREATE);
}

/* Test app initialization after failure to initialize Telemetry Proxy */
void Test_BPNode_AppInit_FailedTlmpInit(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPA_TLMP_Init), CFE_SB_INTERNAL_ERR);
    UtAssert_INT32_NEQ(BPNode_AppInit(), CFE_SUCCESS);
}

/* Test app exit in nominal case */
void Test_BPNode_AppExit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    uint8 i;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_EXIT_CRIT_EID, 
                                "App terminating, error = %d");

    BPNode_AppExit();

    for (i = 0; i < BPNODE_MAX_NUM_CHANNELS; i++)
    {
        UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[i].RunStatus, CFE_ES_RunStatus_APP_EXIT);
        UtAssert_UINT32_EQ(BPNode_AppData.AduInData[i].RunStatus, CFE_ES_RunStatus_APP_EXIT);
    }

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_FailedTimeInit(void)
{
    UT_CheckEvent_t EventTest;

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_INFO_UPDATED);
    UT_SetDeferredRetcode(UT_KEY(BPLib_TIME_Init), 1, BPLIB_TIME_READ_ERROR);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_TIME_INIT_ERR_EID, "Error initializing BPLib Time Management, RC = %d");

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_AppMain_Nominal);
    ADD_TEST(Test_BPNode_AppMain_FailedInit);
    ADD_TEST(Test_BPNode_AppMain_WakeupRecvd);
    ADD_TEST(Test_BPNode_AppMain_WakeupErr);
    ADD_TEST(Test_BPNode_AppMain_CommandRecvd);
    ADD_TEST(Test_BPNode_AppMain_CommandErr);

    ADD_TEST(Test_BPNode_WakeupProcess_CommandRecvd);
    ADD_TEST(Test_BPNode_WakeupProcess_FailTimeMaint);
    ADD_TEST(Test_BPNode_WakeupProcess_FailedTblUpdate);
    ADD_TEST(Test_BPNode_WakeupProcess_NullBuf);
    ADD_TEST(Test_BPNode_WakeupProcess_RecvErr);

    ADD_TEST(Test_BPNode_AppInit_Nominal);
    ADD_TEST(Test_BPNode_AppInit_FailedEvs);
    ADD_TEST(Test_BPNode_AppInit_FailedCmdPipeCreate);
    ADD_TEST(Test_BPNode_AppInit_FailedWakeupPipeCreate);
    ADD_TEST(Test_BPNode_AppInit_FailedCommandSub);
    ADD_TEST(Test_BPNode_AppInit_FailedWakeupSub);
    ADD_TEST(Test_BPNode_AppInit_FailedTblInit);
    ADD_TEST(Test_BPNode_AppInit_FailedFwpInit);
    ADD_TEST(Test_BPNode_AppInit_FailedAduInTasks);
    ADD_TEST(Test_BPNode_AppInit_FailedAduOutTasks);
    ADD_TEST(Test_BPNode_AppInit_FailedTimeInit);
    ADD_TEST(Test_BPNode_AppInit_FailedTlmpInit);
    ADD_TEST(Test_BPNode_AppExit_Nominal);
}
