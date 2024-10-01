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

/*
** \file
**   This file contains the source code for the Telemetry Proxy unit test
*/

/*
** File: fwp_tlmp_tests.c
**
** Purpose:
** Coverage Unit Test cases for the Framework Proxy
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode's Framework Proxy
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */
#include "fwp_tlmp.h"
#include "bpnode_test_utils.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/
void Test_BPA_TLMP_Init_Nominal(void)
{
    UtAssert_INT32_EQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendNodeMibConfigPkt_Nominal(void)
{
    BPLib_NodeMibConfigHkTlm_Payload_t NodeMIBConfigTlmPayload;
    memset(&NodeMIBConfigTlmPayload, 0, sizeof(BPLib_NodeMibConfigHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendNodeMibConfigPkt(&NodeMIBConfigTlmPayload), (int32) CFE_SUCCESS);
}

void Test_BPA_TLMP_SendPerSourceMibConfigPkt_Nominal(void)
{
    BPLib_SourceMibConfigHkTlm_Payload_t SrcMIBConfigTlmPayload;
    memset(&SrcMIBConfigTlmPayload, 0, sizeof(BPLib_SourceMibConfigHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendPerSourceMibConfigPkt(&SrcMIBConfigTlmPayload), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendNodeMibCounterPkt_Nominal(void)
{
    BPLib_NodeMibCountersHkTlm_Payload_t NodeMIBCounterTlmPayload;
    memset(&NodeMIBCounterTlmPayload, 0, sizeof(BPLib_NodeMibCountersHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendNodeMibCounterPkt(&NodeMIBCounterTlmPayload), (int32) CFE_SUCCESS);       
}

void Test_BPA_TLMP_SendPerSourceMibCounterPkt_Nominal(void)
{
    BPLib_SourceMibCountersHkTlm_Payload_t SrcMIBCounterTlmPayload;
    memset(&SrcMIBCounterTlmPayload, 0, sizeof(BPLib_SourceMibCountersHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendPerSourceMibCounterPkt(&SrcMIBCounterTlmPayload), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendChannelContactPkt_Nominal(void)
{
    BPLib_ChannelContactStatHkTlm_Payload_t ChannelContactTlmPayload;
    memset(&ChannelContactTlmPayload, 0, sizeof(BPLib_ChannelContactStatHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendChannelContactPkt(&ChannelContactTlmPayload), (int32) CFE_SUCCESS);        
}

void Test_BPA_TLMP_SendStoragePkt_Nominal(void)
{
    BPLib_StorageHkTlm_Payload_t StorTlmPayload;
    memset(&StorTlmPayload, 0, sizeof(BPLib_StorageHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendStoragePkt(&StorTlmPayload), (int32) CFE_SUCCESS);            
}

void Test_BPA_TLMP_Init_CreatePipeFail(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_CreatePipe), 1, CFE_SB_PIPE_CR_ERR);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail1(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 1, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail2(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail3(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 3, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail4(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 4, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail5(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 5, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_Init_SubscribeFail6(void)
{
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 6, CFE_SB_NO_MESSAGE);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_Init(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendNodeMibConfigPkt_TransmitFail(void)
{
    BPLib_NodeMibConfigHkTlm_Payload_t NodeMIBConfigTlmPayload;
    memset(&NodeMIBConfigTlmPayload, 0, sizeof(BPLib_NodeMibConfigHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendNodeMibConfigPkt(&NodeMIBConfigTlmPayload), (int32) CFE_SUCCESS);
}

void Test_BPA_TLMP_SendPerSourceMibConfigPkt_TransmitFail(void)
{
    BPLib_SourceMibConfigHkTlm_Payload_t SrcMIBConfigTlmPayload;
    memset(&SrcMIBConfigTlmPayload, 0, sizeof(BPLib_SourceMibConfigHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendPerSourceMibConfigPkt(&SrcMIBConfigTlmPayload), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendNodeMibCounterPkt_TransmitFail(void)
{
    BPLib_NodeMibCountersHkTlm_Payload_t NodeMIBCounterTlmPayload;
    memset(&NodeMIBCounterTlmPayload, 0, sizeof(BPLib_NodeMibCountersHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendNodeMibCounterPkt(&NodeMIBCounterTlmPayload), (int32) CFE_SUCCESS);       
}

void Test_BPA_TLMP_SendPerSourceMibCounterPkt_TransmitFail(void)
{
    BPLib_SourceMibCountersHkTlm_Payload_t SrcMIBCounterTlmPayload;
    memset(&SrcMIBCounterTlmPayload, 0, sizeof(BPLib_SourceMibCountersHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendPerSourceMibCounterPkt(&SrcMIBCounterTlmPayload), (int32) CFE_SUCCESS);    
}

void Test_BPA_TLMP_SendChannelContactPkt_TransmitFail(void)
{
    BPLib_ChannelContactStatHkTlm_Payload_t ChannelContactTlmPayload;
    memset(&ChannelContactTlmPayload, 0, sizeof(BPLib_ChannelContactStatHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendChannelContactPkt(&ChannelContactTlmPayload), (int32) CFE_SUCCESS);        
}

void Test_BPA_TLMP_SendStoragePkt_TransmitFail(void)
{
    BPLib_StorageHkTlm_Payload_t StorTlmPayload;
    memset(&StorTlmPayload, 0, sizeof(BPLib_StorageHkTlm_Payload_t));
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_TransmitMsg), 1, CFE_SB_BUFFER_INVALID);
    UtAssert_INT32_NEQ((int32) BPA_TLMP_SendStoragePkt(&StorTlmPayload), (int32) CFE_SUCCESS);            
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_TLMP_Init_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendNodeMibConfigPkt_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendPerSourceMibConfigPkt_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendNodeMibCounterPkt_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendPerSourceMibCounterPkt_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendChannelContactPkt_Nominal);
    ADD_TEST(Test_BPA_TLMP_SendStoragePkt_Nominal);    
    ADD_TEST(Test_BPA_TLMP_Init_CreatePipeFail);
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail1);  
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail2);  
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail3);  
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail4);  
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail5);  
    ADD_TEST(Test_BPA_TLMP_Init_SubscribeFail6);  
    ADD_TEST(Test_BPA_TLMP_SendNodeMibConfigPkt_TransmitFail);
    ADD_TEST(Test_BPA_TLMP_SendPerSourceMibConfigPkt_TransmitFail);
    ADD_TEST(Test_BPA_TLMP_SendNodeMibCounterPkt_TransmitFail);
    ADD_TEST(Test_BPA_TLMP_SendPerSourceMibCounterPkt_TransmitFail);
    ADD_TEST(Test_BPA_TLMP_SendChannelContactPkt_TransmitFail);
    ADD_TEST(Test_BPA_TLMP_SendStoragePkt_TransmitFail);
}