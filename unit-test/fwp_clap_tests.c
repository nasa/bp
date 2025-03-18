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
}

void Test_BPA_CLAP_ContactSetup_ClaInError(void)
{
}

/* Test that the ADU table validation fails when a SendToMsgId is not a valid message ID */
void Test_BPA_CLAP_ContactSetup_ClaOutError(void)
{
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_CLAP_ContactSetup_Nominal);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaInError);
    ADD_TEST(Test_BPA_CLAP_ContactSetup_ClaOutError);
}