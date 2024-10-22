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

/* Test task pipe processing in nominal case */
void Test_BPNode_TaskPipe_Nominal(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            MsgSize = sizeof(BPNode_NoopCmd_t);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);

    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(BPA_DP_NoopCmd, 1);
}

/* Test task pipe processing after receiving an invalid message ID */
void Test_BPNode_TaskPipe_InvalidMsgId(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_INVALID_MSG_ID;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_MID_ERR_EID, 
                                "Invalid command packet,MID = 0x%x");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);

    BPNode_TaskPipe(&Buf);

    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/**
 * Test ground command processing after receiving a valid noop.
 *
 * Tests for valid commands call command stubs in bpnode_cmd_stubs.c.
 * The command stubs do not generate events.
 * The stubs only increment the stub call count for the command.
 */
void Test_BPNode_ProcessGroundCommand_ValidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_NoopCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/**
 * Test ground command processing after receiving an invalid noop.
 *
 * Tests for invalid commands use an invalid command length.
 * The check for an invalid command length is in bpnode_dispatch.c
 * and generates a BPNODE_CMD_LEN_ERR_EID event.
 */

void Test_BPNode_ProcessGroundCommand_InvalidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_NoopCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-all-applications */
void Test_BPNode_ProcessGroundCommand_ValidAddAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_AddAllApplicationsCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAllApplicationsCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-all-applications */
void Test_BPNode_ProcessGroundCommand_InvalidAddAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_AddAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAllApplicationsCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid start-all-applications */
void Test_BPNode_ProcessGroundCommand_ValidStartAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_StartAllApplicationsCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StartAllApplicationsCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid start-all-applications */
void Test_BPNode_ProcessGroundCommand_InvalidStartAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_StartAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StartAllApplicationsCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid verify-bundle-storage */
void Test_BPNode_ProcessGroundCommand_ValidVerifyBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleStorageCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_VerifyBundleStorageCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid verify-bundle-storage */
void Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_VerifyBundleStorageCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid init-bundle-storage */
void Test_BPNode_ProcessGroundCommand_ValidInitBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_INIT_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_InitBundleStorageCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_InitBundleStorageCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid init-bundle-storage */
void Test_BPNode_ProcessGroundCommand_InvalidInitBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_INIT_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_InitBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_InitBundleStorageCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid verify-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_ValidVerifyBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleMetadataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_VerifyBundleMetadataCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid verify-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_VerifyBundleMetadataCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid rebuild-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_ValidRebuildBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REBUILD_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_RebuildBundleMetadataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RebuildBundleMetadataCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid rebuild-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_InvalidRebuildBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REBUILD_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_RebuildBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RebuildBundleMetadataCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid clear-volatile */
void Test_BPNode_ProcessGroundCommand_ValidClearVolatile(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CLEAR_VOLATILE_CC;
    size_t            Size = sizeof(BPNode_ClearVolatileCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ClearVolatileCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid clear-volatile */
void Test_BPNode_ProcessGroundCommand_InvalidClearVolatile(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CLEAR_VOLATILE_CC;
    size_t            Size = sizeof(BPNode_ClearVolatileCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ClearVolatileCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reload-saved-data */
void Test_BPNode_ProcessGroundCommand_ValidReloadSavedData(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RELOAD_SAVED_DATA_CC;
    size_t            Size = sizeof(BPNode_ReloadSavedDataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ReloadSavedDataCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reload-saved-data */
void Test_BPNode_ProcessGroundCommand_InvalidReloadSavedData(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RELOAD_SAVED_DATA_CC;
    size_t            Size = sizeof(BPNode_ReloadSavedDataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ReloadSavedDataCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reset-all-counters */
void Test_BPNode_ProcessGroundCommand_ValidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetAllCountersCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-all-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetAllCountersCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reset-counter */
void Test_BPNode_ProcessGroundCommand_ValidResetCounter(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_COUNTER_CC;
    size_t            Size = sizeof(BPNode_ResetCounterCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetCounterCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-counter */
void Test_BPNode_ProcessGroundCommand_InvalidResetCounter(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_COUNTER_CC;
    size_t            Size = sizeof(BPNode_ResetCounterCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetCounterCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reset-source-counters */
void Test_BPNode_ProcessGroundCommand_ValidResetSourceCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_SOURCE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetSourceCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetSourceCountersCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-source-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetSourceCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_SOURCE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetSourceCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetSourceCountersCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reset-bundle-counters */
void Test_BPNode_ProcessGroundCommand_ValidResetBundleCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_BUNDLE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetBundleCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetBundleCountersCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-bundle-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetBundleCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_BUNDLE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetBundleCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetBundleCountersCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid reset-error-counters */
void Test_BPNode_ProcessGroundCommand_ValidResetErrorCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ERROR_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetErrorCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetErrorCountersCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-error-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetErrorCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ERROR_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetErrorCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ResetErrorCountersCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-application */
void Test_BPNode_ProcessGroundCommand_ValidAddApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_AddApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddApplicationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-application */
void Test_BPNode_ProcessGroundCommand_InvalidAddApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_AddApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddApplicationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-application */
void Test_BPNode_ProcessGroundCommand_ValidRemoveApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_RemoveApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveApplicationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-application */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_RemoveApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveApplicationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid set-registration-state */
void Test_BPNode_ProcessGroundCommand_ValidSetRegistrationState(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_REGISTRATION_STATE_CC;
    size_t            Size = sizeof(BPNode_SetRegistrationStateCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SetRegistrationStateCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid set-registration-state */
void Test_BPNode_ProcessGroundCommand_InvalidSetRegistrationState(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_REGISTRATION_STATE_CC;
    size_t            Size = sizeof(BPNode_SetRegistrationStateCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SetRegistrationStateCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid start-application */
void Test_BPNode_ProcessGroundCommand_ValidStartApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StartApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StartApplicationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid start-application */
void Test_BPNode_ProcessGroundCommand_InvalidStartApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StartApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StartApplicationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid stop-application */
void Test_BPNode_ProcessGroundCommand_ValidStopApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_STOP_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StopApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StopApplicationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid stop-application */
void Test_BPNode_ProcessGroundCommand_InvalidStopApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_STOP_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StopApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_StopApplicationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-auth-sources */
void Test_BPNode_ProcessGroundCommand_ValidAddAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthSourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthSourcesCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-sources */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthSourcesCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-auth-sources */
void Test_BPNode_ProcessGroundCommand_ValidRemoveAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthSourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthSourcesCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-sources */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthSourcesCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_ValidAddAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodySourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthCustodySourcesCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthCustodySourcesCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_ValidRemoveAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodySourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthCustodySourcesCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthCustodySourcesCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-auth-custodians */
void Test_BPNode_ProcessGroundCommand_ValidAddAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodiansCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthCustodiansCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-custodians */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthCustodiansCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-auth-custodians */
void Test_BPNode_ProcessGroundCommand_ValidRemoveAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodiansCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthCustodiansCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-custodians */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthCustodiansCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_ValidAddAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_AddAuthReportToEidCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthReportToEidCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_AddAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddAuthReportToEidCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_ValidRemoveAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthReportToEidCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthReportToEidCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveAuthReportToEidCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-latency */
void Test_BPNode_ProcessGroundCommand_ValidAddLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_LATENCY_CC;
    size_t            Size = sizeof(BPNode_AddLatencyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddLatencyCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-latency */
void Test_BPNode_ProcessGroundCommand_InvalidAddLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_LATENCY_CC;
    size_t            Size = sizeof(BPNode_AddLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddLatencyCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-latency */
void Test_BPNode_ProcessGroundCommand_ValidRemoveLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_LATENCY_CC;
    size_t            Size = sizeof(BPNode_RemoveLatencyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveLatencyCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-latency */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_LATENCY_CC;
    size_t            Size = sizeof(BPNode_RemoveLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveLatencyCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid contact-setup */
void Test_BPNode_ProcessGroundCommand_ValidContactSetup(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_SETUP_CC;
    size_t            Size = sizeof(BPNode_ContactSetupCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactSetupCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-setup */
void Test_BPNode_ProcessGroundCommand_InvalidContactSetup(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_SETUP_CC;
    size_t            Size = sizeof(BPNode_ContactSetupCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactSetupCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid contact-start */
void Test_BPNode_ProcessGroundCommand_ValidContactStart(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_START_CC;
    size_t            Size = sizeof(BPNode_ContactStartCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactStartCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-start */
void Test_BPNode_ProcessGroundCommand_InvalidContactStart(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_START_CC;
    size_t            Size = sizeof(BPNode_ContactStartCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactStartCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid contact-stop */
void Test_BPNode_ProcessGroundCommand_ValidContactStop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_STOP_CC;
    size_t            Size = sizeof(BPNode_ContactStopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactStopCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-stop */
void Test_BPNode_ProcessGroundCommand_InvalidContactStop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_STOP_CC;
    size_t            Size = sizeof(BPNode_ContactStopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactStopCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid contact-teardown */
void Test_BPNode_ProcessGroundCommand_ValidContactTeardown(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_TEARDOWN_CC;
    size_t            Size = sizeof(BPNode_ContactTeardownCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactTeardownCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-teardown */
void Test_BPNode_ProcessGroundCommand_InvalidContactTeardown(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_TEARDOWN_CC;
    size_t            Size = sizeof(BPNode_ContactTeardownCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_ContactTeardownCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-mib-array-key */
void Test_BPNode_ProcessGroundCommand_ValidAddMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_AddMibArrayKeyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddMibArrayKeyCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-mib-array-key */
void Test_BPNode_ProcessGroundCommand_InvalidAddMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_AddMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddMibArrayKeyCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-mib-array-key */
void Test_BPNode_ProcessGroundCommand_ValidRemoveMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_RemoveMibArrayKeyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveMibArrayKeyCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-mib-array-key */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_RemoveMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveMibArrayKeyCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid set-mib-item */
void Test_BPNode_ProcessGroundCommand_ValidSetMibItem(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_MIB_ITEM_CC;
    size_t            Size = sizeof(BPNode_SetMibItemCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SetMibItemCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid set-mib-item */
void Test_BPNode_ProcessGroundCommand_InvalidSetMibItem(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_MIB_ITEM_CC;
    size_t            Size = sizeof(BPNode_SetMibItemCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SetMibItemCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid add-storage-allocation */
void Test_BPNode_ProcessGroundCommand_ValidAddStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_AddStorageAllocationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddStorageAllocationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-storage-allocation */
void Test_BPNode_ProcessGroundCommand_InvalidAddStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_AddStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_AddStorageAllocationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid remove-storage-allocation */
void Test_BPNode_ProcessGroundCommand_ValidRemoveStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_RemoveStorageAllocationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveStorageAllocationCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-storage-allocation */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_RemoveStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_RemoveStorageAllocationCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid perform-self-test */
void Test_BPNode_ProcessGroundCommand_ValidPerformSelfTest(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_PERFORM_SELF_TEST_CC;
    size_t            Size = sizeof(BPNode_PerformSelfTestCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_PerformSelfTestCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid perform-self-test */
void Test_BPNode_ProcessGroundCommand_InvalidPerformSelfTest(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_PERFORM_SELF_TEST_CC;
    size_t            Size = sizeof(BPNode_PerformSelfTestCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_PerformSelfTestCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-node-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_ValidSendNodeMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendNodeMibConfigHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendNodeMibConfigHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-source-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_ValidSendSourceMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendSourceMibConfigHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendSourceMibConfigHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-node-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_ValidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendNodeMibCountersHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendNodeMibCountersHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-source-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_ValidSendSourceMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendSourceMibCountersHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendSourceMibCountersHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-storage-hk-tlm */
void Test_BPNode_ProcessGroundCommand_ValidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendStorageHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-storage-hk-tlm */
void Test_BPNode_ProcessGroundCommand_InvalidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendStorageHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test ground command processing after receiving a valid send-channel-contact-stat-hk-tlm */
void Test_BPNode_ProcessGroundCommand_ValidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC;
    size_t            Size = sizeof(BPNode_SendChannelContactStatHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendChannelContactStatHkCmd, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-channel-contact-stat-hk-tlm */
void Test_BPNode_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC;
    size_t            Size = sizeof(BPNode_SendChannelContactStatHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;
    UT_CheckEvent_t   EventTest;

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID,
        "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPA_DP_SendChannelContactStatHkCmd, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}


/* Test ground command processing after receiving an invalid command code */
void Test_BPNode_ProcessGroundCommand_InvalidCmdCode(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_MSG_FcnCode_t FcnCode = 1000;   /* Invalid CC */
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CC_ERR_EID, NULL);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

/* Test command length verification in nominal case */
void Test_BPNode_VerifyCmdLength_Nominal(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = ActualLength;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);

    BPNode_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 0);
}

/* Test command length verification for an invalid length */
void Test_BPNode_VerifyCmdLength_InvalidLength(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = 2;
    CFE_SB_MsgId_t    MsgId          = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode        = BPNODE_NOOP_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPNode_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test noop command in nominal case */
void TEST_BPA_DP_NoopCmd_Nominal(void)
{
    BPA_DP_NoopCmd();

    UtAssert_STUB_COUNT(BPLib_NC_Noop, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_NoopCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_Noop), BPLIB_UNIMPLEMENTED);

    BPA_DP_NoopCmd();

    UtAssert_STUB_COUNT(BPLib_NC_Noop, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test Add All Applications command nominal case */
void TEST_BPA_DP_AddAllApplicationsCmd_Nominal(void)
{
    BPA_DP_AddAllApplicationsCmd();

    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplications, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddAllApplicationsCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAllApplications), BPLIB_UNIMPLEMENTED);

    BPA_DP_AddAllApplicationsCmd();

    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplications, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Start All Applications command nominal case */
void TEST_BPA_DP_StartAllApplicationsCmd_Nominal(void)
{
    BPA_DP_StartAllApplicationsCmd();

    UtAssert_STUB_COUNT(BPLib_NC_StartAllApplications, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_StartAllApplicationsCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_StartAllApplications), BPLIB_UNIMPLEMENTED);

    BPA_DP_StartAllApplicationsCmd();

    UtAssert_STUB_COUNT(BPLib_NC_StartAllApplications, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Verify Bundle Storage command nominal case */
void TEST_BPA_DP_VerifyBundleStorageCmd_Nominal(void)
{
    BPA_DP_VerifyBundleStorageCmd();

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleStorage, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_VerifyBundleStorageCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_VerifyBundleStorage), BPLIB_UNIMPLEMENTED);

    BPA_DP_VerifyBundleStorageCmd();

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleStorage, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Initialize Bundle Storage command nominal case */
void TEST_BPA_DP_InitBundleStorageCmd_Nominal(void)
{
    BPA_DP_InitBundleStorageCmd();

    UtAssert_STUB_COUNT(BPLib_NC_InitBundleStorage, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_InitBundleStorageCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_InitBundleStorage), BPLIB_UNIMPLEMENTED);

    BPA_DP_InitBundleStorageCmd();

    UtAssert_STUB_COUNT(BPLib_NC_InitBundleStorage, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Verify Bundle Metadata (7.2) command nominal case */
void TEST_BPA_DP_VerifyBundleMetadataCmd_Nominal(void)
{
    BPA_DP_VerifyBundleMetadataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleMetadata, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_VerifyBundleMetadataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_VerifyBundleMetadata), BPLIB_UNIMPLEMENTED);

    BPA_DP_VerifyBundleMetadataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleMetadata, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Rebuild Bundle Metadata command nominal case */
void TEST_BPA_DP_RebuildBundleMetadataCmd_Nominal(void)
{
    BPA_DP_RebuildBundleMetadataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_RebuildBundleMetadata, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RebuildBundleMetadataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RebuildBundleMetadata), BPLIB_UNIMPLEMENTED);

    BPA_DP_RebuildBundleMetadataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_RebuildBundleMetadata, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Clear Volatile command nominal case */
void TEST_BPA_DP_ClearVolatileCmd_Nominal(void)
{
    BPA_DP_ClearVolatileCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ClearVolatile, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ClearVolatileCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ClearVolatile), BPLIB_UNIMPLEMENTED);

    BPA_DP_ClearVolatileCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ClearVolatile, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Reload Saved Data (7.2) command nominal case */
void TEST_BPA_DP_ReloadSavedDataCmd_Nominal(void)
{
    BPA_DP_ReloadSavedDataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedData, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ReloadSavedDataCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ReloadSavedData), BPLIB_UNIMPLEMENTED);

    BPA_DP_ReloadSavedDataCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedData, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Reset All Counters command nominal case */
void TEST_BPA_DP_ResetAllCountersCmd_Nominal(void)
{
    BPA_DP_ResetAllCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountDelivered, 0);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived, 0);
}

void TEST_BPA_DP_ResetAllCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetAllCounters), BPLIB_UNIMPLEMENTED);
    
    BPA_DP_ResetAllCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCounters, 1);
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

    BPA_DP_ResetCounterCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ResetCounter, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ResetCounterCmd_Error(void)
{
    BPNode_ResetCounterCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetCounter), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 2;

    BPA_DP_ResetCounterCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ResetCounter, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Reset Source Counters command nominal case */
void TEST_BPA_DP_ResetSourceCountersCmd_Nominal(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 42;

    BPA_DP_ResetSourceCountersCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ResetSourceCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ResetSourceCountersCmd_Error(void)
{
    BPNode_ResetSourceCountersCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetSourceCounters), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 42;

    BPA_DP_ResetSourceCountersCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ResetSourceCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Reset Bundle Counters command nominal case */
void TEST_BPA_DP_ResetBundleCountersCmd_Nominal(void)
{
    BPA_DP_ResetBundleCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetBundleCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ResetBundleCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetBundleCounters), BPLIB_UNIMPLEMENTED);

    BPA_DP_ResetBundleCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetBundleCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Reset Error Counters command nominal case */
void TEST_BPA_DP_ResetErrorCountersCmd_Nominal(void)
{
    BPA_DP_ResetErrorCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ResetErrorCountersCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ResetErrorCounters), BPLIB_UNIMPLEMENTED);

    BPA_DP_ResetErrorCountersCmd();

    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCounters, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test add-application command nominal case */
void TEST_BPA_DP_AddApplicationCmd_Nominal(void)
{
    BPNode_AddApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    BPA_DP_AddApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test add-application command failure case */
void TEST_BPA_DP_AddApplicationCmd_Error(void)
{
    BPNode_AddApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Return invalid channel ID error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddApplication), BPLIB_ERROR);

    BPA_DP_AddApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);

}

/* Test Remove Application command nominal case */
void TEST_BPA_DP_RemoveApplicationCmd_Nominal(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 100;

    BPA_DP_RemoveApplicationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveApplication, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveApplicationCmd_Error(void)
{
    BPNode_RemoveApplicationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveApplication), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 100;

    BPA_DP_RemoveApplicationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveApplication, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Set Registration State command nominal case */
void TEST_BPA_DP_SetRegistrationStateCmd_Nominal(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 19;

    BPA_DP_SetRegistrationStateCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationState, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SetRegistrationStateCmd_Error(void)
{
    BPNode_SetRegistrationStateCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SetRegistrationState), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 19;

    BPA_DP_SetRegistrationStateCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationState, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test start-application command nominal case */
void TEST_BPA_DP_StartApplicationCmd_Nominal(void)
{
    BPNode_StartApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    BPA_DP_StartApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test start-application command failure case */
void TEST_BPA_DP_StartApplicationCmd_Error(void)
{
    BPNode_StartApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Return invalid channel ID error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_StartApplication), BPLIB_ERROR);

    BPA_DP_StartApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test stop-application command nominal case */
void TEST_BPA_DP_StopApplicationCmd_Nominal(void)
{
    BPNode_StopApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    BPA_DP_StopApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

/* Test stop-application command failure case */
void TEST_BPA_DP_StopApplicationCmd_Error(void)
{
    BPNode_StopApplicationCmd_t TestMsg;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Return generic, unhandled error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_StopApplication), BPLIB_ERROR);

    BPA_DP_StopApplicationCmd(&TestMsg);

    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Authorized Sources command nominal case */
void TEST_BPA_DP_AddAuthSourcesCmd_Nominal(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 4;

    BPA_DP_AddAuthSourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthSources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddAuthSourcesCmd_Error(void)
{
    BPNode_AddAuthSourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthSources), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 4;

    BPA_DP_AddAuthSourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthSources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Authorized Sources command nominal case */
void TEST_BPA_DP_RemoveAuthSourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 5;

    BPA_DP_RemoveAuthSourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthSources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveAuthSourcesCmd_Error(void)
{
    BPNode_RemoveAuthSourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthSources), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 5;

    BPA_DP_RemoveAuthSourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthSources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Authorized Custody Sources command nominal case */
void TEST_BPA_DP_AddAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 6;

    BPA_DP_AddAuthCustodySourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodySources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddAuthCustodySourcesCmd_Error(void)
{
    BPNode_AddAuthCustodySourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthCustodySources), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 6;

    BPA_DP_AddAuthCustodySourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodySources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Authorized Custody Sources command nominal case */
void TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 7;

    BPA_DP_RemoveAuthCustodySourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodySources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveAuthCustodySourcesCmd_Error(void)
{
    BPNode_RemoveAuthCustodySourcesCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthCustodySources), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 7;

    BPA_DP_RemoveAuthCustodySourcesCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodySources, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Authorized Custodians command nominal case */
void TEST_BPA_DP_AddAuthCustodiansCmd_Nominal(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 8;

    BPA_DP_AddAuthCustodiansCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodians, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddAuthCustodiansCmd_Error(void)
{
    BPNode_AddAuthCustodiansCmd_t TestMsg;
    
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthCustodians), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 8;

    BPA_DP_AddAuthCustodiansCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodians, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Authorized Custodians command nominal case */
void TEST_BPA_DP_RemoveAuthCustodiansCmd_Nominal(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 9;

    BPA_DP_RemoveAuthCustodiansCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodians, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveAuthCustodiansCmd_Error(void)
{
    BPNode_RemoveAuthCustodiansCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthCustodians), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 9;

    BPA_DP_RemoveAuthCustodiansCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodians, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Authorized Report-to EID command nominal case */
void TEST_BPA_DP_AddAuthReportToEidCmd_Nominal(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 10;

    BPA_DP_AddAuthReportToEidCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthReportToEid, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddAuthReportToEidCmd_Error(void)
{
    BPNode_AddAuthReportToEidCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddAuthReportToEid), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 10;

    BPA_DP_AddAuthReportToEidCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthReportToEid, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Authorized Report-to EID command nominal case */
void TEST_BPA_DP_RemoveAuthReportToEidCmd_Nominal(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 11;

    BPA_DP_RemoveAuthReportToEidCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthReportToEid, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveAuthReportToEidCmd_Error(void)
{
    BPNode_RemoveAuthReportToEidCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveAuthReportToEid), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 11;

    BPA_DP_RemoveAuthReportToEidCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthReportToEid, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Latency command nominal case */
void TEST_BPA_DP_AddLatencyCmd_Nominal(void)
{
    BPNode_AddLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 12;

    BPA_DP_AddLatencyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddLatency, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddLatencyCmd_Error(void)
{
    BPNode_AddLatencyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddLatency), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 12;

    BPA_DP_AddLatencyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddLatency, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Latency command nominal case */
void TEST_BPA_DP_RemoveLatencyCmd_Nominal(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 13;

    BPA_DP_RemoveLatencyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveLatency, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveLatencyCmd_Error(void)
{
    BPNode_RemoveLatencyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveLatency), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 13;

    BPA_DP_RemoveLatencyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveLatency, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Contact Setup command nominal case */
void TEST_BPA_DP_ContactSetupCmd_Nominal(void)
{
    BPNode_ContactSetupCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 14;

    BPA_DP_ContactSetupCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactSetup, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ContactSetupCmd_Error(void)
{
    BPNode_ContactSetupCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactSetup), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 14;

    BPA_DP_ContactSetupCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactSetup, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Contact Start command nominal case */
void TEST_BPA_DP_ContactStartCmd_Nominal(void)
{
    BPNode_ContactStartCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 15;

    BPA_DP_ContactStartCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStart, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ContactStartCmd_Error(void)
{
    BPNode_ContactStartCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactStart), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 15;

    BPA_DP_ContactStartCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStart, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Contact Stop command nominal case */
void TEST_BPA_DP_ContactStopCmd_Nominal(void)
{
    BPNode_ContactStopCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 16;

    BPA_DP_ContactStopCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStop, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ContactStopCmd_Error(void)
{
    BPNode_ContactStopCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactStop), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 16;

    BPA_DP_ContactStopCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStop, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Contact Teardown command nominal case */
void TEST_BPA_DP_ContactTeardownCmd_Nominal(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;

    TestMsg.Payload.ExampleParameter = 17;

    BPA_DP_ContactTeardownCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactTeardown, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_ContactTeardownCmd_Error(void)
{
    BPNode_ContactTeardownCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_ContactTeardown), BPLIB_UNIMPLEMENTED);

    TestMsg.Payload.ExampleParameter = 17;

    BPA_DP_ContactTeardownCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_ContactTeardown, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add MIB Array Key command nominal case */
void TEST_BPA_DP_AddMibArrayKeyCmd_Nominal(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 18;

    BPA_DP_AddMibArrayKeyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddMibArrayKey, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddMibArrayKeyCmd_Error(void)
{
    BPNode_AddMibArrayKeyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddMibArrayKey), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 18;

    BPA_DP_AddMibArrayKeyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddMibArrayKey, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove MIB Array Key command nominal case */
void TEST_BPA_DP_RemoveMibArrayKeyCmd_Nominal(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 19;

    BPA_DP_RemoveMibArrayKeyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveMibArrayKey, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveMibArrayKeyCmd_Error(void)
{
    BPNode_RemoveMibArrayKeyCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveMibArrayKey), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 19;

    BPA_DP_RemoveMibArrayKeyCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveMibArrayKey, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Set MIB Item command nominal case */
void TEST_BPA_DP_SetMibItemCmd_Nominal(void)
{
    BPNode_SetMibItemCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 20;

    BPA_DP_SetMibItemCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_SetMibItem, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SetMibItemCmd_Error(void)
{
    BPNode_SetMibItemCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SetMibItem), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 20;

    BPA_DP_SetMibItemCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_SetMibItem, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Add Storage Allocation command nominal case */
void TEST_BPA_DP_AddStorageAllocationCmd_Nominal(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 21;

    BPA_DP_AddStorageAllocationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddStorageAllocation, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_AddStorageAllocationCmd_Error(void)
{
    BPNode_AddStorageAllocationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_AddStorageAllocation), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 21;

    BPA_DP_AddStorageAllocationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_AddStorageAllocation, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Remove Storage Allocation command nominal case */
void TEST_BPA_DP_RemoveStorageAllocationCmd_Nominal(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;
    
    TestMsg.Payload.ExampleParameter = 22;

    BPA_DP_RemoveStorageAllocationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveStorageAllocation, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_RemoveStorageAllocationCmd_Error(void)
{
    BPNode_RemoveStorageAllocationCmd_t TestMsg;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_RemoveStorageAllocation), BPLIB_UNIMPLEMENTED);
    
    TestMsg.Payload.ExampleParameter = 22;

    BPA_DP_RemoveStorageAllocationCmd(&TestMsg);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveStorageAllocation, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Perform Self Test (7.2) command nominal case */
void TEST_BPA_DP_PerformSelfTestCmd_Nominal(void)
{
    BPA_DP_PerformSelfTestCmd();

    UtAssert_STUB_COUNT(BPLib_NC_PerformSelfTest, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_PerformSelfTestCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_PerformSelfTest), BPLIB_UNIMPLEMENTED);

    BPA_DP_PerformSelfTestCmd();

    UtAssert_STUB_COUNT(BPLib_NC_PerformSelfTest, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Send Node MIB Config HK command nominal case */
void TEST_BPA_DP_SendNodeMibConfigHkCmd_Nominal(void)
{
    BPA_DP_SendNodeMibConfigHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendNodeMibConfigHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendNodeMibConfigHk), BPLIB_UNIMPLEMENTED);

    BPA_DP_SendNodeMibConfigHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Send Per-Source MIB Config HK command nominal case */
void TEST_BPA_DP_SendSourceMibConfigHkCmd_Nominal(void)
{
    BPA_DP_SendSourceMibConfigHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendSourceMibConfigHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibConfigHk), BPLIB_UNIMPLEMENTED);
    
    BPA_DP_SendSourceMibConfigHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test node MIB counter housekeeping packet transmission in nominal case */
void TEST_BPA_DP_SendNodeMibCountersHkCmd_Nominal(void)
{
    BPA_DP_SendNodeMibCountersHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHk, 1);
}

void TEST_BPA_DP_SendNodeMibCountersHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendNodeMibCountersHk), BPLIB_UNIMPLEMENTED);

    BPA_DP_SendNodeMibCountersHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHk, 1);
}

/* Test Send Per-Source MIB Counter HK command nominal case */
void TEST_BPA_DP_SendSourceMibCountersHkCmd_Nominal(void)
{
    BPA_DP_SendSourceMibCountersHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendSourceMibCountersHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibCountersHk), BPLIB_UNIMPLEMENTED);

    BPA_DP_SendSourceMibCountersHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Send Storage HK command nominal case */
void TEST_BPA_DP_SendStorageHkCmd_Nominal(void)
{
    BPA_DP_SendStorageHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount, 1);
}

void TEST_BPA_DP_SendStorageHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendStorageHk), BPLIB_UNIMPLEMENTED);

    BPA_DP_SendStorageHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHk, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Test Send Channel/Contact Status HK command nominal case */
void TEST_BPA_DP_SendChannelContactStatHkCmd_Nominal(void)
{
    BPA_DP_SendChannelContactStatHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHk, 1);
}

void TEST_BPA_DP_SendChannelContactStatHkCmd_Error(void)
{
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendChannelContactStatHk), BPLIB_UNIMPLEMENTED);

    BPA_DP_SendChannelContactStatHkCmd();

    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHk, 1);
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

    ADD_TEST(Test_BPNode_TaskPipe_Nominal);
    ADD_TEST(Test_BPNode_TaskPipe_InvalidMsgId);

    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidNoop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidNoop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddAllApplications);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddAllApplications);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidStartAllApplications);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidStartAllApplications);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidVerifyBundleStorage);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleStorage);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidInitBundleStorage);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidInitBundleStorage);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidVerifyBundleMetadata);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleMetadata);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRebuildBundleMetadata);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRebuildBundleMetadata);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidClearVolatile);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidClearVolatile);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidReloadSavedData);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidReloadSavedData);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetAllCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetAllCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetCounter);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetCounter);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetSourceCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetSourceCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetBundleCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetBundleCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidResetErrorCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidResetErrorCounters);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSetRegistrationState);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSetRegistrationState);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidStartApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidStartApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidStopApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidStopApplication);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddAuthSources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddAuthSources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveAuthSources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthSources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddAuthCustodySources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodySources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveAuthCustodySources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodySources);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddAuthCustodians);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodians);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveAuthCustodians);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodians);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddAuthReportToEid);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddAuthReportToEid);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveAuthReportToEid);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthReportToEid);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddLatency);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddLatency);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveLatency);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveLatency);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidContactSetup);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidContactSetup);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidContactStart);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidContactStart);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidContactStop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidContactStop);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidContactTeardown);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidContactTeardown);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddMibArrayKey);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddMibArrayKey);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveMibArrayKey);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveMibArrayKey);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSetMibItem);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSetMibItem);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidAddStorageAllocation);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidAddStorageAllocation);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidRemoveStorageAllocation);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidRemoveStorageAllocation);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidPerformSelfTest);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidPerformSelfTest);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendNodeMibConfigHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibConfigHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendSourceMibConfigHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibConfigHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendNodeMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendSourceMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibCountersHk);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendStorageHkTlm);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendStorageHkTlm);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_ValidSendChannelContacStatHkTlm);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm);
    ADD_TEST(Test_BPNode_ProcessGroundCommand_InvalidCmdCode);

    ADD_TEST(Test_BPNode_VerifyCmdLength_Nominal);
    ADD_TEST(Test_BPNode_VerifyCmdLength_InvalidLength);
}
