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
 *  Unit tests for bpnode_gen_worker.c
 */

/*
** Include Files
*/

#include "bpnode_gen_worker.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Test BPNode_GenWorkerCreateTasks when everything succeeds */
void Test_BPNode_GenWorkerCreateTasks_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_GenWorkerCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPNODE_NUM_GEN_WRKR_TASKS * 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPNODE_NUM_GEN_WRKR_TASKS);
}

/* Test BPNode_GenWorkerCreateTasks when the init semaphore fails to create */
void Test_BPNode_GenWorkerCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_GenWorkerCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_SEM_CR_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Failed to create initialization semaphore. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

void Test_BPNode_GenWorkerCreateTasks_WakeupSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_GenWorkerCreateTasks(), OS_SEM_FAILURE);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_GEN_WRKR_SEM_CR_ERR_EID, "[Generic Worker #%d]: Failed to create wakeup semaphore. Error = %d.");

    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_GenWorkerCreateTasks when the child task creation fails */
void Test_BPNode_GenWorkerCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_GenWorkerCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_CREATE_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Failed to create child task. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_GenWorkerCreateTasks when taking the init semaphore fails */
void Test_BPNode_GenWorkerCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_GenWorkerCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Task not running. Init Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_GenWorker_TaskInit when everything succeeds */
void Test_BPNode_GenWorker_TaskInit_Nominal(void)
{
    uint8 WorkerId = BPNODE_NUM_GEN_WRKR_TASKS;
    uint8 ExpWorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.GenWorkerData[ExpWorkerId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_GenWorker_TaskInit(&WorkerId), CFE_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Child Task Initialized.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(WorkerId, ExpWorkerId);
    UtAssert_UINT32_EQ(BPNode_AppData.GenWorkerData[ExpWorkerId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_GenWorker_TaskInit when it fails to get the task ID */
void Test_BPNode_GenWorker_TaskInit_GetIdErr(void)
{
    uint8 WorkerId = BPNODE_NUM_GEN_WRKR_TASKS;
    uint8 ExpWorkerId = BPNODE_NUM_GEN_WRKR_TASKS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_GenWorker_TaskInit(&WorkerId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_NO_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #?]: Failed to get task ID. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(WorkerId, ExpWorkerId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_GenWorker_TaskInit when the task IDs don't match */
void Test_BPNode_GenWorker_TaskInit_MatchIdErr(void)
{
    uint8 WorkerId = BPNODE_NUM_GEN_WRKR_TASKS;
    uint8 ExpWorkerId = BPNODE_NUM_GEN_WRKR_TASKS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.GenWorkerData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_GenWorker_TaskInit(&WorkerId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_INV_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #?]: Task ID does not match any known task IDs. ID = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(WorkerId, ExpWorkerId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_GenWorker_TaskInit when init semaphore give fails */
void Test_BPNode_GenWorker_TaskInit_GiveSemErr(void)
{
    uint8 WorkerId = BPNODE_NUM_GEN_WRKR_TASKS;
    uint8 ExpWorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.GenWorkerData[ExpWorkerId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_GenWorker_TaskInit(&WorkerId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_SEM_INIT_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Failed to give init semaphore. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(WorkerId, ExpWorkerId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_GenWorker_AppMain when semaphore take succeeds */
void Test_BPNode_GenWorker_AppMain_Nominal(void)
{
    uint8 WorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.GenWorkerData[WorkerId].TaskId = TaskId;

    BPNode_GenWorker_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.GenWorkerData[WorkerId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, BPNODE_NUM_JOBS_PER_CYCLE);
}

/* Test BPNode_GenWorker_AppMain when semaphore take fails */
void Test_BPNode_GenWorker_AppMain_SemErr(void)
{
    uint8 WorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_ERROR);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.GenWorkerData[WorkerId].TaskId = TaskId;

    BPNode_GenWorker_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.GenWorkerData[WorkerId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_GEN_WRKR_SEM_TK_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Failure to take semaphore. Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPNode_GenWorker_AppMain_WakeupSemTimeout(void)
{
    uint8 WorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_TIMEOUT);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.GenWorkerData[WorkerId].TaskId = TaskId;

    BPNode_GenWorker_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.GenWorkerData[WorkerId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_SEM_TAKE_TIMEOUT_ERR_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Timed out while waiting for the wakeup semaphore", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test BPNode_GenWorker_AppMain when initialization failed but channel ID is known */
void Test_BPNode_GenWorker_AppMain_InitErr(void)
{
    uint8 WorkerId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.GenWorkerData[WorkerId].TaskId = TaskId;

    BPNode_GenWorker_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.GenWorkerData[WorkerId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_GenWorker_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_GenWorker_AppMain_WorkerIdErr(void)
{
    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_GenWorker_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_GEN_WRKR_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("Terminating Unknown Generic Worker Task.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_GenWorker_TaskExit in nominal shutdown */
void Test_BPNode_GenWorker_TaskExit_Nominal(void)
{
    uint8 WorkerId = 0;

    UtAssert_VOIDCALL(BPNode_GenWorker_TaskExit(WorkerId));
    
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_GEN_WRKR_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[Generic Worker #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_GenWorkerCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_GenWorkerCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_GenWorkerCreateTasks_WakeupSemErr);
    ADD_TEST(Test_BPNode_GenWorkerCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_GenWorkerCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_GenWorker_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_GenWorker_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_GenWorker_TaskInit_MatchIdErr);

    ADD_TEST(Test_BPNode_GenWorker_AppMain_Nominal);
    ADD_TEST(Test_BPNode_GenWorker_AppMain_SemErr);
    ADD_TEST(Test_BPNode_GenWorker_AppMain_WakeupSemTimeout);
    ADD_TEST(Test_BPNode_GenWorker_AppMain_InitErr);
    ADD_TEST(Test_BPNode_GenWorker_AppMain_WorkerIdErr);

    ADD_TEST(Test_BPNode_GenWorker_TaskExit_Nominal);
}
