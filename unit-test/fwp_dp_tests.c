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
#include "bpnode_test_utils.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Test noop command in nominal case */
void TEST_BPA_DP_NoopCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_NoopCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_NoopCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_NoopCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_NoopCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_NoopCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddAllApplicationsCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAllApplicationsCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_AddAllApplicationsCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_StartAllApplicationsCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_StartAllApplicationsCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_StartAllApplicationsCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_VerifyBundleStorageCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_VerifyBundleStorageCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_VerifyBundleStorageCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_InitBundleStorageCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_InitBundleStorageCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_InitBundleStorageCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_VerifyBundleMetadataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_VerifyBundleMetadataCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_VerifyBundleMetadataCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RebuildBundleMetadataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RebuildBundleMetadataCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_RebuildBundleMetadataCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ClearVolatileCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ClearVolatileCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_ClearVolatileCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ReloadSavedDataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ReloadSavedDataCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_ReloadSavedDataCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedDataCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Reset All Counters command nominal case */
void TEST_BPA_DP_ResetAllCountersCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_ResetAllCountersCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived, 0);
}

void TEST_BPA_DP_ResetAllCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetAllCountersCmd), BPLIB_UNIMPLEMENTED);
    
    UtAssert_INT32_EQ(BPA_DP_ResetAllCountersCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived, 0);
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

void TEST_BPA_DP_ResetCounterCmd_Error(void)
{
    BPNode_ResetCounterCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetCounterCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 2;

    UtAssert_INT32_EQ(BPA_DP_ResetCounterCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ResetSourceCountersCmd_Error(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetSourceCountersCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 42;

    UtAssert_INT32_EQ(BPA_DP_ResetSourceCountersCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ResetBundleCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetBundleCountersCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_ResetBundleCountersCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ResetErrorCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetErrorCountersCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_ResetErrorCountersCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCountersCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test add-application command nominal case */
void TEST_BPA_DP_AddApplicationCmd_Nominal(void)
{
    BPNode_AddApplicationCmd_t TestMsg;
    UT_CheckEvent_t            EventTest;
    uint16                     ExpAcceptedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_ADD_APP_INF_EID, 
                            "Successful add-application directive for ChanId=%d");

    UtAssert_INT32_EQ(BPA_DP_AddApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount,
                            ExpAcceptedCount);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test add-application command failure case */
void TEST_BPA_DP_AddApplicationCmd_Error(void)
{
    BPNode_AddApplicationCmd_t TestMsg;
    uint16                     ExpRejectedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_SetDefaultReturnValue(UT_KEY(BPA_ADUP_AddApplication), BPLIB_ERROR);

    UtAssert_INT32_EQ(BPA_DP_AddApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount,
                            ExpRejectedCount);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
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

void TEST_BPA_DP_RemoveApplicationCmd_Error(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveApplicationCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 100;

    UtAssert_INT32_EQ(BPA_DP_RemoveApplicationCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_SetRegistrationStateCmd_Error(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SetRegistrationStateCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 19;

    UtAssert_INT32_EQ(BPA_DP_SetRegistrationStateCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationStateCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test start-application command nominal case */
void TEST_BPA_DP_StartApplicationCmd_Nominal(void)
{
    BPNode_StartApplicationCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;
    uint16                       ExpAcceptedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_STRT_APP_INF_EID, 
                            "Successful start-application directive for ChanId=%d");

    UtAssert_INT32_EQ(BPA_DP_StartApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount,
                            ExpAcceptedCount);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test start-application command failure case */
void TEST_BPA_DP_StartApplicationCmd_Error(void)
{
    BPNode_StartApplicationCmd_t TestMsg;
    uint16                       ExpRejectedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_SetDefaultReturnValue(UT_KEY(BPA_ADUP_StartApplication), BPLIB_ERROR);

    UtAssert_INT32_EQ(BPA_DP_StartApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount,
                            ExpRejectedCount);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test stop-application command nominal case */
void TEST_BPA_DP_StopApplicationCmd_Nominal(void)
{
    BPNode_StopApplicationCmd_t TestMsg;
    UT_CheckEvent_t             EventTest;
    uint16                      ExpAcceptedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_STOP_APP_INF_EID, 
                            "Successful stop-application directive for ChanId=%d");

    UtAssert_INT32_EQ(BPA_DP_StopApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount,
                            ExpAcceptedCount);

    /* Confirm that the event was generated */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test stop-application command failure case */
void TEST_BPA_DP_StopApplicationCmd_Error(void)
{
    BPNode_StopApplicationCmd_t TestMsg;
    uint16                      ExpRejectedCount = 1;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_SetDefaultReturnValue(UT_KEY(BPA_ADUP_StopApplication), BPLIB_ERROR);

    UtAssert_INT32_EQ(BPA_DP_StopApplicationCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount,
                            ExpRejectedCount);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
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

void TEST_BPA_DP_AddAuthSourcesCmd_Error(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthSourcesCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 4;

    UtAssert_INT32_EQ(BPA_DP_AddAuthSourcesCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveAuthSourcesCmd_Error(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthSourcesCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 5;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthSourcesCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddAuthCustodySourcesCmd_Error(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthCustodySourcesCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 6;

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodySourcesCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Error(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthCustodySourcesCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 7;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodySourcesCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddAuthCustodiansCmd_Error(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthCustodiansCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 8;

    UtAssert_INT32_EQ(BPA_DP_AddAuthCustodiansCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveAuthCustodiansCmd_Error(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthCustodiansCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 9;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthCustodiansCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddAuthReportToEidCmd_Error(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthReportToEidCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 10;

    UtAssert_INT32_EQ(BPA_DP_AddAuthReportToEidCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveAuthReportToEidCmd_Error(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthReportToEidCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 11;

    UtAssert_INT32_EQ(BPA_DP_RemoveAuthReportToEidCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddLatencyCmd_Error(void)
{
    BPNode_AddLatencyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddLatencyCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 12;

    UtAssert_INT32_EQ(BPA_DP_AddLatencyCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveLatencyCmd_Error(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveLatencyCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 13;

    UtAssert_INT32_EQ(BPA_DP_RemoveLatencyCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ContactSetupCmd_Error(void)
{
    BPNode_ContactSetupCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactSetupCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 14;

    UtAssert_INT32_EQ(BPA_DP_ContactSetupCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ContactStartCmd_Error(void)
{
    BPNode_ContactStartCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactStartCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 15;

    UtAssert_INT32_EQ(BPA_DP_ContactStartCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ContactStopCmd_Error(void)
{
    BPNode_ContactStopCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactStopCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 16;

    UtAssert_INT32_EQ(BPA_DP_ContactStopCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_ContactTeardownCmd_Error(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactTeardownCmd), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 17;

    UtAssert_INT32_EQ(BPA_DP_ContactTeardownCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddMibArrayKeyCmd_Error(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddMibArrayKeyCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 18;

    UtAssert_INT32_EQ(BPA_DP_AddMibArrayKeyCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveMibArrayKeyCmd_Error(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveMibArrayKeyCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 19;

    UtAssert_INT32_EQ(BPA_DP_RemoveMibArrayKeyCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_SetMibItemCmd_Error(void)
{
    BPNode_SetMibItemCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SetMibItemCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 20;

    UtAssert_INT32_EQ(BPA_DP_SetMibItemCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_AddStorageAllocationCmd_Error(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddStorageAllocationCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 21;

    UtAssert_INT32_EQ(BPA_DP_AddStorageAllocationCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_RemoveStorageAllocationCmd_Error(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveStorageAllocationCmd), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 22;

    UtAssert_INT32_EQ(BPA_DP_RemoveStorageAllocationCmd(&TestMsg), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_PerformSelfTestCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_PerformSelfTestCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_PerformSelfTestCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_SendNodeMibConfigHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendNodeMibConfigHkCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_SendNodeMibConfigHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_SendSourceMibConfigHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibConfigHkCmd), BPLIB_UNIMPLEMENTED);
    
    UtAssert_INT32_EQ(BPA_DP_SendSourceMibConfigHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test node MIB counter housekeeping packet transmission in nominal case */
void TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendNodeMibCountersHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHkCmd , 1);
}

void TEST_BPA_DP_SendNodeMibCountersHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendNodeMibCountersHkCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_SendNodeMibCountersHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHkCmd , 1);
}

/* Test Send Per-Source MIB Counter HK command nominal case */
void TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal(void)
{
    UtAssert_INT32_EQ(BPA_DP_SendSourceMibCountersHkCmd(), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendSourceMibCountersHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibCountersHkCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_SendSourceMibCountersHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
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

void TEST_BPA_DP_SendStorageHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendStorageHkCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_SendStorageHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Send Channel/Contact Status HK command nominal case */
void TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal(void)
{
    BPNode_SendChannelContactStatHkCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UtAssert_INT32_EQ(BPA_DP_SendChannelContactStatHkCmd(&TestMsg), CFE_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHkCmd, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendChannelContactStatHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendChannelContactStatHkCmd), BPLIB_UNIMPLEMENTED);

    UtAssert_INT32_EQ(BPA_DP_SendChannelContactStatHkCmd(), CFE_STATUS_NOT_IMPLEMENTED);
    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHkCmd , 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(TEST_BPA_DP_NoopCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_NoopCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddAllApplicationsCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAllApplicationsCmd_Error);

    ADD_TEST(TEST_BPA_DP_StartAllApplicationsCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StartAllApplicationsCmd_Error);

    ADD_TEST(TEST_BPA_DP_VerifyBundleStorageCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_VerifyBundleStorageCmd_Error);

    ADD_TEST(TEST_BPA_DP_InitBundleStorageCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_InitBundleStorageCmd_Error);

    ADD_TEST(TEST_BPA_DP_VerifyBundleMetadataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_VerifyBundleMetadataCmd_Error);

    ADD_TEST(TEST_BPA_DP_RebuildBundleMetadataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RebuildBundleMetadataCmd_Error);

    ADD_TEST(TEST_BPA_DP_ClearVolatileCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ClearVolatileCmd_Error);

    ADD_TEST(TEST_BPA_DP_ReloadSavedDataCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ReloadSavedDataCmd_Error);

    ADD_TEST(TEST_BPA_DP_ResetAllCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetAllCountersCmd_Error);

    ADD_TEST(TEST_BPA_DP_ResetCounterCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetCounterCmd_Error);

    ADD_TEST(TEST_BPA_DP_ResetSourceCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetSourceCountersCmd_Error);

    ADD_TEST(TEST_BPA_DP_ResetBundleCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetBundleCountersCmd_Error);

    ADD_TEST(TEST_BPA_DP_ResetErrorCountersCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ResetErrorCountersCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddApplicationCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveApplicationCmd_Error);

    ADD_TEST(TEST_BPA_DP_SetRegistrationStateCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SetRegistrationStateCmd_Error);

    ADD_TEST(TEST_BPA_DP_StartApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StartApplicationCmd_Error);

    ADD_TEST(TEST_BPA_DP_StopApplicationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_StopApplicationCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddAuthSourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthSourcesCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveAuthSourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthSourcesCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddAuthCustodySourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthCustodySourcesCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddAuthCustodiansCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthCustodiansCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodiansCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthCustodiansCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddAuthReportToEidCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddAuthReportToEidCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveAuthReportToEidCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveAuthReportToEidCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddLatencyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddLatencyCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveLatencyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveLatencyCmd_Error);

    ADD_TEST(TEST_BPA_DP_ContactSetupCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactSetupCmd_Error);

    ADD_TEST(TEST_BPA_DP_ContactStartCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactStartCmd_Error);

    ADD_TEST(TEST_BPA_DP_ContactStopCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactStopCmd_Error);

    ADD_TEST(TEST_BPA_DP_ContactTeardownCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_ContactTeardownCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddMibArrayKeyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddMibArrayKeyCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveMibArrayKeyCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveMibArrayKeyCmd_Error);

    ADD_TEST(TEST_BPA_DP_SetMibItemCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SetMibItemCmd_Error);

    ADD_TEST(TEST_BPA_DP_AddStorageAllocationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_AddStorageAllocationCmd_Error);

    ADD_TEST(TEST_BPA_DP_RemoveStorageAllocationCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_RemoveStorageAllocationCmd_Error);

    ADD_TEST(TEST_BPA_DP_PerformSelfTestCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_PerformSelfTestCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendNodeMibConfigHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendNodeMibConfigHkCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendNodeMibCountersHkCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendSourceMibConfigHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendSourceMibConfigHkCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendSourceMibCountersHkCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendStorageHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendStorageHkCmd_Error);

    ADD_TEST(TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal);
    ADD_TEST(TEST_BPA_DP_SendChannelContactStatHkCmd_Error);
}
