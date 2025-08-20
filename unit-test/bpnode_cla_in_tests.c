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
 *  Unit tests for bpnode_cla_in.c
 */

/*
** Include Files
*/

#include "bpnode_cla_in.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Test BPNode_ClaInCreateTasks when everything succeeds */
void Test_BPNode_ClaInCreateTasks_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS * 2); /* Account for init, wakeup, and exit semaphore creation */
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaInCreateTasks when the init semaphore fails to create */
void Test_BPNode_ClaInCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_INIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create init semaphore, %s, for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}


/* Test BPNode_ClaInCreateTasks when the exit semaphore fails to create */
void Test_BPNode_ClaInCreateTasks_ExitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_EXIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create exit semaphore, %s, for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaInCreateTasks when the child task creation fails */
void Test_BPNode_ClaInCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CREATE_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create child task for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaInCreateTasks when taking the init semaphore fails */
void Test_BPNode_ClaInCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("CLA In task #%d not running. Init Sem Error = %d.",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_ClaIn_TaskInit when the PSP module can't be found */
void Test_BPNode_ClaIn_TaskInit_FindByNameErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_FindByName), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(0), CFE_PSP_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_FIND_NAME_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't find I/O driver. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 0);
}

void Test_BPNode_ClaIn_TaskInit_DirErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(0), CFE_PSP_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_DIR_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't set I/O direction to input. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
}

void Test_BPNode_ClaIn_TaskInit_RunErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemGive), OS_ERR_INVALID_ID);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(0), OS_ERR_INVALID_ID);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Task not running. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPNode_ClaIn_Setup_Nominal(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    /* Force called function to return values that will create a success return value */
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), CFE_PSP_SUCCESS);

    /* Call function under test and verify return status */
    UtAssert_EQ(BPLib_Status_t, BPNode_ClaIn_Setup(0, 0, "0.0.0.0"), BPLIB_SUCCESS);
#endif
}

void Test_BPNode_ClaIn_Setup_PortErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_Setup(0, 0, "0.0.0.0"), BPLIB_CLA_IO_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_PORT_ERR_EID);
    UtAssert_STRINGBUF_EQ("Couldn't configure port number for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
#endif
}

void Test_BPNode_ClaIn_Setup_IpErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 2, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_Setup(0, 0, "0.0.0.0"), BPLIB_CLA_IO_ERROR);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_IP_ERR_EID);
    UtAssert_STRINGBUF_EQ("Couldn't configure IP address for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 2);
#endif
}

/* Test BPNode_ClaIn_AppMain when app state is started and one CLA is received */
void Test_BPNode_ClaIn_AppMain_Nominal(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;
    BPLib_CLA_ContactRunState_t RunState2 = BPLIB_CLA_EXITED;

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_STOPPED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(RunState), false); /* Exits the run loop */
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(RunState2), false); /* Exits the run loop */

    BPNode_AppData.ConfigPtrs.ContactsConfigPtr->ContactSet[ContactId].IngressBitsPerCycle = 0;
    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

void Test_BPNode_ClaIn_AppMain_GetTaskIDError(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_UNK_EXIT_CRIT_EID,
                                "[CLA In #?]: Terminating unknown task");
}

void Test_BPNode_ClaIn_AppMain_NoContactId(void)
{
    CFE_ES_TaskId_t TaskId;

    TaskId = 1234;

    /* Test setup */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(CFE_ES_TaskId_t), false);

    memset((void*) BPNode_AppData.ClaInData, 0, sizeof(BPNode_ClaInData_t) * BPLIB_MAX_NUM_CONTACTS);

    BPNode_ClaIn_AppMain();

    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_INV_ID_ERR_EID,
                                "[CLA In #?] Could not find a task to process bundles with");
}

void Test_BPNode_ClaIn_AppMain_NotifTimeoutErr(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32_t                    ContactId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;

    TaskId    = 1234;
    ContactId = 0;
    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Pass contact ID gathering process */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR_TIMEOUT);

    /* Enter task loop only once */
    RunState1 = BPLIB_CLA_STARTED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);

    RunState2 = BPLIB_CLA_EXITED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    /* Run the function under test */
    BPNode_ClaIn_AppMain();

    /* Verify the error did not issue an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

void Test_BPNode_ClaIn_AppMain_NotifOtherErr(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32_t                    ContactId;
    BPLib_CLA_ContactRunState_t RunState;

    TaskId    = 1234;
    ContactId = 0;
    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Pass contact ID gathering process */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(BPNode_NotifWait), 1, OS_ERROR);

    /* Enter task loop only once */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, false);
    RunState = BPLIB_CLA_STARTED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);


    /* Run the function under test */
    BPNode_ClaIn_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
    BPNode_Test_Verify_Event(1, BPNODE_CLA_IN_NOTIF_ERR_EID,
                                "[CLA In #%d]: Error pending on notification, RC = %d");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

/* Test BPNode_ClaIn_AppMain when ingress service is disabled */
void Test_BPNode_ClaIn_AppMain_NoIngress(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemTimedWait), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(BPLib_CLA_GetContactRunState, 1);
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

void Test_BPNode_ClaIn_AppMain_FailedProcBundle(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemTimedWait), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), CFE_PSP_ERROR);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
}

void Test_BPNode_ClaIn_AppMain_OneBundle(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;

    ContactId = 0;
    TaskId    = 1234;
    RunState = BPLIB_CLA_STARTED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_CLA_GetContactRunState), BPLIB_SUCCESS);

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR_TIMEOUT);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;
    BPNode_AppData.ClaInData[ContactId].RateLimit = 80000;

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 3);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

void Test_BPNode_ClaIn_AppMain_MaxLimit(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;

    ContactId = 0;
    TaskId    = 1234;
    RunState = BPLIB_CLA_STARTED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_CLA_GetContactRunState), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), BPLIB_SUCCESS);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;
    /* Rate limit will be reached by two bundles of max default size */
    BPNode_AppData.ClaInData[ContactId].RateLimit = 8192 * 2 * 8;
    
    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 3);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

/* Test BPNode_ClaIn_TaskExit in nominal shutdown */
void Test_BPNode_ClaIn_TaskExit_Nominal(void)
{
    uint8 ContactId = 0;

    UtAssert_VOIDCALL(BPNode_ClaIn_TaskExit(ContactId));

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Terminating Task. Run state = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

void Test_BPNode_ClaIn_ProcessBundleInput_NominalUDP(void)
{
    uint8 ContactId;
    size_t BundleSize;

    /* UDP case */
    ContactId = 0;
    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContactId, &BundleSize), CFE_SUCCESS);
}

void Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand(void)
{
    uint8 ContactId = 0;
    size_t BundleSize;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);
    UtAssert_INT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContactId, &BundleSize), BPLIB_TIMEOUT);

    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_IO_READ_ERR_EID,
                            "[CLA In #%d]: Failed to read packet from UDP socket, RC = %d");
}

void Test_BPNode_ClaIn_Start_Nominal(void)
{
    BPLib_Status_t Status;
    uint32 ContId = 0;

    Status = BPNode_ClaIn_Start(ContId);
    
    UtAssert_INT32_EQ(Status, BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

void Test_BPNode_ClaIn_Stop_Nominal(void)
{
    BPLib_Status_t Status;
    uint32 ContId = 0;

    Status = BPNode_ClaIn_Stop(ContId);
    
    UtAssert_INT32_EQ(Status, BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaInCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TakeSemErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_FindByNameErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_DirErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_RunErr);
    ADD_TEST(Test_BPNode_ClaIn_Setup_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_Setup_PortErr);
    ADD_TEST(Test_BPNode_ClaIn_Setup_IpErr);

    ADD_TEST(Test_BPNode_ClaIn_Start_Nominal);

    ADD_TEST(Test_BPNode_ClaIn_Stop_Nominal);

    ADD_TEST(Test_BPNode_ClaIn_AppMain_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_GetTaskIDError);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NoContactId);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NotifTimeoutErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NotifOtherErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NoIngress);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_FailedProcBundle);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_OneBundle);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_MaxLimit);
    
    ADD_TEST(Test_BPNode_ClaIn_TaskExit_Nominal);

    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_NominalUDP);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand);
}