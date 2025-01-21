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
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS * 3);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaOutCreateTasks when the init semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to create initialization semaphore, %s. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 1);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the exit semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_WakeupSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 2, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to create wakeup semaphore, %s. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the exit semaphore fails to create */
void Test_BPNode_ClaOutCreateTasks_ExitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 3, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_EXIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to create exit semaphore. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 0);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when the child task creation fails */
void Test_BPNode_ClaOutCreateTasks_TaskCrErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_CreateChildTask), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CREATE_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to create child task. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 0);
}

/* Test BPNode_ClaOutCreateTasks when taking the init semaphore fails */
void Test_BPNode_ClaOutCreateTasks_TakeSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOutCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Task not running. Init Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 3);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_ClaOut_TaskInit when everything succeeds */
void Test_BPNode_ClaOut_TaskInit_Nominal(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Child Task Initialized.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ExpContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaOut_TaskInit when it fails to get the task ID */
void Test_BPNode_ClaOut_TaskInit_GetIdErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_NO_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #?]: Failed to get task ID. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_ClaOut_TaskInit when the task IDs don't match */
void Test_BPNode_ClaOut_TaskInit_MatchIdErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INV_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #?]: Task ID does not match any known task IDs. ID = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}


/* Test BPNode_ClaOut_TaskInit when the PSP module can't be found */
void Test_BPNode_ClaOut_TaskInit_FindByNameErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_FindByName), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_FIND_NAME_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't find I/O driver. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 0);
}

/* Test BPNode_ClaOut_TaskInit when the port can't be configured */
void Test_BPNode_ClaOut_TaskInit_PortErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CFG_PORT_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't set port number configuration. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
#endif
}

/* Test BPNode_ClaOut_TaskInit when the IP address can't be configured */
void Test_BPNode_ClaOut_TaskInit_IpErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 2, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CFG_IP_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't set IP address configuration. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 2);
#endif
}

/* Test BPNode_ClaOut_TaskInit when the direction can't be set */
void Test_BPNode_ClaOut_TaskInit_DirErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    int32 CmdCount = 3;
#else
    int32 CmdCount = 1;
#endif
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), CmdCount, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CFG_DIR_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't set I/O direction to output. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, CmdCount);
}

/* Test BPNode_ClaOut_TaskInit when the state can't be set to running */
void Test_BPNode_ClaOut_TaskInit_RunErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER
    int32 CmdCount = 4;
#else
    int32 CmdCount = 2;
#endif
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), CmdCount, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Couldn't set I/O state to running. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, CmdCount);
}

/* Test BPNode_ClaOut_TaskInit when init semaphore give fails */
void Test_BPNode_ClaOut_TaskInit_GiveSemErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaOutData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaOut_TaskInit(&ContId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_INIT_SEM_TK_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to give init semaphore. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
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
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

/* Test BPNode_ClaOut_AppMain when max number of bundles are egressed */
void Test_BPNode_ClaOut_AppMain_MaxBundles(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled = true;

    // TODO How to add more bundles?

    BPNode_ClaOut_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

void Test_BPNode_ClaOut_AppMain_TakeSemErr(void)
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
    BPNode_AppData.ClaOutData[ChanId].TaskId = TaskId;
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Run the function under test */
    BPNode_ClaOut_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3); // Events are sent for task init, failed semaphore wait, and task termination, in that order
    BPNode_Test_Verify_Event(1, BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID, "[CLA Out #%d]: Failed to take wakeup semaphore, RC = %d");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaOut_ProcessBundleOutput, 0);
}

void Test_BPNode_ClaOut_AppMain_WakeupSemTimeout(void)
{
    CFE_ES_TaskId_t TaskId;
    uint8 ChanId;

    /* Force a failed task wakeup */
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemTimedWait), 1, OS_SEM_TIMEOUT);

    /* Enter task loop only once */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    /* Pass task ID gathering process */
    TaskId = 1234;
    ChanId = 0;
    BPNode_AppData.ClaOutData[ChanId].TaskId = TaskId;
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskID), CFE_SUCCESS);

    /* Run the function under test */
    BPNode_ClaOut_AppMain();

    /* Verify the error issued an event */
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3); // Events are sent for task init, failed semaphore wait, and task termination, in that order
    BPNode_Test_Verify_Event(1, BPNODE_CLA_OUT_SEM_TK_TIMEOUT_INF_EID, "[CLA Out #%d]: Timed out while waiting for the wakeup semaphore");

    /* Verify that the wakeup activities were skipped when a wakeup fails */
    UtAssert_STUB_COUNT(BPNode_ClaOut_ProcessBundleOutput, 0);
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
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_CLA_OUT_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
}

/* Test BPNode_ClaOut_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_ClaOut_AppMain_ContIdErr(void)
{

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_ClaOut_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("Terminating Unknown CLA Out Task.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
}

/* Test BPNode_ClaOut_AppMain when bundle egress is disabled */
void Test_BPNode_ClaOut_AppMain_NoEgress(void)
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
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
}

void Test_BPNode_ClaOut_AppMain_FailedProcBundle(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(BPNode_ClaOut_ProcessBundleOutput), 1, CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    BPNode_AppData.ClaOutData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled = true;
    BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = 0;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_VOIDCALL(BPNode_ClaOut_AppMain());

    UtAssert_UINT32_EQ(BPNode_AppData.ClaOutData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 3);
}

/* Test BPNode_ClaOut_TaskExit in nominal shutdown */
void Test_BPNode_ClaOut_TaskExit_Nominal(void)
{
    uint8 ContId = 0;

    UtAssert_VOIDCALL(BPNode_ClaOut_TaskExit(ContId));

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_Nominal(void)
{
    uint8 ContId = 0;
    UtAssert_UINT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContId), CFE_SUCCESS);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_FailedBPLibEgress(void)
{
    uint8 ContId = 0;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_ERROR);
    UtAssert_UINT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_OUT_LIB_LOAD_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA Out #%d]: Failed to get bundle for egress. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_CLATimeout(void)
{
    uint8 ContId = 0;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);
    UtAssert_UINT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContId), CFE_SUCCESS);
}

void Test_BPNode_ClaOut_ProcessBundleOutput_NonZeroBuffSize(void)
{
    uint8 ContId = 0;
    BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = 1;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Egress), 1, BPLIB_CLA_TIMEOUT);
    UtAssert_UINT32_EQ(BPNode_ClaOut_ProcessBundleOutput(ContId), CFE_SUCCESS);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_WakeupSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaOutCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_ClaOut_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_MatchIdErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_FindByNameErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_PortErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_IpErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_DirErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_RunErr);
    ADD_TEST(Test_BPNode_ClaOut_TaskInit_GiveSemErr);

    ADD_TEST(Test_BPNode_ClaOut_AppMain_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_MaxBundles);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_TakeSemErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_WakeupSemTimeout);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_NoEgress);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_InitErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_ContIdErr);
    ADD_TEST(Test_BPNode_ClaOut_AppMain_FailedProcBundle);

    ADD_TEST(Test_BPNode_ClaOut_TaskExit_Nominal);

    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_Nominal);
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_FailedBPLibEgress);
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_CLATimeout);
    ADD_TEST(Test_BPNode_ClaOut_ProcessBundleOutput_NonZeroBuffSize);
}