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
 *  Unit tests for fwp_perflogp.c
 */

/*
** Include Files
*/

#include "fwp_perflogp.h"
#include "bpnode_test_utils.h"

/* Test BPA_PERFLOGP_Entry */
void Test_BPA_PERFLOGP_Entry_Nominal(void)
{
    uint32_t PerfLogID = 10;
    UtAssert_VOIDCALL(BPA_PERFLOGP_Entry(PerfLogID));
}

/* Test BPA_PERFLOGP_Exit */
void Test_BPA_PERFLOGP_Exit_Nominal(void)
{
    uint32_t PerfLogID = 10;
    UtAssert_VOIDCALL(BPA_PERFLOGP_Exit(PerfLogID));    
}


/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_PERFLOGP_Entry_Nominal);
    ADD_TEST(Test_BPA_PERFLOGP_Exit_Nominal);
}
