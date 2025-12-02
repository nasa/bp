/*
 * NASA Docket No. GSC-19,559-1, and identified as "Delay/Disruption Tolerant Networking 
 * (DTN) Bundle Protocol (BP) v7 Core Flight System (cFS) Application Build 7.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software distributed under 
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF 
 * ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. The copyright notice to be 
 * included in the software is as follows: 
 *
 * Copyright 2025 United States Government as represented by the Administrator of the 
 * National Aeronautics and Space Administration. All Rights Reserved.
 *
 */

/**
 * \file
 *  Unit tests for bpnode_notif.c
 */

/*
** Include Files
*/

#include "bpnode_notif.h"
#include "bpnode_test_utils.h"

void UT_Handler_OS_CondVarTimedWait(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    BPNode_AppData.ChildStartWorkNotif.Count++;
}


/*
** Function Definitions
*/

/* Test BPNode_NotifInit when everything succeeds */
void Test_BPNode_NotifInit_Nominal(void)
{
    UtAssert_INT32_EQ(BPNode_NotifInit(&BPNode_AppData.ChildStartWorkNotif, "NotifName"), OS_SUCCESS);
}

/* Test BPNode_NotifDestroy succeeds (doesn't crash) */
void Test_BPNode_NotifDestroy_Nominal(void)
{
    BPNode_NotifDestroy(&BPNode_AppData.ChildStartWorkNotif);
}

/* Test BPNode_NotifSet sets the internal flag */
void Test_BPNode_NotifSet_Nominal(void)
{
    BPNode_NotifSet(&BPNode_AppData.ChildStartWorkNotif);
    UtAssert_UINT32_EQ(BPNode_AppData.ChildStartWorkNotif.Count, 1);
} 

/* Test BPNode_NotifSet when null is passed in */
void Test_BPNode_NotifSet_Null(void)
{
    BPNode_NotifSet(NULL);

    UtAssert_STUB_COUNT(OS_CondVarLock, 0);
} 

/* Test BPNode_NotifGetCount returns the right value */
void Test_BPNode_NotifGetCount_Nominal(void)
{
    BPNode_AppData.ChildStartWorkNotif.Count = 1234;

    UtAssert_UINT32_EQ(BPNode_NotifGetCount(&BPNode_AppData.ChildStartWorkNotif), 1234);
} 

/* Test BPNode_NotifGetCount when null is passed in */
void Test_BPNode_NotifGetCount_Null(void)
{
    UtAssert_UINT32_EQ(BPNode_NotifGetCount(NULL), 0);
} 

/* Test BPNode_NotifWait when it runs nominally */
void Test_BPNode_NotifWait_Nominal(void)
{
    int32 Status;
    uint32 OldCount = 1234;
    int32 Timeout = 100;

    BPNode_AppData.ChildStartWorkNotif.Count = 1234;
    UT_SetHandlerFunction(UT_KEY(OS_CondVarTimedWait), UT_Handler_OS_CondVarTimedWait, NULL);

    Status = BPNode_NotifWait(&BPNode_AppData.ChildStartWorkNotif, OldCount, Timeout);

    UtAssert_INT32_EQ(Status, OS_SUCCESS);
} 

/* Test BPNode_NotifWait when there's a timeout */
void Test_BPNode_NotifWait_Timeout(void)
{
    int32 Status;
    uint32 OldCount = 1234;
    int32 Timeout = 100;

    BPNode_AppData.ChildStartWorkNotif.Count = 1234;
    
    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarTimedWait), OS_ERROR_TIMEOUT);

    Status = BPNode_NotifWait(&BPNode_AppData.ChildStartWorkNotif, OldCount, Timeout);

    UtAssert_INT32_EQ(Status, OS_ERROR_TIMEOUT);
} 

/* Test BPNode_NotifWait when there's an unexpected error */
void Test_BPNode_NotifWait_Err(void)
{
    int32 Status;
    uint32 OldCount = 1234;
    int32 Timeout = 100;

    BPNode_AppData.ChildStartWorkNotif.Count = 1234;

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarTimedWait), OS_ERROR);

    Status = BPNode_NotifWait(&BPNode_AppData.ChildStartWorkNotif, OldCount, Timeout);

    UtAssert_INT32_EQ(Status, OS_ERROR);
}

/* Test BPNode_NotifWait when the notification has already been issued */
void Test_BPNode_NotifWait_Ready(void)
{
    int32 Status;
    uint32 OldCount = 1234;
    int32 Timeout = 100;

    BPNode_AppData.ChildStartWorkNotif.Count = 1235;

    Status = BPNode_NotifWait(&BPNode_AppData.ChildStartWorkNotif, OldCount, Timeout);

    UtAssert_INT32_EQ(Status, OS_SUCCESS);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_NotifInit_Nominal);

    ADD_TEST(Test_BPNode_NotifDestroy_Nominal);

    ADD_TEST(Test_BPNode_NotifSet_Nominal);
    ADD_TEST(Test_BPNode_NotifSet_Null);

    ADD_TEST(Test_BPNode_NotifGetCount_Nominal);
    ADD_TEST(Test_BPNode_NotifGetCount_Null);

    ADD_TEST(Test_BPNode_NotifWait_Nominal);
    ADD_TEST(Test_BPNode_NotifWait_Ready);
    ADD_TEST(Test_BPNode_NotifWait_Timeout);
    ADD_TEST(Test_BPNode_NotifWait_Err);
}
