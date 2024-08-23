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
** File: fwp_tablep_tests.c
**
** Purpose:
** Coverage Unit Test cases for the Framework Proxy
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode's Framework Proxy
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */
#include "fwp_tablep.h"
#include "bpnode_test_utils.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

BPNode_TblNameParams_t TblNameParamsArr_test[] = 
{
    {"ADUProxyTable",      ADUP_CONFIG_TABLE_FILE,        0, sizeof(BPNode_ADUProxyTable_t),      NULL, NULL},
    {"ChannelTable",       CHANNEL_TABLE_FILE,            0, sizeof(BPNode_ChannelTable_t),       NULL, NULL},
    {"ContactsTable",      CONTACTS_TABLE_FILE,           0, sizeof(BPNode_ContactsTable_t),      NULL, NULL},
    {"CRSTable",           CRS_TABLE_FILE,                0, sizeof(BPNode_CRSTable_t),           NULL, NULL},
    {"CustodianTable",     CUSTODIAN_TABLE_FILE,          0, sizeof(BPNode_CustodianTable_t),     NULL, NULL},
    {"CustodyTable",       CUSTODY_TABLE_FILE,            0, sizeof(BPNode_CustodyTable_t),       NULL, NULL},
    {"MIBConfigPNTable",   MIB_CONFIG_PN_TABLE_FILE,      0, sizeof(BPNode_MIBConfigPNTable_t),   NULL, NULL},
    {"MIBConfigPSTable",   MIB_CONFIG_PS_TABLE_FILE,      0, sizeof(BPNode_MIBConfigPSTable_t),   NULL, NULL},
    {"ReportToTable",      REPORTTO_TABLE_FILE,           0, sizeof(BPNode_ReportToTable_t),  NULL, NULL},
    {"SrcAuthTable",       SRC_AUTH_TABLE_FILE,           0, sizeof(BPNode_SrcAuthTable_t),       NULL, NULL},
    {"SrcLatencyTable",    SRC_LATENCY_TABLE_FILE,        0, sizeof(BPNode_SrcLatencyTable_t),    NULL, NULL},
    {"StorageTable",       STORAGE_TABLE_FILE,            0, sizeof(BPNode_StorageTable_t),       NULL, NULL}
};

void Test_BPA_TableP_TableInit_Nominal(void)
{    
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UtAssert_INT32_EQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TableP_TableInit_Fail_Register(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TableP_TableInit_Fail_Load(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TableP_TableInit_Fail_GetAddress(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TableP_TableUpdate_Nominal(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);
    UtAssert_INT32_EQ((int32) BPA_TableP_TableUpdate(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TableP_TableUpdate_Fail(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableUpdate(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TableP_SingleTableUpdate_Nominal(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_SUCCESS);
    UtAssert_INT32_EQ((int32) BPA_TableP_SingleTableUpdate(1), (int32) CFE_SUCCESS);    
}

void Test_BPA_TableP_SingleTableUpdate_Fail(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_SingleTableUpdate(1), (int32) CFE_SUCCESS);    
}

void Test_BPNode_ADUPTblValidateFunc_Nominal(void)
{
    BPNode_ADUProxyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.ADUP_Set[0].NumRecvFrmMIDs = 1;
    UtAssert_INT32_EQ((int32) BPNode_ADUPTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_ADUPTblValidateFunc_Invalid(void)
{
    BPNode_ADUProxyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.ADUP_Set[0].NumRecvFrmMIDs = 0;

    UtAssert_INT32_EQ(BPNode_ADUPTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_ChannelConfigTblValidateFunc_Nominal(void)
{
    BPNode_ChannelTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.ChannelSet[0].HopLimit = 10;
    UtAssert_INT32_EQ((int32) BPNode_ChannelConfigTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_ChannelConfigTblValidateFunc_Invalid(void)
{
    BPNode_ChannelTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.ChannelSet[0].HopLimit = 0;

    UtAssert_INT32_EQ(BPNode_ChannelConfigTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_ContactsTblValidateFunc_Nominal(void)
{
    BPNode_ContactsTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.ContactSet[0].PortNum = 10;
    UtAssert_INT32_EQ((int32) BPNode_ContactsTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_ContactsTblValidateFunc_Invalid(void)
{
    BPNode_ContactsTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.ContactSet[0].PortNum = 0;

    UtAssert_INT32_EQ(BPNode_ContactsTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_CRSTblValidateFunc_Nominal(void)
{
    BPNode_CRSTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.CRS_Set[0].SizeTrigger = 10;
    UtAssert_INT32_EQ((int32) BPNode_CRSTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_CRSTblValidateFunc_Invalid(void)
{
    BPNode_CRSTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.CRS_Set[0].SizeTrigger = 0;

    UtAssert_INT32_EQ(BPNode_CRSTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_CustodianAuthTblValidateFunc_Nominal(void)
{
    BPNode_CustodianTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_CustodianAuthTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_CustodianAuthTblValidateFunc_Invalid(void)
{
    BPNode_CustodianTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_CustodianAuthTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_CustodyAuthTblValidateFunc_Nominal(void)
{
    BPNode_CustodyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_CustodyAuthTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_CustodyAuthTblValidateFunc_Invalid(void)
{
    BPNode_CustodyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_CustodyAuthTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_MIBConfigPNTblValidateFunc_Nominal(void)
{
    BPNode_MIBConfigPNTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.BundleAgentNum = 10;
    UtAssert_INT32_EQ((int32) BPNode_MIBConfigPNTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_MIBConfigPNTblValidateFunc_Invalid(void)
{
    BPNode_MIBConfigPNTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.BundleAgentNum = 0;

    UtAssert_INT32_EQ(BPNode_MIBConfigPNTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_MIBConfigPSTblValidateFunc_Nominal(void)
{
    BPNode_MIBConfigPSTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    TestTblData.MIB_PS_Set[0].ParamSetMaxLifetime = 10;
    UtAssert_INT32_EQ((int32) BPNode_MIBConfigPSTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_MIBConfigPSTblValidateFunc_Invalid(void)
{
    BPNode_MIBConfigPSTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */
    TestTblData.MIB_PS_Set[0].ParamSetMaxLifetime = 0;

    UtAssert_INT32_EQ(BPNode_MIBConfigPSTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_ReportToAuthTblValidateFunc_Nominal(void)
{
    BPNode_ReportToTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_ReportToAuthTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_ReportToAuthTblValidateFunc_Invalid(void)
{
    BPNode_ReportToTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_ReportToAuthTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_SrcAuthTblValidateFunc_Nominal(void)
{
    BPNode_SrcAuthTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_SrcAuthTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_SrcAuthTblValidateFunc_Invalid(void)
{
    BPNode_SrcAuthTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_SrcAuthTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_SrcLatencyTblValidateFunc_Nominal(void)
{
    BPNode_SrcLatencyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_SrcLatencyTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_SrcLatencyTblValidateFunc_Invalid(void)
{
    BPNode_SrcLatencyTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_SrcLatencyTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

void Test_BPNode_StorageTblValidateFunc_Nominal(void)
{
    BPNode_StorageTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));
    UtAssert_INT32_EQ((int32) BPNode_StorageTblValidateFunc(&TestTblData), (int32) CFE_SUCCESS);     
}

void Test_BPNode_StorageTblValidateFunc_Invalid(void)
{
    BPNode_StorageTable_t TestTblData;
    memset(&TestTblData, 0, sizeof(TestTblData));

    /* Error case should return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE */

    UtAssert_INT32_NEQ(BPNode_StorageTblValidateFunc(&TestTblData), 
                                                BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE);
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_TableP_TableInit_Nominal);
    ADD_TEST(Test_BPA_TableP_TableInit_Fail_Register);
    ADD_TEST(Test_BPA_TableP_TableInit_Fail_Load);
    ADD_TEST(Test_BPA_TableP_TableInit_Fail_GetAddress);
    
    ADD_TEST(Test_BPA_TableP_TableUpdate_Nominal);
    ADD_TEST(Test_BPA_TableP_TableUpdate_Fail);
    
    ADD_TEST(Test_BPA_TableP_SingleTableUpdate_Nominal);
    ADD_TEST(Test_BPA_TableP_SingleTableUpdate_Fail);
    
    ADD_TEST(Test_BPNode_ADUPTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_ADUPTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_ChannelConfigTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_ChannelConfigTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_ContactsTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_ContactsTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_CRSTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_CRSTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_CustodianAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_CustodianAuthTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_CustodyAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_CustodyAuthTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_MIBConfigPNTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_MIBConfigPNTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_MIBConfigPSTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_MIBConfigPSTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_ReportToAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_ReportToAuthTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_SrcAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_SrcAuthTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_SrcLatencyTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_SrcLatencyTblValidateFunc_Invalid);

    ADD_TEST(Test_BPNode_StorageTblValidateFunc_Nominal);
    ADD_TEST(Test_BPNode_StorageTblValidateFunc_Invalid);
}
