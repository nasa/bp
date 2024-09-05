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
 *  Unit tests for bpnode_dispatch.c
 */

/*
** Include Files
*/

#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

/*
** Function Definitions
*/

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

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(BPNode_NoopCmd, 1);
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

/* Test ground command processing after receiving a valid noop */
void Test_BPNode_ProcessGroundCommand_ValidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_NoopCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid noop */
void Test_BPNode_ProcessGroundCommand_InvalidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_NoopCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddAllApplicationsCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-all-applications */
void Test_BPNode_ProcessGroundCommand_InvalidAddAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_AddAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddAllApplicationsCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_StartAllApplicationsCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid start-all-applications */
void Test_BPNode_ProcessGroundCommand_InvalidStartAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_StartAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_StartAllApplicationsCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_VerifyBundleStorageCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid verify-bundle-storage */
void Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_VerifyBundleStorageCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_InitBundleStorageCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid init-bundle-storage */
void Test_BPNode_ProcessGroundCommand_InvalidInitBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_INIT_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_InitBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_InitBundleStorageCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_VerifyBundleMetadataCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid verify-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_InvalidVerifyBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_VerifyBundleMetadataCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RebuildBundleMetadataCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid rebuild-bundle-metadata */
void Test_BPNode_ProcessGroundCommand_InvalidRebuildBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REBUILD_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_RebuildBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RebuildBundleMetadataCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ClearVolatileCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid clear-volatile */
void Test_BPNode_ProcessGroundCommand_InvalidClearVolatile(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CLEAR_VOLATILE_CC;
    size_t            Size = sizeof(BPNode_ClearVolatileCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ClearVolatileCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ReloadSavedDataCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reload-saved-data */
void Test_BPNode_ProcessGroundCommand_InvalidReloadSavedData(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RELOAD_SAVED_DATA_CC;
    size_t            Size = sizeof(BPNode_ReloadSavedDataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ReloadSavedDataCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ResetAllCountersCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-all-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetAllCountersCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ResetCounterCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-counter */
void Test_BPNode_ProcessGroundCommand_InvalidResetCounter(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_COUNTER_CC;
    size_t            Size = sizeof(BPNode_ResetCounterCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetCounterCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ResetSourceCountersCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-source-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetSourceCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_SOURCE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetSourceCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetSourceCountersCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ResetBundleCountersCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-bundle-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetBundleCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_BUNDLE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetBundleCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetBundleCountersCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ResetErrorCountersCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid reset-error-counters */
void Test_BPNode_ProcessGroundCommand_InvalidResetErrorCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ERROR_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetErrorCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ResetErrorCountersCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddApplicationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-application */
void Test_BPNode_ProcessGroundCommand_InvalidAddApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_AddApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddApplicationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveApplicationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-application */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_RemoveApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveApplicationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SetRegistrationStateCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid set-registration-state */
void Test_BPNode_ProcessGroundCommand_InvalidSetRegistrationState(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_REGISTRATION_STATE_CC;
    size_t            Size = sizeof(BPNode_SetRegistrationStateCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SetRegistrationStateCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_StartApplicationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid start-application */
void Test_BPNode_ProcessGroundCommand_InvalidStartApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StartApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_StartApplicationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_StopApplicationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid stop-application */
void Test_BPNode_ProcessGroundCommand_InvalidStopApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_STOP_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StopApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_StopApplicationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddAuthSourcesCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-sources */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddAuthSourcesCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveAuthSourcesCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-sources */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveAuthSourcesCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddAuthCustodySourcesCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddAuthCustodySourcesCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveAuthCustodySourcesCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-custody-sources */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveAuthCustodySourcesCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddAuthCustodiansCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-custodians */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddAuthCustodiansCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveAuthCustodiansCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-custodians */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveAuthCustodiansCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddAuthReportToEidCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_InvalidAddAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_AddAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddAuthReportToEidCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveAuthReportToEidCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-auth-report-to-eid */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveAuthReportToEidCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddLatencyCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-latency */
void Test_BPNode_ProcessGroundCommand_InvalidAddLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_LATENCY_CC;
    size_t            Size = sizeof(BPNode_AddLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddLatencyCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveLatencyCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-latency */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_LATENCY_CC;
    size_t            Size = sizeof(BPNode_RemoveLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveLatencyCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ContactSetupCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-setup */
void Test_BPNode_ProcessGroundCommand_InvalidContactSetup(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_SETUP_CC;
    size_t            Size = sizeof(BPNode_ContactSetupCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ContactSetupCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ContactStartCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-start */
void Test_BPNode_ProcessGroundCommand_InvalidContactStart(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_START_CC;
    size_t            Size = sizeof(BPNode_ContactStartCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ContactStartCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ContactStopCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-stop */
void Test_BPNode_ProcessGroundCommand_InvalidContactStop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_STOP_CC;
    size_t            Size = sizeof(BPNode_ContactStopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ContactStopCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_ContactTeardownCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid contact-teardown */
void Test_BPNode_ProcessGroundCommand_InvalidContactTeardown(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_TEARDOWN_CC;
    size_t            Size = sizeof(BPNode_ContactTeardownCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_ContactTeardownCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddMibArrayKeyCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-mib-array-key */
void Test_BPNode_ProcessGroundCommand_InvalidAddMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_AddMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddMibArrayKeyCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveMibArrayKeyCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-mib-array-key */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_RemoveMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveMibArrayKeyCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SetMibItemCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid set-mib-item */
void Test_BPNode_ProcessGroundCommand_InvalidSetMibItem(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_MIB_ITEM_CC;
    size_t            Size = sizeof(BPNode_SetMibItemCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SetMibItemCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_AddStorageAllocationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-storage-allocation */
void Test_BPNode_ProcessGroundCommand_InvalidAddStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_AddStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_AddStorageAllocationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_RemoveStorageAllocationCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid remove-storage-allocation */
void Test_BPNode_ProcessGroundCommand_InvalidRemoveStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_RemoveStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_RemoveStorageAllocationCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_PerformSelfTestCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid perform-self-test */
void Test_BPNode_ProcessGroundCommand_InvalidPerformSelfTest(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_PERFORM_SELF_TEST_CC;
    size_t            Size = sizeof(BPNode_PerformSelfTestCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_PerformSelfTestCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SendNodeMibConfigHkCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendNodeMibConfigHkCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SendSourceMibConfigHkCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-config-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendSourceMibConfigHkCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SendNodeMibCountersHkCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendNodeMibCountersHkCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
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

    UtAssert_STUB_COUNT(BPNode_SendSourceMibCountersHkCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-counters-hk */
void Test_BPNode_ProcessGroundCommand_InvalidSendSourceMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendSourceMibCountersHkCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
}

/* Test ground command processing after receiving a valid send-storage-hk-tlm */
void Test_BPNode_ProcessGroundCommand_ValidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_TLM_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkTlmCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendStorageHkTlmCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-storage-hk-tlm */
void Test_BPNode_ProcessGroundCommand_InvalidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_TLM_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkTlmCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendStorageHkTlmCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
}

/* Test ground command processing after receiving a valid send-channel-contact-stat-hk-tlm */
void Test_BPNode_ProcessGroundCommand_ValidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_TLM_CC;
    size_t            Size = sizeof(BPNode_SendChannelContacStatHkTlmCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendChannelContacStatHkTlmCmd, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-channel-contact-stat-hk-tlm */
void Test_BPNode_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_TLM_CC;
    size_t            Size = sizeof(BPNode_SendChannelContacStatHkTlmCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPNode_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPNode_SendChannelContacStatHkTlmCmd, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
}


/* Test ground command processing after receiving an invalid command code */
void Test_BPNode_ProcessGroundCommand_InvalidCmdCode(void)
{
    UT_CheckEvent_t   EventTest;
    CFE_MSG_FcnCode_t FcnCode = 1000;   /* Invalid CC */
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPNode_ProcessGroundCommand(&Buf);

    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CC_ERR_EID, NULL);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
}

/* Test command length verification in nominal case */
void Test_BPNode_VerifyCmdLength_Nominal(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = ActualLength;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);

    BPNode_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
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

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
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
