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
 *  Unit tests for fwp_dp.c
 */

/* ============= */
/* Include Files */
/* ============= */

#include "fwp_dp.h"
#include "bplib_nc_directives.h"
#include "bpnode_test_utils.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Test task pipe processing in nominal case */
void Test_BPA_DP_TaskPipe_Nominal(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            MsgSize = sizeof(BPNode_AddAllApplicationsCmd_t);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);

    BPA_DP_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplications, 1);
}

/* Test task pipe processing after receiving an invalid message ID */
void Test_BPA_DP_TaskPipe_InvalidMsgId(void)
{
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    TestMsgId = CFE_SB_INVALID_MSG_ID;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &TestMsgId, sizeof(TestMsgId), false);

    BPA_DP_TaskPipe(&Buf);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_MID_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid command packet,MID = 0x%x", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/**
 * Test ground command processing after receiving a valid noop.
 *
 * Tests for valid commands call command stubs in bpnode_cmd_stubs.c.
 * The command stubs do not generate events.
 * The stubs only increment the stub call count for the command.
 */
void Test_BPA_DP_ProcessGroundCommand_ValidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size    = sizeof(BPNode_NoopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}

/**
 * Test ground command processing after receiving an invalid noop.
 *
 * Tests for invalid commands use an invalid command length.
 * The check for an invalid command length is in bpnode_dispatch.c
 * and generates a BPNODE_CMD_LEN_ERR_EID event.
 */

void Test_BPA_DP_ProcessGroundCommand_InvalidNoop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_NOOP_CC;
    size_t            Size = sizeof(BPNode_NoopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test ground command processing after receiving a valid add-all-applications */
void Test_BPA_DP_ProcessGroundCommand_ValidAddAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_AddAllApplicationsCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplications, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid add-all-applications */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_AddAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAllApplications, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test ground command processing after receiving a valid start-all-applications */
void Test_BPA_DP_ProcessGroundCommand_ValidStartAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_StartAllApplicationsCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StartAllApplications, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid start-all-applications */
void Test_BPA_DP_ProcessGroundCommand_InvalidStartAllApplications(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_ALL_APPLICATIONS_CC;
    size_t            Size = sizeof(BPNode_StartAllApplicationsCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StartAllApplications, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test ground command processing after receiving a valid verify-bundle-storage */
void Test_BPA_DP_ProcessGroundCommand_ValidVerifyBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleStorageCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleStorage, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid verify-bundle-storage */
void Test_BPA_DP_ProcessGroundCommand_InvalidVerifyBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleStorage, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid init-bundle-storage */
void Test_BPA_DP_ProcessGroundCommand_ValidInitBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_INIT_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_InitBundleStorageCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_InitBundleStorage, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid init-bundle-storage */
void Test_BPA_DP_ProcessGroundCommand_InvalidInitBundleStorage(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_INIT_BUNDLE_STORAGE_CC;
    size_t            Size = sizeof(BPNode_InitBundleStorageCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_InitBundleStorage, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid verify-bundle-metadata */
void Test_BPA_DP_ProcessGroundCommand_ValidVerifyBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleMetadataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleMetadata, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid verify-bundle-metadata */
void Test_BPA_DP_ProcessGroundCommand_InvalidVerifyBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_VERIFY_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_VerifyBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_VerifyBundleMetadata, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid rebuild-bundle-metadata */
void Test_BPA_DP_ProcessGroundCommand_ValidRebuildBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REBUILD_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_RebuildBundleMetadataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RebuildBundleMetadata, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid rebuild-bundle-metadata */
void Test_BPA_DP_ProcessGroundCommand_InvalidRebuildBundleMetadata(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REBUILD_BUNDLE_METADATA_CC;
    size_t            Size = sizeof(BPNode_RebuildBundleMetadataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;


    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RebuildBundleMetadata, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid clear-volatile */
void Test_BPA_DP_ProcessGroundCommand_ValidClearVolatile(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CLEAR_VOLATILE_CC;
    size_t            Size = sizeof(BPNode_ClearVolatileCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ClearVolatile, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid clear-volatile */
void Test_BPA_DP_ProcessGroundCommand_InvalidClearVolatile(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CLEAR_VOLATILE_CC;
    size_t            Size = sizeof(BPNode_ClearVolatileCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ClearVolatile, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reload-saved-data */
void Test_BPA_DP_ProcessGroundCommand_ValidReloadSavedData(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RELOAD_SAVED_DATA_CC;
    size_t            Size = sizeof(BPNode_ReloadSavedDataCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedData, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid reload-saved-data */
void Test_BPA_DP_ProcessGroundCommand_InvalidReloadSavedData(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RELOAD_SAVED_DATA_CC;
    size_t            Size = sizeof(BPNode_ReloadSavedDataCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ReloadSavedData, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reset-all-counters */
void Test_BPA_DP_ProcessGroundCommand_ValidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    /* BPA_DP_ProcessGroundCommand() calls BPLib_NC_ResetAllCounters() */
    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCounters, 1);

    /* BPLib_AS_Increment() shouldn't be called since directive success counter is being reset */
    UtAssert_STUB_COUNT(BPLib_AS_Increment, 0);
}

/* Test ground command processing after receiving an invalid reset-all-counters */
void Test_BPA_DP_ProcessGroundCommand_InvalidResetAllCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ALL_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetAllCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetAllCounters, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reset-counter */
void Test_BPA_DP_ProcessGroundCommand_ValidResetCounter(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_COUNTER_CC;
    size_t            Size = sizeof(BPNode_ResetCounterCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetCounter, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid reset-counter */
void Test_BPA_DP_ProcessGroundCommand_InvalidResetCounter(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_COUNTER_CC;
    size_t            Size = sizeof(BPNode_ResetCounterCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetCounter, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reset-source-counters */
void Test_BPA_DP_ProcessGroundCommand_ValidResetSourceCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_SOURCE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetSourceCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetSourceCounters, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid reset-source-counters */
void Test_BPA_DP_ProcessGroundCommand_InvalidResetSourceCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_SOURCE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetSourceCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetSourceCounters, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reset-bundle-counters */
void Test_BPA_DP_ProcessGroundCommand_ValidResetBundleCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_BUNDLE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetBundleCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetBundleCounters, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid reset-bundle-counters */
void Test_BPA_DP_ProcessGroundCommand_InvalidResetBundleCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_BUNDLE_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetBundleCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetBundleCounters, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid reset-error-counters */
void Test_BPA_DP_ProcessGroundCommand_ValidResetErrorCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ERROR_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetErrorCountersCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCounters, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid reset-error-counters */
void Test_BPA_DP_ProcessGroundCommand_InvalidResetErrorCounters(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_RESET_ERROR_COUNTERS_CC;
    size_t            Size = sizeof(BPNode_ResetErrorCountersCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ResetErrorCounters, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-application */
void Test_BPA_DP_ProcessGroundCommand_ValidAddApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_AddApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddApplication, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-application */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_AddApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddApplication, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-application */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_RemoveApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveApplication, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-application */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_RemoveApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveApplication, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid set-registration-state */
void Test_BPA_DP_ProcessGroundCommand_ValidSetRegistrationState(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_REGISTRATION_STATE_CC;
    size_t            Size = sizeof(BPNode_SetRegistrationStateCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationState, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid set-registration-state */
void Test_BPA_DP_ProcessGroundCommand_InvalidSetRegistrationState(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_REGISTRATION_STATE_CC;
    size_t            Size = sizeof(BPNode_SetRegistrationStateCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SetRegistrationState, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid start-application */
void Test_BPA_DP_ProcessGroundCommand_ValidStartApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StartApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StartApplication, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid start-application */
void Test_BPA_DP_ProcessGroundCommand_InvalidStartApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_START_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StartApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StartApplication, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid stop-application */
void Test_BPA_DP_ProcessGroundCommand_ValidStopApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_STOP_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StopApplicationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StopApplication, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid stop-application */
void Test_BPA_DP_ProcessGroundCommand_InvalidStopApplication(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_STOP_APPLICATION_CC;
    size_t            Size = sizeof(BPNode_StopApplicationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_StopApplication, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-auth-sources */
void Test_BPA_DP_ProcessGroundCommand_ValidAddAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthSourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthSources, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-auth-sources */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthSources, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-auth-sources */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthSourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthSources, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-auth-sources */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthSources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthSourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthSources, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-auth-custody-sources */
void Test_BPA_DP_ProcessGroundCommand_ValidAddAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodySourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodySources, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-auth-custody-sources */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodySources, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-auth-custody-sources */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodySourcesCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodySources, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-auth-custody-sources */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthCustodySources(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodySourcesCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodySources, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-auth-custodians */
void Test_BPA_DP_ProcessGroundCommand_ValidAddAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodiansCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodians, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-auth-custodians */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_AddAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthCustodians, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-auth-custodians */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodiansCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodians, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-auth-custodians */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthCustodians(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_CUSTODIANS_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthCustodiansCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthCustodians, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-auth-report-to-eid */
void Test_BPA_DP_ProcessGroundCommand_ValidAddAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_AddAuthReportToEidCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthReportToEid, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-auth-report-to-eid */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_AddAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddAuthReportToEid, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-auth-report-to-eid */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthReportToEidCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthReportToEid, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-auth-report-to-eid */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthReportToEid(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC;
    size_t            Size = sizeof(BPNode_RemoveAuthReportToEidCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveAuthReportToEid, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-latency */
void Test_BPA_DP_ProcessGroundCommand_ValidAddLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_LATENCY_CC;
    size_t            Size = sizeof(BPNode_AddLatencyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddLatency, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-latency */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_LATENCY_CC;
    size_t            Size = sizeof(BPNode_AddLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddLatency, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-latency */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_LATENCY_CC;
    size_t            Size = sizeof(BPNode_RemoveLatencyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveLatency, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-latency */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveLatency(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_LATENCY_CC;
    size_t            Size = sizeof(BPNode_RemoveLatencyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveLatency, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid contact-setup */
void Test_BPA_DP_ProcessGroundCommand_ValidContactSetup(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_SETUP_CC;
    size_t            Size = sizeof(BPNode_ContactSetupCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactSetup, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid contact-setup */
void Test_BPA_DP_ProcessGroundCommand_InvalidContactSetup(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_SETUP_CC;
    size_t            Size = sizeof(BPNode_ContactSetupCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactSetup, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid contact-start */
void Test_BPA_DP_ProcessGroundCommand_ValidContactStart(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_START_CC;
    size_t            Size = sizeof(BPNode_ContactStartCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStart, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid contact-start */
void Test_BPA_DP_ProcessGroundCommand_InvalidContactStart(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_START_CC;
    size_t            Size = sizeof(BPNode_ContactStartCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStart, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid contact-stop */
void Test_BPA_DP_ProcessGroundCommand_ValidContactStop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_STOP_CC;
    size_t            Size = sizeof(BPNode_ContactStopCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStop, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid contact-stop */
void Test_BPA_DP_ProcessGroundCommand_InvalidContactStop(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_STOP_CC;
    size_t            Size = sizeof(BPNode_ContactStopCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactStop, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid contact-teardown */
void Test_BPA_DP_ProcessGroundCommand_ValidContactTeardown(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_TEARDOWN_CC;
    size_t            Size = sizeof(BPNode_ContactTeardownCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactTeardown, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid contact-teardown */
void Test_BPA_DP_ProcessGroundCommand_InvalidContactTeardown(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_CONTACT_TEARDOWN_CC;
    size_t            Size = sizeof(BPNode_ContactTeardownCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_ContactTeardown, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-mib-array-key */
void Test_BPA_DP_ProcessGroundCommand_ValidAddMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_AddMibArrayKeyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddMibArrayKey, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-mib-array-key */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_AddMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddMibArrayKey, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-mib-array-key */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_RemoveMibArrayKeyCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveMibArrayKey, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-mib-array-key */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveMibArrayKey(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_MIB_ARRAY_KEY_CC;
    size_t            Size = sizeof(BPNode_RemoveMibArrayKeyCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveMibArrayKey, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid set-mib-item */
void Test_BPA_DP_ProcessGroundCommand_ValidSetMibItem(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_MIB_ITEM_CC;
    size_t            Size = sizeof(BPNode_SetMibItemCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SetMibItem, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid set-mib-item */
void Test_BPA_DP_ProcessGroundCommand_InvalidSetMibItem(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SET_MIB_ITEM_CC;
    size_t            Size = sizeof(BPNode_SetMibItemCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SetMibItem, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid add-storage-allocation */
void Test_BPA_DP_ProcessGroundCommand_ValidAddStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_AddStorageAllocationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddStorageAllocation, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid add-storage-allocation */
void Test_BPA_DP_ProcessGroundCommand_InvalidAddStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_ADD_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_AddStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_AddStorageAllocation, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid remove-storage-allocation */
void Test_BPA_DP_ProcessGroundCommand_ValidRemoveStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_RemoveStorageAllocationCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveStorageAllocation, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid remove-storage-allocation */
void Test_BPA_DP_ProcessGroundCommand_InvalidRemoveStorageAllocation(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_REMOVE_STORAGE_ALLOCATION_CC;
    size_t            Size = sizeof(BPNode_RemoveStorageAllocationCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_RemoveStorageAllocation, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid perform-self-test */
void Test_BPA_DP_ProcessGroundCommand_ValidPerformSelfTest(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_PERFORM_SELF_TEST_CC;
    size_t            Size = sizeof(BPNode_PerformSelfTestCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_PerformSelfTest, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
    }

/* Test ground command processing after receiving an invalid perform-self-test */
void Test_BPA_DP_ProcessGroundCommand_InvalidPerformSelfTest(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_PERFORM_SELF_TEST_CC;
    size_t            Size = sizeof(BPNode_PerformSelfTestCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_PerformSelfTest, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid send-node-mib-config-hk */
void Test_BPA_DP_ProcessGroundCommand_ValidSendNodeMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-config-hk */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

void Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibConfigHk_Error(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibConfigHkCmd_t);
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    /* BPLib_NC_SendNodeMibConfigHk() returns an error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendNodeMibConfigHk), BPLIB_ERROR);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibConfigHk, 1);
}

/* Test ground command processing after receiving a valid send-source-mib-config-hk */
void Test_BPA_DP_ProcessGroundCommand_ValidSendSourceMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-config-hk */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibConfigHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibConfigHk_Error(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibConfigHkCmd_t);
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    /* BPLib_NC_SendSourceMibConfigHk() returns an error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibConfigHk), BPLIB_ERROR);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibConfigHk, 1);
}

/* Test ground command processing after receiving a valid send-node-mib-counters-hk */
void Test_BPA_DP_ProcessGroundCommand_ValidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-counters-hk */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibCountersHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

/* Test ground command processing after receiving a valid send-source-mib-counters-hk */
void Test_BPA_DP_ProcessGroundCommand_ValidSendSourceMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-source-mib-counters-hk */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibCountersHk(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

void Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibCountersHk_Error(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC;
    size_t            Size = sizeof(BPNode_SendSourceMibCountersHkCmd_t);
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendSourceMibCountersHk), BPLIB_ERROR);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendSourceMibCountersHk, 1);
}

/* Test ground command processing after receiving a valid send-storage-hk-tlm */
void Test_BPA_DP_ProcessGroundCommand_ValidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-storage-hk-tlm */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendStorageHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
    }

void Test_BPA_DP_ProcessGroundCommand_InvalidSendStorageHkTlm_Error(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_STORAGE_HK_CC;
    size_t            Size = sizeof(BPNode_SendStorageHkCmd_t);
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    /* BPLib_NC_SendSourceMibCountersHk() returns an error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendStorageHk), BPLIB_ERROR);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendStorageHk, 1);
}

/* Test ground command processing after receiving a valid send-channel-contact-stat-hk-tlm */
void Test_BPA_DP_ProcessGroundCommand_ValidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC;
    size_t            Size = sizeof(BPNode_SendChannelContactStatHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-channel-contact-stat-hk-tlm */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC;
    size_t            Size = sizeof(BPNode_SendChannelContactStatHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_BPA_DP_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm_Error(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC;
    size_t            Size = sizeof(BPNode_SendChannelContactStatHkCmd_t);
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    /* BPLib_NC_SendChannelContactStatHk() returns an error code */
    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_SendChannelContactStatHk), BPLIB_ERROR);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendChannelContactStatHk, 1);
}

/* Test ground command processing after receiving a valid send-node-mib-reports command */
void Test_BPA_DP_ProcessGroundCommand_ValSendReports(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_REPORTS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibReportsHkCmd_t);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibReportsHk, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test ground command processing after receiving an invalid send-node-mib-reports */
void Test_BPA_DP_ProcessGroundCommand_InvalidSendReports(void)
{
    CFE_MSG_FcnCode_t FcnCode = BPNODE_SEND_NODE_MIB_REPORTS_HK_CC;
    size_t            Size = sizeof(BPNode_SendNodeMibReportsHkCmd_t) - 1; /* Invalid length */
    CFE_SB_MsgId_t    MsgId = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_NC_SendNodeMibReportsHk, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CMD_LEN_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}


/* Test ground command processing after receiving an invalid command code */
void Test_BPA_DP_ProcessGroundCommand_InvalidCode(void)
{
    CFE_MSG_FcnCode_t FcnCode = 1000;   /* Invalid CC */
    CFE_SB_Buffer_t   Buf;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPA_DP_ProcessGroundCommand(&Buf);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_CC_ERR_EID);
    UtAssert_STRINGBUF_EQ("Invalid ground command code: CC = %d", BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test command length verification in nominal case */
void Test_BPA_DP_VerifyCmdLength_Nominal(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = ActualLength;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);

    BPA_DP_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test command length verification for an invalid length */
void Test_BPA_DP_VerifyCmdLength_InvalidLength(void)
{
    size_t            ActualLength   = 1;
    size_t            ExpectedLength = 2;
    CFE_SB_MsgId_t    MsgId          = CFE_SB_ValueToMsgId(BPNODE_CMD_MID);
    CFE_MSG_FcnCode_t FcnCode        = BPNODE_NOOP_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ActualLength, sizeof(ActualLength), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);

    BPA_DP_VerifyCmdLength(NULL, ExpectedLength);

    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_DP_TaskPipe_Nominal);
    ADD_TEST(Test_BPA_DP_TaskPipe_InvalidMsgId);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidNoop);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidNoop);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddAllApplications);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddAllApplications);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidStartAllApplications);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidStartAllApplications);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidVerifyBundleStorage);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidVerifyBundleStorage);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidInitBundleStorage);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidInitBundleStorage);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidVerifyBundleMetadata);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidVerifyBundleMetadata);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRebuildBundleMetadata);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRebuildBundleMetadata);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidClearVolatile);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidClearVolatile);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidReloadSavedData);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidReloadSavedData);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidResetAllCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidResetAllCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidResetCounter);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidResetCounter);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidResetSourceCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidResetSourceCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidResetBundleCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidResetBundleCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidResetErrorCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidResetErrorCounters);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSetRegistrationState);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSetRegistrationState);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidStartApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidStartApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidStopApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidStopApplication);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddAuthSources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthSources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthSources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthSources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddAuthCustodySources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthCustodySources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthCustodySources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthCustodySources);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddAuthCustodians);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthCustodians);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthCustodians);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthCustodians);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddAuthReportToEid);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddAuthReportToEid);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveAuthReportToEid);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveAuthReportToEid);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddLatency);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddLatency);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveLatency);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveLatency);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidContactSetup);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidContactSetup);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidContactStart);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidContactStart);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidContactStop);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidContactStop);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidContactTeardown);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidContactTeardown);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddMibArrayKey);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddMibArrayKey);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveMibArrayKey);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveMibArrayKey);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSetMibItem);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSetMibItem);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidAddStorageAllocation);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidAddStorageAllocation);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidRemoveStorageAllocation);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidRemoveStorageAllocation);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidPerformSelfTest);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidPerformSelfTest);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendNodeMibConfigHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibConfigHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibConfigHk_Error);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendSourceMibConfigHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibConfigHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibConfigHk_Error);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendNodeMibCountersHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendNodeMibCountersHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendSourceMibCountersHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibCountersHk);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendSourceMibCountersHk_Error);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendStorageHkTlm);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendStorageHkTlm);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendStorageHkTlm_Error);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValidSendChannelContacStatHkTlm);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendChannelContacStatHkTlm_Error);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_ValSendReports);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidSendReports);
    ADD_TEST(Test_BPA_DP_ProcessGroundCommand_InvalidCode);
    ADD_TEST(Test_BPA_DP_VerifyCmdLength_Nominal);
    ADD_TEST(Test_BPA_DP_VerifyCmdLength_InvalidLength);
}
