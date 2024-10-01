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

/* ============= */
/* Include Files */
/* ============= */

#include "fwp_dp.h"
#include "bplib_eventids.h"
#include "bpnode_test_utils.h"

/* ======= */
/* Globals */
/* ======= */

UT_CheckEvent_t EventTest;

/* ==================== */
/* Function Definitions */
/* ==================== */

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

    UtAssert_INT32_EQ(BPA_DP_NoopCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_NoopCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add All Applications command nominal case */
void TEST_BPA_DP_AddAllApplicationsCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_AddAllApplicationsCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplicationsCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Start All Applications command nominal case */
void TEST_BPA_DP_StartAllApplicationsCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_StartAllApplicationsCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_StartAllApplicationsCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Verify Bundle Storage command nominal case */
void TEST_BPA_DP_VerifyBundleStorageCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_VerifyBundleStorageCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleStorageCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Initialize Bundle Storage command nominal case */
void TEST_BPA_DP_InitBundleStorageCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_InitBundleStorageCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_InitBundleStorageCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Verify Bundle Metadata (7.2) command nominal case */
void TEST_BPA_DP_VerifyBundleMetadataCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_VerifyBundleMetadataCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleMetadataCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Rebuild Bundle Metadata command nominal case */
void TEST_BPA_DP_RebuildBundleMetadataCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_RebuildBundleMetadataCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RebuildBundleMetadataCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Clear Volatile command nominal case */
void TEST_BPA_DP_ClearVolatileCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ClearVolatileCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ClearVolatileCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reload Saved Data (7.2) command nominal case */
void TEST_BPA_DP_ReloadSavedDataCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ReloadSavedDataCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedDataCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset All Counters command nominal case */
void TEST_BPA_DP_ResetAllCountersCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ResetAllCountersCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset Counter command nominal case */
void TEST_BPA_DP_ResetCounterCmd_Nominal(void)
{
    BPNode_ResetCounterCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 2;

    UtAssert_INT32_EQ(BPA_DP_ResetCounterCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetCounterCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset Source Counters command nominal case */
void TEST_BPA_DP_ResetSourceCountersCmd_Nominal(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 42;

    UtAssert_INT32_EQ(BPA_DP_ResetSourceCountersCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetSourceCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset Bundle Counters command nominal case */
void TEST_BPA_DP_ResetBundleCountersCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ResetBundleCountersCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetBundleCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset Error Counters command nominal case */
void TEST_BPA_DP_ResetErrorCountersCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ResetErrorCountersCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Application command nominal case */
void TEST_BPA_DP_AddApplicationCmd_Nominal(void)
{
    BPNode_AddApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 13;

    UtAssert_INT32_EQ(BPA_DP_AddApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddApplicationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Application command nominal case */
void TEST_BPA_DP_RemoveApplicationCmd_Nominal(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 100;

    UtAssert_INT32_EQ(BPA_DP_RemoveApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveApplicationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Set Registration State command nominal case */
void TEST_BPA_DP_SetRegistrationStateCmd_Nominal(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 19;

    UtAssert_INT32_EQ(BPA_DP_SetRegistrationStateCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationStateCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Start Application command nominal case */
void TEST_BPA_DP_StartApplicationCmd_Nominal(void)
{
    BPNode_StartApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 1;

    UtAssert_INT32_EQ(BPA_DP_StartApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_StartApplicationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Stop Application command nominal case */
void TEST_BPA_DP_StopApplicationCmd_Nominal(void)
{
    BPNode_StopApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 3;

    UtAssert_INT32_EQ(BPA_DP_StopApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_StopApplicationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Authorized Sources command nominal case */
void TEST_BPA_DP_AddAuthSourcesCmd_Nominal(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 4;

    UtAssert_INT32_EQ(BPA_DP_AddAuthSourcesCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddAuthSourcesCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Authorized Sources command nominal case */
void TEST_BPA_DP_RemoveAuthSourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 5;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthSourcesCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthSourcesCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Authorized Custody Sources command nominal case */
void TEST_BPA_DP_AddAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 6;

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodySourcesCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodySourcesCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Authorized Custody Sources command nominal case */
void TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 7;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodySourcesCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodySourcesCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Authorized Custodians command nominal case */
void TEST_BPA_DP_AddAuthCustodiansCmd_Nominal(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 8;

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodiansCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodiansCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Authorized Custodians command nominal case */
void TEST_BPA_DP_RemoveAuthCustodiansCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 9;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodiansCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodiansCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Authorized Report-to EID command nominal case */
void TEST_BPA_DP_AddAuthReportToEidCmd_Nominal(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 10;

    UtAssert_INT32_EQ(BPA_DP_AddAuthReportToEidCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddAuthReportToEidCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Authorized Report-to EID command nominal case */
void TEST_BPA_DP_RemoveAuthReportToEidCmd_Nominal(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 11;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthReportToEidCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthReportToEidCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Latency command nominal case */
void TEST_BPA_DP_AddLatencyCmd_Nominal(void)
{
    BPNode_AddLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 12;

    UtAssert_INT32_EQ(BPA_DP_AddLatencyCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddLatencyCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Latency command nominal case */
void TEST_BPA_DP_RemoveLatencyCmd_Nominal(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 13;

    UtAssert_INT32_EQ(BPA_DP_RemoveLatencyCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveLatencyCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Contact Setup command nominal case */
void TEST_BPA_DP_ContactSetupCmd_Nominal(void)
{
    BPNode_ContactSetupCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 14;

    UtAssert_INT32_EQ(BPA_DP_ContactSetupCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ContactSetupCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Contact Start command nominal case */
void TEST_BPA_DP_ContactStartCmd_Nominal(void)
{
    BPNode_ContactStartCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 15;

    UtAssert_INT32_EQ(BPA_DP_ContactStartCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ContactStartCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Contact Stop command nominal case */
void TEST_BPA_DP_ContactStopCmd_Nominal(void)
{
    BPNode_ContactStopCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 16;

    UtAssert_INT32_EQ(BPA_DP_ContactStopCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ContactStopCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Contact Teardown command nominal case */
void TEST_BPA_DP_ContactTeardownCmd_Nominal(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 17;

    UtAssert_INT32_EQ(BPA_DP_ContactTeardownCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ContactTeardownCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add MIB Array Key command nominal case */
void TEST_BPA_DP_AddMibArrayKeyCmd_Nominal(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 18;

    UtAssert_INT32_EQ(BPA_DP_AddMibArrayKeyCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddMibArrayKeyCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove MIB Array Key command nominal case */
void TEST_BPA_DP_RemoveMibArrayKeyCmd_Nominal(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 19;

    UtAssert_INT32_EQ(BPA_DP_RemoveMibArrayKeyCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveMibArrayKeyCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Set MIB Item command nominal case */
void TEST_BPA_DP_SetMibItemCmd_Nominal(void)
{
    BPNode_SetMibItemCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 20;

    UtAssert_INT32_EQ(BPA_DP_SetMibItemCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SetMibItemCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add Storage Allocation command nominal case */
void TEST_BPA_DP_AddStorageAllocationCmd_Nominal(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 21;

    UtAssert_INT32_EQ(BPA_DP_AddStorageAllocationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_AddStorageAllocationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Remove Storage Allocation command nominal case */
void TEST_BPA_DP_RemoveStorageAllocationCmd_Nominal(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 22;

    UtAssert_INT32_EQ(BPA_DP_RemoveStorageAllocationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_RemoveStorageAllocationCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Perform Self Test (7.2) command nominal case */
void TEST_BPA_DP_PerformSelfTestCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_PerformSelfTestCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_PerformSelfTestCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Node MIB Config HK command nominal case */
void TEST_BPA_DP_SendNodeMibConfigHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendNodeMibConfigHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Per-Source MIB Config HK command nominal case */
void TEST_BPA_DP_SendSourceMibConfigHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendSourceMibConfigHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test node MIB counter housekeeping packet transmission in nominal case */
void TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendNodeMibCountersHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Per-Source MIB Counter HK command nominal case */
void TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendSourceMibCountersHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Storage HK command nominal case */
void TEST_BPA_DP_SendStorageHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendStorageHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Channel/Contact Status HK command nominal case */
void TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendChannelContactStatHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
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
