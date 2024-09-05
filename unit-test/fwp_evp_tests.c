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
 *  Unit tests for fwp_evp.c
 */

/* ============= */
/* Include Files */
/* ============= */
#include "fwp_evp.h"
#include "bpnode_test_utils.h"

/* ================= */
/* Handler Functions */
/* ================= */
// void Test_CFE_EVS_Register_GetArgs_Handler(void* UserObj, UT_EntryKey_t FuncKey, 
//                                              const UT_StubContext_t* Context)

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Test a nominal call to BPA_EVP_Init() */
void Test_BPA_EVP_Init_Nominal(void)
{
    BPLib_Status_t Status;

    /* TODO: Create utility to check EVP_Init arguments */
    // UT_SetHandlerFunction(UT_KEY(CFE_EVS_Register), &Test_CFE_EVS_Register_GetArgs_Handler, ???);

    /* Call the function under test */
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_True(Status == BPLIB_SUCCESS);
}

void Test_BPA_EVP_Init_BadReturn(void)
{
    /* Set return code for CFE_EVS_Register to be non-success */

    /* TODO: Create utility to check EVP_Init arguments */
    // UT_SetHandlerFunction(UT_KEY(CFE_EVS_Register), &Test_CFE_EVS_Register_GetArgs_Handler, ???);

    /* TODO: Verify that the EVS function that is being proxied, was called */
}

void Test_BPA_EVP_SendEvent_Nominal(void)
{
    /* TODO: Add all possible nominal cases */
}

void Test_BPA_EVP_SendEvent_TruncatedString(void)
{
    /* TODO: - Create char array of size BPLIB_EM_MAX_MESSAGE_LENGTH + 5 */
    /*       - Loop over array assigning each element in array to A (set last character to H) */
    /*       - Pass character array to BPLib_EM_SendEvent as EventText argument */
}

void Test_BPA_EVP_SendEvent_LenError(void)
{
    /* TODO: Test if len < 2 */
    /* TODO: test if len > mission max len */
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_EVP_Init_Nominal);
    ADD_TEST(Test_BPA_EVP_Init_BadReturn);
    ADD_TEST(Test_BPA_EVP_SendEvent_Nominal);
    ADD_TEST(Test_BPA_EVP_SendEvent_TruncatedString);
    ADD_TEST(Test_BPA_EVP_SendEvent_LenError);
}
