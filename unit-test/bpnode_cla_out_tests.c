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
 *  Unit tests for bpnode_cla_out.c
 */

/*
** Include Files
*/

#include "bpnode_cla_out.h"
#include "bpnode_test_utils.h"

/*
** Function Definitions
*/

/* Test BPNode_ClaOutCreateTasks when everything succeeds */
void Test_BPNode_ClaOutCreateTasks_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaOutCreateTasks when the init semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_InitSemErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_INIT_SEM_ERR_EID, 
                    "Failed to create the CLA Out #%d task init semaphore. Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the child task creation fails */
void Test_BPNode_ClaOutCreateTasks_TaskCrErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_CREATE_ERR_EID, 
                    "Failed to create the CLA Out #%d child task. Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when taking the init semaphore fails */
void Test_BPNode_ClaOutCreateTasks_TakeSemErr(void)
{
    UT_CheckEvent_t EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_RUN_ERR_EID, 
                    "CLA Out #%d task not running. Init Sem Error = %d.");

    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

void Test_BPNode_ClaOutCreateTasks_IODFindByNameErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_FindByName), 1, CFE_PSP_ERROR);
    UtAssert_INT32_NEQ(BPNode_ClaOutCreateTasks(), CFE_SUCCESS);
    
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS);
}

void Test_BPNode_ClaOutCreateTasks_IODCommandDirErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);
    UtAssert_INT32_NEQ(BPNode_ClaOutCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS);
}

void Test_BPNode_ClaOutCreateTasks_IODCommandRunningErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 2, CFE_PSP_ERROR);
    UtAssert_INT32_NEQ(BPNode_ClaOutCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaOut_TaskInit when everything succeeds */
void Test_BPNode_ClaOut_TaskInit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_INIT_INF_EID, 
                    "[CLA Out #%d]: Child Task Initialized.");

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_SUCCESS);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ExpContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaOut_TaskInit when it fails to get the task ID */
void Test_BPNode_ClaOut_TaskInit_GetIdErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_NO_ID_ERR_EID, 
                    "[CLA Out #?]: Failed to get task ID. Error = %d");

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_ClaOut_TaskInit when the task IDs don't match */
void Test_BPNode_ClaOut_TaskInit_MatchIdErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_INV_ID_ERR_EID, 
                    "[CLA Out #?]: Task ID does not match any known task IDs. ID = %d");

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_ClaOut_TaskInit when init semaphore give fails */
void Test_BPNode_ClaOut_TaskInit_GiveSemErr(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_INIT_SEM_TK_ERR_EID, 
                    "[CLA Out #%d]: Failed to give init semaphore. Error = %d");

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), OS_SEM_FAILURE);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaOut_AppMain when app state is started */
void Test_BPNode_ClaOut_AppMain_Nominal(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled = true;

    BPNode_ClaOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaOut_AppMain when initialization failed but channel ID is known */
void Test_BPNode_ClaOut_AppMain_InitErr(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;

    BPNode_ClaOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaOut_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_ClaOut_AppMain_ContIdErr(void)
{
    UT_CheckEvent_t EventTest;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID, 
                                "Terminating Unknown CLA Out Task.");

    BPNode_ClaOut_AppMain();

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaOut_AppMain when app state is stopped */
void Test_BPNode_ClaOut_AppMain_AppStopped(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled = false;

    BPNode_ClaOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
}

void Test_BPNode_ClaOut_AppMain_FailedProcBundle(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(BPNode_CLA_ProcessBundleOutput), 1, CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled = true;
    BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = 0;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_VOIDCALL(BPNode_ClaOut_AppMain());

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);    
}

/* Test BPNode_ClaOut_TaskExit in nominal shutdown */
void Test_BPNode_ClaOut_TaskExit_Nominal(void)
{
    UT_CheckEvent_t EventTest;
    uint8 ContId = 0;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CLA_OUT_EXIT_CRIT_EID, 
                                "[CLA Out #%d]: Terminating Task. RunStatus = %d.");

    UtAssert_VOIDCALL(BPNode_ClaOut_TaskExit(ContId));
    
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

void Test_BPNode_CLA_ProcessBundleOutput_Nominal(void)
{
    uint8 ContId = 0;
    BPNode_ClaOutData_t CLAEgress;
    memset(&CLAEgress, 0, sizeof(BPNode_ClaOutData_t));
    UtAssert_UINT32_EQ(BPNode_CLA_ProcessBundleOutput(&CLAEgress, ContId), CFE_SUCCESS);
}

void Test_BPNode_CLA_ProcessBundleOutput_FailedBPLibEgress(void)
{
    uint8 ContId = 0;
    BPNode_ClaOutData_t CLAEgress;
    memset(&CLAEgress, 0, sizeof(BPNode_ClaOutData_t));
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_UINT32_NEQ(BPNode_CLA_ProcessBundleOutput(&CLAEgress, ContId), CFE_SUCCESS);
}

void Test_BPNode_CLA_ProcessBundleOutput_CLATimeout(void)
{
    uint8 ContId = 0;
    BPNode_ClaOutData_t CLAEgress;
    memset(&CLAEgress, 0, sizeof(BPNode_ClaOutData_t));
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);
    UtAssert_UINT32_EQ(BPNode_CLA_ProcessBundleOutput(&CLAEgress, ContId), CFE_SUCCESS);
}

void Test_BPNode_CLA_ProcessBundleOutput_NonZeroBuffSize(void)
{
    uint8 ContId = 0;
    BPNode_ClaOutData_t CLAEgress;
    CLAEgress.CurrentBufferSize = 1;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);
    UtAssert_UINT32_EQ(BPNode_CLA_ProcessBundleOutput(&CLAEgress, ContId), CFE_SUCCESS);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TakeSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_IODFindByNameErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_IODCommandDirErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_IODCommandRunningErr);

    ADD_TEST(Test_BPNode_ClaOut_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_MatchIdErr);

    ADD_TEST(Test_BPNode_ClaOut_AppMain_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_AppStopped);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_InitErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_ContIdErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_FailedProcBundle);

    ADD_TEST(Test_BPNode_ClaOut_TaskExit_Nominal);
    
    ADD_TEST(Test_BPNode_CLA_ProcessBundleOutput_Nominal);
    ADD_TEST(Test_BPNode_CLA_ProcessBundleOutput_FailedBPLibEgress);
    ADD_TEST(Test_BPNode_CLA_ProcessBundleOutput_CLATimeout);
    ADD_TEST(Test_BPNode_CLA_ProcessBundleOutput_NonZeroBuffSize);    
}