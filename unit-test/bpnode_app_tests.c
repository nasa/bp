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
 *  Unit tests for bpnode_app.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "fwp_tablep.h"
#include "fwp_dp.h"

/* Handler to set table pointers to test tables */
void UT_BPA_TABLEP_Init_Handler(void *UserObj, UT_EntryKey_t FuncKey, 
                                                const UT_StubContext_t *Context)
{
    BPNode_AppData.AduTblPtr = &TestAduTbl;
    BPNode_AppData.ChanTblPtr = &TestChanTbl;
    BPNode_AppData.ContactsTblPtr = &TestContactsTbl;
    BPNode_AppData.CrsTblPtr = &TestCrsTbl;
    BPNode_AppData.CustodianTblPtr = &TestCustodianTbl;
    BPNode_AppData.CustodyTblPtr = &TestCustodyTbl;
    BPNode_AppData.MibPnTblPtr = &TestMibPnTbl;
    BPNode_AppData.MibPsTblPtr = &TestMibPsTbl;
    BPNode_AppData.ReportTblPtr = &TestReportTbl;
    BPNode_AppData.AuthTblPtr = &TestAuthTbl;
    BPNode_AppData.LatTblPtr = &TestLatencyTbl;
    BPNode_AppData.StorTblPtr = &TestStorTbl;
}


/*
** Function Definitions
*/

/* Test app main loop in nominal case */
void Test_BPNode_AppMain_Nominal(void)
{
    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    /*
     * BPNode_AppMain does not return a value,
     * but it has several internal decision points
     * that need to be exercised here.
     *
     * First call it in "nominal" mode where all
     * dependent calls should be successful by default.
     */
    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}

/* Test app main loop after initialization failure */
void Test_BPNode_AppMain_FailedInit(void)
{
    /* Failure of BPNode_AppInit()*/
    UT_SetDeferredRetcode(UT_KEY(BPLib_EM_Init), 1, CFE_EVS_INVALID_PARAMETER);

    BPNode_AppMain();

    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_ERROR);
}

/* Test app main loop after receiving a wakeup (but no command) */
void Test_BPNode_AppMain_WakeupRecvd(void)
{
    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    /* Receive wakeup message but no command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
}

/* Test app main loop after wakeup pipe read error */
void Test_BPNode_AppMain_WakeupErr(void)
{
    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    /* Wakeup pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_PIPE_ERR_EID);
    UtAssert_STRINGBUF_EQ("SB Pipe Read Error, App Will Exit", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app main loop after command pipe read error */
void Test_BPNode_AppMain_CommandErr(void)
{
    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    /* Command pipe read error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_TIME_OUT);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[1].EventID, BPNODE_PIPE_ERR_EID);
    UtAssert_STRINGBUF_EQ("SB Pipe Read Error, App Will Exit", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[1].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app main loop after receiving one command */
void Test_BPNode_AppMain_CommandRecvd(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    BPNode_AppMain();

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 3);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 1);
    UtAssert_UINT32_EQ(BPNode_AppData.RunStatus, CFE_ES_RunStatus_APP_RUN);
}


/* Test wakeup process after receiving one command */
void Test_BPNode_WakeupProcess_CommandRecvd(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
}

/* Test wakeup process after failing to give a semaphore */
void Test_BPNode_WakeupProcess_FailSem(void)
{
    /* Fail sem give */
    UT_SetDeferredRetcode(UT_KEY(OS_BinSemGive), 1, OS_ERROR);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPA_TABLEP_TableUpdate, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_WKP_SEM_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error giving Generic Worker #%d its semaphore, RC = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test wakeup process after failing Time maintenance activities */
void Test_BPNode_WakeupProcess_FailTimeMaint(void)
{
    /* Fail Time activities */
    UT_SetDeferredRetcode(UT_KEY(BPLib_TIME_MaintenanceActivities), 1, BPLIB_TIME_WRITE_ERROR);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPA_TABLEP_TableUpdate, 1);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_TIME_WKP_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error doing time maintenance activities, RC = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}


/* Test wakeup process after getting an updated table */
void Test_BPNode_WakeupProcess_FailedTblUpdate(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Successful receipt of one command */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_NO_MESSAGE);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &BufPtr, sizeof(BufPtr), false);

    UT_SetDefaultReturnValue(UT_KEY(BPA_TABLEP_TableUpdate), CFE_TBL_ERR_INVALID_HANDLE);
    UtAssert_INT32_NEQ(BPNode_WakeupProcess(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 0);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_TBL_ADDR_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error Updating Table from Table Proxy, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test wakeup process after receiving null buffer */
void Test_BPNode_WakeupProcess_NullBuf(void)
{
    CFE_SB_Buffer_t  Buf;
    CFE_SB_Buffer_t *BufPtr = &Buf;

    /* Receipt of a null buffer */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 2, CFE_SB_PIPE_RD_ERR);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), NULL, sizeof(BufPtr), false);

    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SB_PIPE_RD_ERR);

    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 2);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test wakeup process after command receive error */
void Test_BPNode_WakeupProcess_RecvErr(void)
{
    /* Command receive error */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_ReceiveBuffer), 1, CFE_SB_PIPE_RD_ERR);
 
    UtAssert_INT32_EQ(BPNode_WakeupProcess(), CFE_SB_PIPE_RD_ERR);
 
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(BPA_DP_TaskPipe, 0);
    UtAssert_STUB_COUNT(OS_BinSemGive, BPNODE_NUM_GEN_WRKR_TASKS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_Nominal(void)
{
    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SUCCESS);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_INIT_INF_EID);
    UtAssert_STRINGBUF_EQ("BPNode Initialized: %s", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization after event management initialization failure */
void Test_BPNode_AppInit_FailedEvs(void)
{
    /* Failure to register with event management */
    UT_SetDeferredRetcode(UT_KEY(BPLib_EM_Init), 1, CFE_EVS_INVALID_PARAMETER);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_EVS_INVALID_PARAMETER);

    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);

}

/* Test app initialization after command pipe creation failure */
void Test_BPNode_AppInit_FailedCmdPipeCreate(void)
{
    /* Failure to create command pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CR_CMD_PIPE_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error creating SB Command Pipe, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization after wakeup pipe creation failure */
void Test_BPNode_AppInit_FailedWakeupPipeCreate(void)
{
    /* Failure to create wakeup pipe */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 2, CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CR_WKP_PIPE_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error creating SB Wakeup Pipe, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization after failure to subscribe to commands */
void Test_BPNode_AppInit_FailedCommandSub(void)
{
    /* Failure to subscribe to commands */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_SUB_CMD_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error Subscribing to Commands, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization after failure to subscribe to wakeups */
void Test_BPNode_AppInit_FailedWakeupSub(void)
{
    /* Failure to subscribe to wakeups */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SB_BAD_ARGUMENT);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_SUB_WKP_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error Subscribing to wakeup messages, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization after failure to register table */
void Test_BPNode_AppInit_FailedTblInit(void)
{
    /* Failure to call BPA_TABLEP_TableInit() */    
    UT_SetDefaultReturnValue(UT_KEY(BPA_TABLEP_TableInit), CFE_TBL_ERR_INVALID_HANDLE);

    UtAssert_INT32_NEQ(BPNode_AppInit(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_TBL_ADDR_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error Getting Table from Table Proxy, RC = 0x%08lX", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}


/* Test app initialization when FWP initialization fails */
void Test_BPNode_AppInit_FailedFwpInit(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPLib_FWP_Init), 1, BPLIB_FWP_CALLBACK_INIT_ERROR);

    UtAssert_INT32_EQ(BPNode_AppInit(), BPLIB_FWP_CALLBACK_INIT_ERROR);
    
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, BPNODE_FWP_INIT_ERR_EID);
    UtAssert_STRINGBUF_EQ("BPNode: Failure initializing function callbacks, RC = 0x%08lX", CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
                            context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);
}

/* Test app initialization after failure to create ADU in child tasks */
void Test_BPNode_AppInit_FailedAduInTasks(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPNode_AduInCreateTasks), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_CHILD_TASK_CREATE);
}

/* Test app initialization after failure to create ADU out child tasks */
void Test_BPNode_AppInit_FailedAduOutTasks(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPNode_AduOutCreateTasks), 1, CFE_ES_ERR_CHILD_TASK_CREATE);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_CHILD_TASK_CREATE);
}

/* Test adding one application automatically at startup */
void Test_BPNode_AppInit_AutoAddApp(void)
{
    /* Set channel 0 to be added automatically */
    TestChanTbl.Configs[0].AddAutomatically = true;

    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_SUCCESS);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_AUTO_ADD_APP_INF_EID);
    UtAssert_STRINGBUF_EQ("Automatically added app configurations for ChanId=%d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    UtAssert_STUB_COUNT(BPA_ADUP_AddApplication, 1);
    UtAssert_STUB_COUNT(BPA_ADUP_StartApplication, 1);
}

/* Test adding one application automatically at startup failed */
void Test_BPNode_AppInit_AutoAddAppFail(void)
{
    /* Set channel 0 to be added automatically */
    TestChanTbl.Configs[0].AddAutomatically = true;

    UT_SetHandlerFunction(UT_KEY(BPA_TABLEP_TableInit), UT_BPA_TABLEP_Init_Handler, NULL);
    UT_SetDeferredRetcode(UT_KEY(BPA_ADUP_StartApplication), 1, BPLIB_ERROR);

    UtAssert_INT32_EQ(BPNode_AppInit(), BPLIB_ERROR);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(BPA_ADUP_AddApplication, 1);
    UtAssert_STUB_COUNT(BPA_ADUP_StartApplication, 1);
}

/* Test app exit in nominal case */
void Test_BPNode_AppExit_Nominal(void)
{
    uint8 i;

    BPNode_AppExit();

    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        UtAssert_UINT32_EQ(BPNode_AppData.AduOutData[i].RunStatus, CFE_ES_RunStatus_APP_EXIT);
        UtAssert_UINT32_EQ(BPNode_AppData.AduInData[i].RunStatus, CFE_ES_RunStatus_APP_EXIT);
    }

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_EXIT_CRIT_EID);
    UtAssert_STRINGBUF_EQ("App terminating, error = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_FailedTimeInit(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPLib_TIME_Init), 1, BPLIB_TIME_READ_ERROR);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
    
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_TIME_INIT_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error initializing BPLib Time Management, RC = %d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_FailedClaIn(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPNode_ClaInCreateTasks), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test app initialization in nominal case */
void Test_BPNode_AppInit_FailedClaOut(void)
{
    UT_SetDeferredRetcode(UT_KEY(BPNode_ClaOutCreateTasks), 1, CFE_ES_ERR_RESOURCEID_NOT_VALID);

    UtAssert_INT32_EQ(BPNode_AppInit(), CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);

}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_AppMain_Nominal);
    ADD_TEST(Test_BPNode_AppMain_FailedInit);
    ADD_TEST(Test_BPNode_AppMain_WakeupRecvd);
    ADD_TEST(Test_BPNode_AppMain_WakeupErr);
    ADD_TEST(Test_BPNode_AppMain_CommandRecvd);
    ADD_TEST(Test_BPNode_AppMain_CommandErr);

    ADD_TEST(Test_BPNode_WakeupProcess_CommandRecvd);
    ADD_TEST(Test_BPNode_WakeupProcess_FailTimeMaint);
    ADD_TEST(Test_BPNode_WakeupProcess_FailedTblUpdate);
    ADD_TEST(Test_BPNode_WakeupProcess_NullBuf);
    ADD_TEST(Test_BPNode_WakeupProcess_RecvErr);

    ADD_TEST(Test_BPNode_AppInit_Nominal);
    ADD_TEST(Test_BPNode_AppInit_FailedEvs);
    ADD_TEST(Test_BPNode_AppInit_FailedCmdPipeCreate);
    ADD_TEST(Test_BPNode_AppInit_FailedWakeupPipeCreate);
    ADD_TEST(Test_BPNode_AppInit_FailedCommandSub);
    ADD_TEST(Test_BPNode_AppInit_FailedWakeupSub);
    ADD_TEST(Test_BPNode_AppInit_FailedTblInit);
    ADD_TEST(Test_BPNode_AppInit_FailedFwpInit);
    ADD_TEST(Test_BPNode_AppInit_FailedAduInTasks);
    ADD_TEST(Test_BPNode_AppInit_FailedAduOutTasks);
    ADD_TEST(Test_BPNode_AppInit_FailedTimeInit);
    ADD_TEST(Test_BPNode_AppInit_FailedClaIn);
    ADD_TEST(Test_BPNode_AppInit_FailedClaOut);    
    ADD_TEST(Test_BPNode_AppInit_AutoAddApp);
    ADD_TEST(Test_BPNode_AppInit_AutoAddAppFail);
    ADD_TEST(Test_BPNode_AppExit_Nominal);
}
