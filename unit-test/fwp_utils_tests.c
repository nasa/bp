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

/* ======== */
/* Includes */
/* ======== */

#include "bplib.h"
#include "fwp_utils.h"
#include "bpnode_test_utils.h"

/* ==================== */
/* Function Definitions */
/* ==================== */

void Test_BPA_BPLib_Success_Status_Translate(void)
{
    CFE_Status_t CFE_Status;

    /* Convert BPLib success-type return code into a cFE success-type return code */
    CFE_Status = BPA_BPLib_Status_Translate(BPLIB_SUCCESS);

    /* Verify MSB is 0 since success return statuses have MSB of 0 */
    UtAssert_EQ(uint16, CFE_Status, CFE_SUCCESS);
}

void Test_BPA_BPLib_Error_Status_Translate(void)
{
    CFE_Status_t CFE_Status;

    /* Convert BPLib error-type return code into a cFE error-type return code */
    CFE_Status = BPA_BPLib_Status_Translate(BPLIB_ERROR);

    UtAssert_BOOL_TRUE(CFE_Status & CFE_SEVERITY_ERROR);
}

void Test_BPA_CFE_Success_Status_Translate(void)
{
    BPLib_Status_t BPLib_Status;

    /* Convert cFE success-type return code into a BPLib success-type return code */
    BPLib_Status = BPA_CFE_Status_Translate(CFE_SUCCESS);

    /* Verify the return code is a success type, accounting for different types of success */
    UtAssert_GTEQ(BPLib_Status_t, BPLib_Status, BPLIB_SUCCESS);
}

void Test_BPA_CFE_Error_Status_Translate(void)
{
    BPLib_Status_t BPLib_Status;

    /* Convert cFE error-type return code into a BPLib error-type return code */
    BPLib_Status = BPA_CFE_Status_Translate(CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify the return code is an error type, accounting for different types of errors */
    UtAssert_LTEQ(BPLib_Status_t, BPLib_Status, BPLIB_ERROR);
}

void Test_BPA_CFE_TblUpdate_Status_Translate(void)
{
    BPLib_Status_t BPLib_Status;

    /* Convert cFE error-type return code into a BPLib table-update-type return code */
    BPLib_Status = BPA_CFE_Status_Translate(CFE_TBL_INFO_UPDATED);

    /* Verify the return code is a table update type */
    UtAssert_EQ(BPLib_Status_t, BPLib_Status, BPLIB_TBL_UPDATED);
}

void Test_BPA_CFE_Info_Status_Translate(void)
{
    BPLib_Status_t BPLib_Status;

    /* Convert cFE info-type return code into a BPLib error-type return code */
    BPLib_Status = BPA_CFE_Status_Translate(CFE_STATUS_NO_COUNTER_INCREMENT);

    /* Verify the return code is an error type, accounting for different types of errors */
    UtAssert_LTEQ(BPLib_Status_t, BPLib_Status, BPLIB_ERROR);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_BPLib_Success_Status_Translate);
    ADD_TEST(Test_BPA_BPLib_Error_Status_Translate);
    ADD_TEST(Test_BPA_CFE_Success_Status_Translate);
    ADD_TEST(Test_BPA_CFE_Error_Status_Translate);
    ADD_TEST(Test_BPA_CFE_TblUpdate_Status_Translate);
    ADD_TEST(Test_BPA_CFE_Info_Status_Translate);
}
