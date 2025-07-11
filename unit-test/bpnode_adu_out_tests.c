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

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CHANNELS * 2); /* Each channel creates an init, wakeup, and exit semaphore */
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CHANNELS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CHANNELS);
}

/* Test BPNode_AduOutCreateTasks when the init semaphore fails to create */
void Test_BPNode_AduOutCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_INIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Failed to create initialization semaphore, %s. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduOutCreateTasks when the exit semaphore fails to create */
void Test_BPNode_AduOutCreateTasks_ExitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), OS_SEM_FAILURE);

    /* Verify that wake up semaphore error during creation created an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_ADU_OUT_EXIT_SEM_ERR_EID, "[ADU Out #%d]: Failed to create exit semaphore, %s. Error = %d.");

    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduOutCreateTasks when the child task creation fails */
void Test_BPNode_AduOutCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_CREATE_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Failed to create child task. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_AduOutCreateTasks when taking the init semaphore fails */
void Test_BPNode_AduOutCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Task not running. Init Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_AduOut_TaskInit when everything succeeds */
void Test_BPNode_AduOut_TaskInit_Nominal(void)
{
    uint32 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint32 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Child Task Initialized.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ExpChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduOut_TaskInit when it fails to get the task ID */
void Test_BPNode_AduOut_TaskInit_GetIdErr(void)
{
    uint32 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint32 ExpChanId = BPLIB_MAX_NUM_CHANNELS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_NO_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #?]: Failed to get task ID. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduOut_TaskInit when the task IDs don't match */
void Test_BPNode_AduOut_TaskInit_MatchIdErr(void)
{
    uint32 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint32 ExpChanId = BPLIB_MAX_NUM_CHANNELS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_INV_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #?]: Task ID does not match any known task IDs. ID = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_AduOut_TaskInit when init semaphore give fails */
void Test_BPNode_AduOut_TaskInit_GiveSemErr(void)
{
    uint32 ChanId = BPLIB_MAX_NUM_CHANNELS;
    uint32 ExpChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.AduOutData[ExpChanId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_AduOut_TaskInit(&ChanId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_INIT_SEM_TK_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Failed to give init semaphore. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ChanId, ExpChanId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_AduOut_AppMain when app state is started */
void Test_BPNode_AduOut_AppMain_Nominal(void)
{
    uint32 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;
    // size_t AduSize = 10;
    // uint16 i;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STARTED);
    // BPNode_UT_BundleProcessLoops(BPNODE_ADU_OUT_MAX_ADUS_PER_CYCLE);

    // for (i = 0; i < BPNODE_ADU_OUT_MAX_ADUS_PER_CYCLE; i++)
    // {
    //     UT_SetDataBuffer(UT_KEY(BPA_ADUP_Out), &AduSize, sizeof(AduSize), false);
    // }

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;
    BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].EgressBitsPerCycle = 10000000000;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(BPLib_NC_GetAppState, 1);
//    UtAssert_STUB_COUNT(BPNode_NotifIsSet, BPNODE_ADU_OUT_MAX_ADUS_PER_CYCLE);
}

void Test_BPNode_AduOut_AppMain_NotifErr(void)
{
    CFE_ES_TaskId_t TaskId;
    uint32 ChanId;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR);

    /* Enter task loop only once */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ChanId = 0;
    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Run the function under test */
    BPNode_AduOut_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3); // Events are sent for task init, failed semaphore wait, and task termination, in that order
    BPNode_Test_Verify_Event(1, BPNODE_ADU_OUT_NOTIF_ERR_EID, "[ADU Out #%d]: Error pending on notification, RC = %d");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPLib_NC_GetAppState, 0);
}

void Test_BPNode_AduOut_AppMain_NotifTimeout(void)
{
    CFE_ES_TaskId_t TaskId;
    uint32 ChanId;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR_TIMEOUT);

    /* Enter task loop only once */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ChanId = 0;
    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Run the function under test */
    BPNode_AduOut_AppMain();

    /* Verify the error did not issue an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2); // Events are sent for task init and task termination, in that order
 
    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPLib_NC_GetAppState, 0);
}

/* Test BPNode_AduOut_AppMain when initialization failed but channel ID is known */
void Test_BPNode_AduOut_AppMain_InitErr(void)
{
    uint32 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_AduOut_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_AduOut_AppMain_ChanIdErr(void)
{
    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_AduOut_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_ADU_OUT_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("Terminating Unknown ADU Out Task.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_AduOut_AppMain when app state is stopped */
void Test_BPNode_AduOut_AppMain_AppStopped(void)
{
    uint32 ChanId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_GetAppState), BPLIB_NC_APP_STATE_STOPPED);

    BPNode_AppData.AduOutData[ChanId].TaskId = TaskId;

    BPNode_AduOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[ChanId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(BPNode_NotifWait, 1);
}

/* Test BPNode_AduOut_TaskExit in nominal shutdown */
void Test_BPNode_AduOut_TaskExit_Nominal(void)
{
    uint32 ChanId = 0;

    UtAssert_VOIDCALL(BPNode_AduOut_TaskExit(ChanId));

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_AduOutCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_AduOutCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_AduOut_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_AduOut_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_AduOut_TaskInit_MatchIdErr);
    ADD_TEST(Test_BPNode_AduOut_TaskInit_GiveSemErr);

    ADD_TEST(Test_BPNode_AduOut_AppMain_Nominal);
    ADD_TEST(Test_BPNode_AduOut_AppMain_NotifErr);
    ADD_TEST(Test_BPNode_AduOut_AppMain_NotifTimeout);
    ADD_TEST(Test_BPNode_AduOut_AppMain_InitErr);
    ADD_TEST(Test_BPNode_AduOut_AppMain_ChanIdErr);
    ADD_TEST(Test_BPNode_AduOut_AppMain_AppStopped);

    ADD_TEST(Test_BPNode_AduOut_TaskExit_Nominal);
}
