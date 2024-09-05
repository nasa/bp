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
 *  Unit tests for bpnode_cmds.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

/*
** Function Definitions
*/

/* Test noop command in nominal case */
void Test_BPNode_NoopCmd_Nominal(void)
{
    BPNode_NoopCmd_t TestMsg;
    UT_CheckEvent_t  EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Test dispatch of noop */

    /**
     *  The UT_CHECKEVENT_SETUP ExpectedFormat string must match the
     *  event message format string with format specifiers.
     *
     *  For example, the No-op test ExpectedFormat string must match the No-op
     *  command event message format string, including the "%d" format specifiers.
     *
     *  Use NULL for the ExpectedFormat to ignore the event message format string.
     */

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_NOOP_INF_EID, "No-op command. Version %d.%d.%d.");

    UtAssert_INT32_EQ(BPNode_NoopCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add All Applications command nominal case */
void Test_BPNode_AddAllApplicationsCmd_Nominal(void)
{
    BPNode_AddAllApplicationsCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add all applications command not implemented");

    UtAssert_INT32_EQ(BPNode_AddAllApplicationsCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Start All Applications command nominal case */
void Test_BPNode_StartAllApplicationsCmd_Nominal(void)
{
    BPNode_StartAllApplicationsCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Start all applications command not implemented");

    UtAssert_INT32_EQ(BPNode_StartAllApplicationsCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Verify Bundle Storage command nominal case */
void Test_BPNode_VerifyBundleStorageCmd_Nominal(void)
{
    BPNode_VerifyBundleStorageCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Verify bundle storage command not implemented");

    UtAssert_INT32_EQ(BPNode_VerifyBundleStorageCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Initialize Bundle Storage command nominal case */
void Test_BPNode_InitBundleStorageCmd_Nominal(void)
{
    BPNode_InitBundleStorageCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Initialize bundle storage command not implemented");

    UtAssert_INT32_EQ(BPNode_InitBundleStorageCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Verify Bundle Metadata (7.2) command nominal case */
void Test_BPNode_VerifyBundleMetadataCmd_Nominal(void)
{
    BPNode_VerifyBundleMetadataCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Verify bundle metadata (7.2) command not implemented");

    UtAssert_INT32_EQ(BPNode_VerifyBundleMetadataCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Rebuild Bundle Metadata command nominal case */
void Test_BPNode_RebuildBundleMetadataCmd_Nominal(void)
{
    BPNode_RebuildBundleMetadataCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Rebuild bundle metadata command not implemented");

    UtAssert_INT32_EQ(BPNode_RebuildBundleMetadataCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Clear Volatile command nominal case */
void Test_BPNode_ClearVolatileCmd_Nominal(void)
{
    BPNode_ClearVolatileCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Clear volatile command not implemented");

    UtAssert_INT32_EQ(BPNode_ClearVolatileCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reload Saved Data (7.2) command nominal case */
void Test_BPNode_ReloadSavedDataCmd_Nominal(void)
{
    BPNode_ReloadSavedDataCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reload saved data (7.2) command not implemented");

    UtAssert_INT32_EQ(BPNode_ReloadSavedDataCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reset All Counters command nominal case */
void Test_BPNode_ResetAllCountersCmd_Nominal(void)
{
    BPNode_ResetAllCountersCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset all counters command not implemented");

    UtAssert_INT32_EQ(BPNode_ResetAllCountersCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reset Counter command nominal case */
void Test_BPNode_ResetCounterCmd_Nominal(void)
{
    BPNode_ResetCounterCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset counter command not implemented");

    UtAssert_INT32_EQ(BPNode_ResetCounterCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reset Source Counters command nominal case */
void Test_BPNode_ResetSourceCountersCmd_Nominal(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset source counters command not implemented");

    UtAssert_INT32_EQ(BPNode_ResetSourceCountersCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reset Bundle Counters command nominal case */
void Test_BPNode_ResetBundleCountersCmd_Nominal(void)
{
    BPNode_ResetBundleCountersCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset bundle counters command not implemented");

    UtAssert_INT32_EQ(BPNode_ResetBundleCountersCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Reset Error Counters command nominal case */
void Test_BPNode_ResetErrorCountersCmd_Nominal(void)
{
    BPNode_ResetErrorCountersCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset error counters command not implemented");

    UtAssert_INT32_EQ(BPNode_ResetErrorCountersCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Application command nominal case */
void Test_BPNode_AddApplicationCmd_Nominal(void)
{
    BPNode_AddApplicationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add application command not implemented");

    UtAssert_INT32_EQ(BPNode_AddApplicationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Application command nominal case */
void Test_BPNode_RemoveApplicationCmd_Nominal(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove application command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveApplicationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Set Registration State command nominal case */
void Test_BPNode_SetRegistrationStateCmd_Nominal(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Set registration state command not implemented");

    UtAssert_INT32_EQ(BPNode_SetRegistrationStateCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Start Application command nominal case */
void Test_BPNode_StartApplicationCmd_Nominal(void)
{
    BPNode_StartApplicationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Start application command not implemented");

    UtAssert_INT32_EQ(BPNode_StartApplicationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Stop Application command nominal case */
void Test_BPNode_StopApplicationCmd_Nominal(void)
{
    BPNode_StopApplicationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Stop application command not implemented");

    UtAssert_INT32_EQ(BPNode_StopApplicationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Authorized Sources command nominal case */
void Test_BPNode_AddAuthSourcesCmd_Nominal(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add authorized sources command not implemented");

    UtAssert_INT32_EQ(BPNode_AddAuthSourcesCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Authorized Sources command nominal case */
void Test_BPNode_RemoveAuthSourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove authorized sources command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveAuthSourcesCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Authorized Custody Sources command nominal case */
void Test_BPNode_AddAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add authorized custody sources command not implemented");

    UtAssert_INT32_EQ(BPNode_AddAuthCustodySourcesCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Authorized Custody Sources command nominal case */
void Test_BPNode_RemoveAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove authorized custody sources command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveAuthCustodySourcesCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Authorized Custodians command nominal case */
void Test_BPNode_AddAuthCustodiansCmd_Nominal(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add authorized custodians command not implemented");

    UtAssert_INT32_EQ(BPNode_AddAuthCustodiansCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Authorized Custodians command nominal case */
void Test_BPNode_RemoveAuthCustodiansCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove authorized custodians command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveAuthCustodiansCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Authorized Report-to EID command nominal case */
void Test_BPNode_AddAuthReportToEidCmd_Nominal(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add authorized report-to eid command not implemented");

    UtAssert_INT32_EQ(BPNode_AddAuthReportToEidCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Authorized Report-to EID command nominal case */
void Test_BPNode_RemoveAuthReportToEidCmd_Nominal(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove authorized report-to eid command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveAuthReportToEidCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Latency command nominal case */
void Test_BPNode_AddLatencyCmd_Nominal(void)
{
    BPNode_AddLatencyCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add latency command not implemented");

    UtAssert_INT32_EQ(BPNode_AddLatencyCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Latency command nominal case */
void Test_BPNode_RemoveLatencyCmd_Nominal(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove latency command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveLatencyCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Contact Setup command nominal case */
void Test_BPNode_ContactSetupCmd_Nominal(void)
{
    BPNode_ContactSetupCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Contact setup command not implemented");

    UtAssert_INT32_EQ(BPNode_ContactSetupCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Contact Start command nominal case */
void Test_BPNode_ContactStartCmd_Nominal(void)
{
    BPNode_ContactStartCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Contact start command not implemented");

    UtAssert_INT32_EQ(BPNode_ContactStartCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Contact Stop command nominal case */
void Test_BPNode_ContactStopCmd_Nominal(void)
{
    BPNode_ContactStopCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Contact stop command not implemented");

    UtAssert_INT32_EQ(BPNode_ContactStopCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Contact Teardown command nominal case */
void Test_BPNode_ContactTeardownCmd_Nominal(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Contact teardown command not implemented");

    UtAssert_INT32_EQ(BPNode_ContactTeardownCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add MIB Array Key command nominal case */
void Test_BPNode_AddMibArrayKeyCmd_Nominal(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add mib array key command not implemented");

    UtAssert_INT32_EQ(BPNode_AddMibArrayKeyCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove MIB Array Key command nominal case */
void Test_BPNode_RemoveMibArrayKeyCmd_Nominal(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove mib array key command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveMibArrayKeyCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Set MIB Item command nominal case */
void Test_BPNode_SetMibItemCmd_Nominal(void)
{
    BPNode_SetMibItemCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Set mib item command not implemented");

    UtAssert_INT32_EQ(BPNode_SetMibItemCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Add Storage Allocation command nominal case */
void Test_BPNode_AddStorageAllocationCmd_Nominal(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Add storage allocation command not implemented");

    UtAssert_INT32_EQ(BPNode_AddStorageAllocationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Remove Storage Allocation command nominal case */
void Test_BPNode_RemoveStorageAllocationCmd_Nominal(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Remove storage allocation command not implemented");

    UtAssert_INT32_EQ(BPNode_RemoveStorageAllocationCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Perform Self Test (7.2) command nominal case */
void Test_BPNode_PerformSelfTestCmd_Nominal(void)
{
    BPNode_PerformSelfTestCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Perform self test (7.2) command not implemented");

    UtAssert_INT32_EQ(BPNode_PerformSelfTestCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Send Node MIB Config HK command nominal case */
void Test_BPNode_SendNodeMibConfigHkCmd_Nominal(void)
{
    BPNode_SendNodeMibConfigHkCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Send node mib config hk command not implemented");

    UtAssert_INT32_EQ(BPNode_SendNodeMibConfigHkCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test node MIB counter housekeeping packet transmission in nominal case */
void Test_BPNode_SendNodeMibCountersHkCmd_Nominal(void)
{
    CFE_MSG_Message_t *MsgSend;
    CFE_MSG_Message_t *MsgTimestamp;

    /* Set up to capture send message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TransmitMsg), &MsgSend, sizeof(MsgSend), false);

    /* Set up to capture timestamp message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TimeStampMsg), &MsgTimestamp, sizeof(MsgTimestamp), false);

    /* Call unit under test, NULL pointer confirms command access is through APIs */
    BPNode_SendNodeMibCountersHkCmd(NULL);

    /* Confirm message sent*/
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_ADDRESS_EQ(MsgSend, &BPNode_AppData.NodeMibCountersHkTlm);

    /* Confirm timestamp msg address */
    UtAssert_STUB_COUNT(CFE_SB_TimeStampMsg, 1);
    UtAssert_ADDRESS_EQ(MsgTimestamp, &BPNode_AppData.NodeMibCountersHkTlm);
}

/* Test Send Per-Source MIB Config HK command nominal case */
void Test_BPNode_SendSourceMibConfigHkCmd_Nominal(void)
{
    BPNode_SendSourceMibConfigHkCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Send per-source mib config hk command not implemented");

    UtAssert_INT32_EQ(BPNode_SendSourceMibConfigHkCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Send Per-Source MIB Counter HK command nominal case */
void Test_BPNode_SendSourceMibCountersHkCmd_Nominal(void)
{
    BPNode_SendSourceMibCountersHkCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Send per-source mib counter hk command not implemented");

    UtAssert_INT32_EQ(BPNode_SendSourceMibCountersHkCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Send Storage HK command nominal case */
void Test_BPNode_SendStorageHkTlmCmd_Nominal(void)
{
    BPNode_SendStorageHkTlmCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Send storage hk command not implemented");

    UtAssert_INT32_EQ(BPNode_SendStorageHkTlmCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test Send Channel/Contact Status HK command nominal case */
void Test_BPNode_SendChannelContacStatHkTlmCmd_Nominal(void)
{
    BPNode_SendChannelContacStatHkTlmCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Send channel/contact status hk command not implemented");

    UtAssert_INT32_EQ(BPNode_SendChannelContacStatHkTlmCmd(&TestMsg), CFE_SUCCESS);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_NoopCmd_Nominal);
    ADD_TEST(Test_BPNode_AddAllApplicationsCmd_Nominal);
    ADD_TEST(Test_BPNode_StartAllApplicationsCmd_Nominal);
    ADD_TEST(Test_BPNode_VerifyBundleStorageCmd_Nominal);
    ADD_TEST(Test_BPNode_InitBundleStorageCmd_Nominal);
    ADD_TEST(Test_BPNode_VerifyBundleMetadataCmd_Nominal);
    ADD_TEST(Test_BPNode_RebuildBundleMetadataCmd_Nominal);
    ADD_TEST(Test_BPNode_ClearVolatileCmd_Nominal);
    ADD_TEST(Test_BPNode_ReloadSavedDataCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetAllCountersCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetCounterCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetSourceCountersCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetBundleCountersCmd_Nominal);
    ADD_TEST(Test_BPNode_ResetErrorCountersCmd_Nominal);
    ADD_TEST(Test_BPNode_AddApplicationCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveApplicationCmd_Nominal);
    ADD_TEST(Test_BPNode_SetRegistrationStateCmd_Nominal);
    ADD_TEST(Test_BPNode_StartApplicationCmd_Nominal);
    ADD_TEST(Test_BPNode_StopApplicationCmd_Nominal);
    ADD_TEST(Test_BPNode_AddAuthSourcesCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveAuthSourcesCmd_Nominal);
    ADD_TEST(Test_BPNode_AddAuthCustodySourcesCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveAuthCustodySourcesCmd_Nominal);
    ADD_TEST(Test_BPNode_AddAuthCustodiansCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveAuthCustodiansCmd_Nominal);
    ADD_TEST(Test_BPNode_AddAuthReportToEidCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveAuthReportToEidCmd_Nominal);
    ADD_TEST(Test_BPNode_AddLatencyCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveLatencyCmd_Nominal);
    ADD_TEST(Test_BPNode_ContactSetupCmd_Nominal);
    ADD_TEST(Test_BPNode_ContactStartCmd_Nominal);
    ADD_TEST(Test_BPNode_ContactStopCmd_Nominal);
    ADD_TEST(Test_BPNode_ContactTeardownCmd_Nominal);
    ADD_TEST(Test_BPNode_AddMibArrayKeyCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveMibArrayKeyCmd_Nominal);
    ADD_TEST(Test_BPNode_SetMibItemCmd_Nominal);
    ADD_TEST(Test_BPNode_AddStorageAllocationCmd_Nominal);
    ADD_TEST(Test_BPNode_RemoveStorageAllocationCmd_Nominal);
    ADD_TEST(Test_BPNode_PerformSelfTestCmd_Nominal);
    ADD_TEST(Test_BPNode_SendNodeMibConfigHkCmd_Nominal);
    ADD_TEST(Test_BPNode_SendNodeMibCountersHkCmd_Nominal);
    ADD_TEST(Test_BPNode_SendSourceMibConfigHkCmd_Nominal);
    ADD_TEST(Test_BPNode_SendSourceMibCountersHkCmd_Nominal);
    ADD_TEST(Test_BPNode_SendStorageHkTlmCmd_Nominal);
    ADD_TEST(Test_BPNode_SendChannelContacStatHkTlmCmd_Nominal);
}
