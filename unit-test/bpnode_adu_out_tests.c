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
 *  Unit tests for bpnode_adu_out.c
 */

/*
** Include Files
*/

#include "bpnode_adu_out.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Test BPNode_AduOutCreateTasks when everything succeeds */
void Test_BPNode_AduOutCreateTasks_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPNODE_MAX_NUM_CHANNELS);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPNODE_MAX_NUM_CHANNELS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPNODE_MAX_NUM_CHANNELS);
}

/* Test BPNode_AduOutCreateTasks when the init semaphore fails to create */
void Test_BPNode_AduOutCreateTasks_InitSemErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_INIT_SEM_ERR_EID, 
                    "Failed to create the ADU Out #%d task init semaphore. Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduOutCreateTasks when the child task creation fails */
void Test_BPNode_AduOutCreateTasks_TaskCrErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_CREATE_ERR_EID, 
                    "Failed to create the ADU Out #%d child task. Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduOutCreateTasks when taking the init semaphore fails */
void Test_BPNode_AduOutCreateTasks_TakeSemErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_RUN_ERR_EID, 
                    "ADU Out #%d task not running. Init Sem Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_AduOut_TaskInit when everything succeeds */
void Test_BPNode_AduOut_TaskInit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS;
    uint8 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_INIT_INF_EID, 
                    "[ADU Out #%d]: Child Task Initialized.");

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_SUCCESS);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ExpChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduOut_TaskInit when it fails to get the task ID */
void Test_BPNode_AduOut_TaskInit_GetIdErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS;
    uint8 ExpChanId = BPNODE_MAX_NUM_CHANNELS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_NO_ID_ERR_EID, 
                    "[ADU Out #?]: Failed to get task ID. Error = %d");

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduOut_TaskInit when the task IDs don't match */
void Test_BPNode_AduOut_TaskInit_MatchIdErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS;
    uint8 ExpChanId = BPNODE_MAX_NUM_CHANNELS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_INV_ID_ERR_EID, 
                    "[ADU Out #?]: Task ID does not match any known task IDs. ID = %d");

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduOut_TaskInit when init semaphore give fails */
void Test_BPNode_AduOut_TaskInit_GiveSemErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ChanId = BPNODE_MAX_NUM_CHANNELS;
    uint8 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID, 
                    "[ADU Out #%d]: Failed to give init semaphore. Error = %d");

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduOut_AppMain when app state is started */
void Test_BPNode_AduOut_AppMain_Nominal(void)
{
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;
    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STARTED;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_AduOut_AppMain when initialization failed but channel ID is known */
void Test_BPNode_AduOut_AppMain_InitErr(void)
{
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_AduOut_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_AduOut_AppMain_ChanIdErr(void)
{
    UT_CheckEvent_t EventTest;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID, 
                                "Terminating Unknown ADU Out Task.");

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_AduOut_AppMain when app state is stopped */
void Test_BPNode_AduOut_AppMain_AppStopped(void)
{
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;
    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STOPPED;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
}

/* Test BPNode_AduOut_TaskExit in nominal shutdown */
void Test_BPNode_AduOut_TaskExit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ChanId = 0;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADU_OUT_EXIT_CRIT_EID, 
                                "[ADU Out #%d]: Terminating Task. RunStatus = %d.");

    UtAssert_VOIDCALL(BPNode_AduOut_TaskExit(ChanId));
    
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_AduOutCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_AduOut_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_AduOut_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_AduOut_TaskInit_MatchIdErr);

    ADD_TEST(Test_BPNode_AduOut_AppMain_Nominal);
    ADD_TEST(Test_BPNode_AduOut_AppMain_AppStopped);
    ADD_TEST(Test_BPNode_AduOut_AppMain_InitErr);
    ADD_TEST(Test_BPNode_AduOut_AppMain_ChanIdErr);

    ADD_TEST(Test_BPNode_AduOut_TaskExit_Nominal);
}
