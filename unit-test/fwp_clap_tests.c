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
 *  Unit tests for fwp_clap.c
 */

/* ======== */
/* Includes */
/* ======== */

#include "fwp_clap.h"
#include "bpnode_test_utils.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

void Test_BPA_CLAP_ContactSetup_Nominal(void)
{
    BPLib_Status_t Status;
    BPLib_CLA_ContactsSet_t ContactInfo;

    memset((void*) &ContactInfo, 0, sizeof(BPLib_CLA_ContactsSet_t));
    strcpy(ContactInfo.ClaInAddr, "0.0.0.0");
    strcpy(ContactInfo.ClaOutAddr, "0.0.0.0");

    /* Force called functions to return success */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_SUCCESS);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, ContactInfo);

    /* Verify that the function ran successfully */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);

    /* Verify that the call functions were called the correct amount */
    UtAssert_STUB_COUNT(BPNode_ClaIn_Setup, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Setup, 1);
}

void Test_BPA_CLAP_ContactSetup_ClaInError(void)
{
    BPLib_Status_t Status;
    BPLib_CLA_ContactsSet_t ContactInfo;

    memset((void*) &ContactInfo, 0, sizeof(BPLib_CLA_ContactsSet_t));
    strcpy(ContactInfo.ClaInAddr, "0.0.0.0");
    strcpy(ContactInfo.ClaOutAddr, "0.0.0.0");

    /* Force called functions to cause BPA_CLAP_ContactSetup to return a failure */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_CLA_IO_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_SUCCESS);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, ContactInfo);

    /* Verify that the function ran successfully */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_CLA_IO_ERROR);

    /* Verify that the call functions were called the correct amount */
    UtAssert_STUB_COUNT(BPNode_ClaIn_Setup, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Setup, 0);
}

/* Test that the ADU table validation fails when a SendToMsgId is not a valid message ID */
void Test_BPA_CLAP_ContactSetup_ClaOutError(void)
{
    BPLib_Status_t Status;
    BPLib_CLA_ContactsSet_t ContactInfo;

    memset((void*) &ContactInfo, 0, sizeof(BPLib_CLA_ContactsSet_t));
    strcpy(ContactInfo.ClaInAddr, "0.0.0.0");
    strcpy(ContactInfo.ClaOutAddr, "0.0.0.0");

    /* Force called functions to cause BPA_CLAP_ContactSetup to return a failure */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_CLA_IO_ERROR);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, ContactInfo);

    /* Verify that the function ran successfully */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_CLA_IO_ERROR);

    /* Verify that the call functions were called the correct amount */
    UtAssert_STUB_COUNT(BPNode_ClaIn_Setup, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Setup, 1);
}

void Test_BPA_CLAP_ContactStart_Nominal(void)
{
    uint32 ContId = 0;

    UtAssert_INT32_EQ(BPA_CLAP_ContactStart(ContId), BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPNode_ClaIn_Start, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Start, 1);    
}

void Test_BPA_CLAP_ContactStop_Nominal(void)
{
    uint32 ContId = 0;

    UtAssert_INT32_EQ(BPA_CLAP_ContactStop(ContId), BPLIB_SUCCESS);
    UtAssert_STUB_COUNT(BPNode_ClaIn_Stop, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Stop, 1);    
}

void Test_BPA_CLAP_ContactTeardown_Nominal(void)
{
    uint32 ContId = 0;

    UtAssert_VOIDCALL(BPA_CLAP_ContactTeardown(ContId));
    UtAssert_STUB_COUNT(BPNode_ClaIn_Teardown, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Teardown, 1);    
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_CLAP_ContactSetup_Nominal);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaInError);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaOutError);

    ADD_TEST(Test_BPA_CLAP_ContactStart_Nominal);
    ADD_TEST(Test_BPA_CLAP_ContactStop_Nominal);
    ADD_TEST(Test_BPA_CLAP_ContactTeardown_Nominal);
}