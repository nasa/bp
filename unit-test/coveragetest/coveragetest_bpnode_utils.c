/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
** File: coveragetest_bpnode_utils.c
**
** Purpose:
** Coverage Unit Test cases for the BPNode Application
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode application.
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */
#include "bplib.h"
#include "bpnode_coveragetest_common.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_BPNode_TblValidationFunc(void)
{
    /*
     * Test Case For:
     * CFE_Status_t BPNode_TblValidationFunc( void *TblData )
     */
    BPNode_ExampleTable_t TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));

    /* nominal case (0) should succeed */
    UtAssert_INT32_EQ(BPNode_TblValidationFunc(&TestTblData), CFE_SUCCESS);

    /* error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.Int1 = 1 + BPNODE_TBL_ELEMENT_1_MAX;
    UtAssert_INT32_EQ(BPNode_TblValidationFunc(&TestTblData), BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}


/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(BPNode_TblValidationFunc);
}
