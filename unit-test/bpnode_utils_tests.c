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
 *  Unit tests for bpnode_utils.c
 */

/*
** Include Files
*/

#include "bplib.h"
#include "bpnode_test_utils.h"

/*
** Function Definitions
*/

/* Test nominal table validation */
void Test_BPNode_TblValidationFunc_Nominal(void)
{
    BPNode_ExampleTable_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Nominal case (0) should succeed */
    UtAssert_INT32_EQ(BPNode_TblValidationFunc(&TestTblData), CFE_SUCCESS);
}

/* Test invalid table validation */
void Test_BPNode_TblValidationFunc_Invalid(void)
{
    BPNode_ExampleTable_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.Int1 = 1 + BPNODE_TBL_ELEMENT_1_MAX;

    UtAssert_INT32_EQ(BPNode_TblValidationFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPNode_TblValidationFunc_Nominal);
    ADD_TEST(Test_BPNode_TblValidationFunc_Invalid);

}
