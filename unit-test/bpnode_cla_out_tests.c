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

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, (BPLIB_MAX_NUM_CONTACTS * 2));
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaOutCreateTasks when the init semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create init semaphore, %s, for CLA Out #%d. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the exit semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_ExitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_EXIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create exit semaphore, %s, for CLA Out #%d. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the child task creation fails */
void Test_BPNode_ClaOutCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CREATE_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create child task for CLA Out #%d. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when taking the init semaphore fails */
void Test_BPNode_ClaOutCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("CLA Out task #%d not running. Init Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_ClaOut_TaskInit when everything succeeds */
void Test_BPNode_ClaOut_TaskInit_Nominal(void)
{
    uint32 ContactId = 0;

    /* Test setup */
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_FindByName), CFE_PSP_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), CFE_PSP_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemGive), OS_SUCCESS);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(ContactId), OS_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Child task initialized", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaOut_TaskInit when the PSP module can't be found */
void Test_BPNode_ClaOut_TaskInit_FindByNameErr(void)
{
    uint32 ContactId       = BPLIB_MAX_NUM_CONTACTS - 1;
    uint32 ExpContactId    = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContactId].TaskId = TaskId;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_FindByName), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(ContactId), CFE_PSP_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_FIND_NAME_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't find I/O driver. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 0);
}

/* Test BPNode_ClaOut_TaskInit when the direction can't be set */
void Test_BPNode_ClaOut_TaskInit_DirErr(void)
{
    uint32 ContactId       = BPLIB_MAX_NUM_CONTACTS - 1;
    uint32 ExpContactId    = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContactId].TaskId = TaskId;

    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_FindByName), CFE_PSP_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(ContactId), CFE_PSP_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CFG_DIR_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't set I/O direction to output. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test BPNode_ClaOut_TaskInit when init semaphore give fails */
void Test_BPNode_ClaOut_TaskInit_GiveSemErr(void)
{
    uint32 ContactId       = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(ContactId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Task not running. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaOut_AppMain when app state is started */
void Test_BPNode_ClaOut_AppMain_NoBundleAvailable(void)
{
    uint32                      ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    ContactId = BPLIB_MAX_NUM_CONTACTS - 1;
    TaskId    = 1234;
    RunState1 = BPLIB_CLA_STARTED;
    RunState2 = BPLIB_CLA_EXITED;

    BPNode_AppData.ConfigPtrs.ContactsConfigPtr->ContactSet[ContactId].EgressBitsPerCycle = 100000;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);

    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;

    BPNode_ClaOut_AppMain();

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(BPLib_AS_Increment, 0);
    UtAssert_STUB_COUNT(BPNode_NotifWait, 1);
}

/* Test BPNode_ClaOut_AppMain when max number of bundles are egressed */
void Test_BPNode_ClaOut_AppMain_SingleBundle(void)
{
    uint32                      ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    ContactId = BPLIB_MAX_NUM_CONTACTS - 1;
    TaskId    = 1234;
    RunState1  = BPLIB_CLA_STARTED;
    RunState2  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);

    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;
    BPNode_AppData.ConfigPtrs.ContactsConfigPtr->ContactSet[ContactId].EgressBitsPerCycle = 100000;

    // TODO How to add more bundles?

    BPNode_ClaOut_AppMain();

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(BPNode_NotifWait, 1);
}

void Test_BPNode_ClaOut_AppMain_NotifErr(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32                      ContactId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ContactId = 0;
    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;
    RunState1 = BPLIB_CLA_STARTED;
    RunState2 = BPLIB_CLA_EXITED;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    /* Run the function under test */
    BPNode_ClaOut_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3); // Events are sent for task init, failed semaphore wait, and task termination, in that order
    BPNode_Test_Verify_Event(1, BPNODE_CLA_OUT_NOTIF_ERR_EID, "[CLA Out #%d]: Error pending on notification, RC = %d");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaOut_ProcessBundleOutput, 0);
}

void Test_BPNode_ClaOut_AppMain_NotifTimeout(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32                      ContactId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR_TIMEOUT);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ContactId = BPLIB_MAX_NUM_CONTACTS - 1;
    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;
    RunState1 = BPLIB_CLA_STARTED;
    RunState2 = BPLIB_CLA_EXITED;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    /* Run the function under test */
    BPNode_ClaOut_AppMain();

    /* Verify the error did not issue an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2); // Events are sent for task init and task termination, in that order

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaOut_ProcessBundleOutput, 0);
}

/* Test BPNode_ClaOut_AppMain when initialization failed but channel ID is known */
void Test_BPNode_ClaOut_AppMain_InitErr(void)
{
    uint32          ContactId;
    CFE_ES_TaskId_t TaskId;

    ContactId = BPLIB_MAX_NUM_CONTACTS - 1;
    TaskId    = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;

    BPNode_ClaOut_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Terminating Task. Run state = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

/* Test BPNode_ClaOut_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_ClaOut_AppMain_ContactIdErr(void)
{
    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);
    
    BPNode_ClaOut_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #?]: Terminating unknown task", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
}

/* Test BPNode_ClaOut_AppMain when bundle egress is disabled */
void Test_BPNode_ClaOut_AppMain_NoEgress(void)
{
    uint32                      ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    ContactId = 0;
    TaskId    = 1234;
    RunState1 = BPLIB_CLA_STARTED;
    RunState2 = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;

    BPNode_ClaOut_AppMain();

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

void Test_BPNode_ClaOut_AppMain_FailedProcBundle(void)
{
    uint32                      ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    ContactId = 0;
    TaskId    = 1234;
    RunState1 = BPLIB_CLA_STARTED;
    RunState2 = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDeferredRetcode(UT_KEY(BPNode_ClaOut_ProcessBundleOutput), 1, CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    BPNode_AppData.ClaOutData[ContactId].TaskId = TaskId;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_VOIDCALL(BPNode_ClaOut_AppMain());

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
}

/* Test BPNode_ClaOut_TaskExit in nominal shutdown */
void Test_BPNode_ClaOut_TaskExit_Nominal(void)
{
    uint32 ContactId = 0;

    UtAssert_VOIDCALL(BPNode_ClaOut_TaskExit(ContactId));

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Terminating Task. Run state = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_PSP_Nominal(void)
{
    uint32 ContactId;
    size_t BundleSize;

    ContactId = 0;

    UtAssert_UINT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContactId, &BundleSize), CFE_SUCCESS);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_FailedBPLibEgress(void)
{
    uint32 ContactId = 0;
    size_t BundleSize;

    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_INT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContactId, &BundleSize), BPLIB_ERROR);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_LIB_LOAD_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to get bundle for egress. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_CLATimeout(void)
{
    uint32 ContactId = 0;
    size_t BundleSize;
    
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);
    UtAssert_INT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContactId, &BundleSize), BPLIB_CLA_TIMEOUT);
}

void Test_BPNode_ClaOut_Setup_Nominal(void)
{
    BPLib_Status_t Status;
    uint32 ContId = 0;
    int32 PortNum = 100;
    const char *IpAddr = "127.0.0.1";

    Status = BPNode_ClaOut_Setup(ContId, PortNum, (char *) IpAddr);
    
    UtAssert_INT32_EQ(Status, BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

void Test_BPNode_ClaOut_Start_Nominal(void)
{
    BPLib_Status_t Status;
    uint32 ContId = 0;

    Status = BPNode_ClaOut_Start(ContId);
    
    UtAssert_INT32_EQ(Status, BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

void Test_BPNode_ClaOut_Stop_Nominal(void)
{
    BPLib_Status_t Status;
    uint32 ContId = 0;

    Status = BPNode_ClaOut_Stop(ContId);
    
    UtAssert_INT32_EQ(Status, BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

void Test_BPNode_ClaOut_DeleteSems_Nominal(void)
{
    uint32 ContId = 0;

    (void) OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContId].InitSemId, "Name", 0, 0);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaOutData[ContId].ExitSemId, "Name", 0, 0);

    BPNode_ClaOut_DeleteSems(ContId);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);    
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_ClaOut_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_FindByNameErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_DirErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_GiveSemErr);

    ADD_TEST(Test_BPNode_ClaOut_AppMain_NoBundleAvailable);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_SingleBundle);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_NotifErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_NotifTimeout);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_InitErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_ContactIdErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_NoEgress);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_FailedProcBundle);

    ADD_TEST(Test_BPNode_ClaOut_TaskExit_Nominal);
<<<<<<< HEAD
=======

    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_SB_Nominal);
>>>>>>> 886adf78509bb946ad3716df3639bca4ef9f39b8
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_PSP_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_FailedBPLibEgress);
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_CLATimeout);

    ADD_TEST(Test_BPNode_ClaOut_Setup_Nominal);

    ADD_TEST(Test_BPNode_ClaOut_Start_Nominal);

    ADD_TEST(Test_BPNode_ClaOut_Stop_Nominal);

    ADD_TEST(Test_BPNode_ClaOut_DeleteSems_Nominal);
}
