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
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS * 3); /* Account for init, wakeup, and exit semaphore creation */
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

/* Test BPNode_ClaInCreateTasks when the wakeup semaphore fails to create */
void Test_BPNode_ClaInCreateTasks_WakeupSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_WAKEUP_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create wakeup semaphore, %s, for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaInCreateTasks when the exit semaphore fails to create */
void Test_BPNode_ClaInCreateTasks_ExitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 3, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_EXIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create exit semaphore, %s, for CLA In #%d. Error = %d",
                            BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec,
                            BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
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
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
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
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
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

void Test_BPNode_ClaIn_TaskInit_CreatePipeErr(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SB_BAD_ARGUMENT);

    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_TaskInit(BPNODE_CLA_SB_CONTACT_ID), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_CREATE_PIPE_ERR_EID,
                                "[CLA In #%d]: Error creating CLA In task SB pipe, RC = 0x%08lX");
}

void Test_BPNode_ClaIn_TaskInit_SubscribeErr(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SB_MAX_MSGS_MET);

    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_TaskInit(BPNODE_CLA_SB_CONTACT_ID), CFE_SB_MAX_MSGS_MET);

    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_SUB_ERR_EID,
                                "[CLA In #%d]: Error subscribing to CLA In task messages, RC = 0x%08lX");
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
    char                        NameBuff[OS_MAX_API_NAME];

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(RunState), false); /* Exits the run loop */
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemTimedWait), OS_SUCCESS);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Create task semaphores */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

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

void Test_BPNode_ClaIn_AppMain_TakeSemTimeoutErr(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32_t                    ContactId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;
    char                        NameBuff[OS_MAX_API_NAME];

    TaskId    = 1234;
    ContactId = 0;
    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Pass contact ID gathering process */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_TIMEOUT);

    /* Enter task loop only once */
    RunState1 = BPLIB_CLA_STARTED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);

    RunState2 = BPLIB_CLA_EXITED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    /* Create task semaphores */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

    /* Run the function under test */
    BPNode_ClaIn_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
    BPNode_Test_Verify_Event(1, BPNODE_CLA_IN_SEM_TK_TIMEOUT_INF_EID,
                                "[CLA In #%d]: Wakeup semaphore wait timed out");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

void Test_BPNode_ClaIn_AppMain_TakeSemOtherErr(void)
{
    CFE_ES_TaskId_t             TaskId;
    uint32_t                    ContactId;
    BPLib_CLA_ContactRunState_t RunState1;
    BPLib_CLA_ContactRunState_t RunState2;
    char                        NameBuff[OS_MAX_API_NAME];

    TaskId    = 1234;
    ContactId = 0;
    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Pass contact ID gathering process */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    /* Enter task loop only once */
    RunState1 = BPLIB_CLA_STARTED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState1, sizeof(BPLib_CLA_ContactRunState_t), false);

    RunState2 = BPLIB_CLA_EXITED;
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState2, sizeof(BPLib_CLA_ContactRunState_t), false);

    /* Create task semaphores */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

    /* Run the function under test */
    BPNode_ClaIn_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
    BPNode_Test_Verify_Event(1, BPNODE_CLA_IN_WAKEUP_SEM_ERR_EID,
                                "[CLA In #%d]: Wakeup semaphore take failed, RC = %d");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

/* Test BPNode_ClaIn_AppMain when ingress service is disabled */
void Test_BPNode_ClaIn_AppMain_NoIngress(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;
    char                        NameBuff[OS_MAX_API_NAME];

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemTimedWait), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Create task semaphores */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(BPLib_CLA_GetContactRunState, 2);
    UtAssert_STUB_COUNT(BPNode_ClaIn_ProcessBundleInput, 0);
}

void Test_BPNode_ClaIn_AppMain_FailedProcBundle(void)
{
    uint32_t                    ContactId;
    CFE_ES_TaskId_t             TaskId;
    BPLib_CLA_ContactRunState_t RunState;
    char                        NameBuff[OS_MAX_API_NAME];

    ContactId = 0;
    TaskId    = 1234;
    RunState  = BPLIB_CLA_EXITED;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_BinSemTimedWait), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(BPLib_CLA_GetContactRunState), &RunState, sizeof(BPLib_CLA_ContactRunState_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_PSP_IODriver_Command), CFE_PSP_ERROR);

    BPNode_AppData.ClaInData[ContactId].TaskId = TaskId;

    /* Create task semaphores */
    snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].InitSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].WakeupSemId, NameBuff, 0, 0);

    snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, ContactId);
    (void) OS_BinSemCreate(&BPNode_AppData.ClaInData[ContactId].ExitSemId, NameBuff, 0, 0);

    BPNode_ClaIn_AppMain();

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
    UtAssert_STUB_COUNT(BPNode_NotifIsSet, 0);
}

/* Test BPNode_ClaIn_TaskExit in nominal shutdown */
// void Test_BPNode_ClaIn_TaskExit_Nominal(void)
// {
//     uint8 ContactId = 0;

//     UtAssert_VOIDCALL(BPNode_ClaIn_TaskExit(ContactId));

//     UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_EXIT_CRIT_EID);
//     UtAssert_STRINGBUF_EQ("[CLA In #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
//                             context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
//     UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
//     UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
//     UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
// }

void Test_BPNode_ClaIn_ProcessBundleInput_NominalUDP(void)
{
    uint8 ContactId;

    /* UDP case */
    ContactId = 0;
    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SUCCESS);
}

void Test_BPNode_ClaIn_ProcessBundleInput_NominalSB(void)
{
    uint8  ContactId;
    size_t MsgSize;
    CFE_SB_Buffer_t Buf;
    CFE_SB_Buffer_t* BufPtr;

    BufPtr    = &Buf;
    ContactId = BPNODE_CLA_SB_CONTACT_ID;
    MsgSize   = 42;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    /* Run function under test */
    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SUCCESS);

    /* Verify that the function ran as expected */
    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 1);
}

void Test_BPNode_ClaIn_ProcessBundleInput_ReceiveBufferErr(void)
{
    uint8 ContactId;
    size_t MsgSize;

    MsgSize   = 42;
    ContactId = BPNODE_CLA_SB_CONTACT_ID;
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_BAD_ARGUMENT);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);

    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 0);
    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_RECV_BUFF_ERR_EID,
                                "[CLA In #%d]: Failed to receive from the SB buffer. Error = %d");
}

void Test_BPNode_ClaIn_ProcessBundleInput_ReceiveBufferTimeout(void)
{
    uint8 ContactId;
    size_t MsgSize;

    ContactId = BPNODE_CLA_SB_CONTACT_ID;
    MsgSize   = 42;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_TIME_OUT);

    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SB_TIME_OUT);

    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 0);
}

void Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand(void)
{
    uint8 ContactId = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);
    UtAssert_UINT32_NEQ(BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_PSP_ERROR);

    BPNode_Test_Verify_Event(0, BPNODE_CLA_IN_IO_READ_ERR_EID,
                            "[CLA In #%d]: Failed to read packet from UDP socket, RC = %d");
}

void Test_BPNode_ClaIn_ProcessBundleInput_SB_MsgSizeZero(void)
{
    uint8 ContactId;
    size_t MsgSize;
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t* BufPtr;

    BufPtr    = &Buf;
    ContactId = BPNODE_CLA_SB_CONTACT_ID;
    MsgSize   = 0;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    UtAssert_EQ(CFE_Status_t, BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 0);
}

void Test_BPNode_ClaIn_ProcessBundleInput_FailedBPLibIngress(void)
{
    uint8 ContactId;
    size_t MsgSize;
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t* BufPtr;

    BufPtr    = &Buf;
    ContactId = 0;
    MsgSize   = 42;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Ingress), 1, BPLIB_ERROR);

    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
}

void Test_BPNode_ClaIn_ProcessBundleInput_CLA_IngressTimeout(void)
{
    uint8  ContactId;
    size_t MsgSize;
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t* BufPtr;

    BufPtr    = &Buf;
    ContactId = 0;
    MsgSize   = 42;

    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Ingress), 1, BPLIB_CLA_TIMEOUT);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(size_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContactId), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaInCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_WakeupSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TakeSemErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_FindByNameErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_DirErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_RunErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_CreatePipeErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_SubscribeErr);
    ADD_TEST(Test_BPNode_ClaIn_Setup_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_Setup_PortErr);
    ADD_TEST(Test_BPNode_ClaIn_Setup_IpErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_GetTaskIDError);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NoContactId);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_TakeSemTimeoutErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_TakeSemOtherErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NoIngress);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_FailedProcBundle);
    // ADD_TEST(Test_BPNode_ClaIn_TaskExit_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_NominalUDP);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_NominalSB);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_ReceiveBufferErr);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_ReceiveBufferTimeout);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_SB_MsgSizeZero);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_FailedBPLibIngress);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_CLA_IngressTimeout);
}