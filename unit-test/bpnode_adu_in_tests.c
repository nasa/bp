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
 *  Unit tests for bpnode_adu_in.c
 */

/*
** Include Files
*/

#include "bpnode_adu_in.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Test BPNode_AduInCreateTasks when everything succeeds */
void Test_BPNode_AduInCreateTasks_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_AduInCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CHANNELS * 2); /* An init and wake up semaphore are created per channel */
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CHANNELS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CHANNELS);
}

/* Test BPNode_AduInCreateTasks when the init semaphore fails to create */
void Test_BPNode_AduInCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduInCreateTasks(), OS_SEM_FAILURE);

    /* Verify init semaphore creation error created event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_ADU_IN_INIT_SEM_ERR_EID, "[ADU In #%d]: Failed to create task initialization semaphore, %s. Error = %d");

    /* Verify expected operations */
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduInCreateTasks when the wakeup semaphore fails to create */
void Test_BPNode_AduInCreateTasks_WakeupSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduInCreateTasks(), OS_SEM_FAILURE);

    /* Verify wake up semaphore creation error created event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_ADU_IN_WAKEUP_SEM_ERR_EID, "[ADU In #%d]: Failed to create wakeup semaphore, %s. Error = %d.");

    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduInCreateTasks when the child task creation fails */
void Test_BPNode_AduInCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AduInCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    /* Verify event was issued */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_ADU_IN_CREATE_ERR_EID, "[ADU In #%d]: Failed to create child task. Error = %d.");

    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduInCreateTasks when taking the init semaphore fails */
void Test_BPNode_AduInCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduInCreateTasks(), OS_SEM_FAILURE);

    /* Verify event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_ADU_IN_RUN_ERR_EID, "[ADU In #%d]: Task not running. Init Sem Error = %d.");

    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_AduIn_TaskInit when everything succeeds */
void Test_BPNode_AduIn_TaskInit_Nominal(void)
{
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint8 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduInData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_AduIn_TaskInit(&ChanId), CFE_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #%d]: Child Task Initialized.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ExpChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduIn_TaskInit when it fails to get the task ID */
void Test_BPNode_AduIn_TaskInit_GetIdErr(void)
{
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint8 ExpChanId = BPLIB_MAX_NUM_CHANNELS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_AduIn_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_NO_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #?]: Failed to get task ID. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduIn_TaskInit when the task IDs don't match */
void Test_BPNode_AduIn_TaskInit_MatchIdErr(void)
{
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint8 ExpChanId = BPLIB_MAX_NUM_CHANNELS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduInData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_AduIn_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_INV_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #?]: Task ID does not match any known task IDs. ID = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduIn_TaskInit when pipe creation fails */
void Test_BPNode_AduIn_TaskInit_PipeErr(void)
{
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint8 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduInData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_PIPE_CR_ERR);

    UtAssert_INT32_EQ(BPNode_AduIn_TaskInit(&ChanId), CFE_SB_PIPE_CR_ERR);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_CR_PIPE_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #%d]: Error creating SB ADU Pipe, Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduIn_TaskInit when init semaphore give fails */
void Test_BPNode_AduIn_TaskInit_GiveSemErr(void)
{
    uint8 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint8 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduInData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduIn_TaskInit(&ChanId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_INIT_SEM_TK_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #%d]: Failed to give init semaphore. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduIn_AppMain when app state is started and one ADU is received */
void Test_BPNode_AduIn_AppMain_Nominal(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_TIME_OUT);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STARTED);

    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;

    BPNode_AduIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPA_ADUP_In, 1);
}

void Test_BPNode_AduIn_AppMain_TakeSemErr(void)
{
    CFE_ES_TaskId_t TaskId;
    uint8 ChanId;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    /* Enter task loop only once */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ChanId = 0;
    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Run the function under test */
    BPNode_AduIn_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3); // Events are sent for task init, failed semaphore wait, and task termination, in that order
    BPNode_Test_Verify_Event(0, BPNODE_ADU_IN_INIT_INF_EID, "[ADU In #%d]: Child Task Initialized.");
    BPNode_Test_Verify_Event(1, BPNODE_ADU_IN_WAKEUP_SEM_ERR_EID, "[ADU In #%d]: Failed to take wakeup semaphore, RC = %d");
    BPNode_Test_Verify_Event(2, BPNODE_ADU_IN_EXIT_CRIT_EID, "[ADU In #%d]: Terminating Task. RunStatus = %d.");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPLib_NC_GetAppState, 0);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
    UtAssert_STUB_COUNT(BPA_ADUP_In, 0);
}

/* Test BPNode_AduIn_AppMain when app state is started and a null ADU is received */
void Test_BPNode_AduIn_AppMain_NullBuf(void)
{
    CFE_SB_Buffer_t *BufPtr = NULL;
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_TIME_OUT);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STARTED);

    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;

    BPNode_AduIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPA_ADUP_In, 0);
}

/* Test BPNode_AduIn_AppMain when initialization failed but channel ID is known */
void Test_BPNode_AduIn_AppMain_InitErr(void)
{
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;

    BPNode_AduIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
}

/* Test BPNode_AduIn_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_AduIn_AppMain_ChanIdErr(void)
{
    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_AduIn_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_ADU_IN_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("Terminating Unknown ADU In Task.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
}

/* Test BPNode_AduIn_AppMain when app state is stopped */
void Test_BPNode_AduIn_AppMain_AppStopped(void)
{
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STOPPED);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;

    BPNode_AduIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
}

/* Test BPNode_AduIn_AppMain when the app was just stopped and the pipe needs to be cleared */
void Test_BPNode_AduIn_AppMain_ClearPipe(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;
    uint8 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    /* Clear one message from pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE); 
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STOPPED);

    BPNode_AppData.AduInData[ChanId].TaskId = TaskId;
    BPNode_AppData.AduInData[ChanId].ClearPipe = true;

    BPNode_AduIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduInData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPA_ADUP_In, 0);
    UtAssert_BOOL_FALSE(BPNode_AppData.AduInData[ChanId].ClearPipe);
}

/* Test BPNode_AduIn_TaskExit in nominal shutdown */
void Test_BPNode_AduIn_TaskExit_Nominal(void)
{
    uint8 ChanId = 0;

    UtAssert_VOIDCALL(BPNode_AduIn_TaskExit(ChanId));
    
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

// TODO: Add OS_SEM_TIMEOUT case

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_AduInCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_AduInCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_AduInCreateTasks_WakeupSemErr);
    ADD_TEST(Test_BPNode_AduInCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_AduInCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_AduIn_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_AduIn_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_AduIn_TaskInit_MatchIdErr);
    ADD_TEST(Test_BPNode_AduIn_TaskInit_PipeErr);
    ADD_TEST(Test_BPNode_AduIn_TaskInit_GiveSemErr);

    ADD_TEST(Test_BPNode_AduIn_AppMain_Nominal);
    ADD_TEST(Test_BPNode_AduIn_AppMain_TakeSemErr);
    ADD_TEST(Test_BPNode_AduIn_AppMain_NullBuf);
    ADD_TEST(Test_BPNode_AduIn_AppMain_InitErr);
    ADD_TEST(Test_BPNode_AduIn_AppMain_ChanIdErr);
    ADD_TEST(Test_BPNode_AduIn_AppMain_AppStopped);
    ADD_TEST(Test_BPNode_AduIn_AppMain_ClearPipe);

    ADD_TEST(Test_BPNode_AduIn_TaskExit_Nominal);
}
