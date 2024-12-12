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
    UtAssert_STUB_COUNT(OS_BinSemCreate, BPLIB_MAX_NUM_CONTACTS * 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, BPLIB_MAX_NUM_CONTACTS);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, BPLIB_MAX_NUM_CONTACTS);
}

/* Test BPNode_ClaInCreateTasks when the init semaphore fails to create */
void Test_BPNode_ClaInCreateTasks_InitSemErr(void)
{
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemCreate), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaInCreateTasks(), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_INIT_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Failed to create the CLA In #%d task init semaphore. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
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
    UtAssert_STRINGBUF_EQ("Failed to create the CLA In #%d task exit semaphore. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
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
    UtAssert_STRINGBUF_EQ("Failed to create the CLA In #%d child task. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
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
    UtAssert_STRINGBUF_EQ("CLA In #%d task not running. Init Sem Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_BinSemCreate, 2);
    UtAssert_STUB_COUNT(CFE_ES_CreateChildTask, 1);
    UtAssert_STUB_COUNT(OS_BinSemTimedWait, 1);
}

/* Test BPNode_ClaIn_TaskInit when everything succeeds */
void Test_BPNode_ClaIn_TaskInit_Nominal(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_SUCCESS);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Child Task Initialized.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_UINT32_EQ(BPNode_AppData.ClaInData[ExpContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 0);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaIn_TaskInit when it fails to get the task ID */
void Test_BPNode_ClaIn_TaskInit_GetIdErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;

    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_NO_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #?]: Failed to get task ID. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 0);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_ClaIn_TaskInit when the task IDs don't match */
void Test_BPNode_ClaIn_TaskInit_MatchIdErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = BPLIB_MAX_NUM_CONTACTS;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[0].TaskId = 5678;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_INV_ID_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #?]: Task ID does not match any known task IDs. ID = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 0);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, 0);
}

/* Test BPNode_ClaIn_TaskInit when the PSP module can't be found */
void Test_BPNode_ClaIn_TaskInit_FindByNameErr(void)
{
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_FindByName), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_FIND_NAME_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't find I/O driver. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);    
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 0);
}

/* Test BPNode_ClaIn_TaskInit when the port can't be configured */
void Test_BPNode_ClaIn_TaskInit_PortErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER 
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_PORT_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't set port number configuration. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);    
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
#endif
}

/* Test BPNode_ClaIn_TaskInit when the IP address can't be configured */
void Test_BPNode_ClaIn_TaskInit_IpErr(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER 
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 2, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_IP_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't set IP address configuration. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);    
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_FindByName, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 2);
#endif
}

/* Test BPNode_ClaIn_TaskInit when the direction can't be set */
void Test_BPNode_ClaIn_TaskInit_DirErr(void)
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
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), CmdCount, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_DIR_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't set I/O direction to input. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);    
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, CmdCount);
}

/* Test BPNode_ClaIn_TaskInit when the state can't be set to running */
void Test_BPNode_ClaIn_TaskInit_RunErr(void)
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
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), CmdCount, CFE_PSP_ERROR);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_CFG_SET_RUN_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Couldn't set I/O state to running. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);    
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, CmdCount);
}

/* Test BPNode_ClaIn_TaskInit when init semaphore give fails */
void Test_BPNode_ClaIn_TaskInit_GiveSemErr(void)
{
    uint8 ContId = BPNODE_MAX_NUM_CONTACTS;
    uint8 ExpContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    BPNode_AppData.ClaInData[ExpContId].TaskId = TaskId;

    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    UtAssert_INT32_EQ(BPNode_ClaIn_TaskInit(&ContId), OS_SEM_FAILURE);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_INIT_SEM_TK_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Failed to give init semaphore. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_UINT8_EQ(ContId, ExpContId);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_GetTaskID, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, 1);
}

/* Test BPNode_ClaIn_AppMain when app state is started and one CLA is received */
void Test_BPNode_ClaIn_AppMain_Nominal(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    BPNode_AppData.ClaInData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaInData[ContId].IngressServiceEnabled = true;

    BPNode_ClaIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaInData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaIn_AppMain when initialization failed but channel ID is known */
void Test_BPNode_ClaIn_AppMain_InitErr(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_SEM_FAILURE);

    BPNode_AppData.ClaInData[ContId].TaskId = TaskId;

    BPNode_ClaIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaInData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_ERROR);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_CLA_IN_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaIn_AppMain when initialization failed and channel ID is unknown */
void Test_BPNode_ClaIn_AppMain_ContIdErr(void)
{
    /* Test setup */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskID), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    BPNode_ClaIn_AppMain();

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_CLA_IN_UNK_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("Terminating Unknown CLA In Task.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 0);
    UtAssert_STUB_COUNT(OS_TaskDelay, 0);
}

/* Test BPNode_ClaIn_AppMain when ingress service is disabled */
void Test_BPNode_ClaIn_AppMain_NoIngress(void)
{
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    BPNode_AppData.ClaInData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaInData[ContId].IngressServiceEnabled = false;

    BPNode_ClaIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaInData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
}

void Test_BPNode_ClaIn_AppMain_FailedProcBundle(void)
{
#ifdef BPNODE_CLA_UDP_DRIVER 
    int32 CmdCount = 5;
#else
    int32 CmdCount = 3;
#endif
    uint8 ContId = 0;
    CFE_ES_TaskId_t TaskId = 1234;

    /* Test setup */
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskID), &TaskId, sizeof(TaskId), false);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);

    BPNode_AppData.ClaInData[ContId].TaskId = TaskId;
    BPNode_AppData.ClaInData[ContId].IngressServiceEnabled = true;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), CmdCount, CFE_PSP_ERROR);

    BPNode_ClaIn_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.ClaInData[ContId].RunStatus,
                                                        CFE_ES_RunStatus_APP_RUN);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 2);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, CmdCount + 1);
}

/* Test BPNode_ClaIn_TaskExit in nominal shutdown */
void Test_BPNode_ClaIn_TaskExit_Nominal(void)
{
    uint8 ContId = 0;

    UtAssert_VOIDCALL(BPNode_ClaIn_TaskExit(ContId));
    
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Terminating Task. RunStatus = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);
}

void Test_BPNode_ClaIn_ProcessBundleInput_Nominal(void)
{
    uint8 ContId = 0;

    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContId), CFE_SUCCESS);
}

void Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand(void)
{
    uint8 ContId = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_IODriver_Command), 1, CFE_PSP_ERROR);
    UtAssert_UINT32_NEQ(BPNode_ClaIn_ProcessBundleInput(ContId), CFE_SUCCESS);
}

void Test_BPNode_ClaIn_ProcessBundleInput_FailedBPLibIngress(void)
{
    uint8 ContId = 0;
    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Ingress), 1, BPLIB_ERROR);
    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContId), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CLA_IN_LIB_PROC_ERR_EID);
    UtAssert_STRINGBUF_EQ("[CLA In #%d]: Failed to ingress bundle. Error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPNode_ClaIn_ProcessBundleInput_CLATimeout(void)
{
    uint8 ContId = 0;

    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Ingress), 1, BPLIB_CLA_TIMEOUT);
    
    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContId), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 1);
    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 1);
}

void Test_BPNode_ClaIn_ProcessBundleInput_NonZeroBuffSize(void)
{
    uint8 ContId = 0;

    BPNode_AppData.ClaInData[ContId].CurrentBufferSize = 1;

    UT_SetDeferredRetcode(UT_KEY(BPLib_CLA_Ingress), 1, BPLIB_CLA_TIMEOUT);
    
    UtAssert_UINT32_EQ(BPNode_ClaIn_ProcessBundleInput(ContId), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_PSP_IODriver_Command, 0);
    UtAssert_STUB_COUNT(BPLib_CLA_Ingress, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_ClaInCreateTasks_Nominal);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_InitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_ExitSemErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TaskCrErr);
    ADD_TEST(Test_BPNode_ClaInCreateTasks_TakeSemErr);

    ADD_TEST(Test_BPNode_ClaIn_TaskInit_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_GetIdErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_MatchIdErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_FindByNameErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_PortErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_IpErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_DirErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_RunErr);
    ADD_TEST(Test_BPNode_ClaIn_TaskInit_GiveSemErr);

    ADD_TEST(Test_BPNode_ClaIn_AppMain_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_InitErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_ContIdErr);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_NoIngress);
    ADD_TEST(Test_BPNode_ClaIn_AppMain_FailedProcBundle);

    ADD_TEST(Test_BPNode_ClaIn_TaskExit_Nominal);
    
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_Nominal);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_FailedIODCommand);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_FailedBPLibIngress);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_CLATimeout);
    ADD_TEST(Test_BPNode_ClaIn_ProcessBundleInput_NonZeroBuffSize);
}