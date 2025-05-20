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
 *  Unit tests for fwp_adup.c
 */

/*
** Include Files
*/

#include "fwp_adup.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Test nominal table validation */
void Test_BPA_ADUP_ValidateConfigTbl_Nominal(void)
{
    BPA_ADUP_Table_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    TestTblData.Entries[0].NumRecvFrmMsgIds = 1;
        
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_IsValidMsgId), true);

    UtAssert_INT32_EQ((int32) BPA_ADUP_ValidateConfigTbl(&TestTblData), (int32) CFE_SUCCESS);     
}

/* Test that the ADU table validation fails when a NumRecvFrmMsgIds value is out of range */
void Test_BPA_ADUP_ValidateConfigTbl_InvNumRecv(void)
{
    BPA_ADUP_Table_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_IsValidMsgId), true);

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.Entries[0].NumRecvFrmMsgIds = BPNODE_MAX_CHAN_SUBSCRIPTION + 1;

    UtAssert_INT32_EQ(BPA_ADUP_ValidateConfigTbl(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

/* Test that the ADU table validation fails when a SendToMsgId is not a valid message ID */
void Test_BPA_ADUP_ValidateConfigTbl_InvSendTo(void)
{
    BPA_ADUP_Table_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));    

    /* Set the first check to fail */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_IsValidMsgId), 1, false);

    UtAssert_INT32_EQ(BPA_ADUP_ValidateConfigTbl(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

/* Test that the ADU table validation fails when a RecvFrmMsgIds is not a valid message ID */
void Test_BPA_ADUP_ValidateConfigTbl_InvRecvFrm(void)
{
    BPA_ADUP_Table_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    TestTblData.Entries[0].NumRecvFrmMsgIds = 1;

    /* Set the first check to succeed and second check to fail */
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_IsValidMsgId), 1, true);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_IsValidMsgId), 1, false);

    UtAssert_INT32_EQ(BPA_ADUP_ValidateConfigTbl(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

/* Test BPA_ADUP_In */
void Test_BPA_ADUP_In_Nominal(void)
{
    CFE_SB_Buffer_t Buf;
    uint8_t ChanId = 0;
    CFE_MSG_Size_t Size = 10;

    /* Set global data */
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = 100;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    UtAssert_INT32_EQ(BPA_ADUP_In(&Buf, ChanId), BPLIB_SUCCESS);
    
    UtAssert_STUB_COUNT(BPLib_PI_Ingress, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test BPA_ADUP_In when the payload is too big*/
void Test_BPA_ADUP_In_SizeErr(void)
{
    CFE_SB_Buffer_t Buf;
    uint8_t ChanId = 0;
    CFE_MSG_Size_t Size = 100;

    /* Set global data */
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = 10;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    UtAssert_INT32_EQ(BPA_ADUP_In(&Buf, ChanId), BPLIB_ERROR);

    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_IN_TOO_BIG_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU In #%d]: Received an ADU too big to ingest, Size=%ld, MaxBundlePayloadSize=%d", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);

    UtAssert_STUB_COUNT(BPLib_AS_Increment, 0);
    UtAssert_STUB_COUNT(BPLib_PI_Ingress, 0);
}

/* Test BPA_ADUP_In when ingress fails */
void Test_BPA_ADUP_In_IngressErr(void)
{
    CFE_SB_Buffer_t Buf;
    uint8_t ChanId = 0;
    CFE_MSG_Size_t Size = 10;

    /* Set global data */
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = 100;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);
    UT_SetDefaultReturnValue(UT_KEY(BPLib_PI_Ingress), BPLIB_NULL_PTR_ERROR);

    UtAssert_INT32_EQ(BPA_ADUP_In(&Buf, ChanId), BPLIB_NULL_PTR_ERROR);

    UtAssert_STUB_COUNT(BPLib_PI_Ingress, 1);
}

/* Test BPA_ADUP_Out in nominal case with no wrapping */
void Test_BPA_ADUP_Out_Nominal(void)
{
    uint8_t ChanId = 0;
    size_t SizeVal = BPNODE_ADU_OUT_MAX_ADU_OUT_BYTES;

    UT_SetDataBuffer(UT_KEY(BPLib_PI_Egress), &SizeVal, sizeof(SizeVal), false);

    UtAssert_INT32_EQ(BPA_ADUP_Out(ChanId, BPNODE_ADU_IN_PI_Q_TIMEOUT), BPLIB_SUCCESS);
    
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}


/* Test BPA_ADUP_Out when wrapping is true */
void Test_BPA_ADUP_Out_Wrapping(void)
{
    uint8_t ChanId = 0;
    size_t SizeVal = BPNODE_ADU_OUT_MAX_ADU_OUT_BYTES;

    UT_SetDataBuffer(UT_KEY(BPLib_PI_Egress), &SizeVal, sizeof(SizeVal), false);

    BPNode_AppData.AduOutData[ChanId].AduWrapping = true;

    UtAssert_INT32_EQ(BPA_ADUP_Out(ChanId, BPNODE_ADU_IN_PI_Q_TIMEOUT), BPLIB_SUCCESS);
    
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_STUB_COUNT(CFE_MSG_SetMsgId, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test BPA_ADUP_Out when egress fails */
void Test_BPA_ADUP_Out_EgressErr(void)
{
    uint8_t ChanId = 0;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PI_Egress), BPLIB_ERROR);

    UtAssert_INT32_EQ(BPA_ADUP_Out(ChanId, BPNODE_ADU_IN_PI_Q_TIMEOUT), BPLIB_ERROR);
    
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 0);
    UtAssert_STUB_COUNT(BPLib_AS_Increment, 0);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_OUT_PI_OUT_ERR_EID);
    UtAssert_STRINGBUF_EQ("[ADU Out #%d]: Failed to egress an ADU. Error = %d.", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test BPA_ADUP_Out when PI times out with nothing in the queue */
void Test_BPA_ADUP_Out_Timeout(void)
{
    uint8_t ChanId = 0;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PI_Egress), BPLIB_PI_TIMEOUT);

    UtAssert_INT32_EQ(BPA_ADUP_Out(ChanId, BPNODE_ADU_IN_PI_Q_TIMEOUT), BPLIB_PI_TIMEOUT);
    
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 0);
    UtAssert_STUB_COUNT(BPLib_AS_Increment, 0);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test BPA_ADUP_AddApplication */
void Test_BPA_ADUP_AddApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    /* Set test data */
    TestAduTbl.Entries[ChanId].NumRecvFrmMsgIds = 1;
    TestAduTbl.Entries[ChanId].RecvFrmMsgIds[0] = CFE_SB_ValueToMsgId(0x1801);
    TestAduTbl.Entries[ChanId].SendToMsgId = CFE_SB_ValueToMsgId(0x0800);

    TestChanTbl.Configs[ChanId].AddAutomatically = true;
    TestChanTbl.Configs[ChanId].AduUnwrapping = false;
    TestChanTbl.Configs[ChanId].AduWrapping = true;
    TestChanTbl.Configs[ChanId].MaxBundlePayloadSize = 1234;

    BPNode_AppData.AduProxyTablePtr                = &TestAduTbl;
    BPNode_AppData.ConfigPtrs.ChanConfigPtr = &TestChanTbl;

    UtAssert_INT32_EQ(BPA_ADUP_AddApplication(ChanId), BPLIB_SUCCESS);

    /* Verify ADU configs were set properly */
    UtAssert_INT32_EQ(CFE_SB_MsgIdToValue(BPNode_AppData.AduOutData[ChanId].SendToMsgId), 
                      CFE_SB_MsgIdToValue(TestAduTbl.Entries[ChanId].SendToMsgId));
    UtAssert_INT32_EQ(BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds, 
                            TestAduTbl.Entries[ChanId].NumRecvFrmMsgIds);       
    UtAssert_INT32_EQ(CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[0]), 
                      CFE_SB_MsgIdToValue(TestAduTbl.Entries[ChanId].RecvFrmMsgIds[0]));       
    UtAssert_INT32_EQ(BPNode_AppData.AduState[ChanId].AddAutomatically, 
                            TestChanTbl.Configs[ChanId].AddAutomatically);
    UtAssert_INT32_EQ(BPNode_AppData.AduInData[ChanId].AduUnwrapping, 
                            TestChanTbl.Configs[ChanId].AduUnwrapping);       
    UtAssert_INT32_EQ(BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize, 
                            TestChanTbl.Configs[ChanId].MaxBundlePayloadSize);       
    UtAssert_INT32_EQ(BPNode_AppData.AduOutData[ChanId].AduWrapping, 
                            TestChanTbl.Configs[ChanId].AduWrapping);       

}

/* Test BPA_ADUP_StartApplication in nominal case */
void Test_BPA_ADUP_StartApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = 1;

    UtAssert_INT32_EQ(BPA_ADUP_StartApplication(ChanId), BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);\
}

/* Test BPA_ADUP_StartApplication when subscribing fails */
void Test_BPA_ADUP_StartApplication_SubErr(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = 1;

    /* Set CFE_SB_Subscribe to fail */
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPA_ADUP_StartApplication(ChanId), BPLIB_ERROR);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_START_SUB_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error subscribing to ADU on channel #%d, Error = %d, MsgId = 0x%x", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test BPA_ADUP_StopApplication */
void Test_BPA_ADUP_StopApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = 1;

    UtAssert_INT32_EQ(BPA_ADUP_StopApplication(ChanId), BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(CFE_SB_Unsubscribe, 1);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Test BPA_ADUP_StopApplication when subscribing fails */
void Test_BPA_ADUP_StopApplication_SubErr(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = 1;

    /* Set CFE_SB_Unsubscribe to fail */
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Unsubscribe), CFE_SB_BAD_ARGUMENT);

    UtAssert_INT32_EQ(BPA_ADUP_StopApplication(ChanId), BPLIB_ERROR);
    UtAssert_STUB_COUNT(CFE_SB_Unsubscribe, 1);
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[0].EventID, BPNODE_ADU_STOP_UNSUB_ERR_EID);
    UtAssert_STRINGBUF_EQ("Error unsubscribing from ADU on channel #%d, Error = %d, MsgId = 0x%x", BPLIB_EM_EXPANDED_EVENT_SIZE, 
                            context_BPLib_EM_SendEvent[0].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

/* Test BPA_ADUP_RemoveApplication */
void Test_BPA_ADUP_RemoveApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    UtAssert_INT32_EQ(BPA_ADUP_RemoveApplication(ChanId), BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPLib_EM_SendEvent, 0);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_ADUP_ValidateConfigTbl_Nominal);
    ADD_TEST(Test_BPA_ADUP_ValidateConfigTbl_InvNumRecv);
    ADD_TEST(Test_BPA_ADUP_ValidateConfigTbl_InvRecvFrm);
    ADD_TEST(Test_BPA_ADUP_ValidateConfigTbl_InvSendTo);

    ADD_TEST(Test_BPA_ADUP_In_Nominal);
    ADD_TEST(Test_BPA_ADUP_In_SizeErr);
    ADD_TEST(Test_BPA_ADUP_In_IngressErr);

    ADD_TEST(Test_BPA_ADUP_Out_Nominal);
    ADD_TEST(Test_BPA_ADUP_Out_Wrapping);
    ADD_TEST(Test_BPA_ADUP_Out_EgressErr);
    ADD_TEST(Test_BPA_ADUP_Out_Timeout);

    ADD_TEST(Test_BPA_ADUP_AddApplication_Nominal);
    
    ADD_TEST(Test_BPA_ADUP_StartApplication_Nominal);
    ADD_TEST(Test_BPA_ADUP_StartApplication_SubErr);

    ADD_TEST(Test_BPA_ADUP_StopApplication_Nominal);
    ADD_TEST(Test_BPA_ADUP_StopApplication_SubErr);

    ADD_TEST(Test_BPA_ADUP_RemoveApplication_Nominal);
}
