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
    UtAssert_VOIDCALL(BPA_TLMP_Init());
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

void Test_BPA_TLMP_SendNodeMibReportsPkt_Nominal(void)
{
    BPLib_NodeMibReportsHkTlm_Payload_t Payload;
    memset(&Payload, 0, sizeof(BPLib_NodeMibReportsHkTlm_Payload_t));
    UtAssert_INT32_EQ((int32) BPA_TLMP_SendNodeMibReportsPkt(&Payload), (int32) CFE_SUCCESS);            
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
    ADD_TEST(Test_BPA_TLMP_SendNodeMibReportsPkt_Nominal);
}