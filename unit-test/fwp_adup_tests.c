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
    uint8_t ChanId = 0;
    uint16 ExpAduCountRecv = 1;
    CFE_MSG_Size_t Size = 10;

    /* Set global data */
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = 100;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &Size, sizeof(Size), false);

    UtAssert_INT32_EQ(BPA_ADUP_In(&Buf, ChanId), BPLIB_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.AduInData[ChanId].AduCountReceived, ExpAduCountRecv);
}

/* Test BPA_ADUP_Out */
void Test_BPA_ADUP_Out_Nominal(void)
{
    CFE_SB_Buffer_t Buf;
    uint8_t ChanId = 0;
    uint16 ExpAduCountDel = 1;

    UtAssert_INT32_EQ(BPA_ADUP_Out(&Buf, ChanId), BPLIB_SUCCESS);
    UtAssert_UINT16_EQ(BPNode_AppData.AduOutData[ChanId].AduCountDelivered, ExpAduCountDel);
}

/* Test BPA_ADUP_AddApplication */
void Test_BPA_ADUP_AddApplication_Nominal(void)
{
    uint8_t ChanId = 0;
    BPA_ADUP_Table_t AduTbl;
    BPNode_ChannelTable_t ChanTbl;

    memset(&AduTbl, 0, sizeof(AduTbl));
    memset(&ChanTbl, 0, sizeof(ChanTbl));

    BPNode_AppData.TblNameParamsArr[BPNODE_ADU_TBL_IDX].TablePtr = &AduTbl;
    BPNode_AppData.TblNameParamsArr[BPNODE_CHAN_TBL_IDX].TablePtr = &ChanTbl;

    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STOPPED;

    UtAssert_INT32_EQ(BPA_ADUP_AddApplication(ChanId), BPLIB_SUCCESS);
}

/* Test BPA_ADUP_StartApplication */
void Test_BPA_ADUP_StartApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_ADDED;

    UtAssert_INT32_EQ(BPA_ADUP_StartApplication(ChanId), BPLIB_SUCCESS);
}

/* Test BPA_ADUP_StopApplication */
void Test_BPA_ADUP_StopApplication_Nominal(void)
{
    uint8_t ChanId = 0;

    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STARTED;

    UtAssert_INT32_EQ(BPA_ADUP_StopApplication(ChanId), BPLIB_SUCCESS);
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
