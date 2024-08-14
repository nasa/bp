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

void Test_BPA_TableP_TableInit(void)
{
    UtAssert_INT32_EQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TableP_TableUpdate(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_TableUpdate(), (int32) CFE_SUCCESS);    
}

void Test_BPA_TableP_SingleTableUpdate(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetStatus), CFE_TBL_INFO_UPDATE_PENDING);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Update), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TableP_SingleTableUpdate(1), (int32) CFE_SUCCESS);    
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_TableP_TableInit);
    ADD_TEST(Test_BPA_TableP_TableUpdate);
    ADD_TEST(Test_BPA_TableP_SingleTableUpdate);
}
