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

/*
** \file
**   This file contains the source code for the Table Proxy unit test
*/

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
#include "bplib.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

BPNode_TblNameParams_t TblNameParamsArr_test[] =
{
    {"ADUProxyTable",      ADUP_CONFIG_TABLE_FILE,        0, sizeof(BPA_ADUP_Table_t),            NULL, NULL},
    {"ChannelTable",       CHANNEL_TABLE_FILE,            0, sizeof(BPLib_PI_ChannelTable_t),     NULL, NULL},
    {"ContactsTable",      CONTACTS_TABLE_FILE,           0, sizeof(BPLib_CLA_ContactsTable_t),   NULL, NULL},
    {"CRSTable",           CRS_TABLE_FILE,                0, sizeof(BPLib_ARP_CRSTable_t),        NULL, NULL},
    {"CustodianTable",     CUSTODIAN_TABLE_FILE,          0, sizeof(BPLib_PDB_CustodianTable_t),  NULL, NULL},
    {"CustodyTable",       CUSTODY_TABLE_FILE,            0, sizeof(BPLib_PDB_CustodyTable_t),    NULL, NULL},
    {"MIBConfigPNTable",   MIB_CONFIG_PN_TABLE_FILE,      0, sizeof(BPLib_NC_MIBConfigPNTable_t), NULL, NULL},
    {"MIBConfigPSTable",   MIB_CONFIG_PS_TABLE_FILE,      0, sizeof(BPLib_NC_MIBConfigPSTable_t), NULL, NULL},
    {"ReportToTable",      REPORTTO_TABLE_FILE,           0, sizeof(BPLib_PDB_ReportToTable_t),   NULL, NULL},
    {"SrcAuthTable",       SRC_AUTH_TABLE_FILE,           0, sizeof(BPLib_PDB_SrcAuthTable_t),    NULL, NULL},
    {"SrcLatencyTable",    SRC_LATENCY_TABLE_FILE,        0, sizeof(BPLib_PDB_SrcLatencyTable_t), NULL, NULL},
    {"StorageTable",       STORAGE_TABLE_FILE,            0, sizeof(BPLib_STOR_StorageTable_t),   NULL, NULL}
};

void Test_BPA_TABLEP_TableInit_Nominal(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UtAssert_INT32_EQ((int32) BPA_TABLEP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableInit_Fail_Register(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TABLEP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableInit_Fail_Load(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TABLEP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableInit_Fail_GetAddress(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TABLEP_TableInit(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableUpdate_Nominal(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);
    UtAssert_INT32_EQ((int32) BPA_TABLEP_TableUpdate(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableUpdate_Nominal_Info_Updated(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UtAssert_INT32_EQ((int32) BPA_TABLEP_TableUpdate(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableUpdate_Fail(void)
{
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr_test;
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);
    UtAssert_INT32_NEQ((int32) BPA_TABLEP_TableUpdate(), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_SingleTableUpdate_Nominal(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Modified), CFE_SUCCESS);
    UtAssert_INT32_EQ((int32) BPA_TABLEP_SingleTableUpdate(1), (int32) CFE_SUCCESS);
}

void Test_BPA_TABLEP_PI_ValidateConfigs_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PI_ChannelTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PI_ValidateConfigs), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PI_ValidateConfigs((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PI_ValidateConfigs_Error(void)
{
    CFE_Status_t Status;
    BPLib_PI_ChannelTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PI_ValidateConfigs), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PI_ValidateConfigs((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_CLA_ContactsTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_CLA_ContactsTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_CLA_ContactsTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_CLA_ContactsTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_CLA_ContactsTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_CLA_ContactsTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_CLA_ContactsTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_CLA_ContactsTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_ARP_CRSTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_ARP_CRSTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_ARP_CRSTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_ARP_CRSTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_ARP_CRSTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_ARP_CRSTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_ARP_CRSTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_ARP_CRSTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_PDB_CustodianAuthTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PDB_CustodianTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_CustodianAuthTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_CustodianAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PDB_CustodianAuthTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_PDB_CustodianTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_CustodianAuthTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_CustodianAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_PDB_CustodyAuthTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PDB_CustodyTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_CustodyAuthTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_CustodyAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PDB_CustodyAuthTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_PDB_CustodyTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_CustodyAuthTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_CustodyAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_NC_MIBConfigPNTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_NC_MIBConfigPNTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_MIBConfigPNTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_NC_MIBConfigPNTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_NC_MIBConfigPNTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_NC_MIBConfigPNTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_MIBConfigPNTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_NC_MIBConfigPNTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_NC_MIBConfigPSTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_NC_MIBConfigPSTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_MIBConfigPSTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_NC_MIBConfigPSTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_NC_MIBConfigPSTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_NC_MIBConfigPSTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_NC_MIBConfigPSTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_NC_MIBConfigPSTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_PDB_ReportToAuthTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PDB_ReportToTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_ReportToAuthTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_ReportToAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PDB_ReportToAuthTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_PDB_ReportToTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_ReportToAuthTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_ReportToAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_PDB_SrcAuthTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PDB_SrcAuthTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_SrcAuthTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_SrcAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PDB_SrcAuthTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_PDB_SrcAuthTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_SrcAuthTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_SrcAuthTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_PDB_SrcLatencyTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_PDB_SrcLatencyTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_SrcLatencyTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_SrcLatencyTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_PDB_SrcLatencyTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_PDB_SrcLatencyTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_PDB_SrcLatencyTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_PDB_SrcLatencyTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

void Test_BPA_TABLEP_STOR_StorageTblValidateFunc_Nominal(void)
{
    CFE_Status_t Status;
    BPLib_STOR_StorageTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_STOR_StorageTblValidateFunc), BPLIB_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_SUCCESS);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_STOR_StorageTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_SUCCESS */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_SUCCESS);
}

void Test_BPA_TABLEP_STOR_StorageTblValidateFunc_Error(void)
{
    CFE_Status_t Status;
    BPLib_STOR_StorageTable_t* TblData;

    UT_SetDefaultReturnValue(UT_KEY(BPLib_STOR_StorageTblValidateFunc), BPLIB_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(BPA_BPLib_Status_Translate), CFE_STATUS_NOT_IMPLEMENTED);

    memset(&TblData, 0, sizeof(TblData));
    Status = BPA_TABLEP_STOR_StorageTblValidateFunc((void*) TblData);

    UtAssert_EQ(CFE_Status_t, Status, CFE_STATUS_NOT_IMPLEMENTED);

    /* Verify that BPA_BPLib_Status_Translate received BPLIB_ERROR */
    UtAssert_EQ(BPLib_Status_t, Context_BPLib_Status[0], BPLIB_ERROR);
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_TABLEP_TableInit_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableInit_Fail_Register);
    ADD_TEST(Test_BPA_TABLEP_TableInit_Fail_Load);
    ADD_TEST(Test_BPA_TABLEP_TableInit_Fail_GetAddress);

    ADD_TEST(Test_BPA_TABLEP_TableUpdate_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableUpdate_Nominal_Info_Updated);
    ADD_TEST(Test_BPA_TABLEP_TableUpdate_Fail);

    ADD_TEST(Test_BPA_TABLEP_SingleTableUpdate_Nominal);

    ADD_TEST(Test_BPA_TABLEP_PI_ValidateConfigs_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PI_ValidateConfigs_Error);

    ADD_TEST(Test_BPA_TABLEP_CLA_ContactsTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_CLA_ContactsTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_ARP_CRSTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_ARP_CRSTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_PDB_CustodianAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PDB_CustodianAuthTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_PDB_CustodyAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PDB_CustodyAuthTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_NC_MIBConfigPNTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_NC_MIBConfigPNTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_NC_MIBConfigPSTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_NC_MIBConfigPSTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_PDB_ReportToAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PDB_ReportToAuthTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_PDB_SrcAuthTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PDB_SrcAuthTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_PDB_SrcLatencyTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_PDB_SrcLatencyTblValidateFunc_Error);

    ADD_TEST(Test_BPA_TABLEP_STOR_StorageTblValidateFunc_Nominal);
    ADD_TEST(Test_BPA_TABLEP_STOR_StorageTblValidateFunc_Error);
}