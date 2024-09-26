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
 *  Unit tests for fwp_dp.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"

#include "fwp_dp.h"
#include "bplib_eventids.h"

/* ======= */
/* Globals */
/* ======= */
UT_CheckEvent_t EventTest;
uint16 AcceptanceCounter = 1;

/*
** Function Definitions
*/

/* Test noop command in nominal case */
void TEST_BPA_DP_NoopCmd_Nominal(void)
{
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

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_NOOP_EID, "No-op command. %s");

    UtAssert_INT32_EQ(BPA_DP_NoopCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add All Applications command nominal case */
void TEST_BPA_DP_AddAllApplicationsCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_ALL_APPS_SUCCESS_EID, "Add all applications command is unimplemented");

    UtAssert_INT32_EQ(BPA_DP_AddAllApplicationsCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Start All Applications command nominal case */
void TEST_BPA_DP_StartAllApplicationsCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_START_ALL_APPS_SUCCESS_EID, "Start all applications command not implemented");

    UtAssert_INT32_EQ(BPA_DP_StartAllApplicationsCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Verify Bundle Storage command nominal case */
void TEST_BPA_DP_VerifyBundleStorageCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_VERIF_BNDL_SUCCESS_EID, "Verify bundle storage command not implemented");

    UtAssert_INT32_EQ(BPA_DP_VerifyBundleStorageCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Initialize Bundle Storage command nominal case */
void TEST_BPA_DP_InitBundleStorageCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_INIT_BNDL_STOR_SUCCESS_EID, "Initialize bundle storage command not implemented");

    UtAssert_INT32_EQ(BPA_DP_InitBundleStorageCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Verify Bundle Metadata (7.2) command nominal case */
void TEST_BPA_DP_VerifyBundleMetadataCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_VERIF_BNDL_META_SUCCESS_EID, "Verify bundle metadata (7.2) command not implemented");

    UtAssert_INT32_EQ(BPA_DP_VerifyBundleMetadataCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Rebuild Bundle Metadata command nominal case */
void TEST_BPA_DP_RebuildBundleMetadataCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_REBUILD_BNDL_META_SUCCESS_EID, "Rebuild bundle metadata command not implemented");

    UtAssert_INT32_EQ(BPA_DP_RebuildBundleMetadataCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Clear Volatile command nominal case */
void TEST_BPA_DP_ClearVolatileCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_CLR_VOLATILE_SUCCESS_EID, "Clear volatile command not implemented");

    UtAssert_INT32_EQ(BPA_DP_ClearVolatileCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reload Saved Data (7.2) command nominal case */
void TEST_BPA_DP_ReloadSavedDataCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RELOAD_SVD_DATA_SUCCESS_EID, "Reload saved data (7.2) command not implemented");

    UtAssert_INT32_EQ(BPA_DP_ReloadSavedDataCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reset All Counters command nominal case */
void TEST_BPA_DP_ResetAllCountersCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RESET_ALL_CTRS_SUCCESS_EID, "Reset all counters command not implemented");

    UtAssert_INT32_EQ(BPA_DP_ResetAllCountersCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reset Counter command nominal case */
void TEST_BPA_DP_ResetCounterCmd_Nominal(void)
{
    BPNode_ResetCounterCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 2;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset counter command not implemented, received 2 in payload");

    UtAssert_INT32_EQ(BPA_DP_ResetCounterCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reset Source Counters command nominal case */
void TEST_BPA_DP_ResetSourceCountersCmd_Nominal(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 42;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RESET_SRC_CTRS_SUCCESS_EID, "Reset source counters command not implemented, received 42 in payload");

    UtAssert_INT32_EQ(BPA_DP_ResetSourceCountersCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reset Bundle Counters command nominal case */
void TEST_BPA_DP_ResetBundleCountersCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_RESET_INF_EID, "Reset bundle counters command not implemented");

    UtAssert_INT32_EQ(BPA_DP_ResetBundleCountersCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Reset Error Counters command nominal case */
void TEST_BPA_DP_ResetErrorCountersCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RESET_ERR_CTRS_SUCCESS_EID, "Reset error counters command not implemented");

    UtAssert_INT32_EQ(BPA_DP_ResetErrorCountersCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Application command nominal case */
void TEST_BPA_DP_AddApplicationCmd_Nominal(void)
{
    BPNode_AddApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 13;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_APP_SUCCESS_EID, "Add application command not implemented, received 13 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddApplicationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Application command nominal case */
void TEST_BPA_DP_RemoveApplicationCmd_Nominal(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 100;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_APP_SUCCESS_EID, "Remove application command not implemented, received 100 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveApplicationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Set Registration State command nominal case */
void TEST_BPA_DP_SetRegistrationStateCmd_Nominal(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 19;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SET_REGI_STAT_SUCCESS_EID, "Set registration state command not implemented, received %d in payload");

    UtAssert_INT32_EQ(BPA_DP_SetRegistrationStateCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Start Application command nominal case */
void TEST_BPA_DP_StartApplicationCmd_Nominal(void)
{
    BPNode_StartApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 1;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_START_APP_SUCCESS_EID, "Start application command not implemented, received 1 in payload");

    UtAssert_INT32_EQ(BPA_DP_StartApplicationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Stop Application command nominal case */
void TEST_BPA_DP_StopApplicationCmd_Nominal(void)
{
    BPNode_StopApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 3;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_STOP_APP_SUCCESS_EID, "Stop application command not implemented, received 3 in payload");

    UtAssert_INT32_EQ(BPA_DP_StopApplicationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Authorized Sources command nominal case */
void TEST_BPA_DP_AddAuthSourcesCmd_Nominal(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 4;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_AUTH_SRCS_SUCCESS_EID, "Add authorized sources command not implemented, received 4 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddAuthSourcesCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Authorized Sources command nominal case */
void TEST_BPA_DP_RemoveAuthSourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 5;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_AUTH_SRCS_SUCCESS_EID, "Remove authorized sources command not implemented, received 5 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthSourcesCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Authorized Custody Sources command nominal case */
void TEST_BPA_DP_AddAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 6;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_AUTH_CUST_SRCS_SUCESS_EID, "Add authorized custody sources command not implemented, received 6 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodySourcesCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Authorized Custody Sources command nominal case */
void TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 7;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_AUTH_CUST_SRCS_SUCCESS_EID, "Remove authorized custody sources command not implemented, received 7 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodySourcesCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Authorized Custodians command nominal case */
void TEST_BPA_DP_AddAuthCustodiansCmd_Nominal(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 8;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_AUTH_CUSTODIANS_SUCCESS_EID, "Add authorized custodians command not implemented, received 8 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodiansCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Authorized Custodians command nominal case */
void TEST_BPA_DP_RemoveAuthCustodiansCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 9;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_AUTH_CUSTODIANS_SUCCESS_EID, "Remove authorized custodians command not implemented, received 9 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodiansCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Authorized Report-to EID command nominal case */
void TEST_BPA_DP_AddAuthReportToEidCmd_Nominal(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 10;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_AUTH_RPT_EID_SUCCESS_EID, "Add authorized report-to eid command not implemented, received 10 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddAuthReportToEidCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Authorized Report-to EID command nominal case */
void TEST_BPA_DP_RemoveAuthReportToEidCmd_Nominal(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 11;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_AUTH_RPT_EID_SUCCESS_EID, "Remove authorized report-to eid command not implemented, received 11 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthReportToEidCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Latency command nominal case */
void TEST_BPA_DP_AddLatencyCmd_Nominal(void)
{
    BPNode_AddLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 12;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_LATENCY_SUCCESS_EID, "Add latency command not implemented, received 12 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddLatencyCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Latency command nominal case */
void TEST_BPA_DP_RemoveLatencyCmd_Nominal(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 13;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_LATENCY_SUCCESS_EID, "Remove latency command not implemented, received 13 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveLatencyCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Contact Setup command nominal case */
void TEST_BPA_DP_ContactSetupCmd_Nominal(void)
{
    BPNode_ContactSetupCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 14;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_CONTACT_SETUP_SUCCESS_EID, "Contact setup command not implemented, received 14 in payload");

    UtAssert_INT32_EQ(BPA_DP_ContactSetupCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Contact Start command nominal case */
void TEST_BPA_DP_ContactStartCmd_Nominal(void)
{
    BPNode_ContactStartCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 15;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_CONTACT_START_SUCCESS_EID, "Contact start command not implemented, received 15 in payload");

    UtAssert_INT32_EQ(BPA_DP_ContactStartCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Contact Stop command nominal case */
void TEST_BPA_DP_ContactStopCmd_Nominal(void)
{
    BPNode_ContactStopCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 16;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_CONTACT_STOP_SUCCESS_EID, "Contact stop command not implemented, received 16 in payload");

    UtAssert_INT32_EQ(BPA_DP_ContactStopCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Contact Teardown command nominal case */
void TEST_BPA_DP_ContactTeardownCmd_Nominal(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 17;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_CONTACT_TEARDOWN_SUCCESS_EID, "Contact teardown command not implemented, received 17 in payload");

    UtAssert_INT32_EQ(BPA_DP_ContactTeardownCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add MIB Array Key command nominal case */
void TEST_BPA_DP_AddMibArrayKeyCmd_Nominal(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 18;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_MIB_ARR_KEY_SUCCESS_EID, "Add mib array key command not implemented, received 18 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddMibArrayKeyCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove MIB Array Key command nominal case */
void TEST_BPA_DP_RemoveMibArrayKeyCmd_Nominal(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 19;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_MIB_ARR_KEY_SUCCESS_EID, "Remove mib array key command not implemented, received 19 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveMibArrayKeyCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Set MIB Item command nominal case */
void TEST_BPA_DP_SetMibItemCmd_Nominal(void)
{
    BPNode_SetMibItemCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 20;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SET_MIB_ITEM_SUCCESS_EID, "Set mib item command not implemented, received 20 in payload");

    UtAssert_INT32_EQ(BPA_DP_SetMibItemCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Add Storage Allocation command nominal case */
void TEST_BPA_DP_AddStorageAllocationCmd_Nominal(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 21;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_ADD_STOR_ALLOC_SUCCESS_EID, "Add storage allocation command not implemented, received 21 in payload");

    UtAssert_INT32_EQ(BPA_DP_AddStorageAllocationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Remove Storage Allocation command nominal case */
void TEST_BPA_DP_RemoveStorageAllocationCmd_Nominal(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 22;

    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_RM_STOR_ALLOC_SUCCESS_EID, "Remove storage allocation command not implemented, received 22 in payload");

    UtAssert_INT32_EQ(BPA_DP_RemoveStorageAllocationCmd(&TestMsg), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Perform Self Test (7.2) command nominal case */
void TEST_BPA_DP_PerformSelfTestCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_PERFORM_SELF_TEST_SUCCESS_EID, "Perform self test (7.2) command not implemented");

    UtAssert_INT32_EQ(BPA_DP_PerformSelfTestCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Send Node MIB Config HK command nominal case */
void TEST_BPA_DP_SendNodeMibConfigHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_NODE_MIB_CFG_HK_SUCCESS_EID, "Send node mib config hk command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendNodeMibConfigHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Send Per-Source MIB Config HK command nominal case */
void TEST_BPA_DP_SendSourceMibConfigHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_SRC_MIB_CFG_HK_SUCCESS_EID, "Send per-source mib config hk command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendSourceMibConfigHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test node MIB counter housekeeping packet transmission in nominal case */
void TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_NODE_MIB_CTRS_HK_SUCCESS_EID, "Send node mib counters HK command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendNodeMibCountersHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Send Per-Source MIB Counter HK command nominal case */
void TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_SRC_MIB_CTRS_HK_SUCCESS_EID, "Send per-source mib counter hk command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendSourceMibCountersHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Send Storage HK command nominal case */
void TEST_BPA_DP_SendStorageHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_STOR_HK_SUCCESS_EID, "Send storage hk command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendStorageHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Test Send Channel/Contact Status HK command nominal case */
void TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal(void)
{
    UT_CHECKEVENT_SETUP(&EventTest, BPLIB_SEND_CHAN_CONTACT_STAT_HK_SUCCESS_EID, "Send channel/contact status hk command not implemented");

    UtAssert_INT32_EQ(BPA_DP_SendChannelContactStatHkCmd(), BPLIB_SUCCESS);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, AcceptanceCounter++);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(TEST_BPA_DP_NoopCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAllApplicationsCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StartAllApplicationsCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_VerifyBundleStorageCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_InitBundleStorageCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_VerifyBundleMetadataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RebuildBundleMetadataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ClearVolatileCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ReloadSavedDataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetAllCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetCounterCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetSourceCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetBundleCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetErrorCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SetRegistrationStateCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StartApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StopApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthSourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthSourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthCustodySourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthCustodiansCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodiansCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthReportToEidCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthReportToEidCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddLatencyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveLatencyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactSetupCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactStartCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactStopCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactTeardownCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddMibArrayKeyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveMibArrayKeyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SetMibItemCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddStorageAllocationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveStorageAllocationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_PerformSelfTestCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendNodeMibConfigHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendSourceMibConfigHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendStorageHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal);
}
