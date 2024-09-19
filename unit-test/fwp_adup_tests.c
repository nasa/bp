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
 *  Unit tests for fwp_adup.c
 */

/*
** Include Files
*/

#include "fwp_adup.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/


/* Test BPA_ADUP_In */
void Test_BPA_ADUP_In_Nominal(void)
{
    CFE_SB_Buffer_t Buf;
    uint16 ExpAduCountRecv = 1;

    UtAssert_INT32_EQ(BPA_ADUP_In(&Buf), BPLIB_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived, 
                                ExpAduCountRecv);
}

/* Test BPA_ADUP_Out */
void Test_BPA_ADUP_Out_Nominal(void)
{
    CFE_SB_Buffer_t Buf;

    UtAssert_INT32_EQ(BPA_ADUP_Out(&Buf), BPLIB_SUCCESS);
}

/* Test BPA_ADUP_AddApplication */
void Test_BPA_ADUP_AddApplication_Nominal(void)
{
    BPA_ADUP_Configs_t Configs;

    UtAssert_VOIDCALL(BPA_ADUP_AddApplication(&Configs));
}

/* Test BPA_ADUP_StartApplication */
void Test_BPA_ADUP_StartApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    UtAssert_VOIDCALL(BPA_ADUP_StartApplication(ChanId));
}

/* Test BPA_ADUP_StopApplication */
void Test_BPA_ADUP_StopApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    UtAssert_VOIDCALL(BPA_ADUP_StopApplication(ChanId));
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_ADUP_In_Nominal);
    ADD_TEST(Test_BPA_ADUP_Out_Nominal);
    ADD_TEST(Test_BPA_ADUP_AddApplication_Nominal);
    ADD_TEST(Test_BPA_ADUP_StartApplication_Nominal);
    ADD_TEST(Test_BPA_ADUP_StopApplication_Nominal);
}
