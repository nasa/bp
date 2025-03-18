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

    /* Force called functions to return success */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_SUCCESS);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, "0.0.0.0", BPLIB_MAX_NUM_CONTACTS - 1);

    /* Verify that the function ran successfully */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);

    /* Verify that the call functions were called the correct amount */
    UtAssert_STUB_COUNT(BPNode_ClaIn_Setup, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Setup, 1);
}

void Test_BPA_CLAP_ContactSetup_ClaInError(void)
{
    BPLib_Status_t Status;

    /* Force called functions to cause BPA_CLAP_ContactSetup to return a failure */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_CLA_IO_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_SUCCESS);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, "0.0.0.0", BPLIB_MAX_NUM_CONTACTS - 1);

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

    /* Force called functions to cause BPA_CLAP_ContactSetup to return a failure */
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaIn_Setup), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPNode_ClaOut_Setup), BPLIB_CLA_INIT_SEM_ERROR);

    /* Run the function under test */
    Status = BPA_CLAP_ContactSetup(0, "0.0.0.0", BPLIB_MAX_NUM_CONTACTS - 1);

    /* Verify that the function ran successfully */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_CLA_INIT_SEM_ERROR);

    /* Verify that the call functions were called the correct amount */
    UtAssert_STUB_COUNT(BPNode_ClaIn_Setup, 1);
    UtAssert_STUB_COUNT(BPNode_ClaOut_Setup, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_CLAP_ContactSetup_Nominal);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaInError);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaOutError);
}