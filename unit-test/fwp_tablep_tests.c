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

void Test_BPA_TABLEP_TableInit_InfoUpdated_Nominal(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UtAssert_EQ(CFE_Status_t, BPA_TABLEP_TableInit(), CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableInit_Success_Nominal(void)
{
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);
    UtAssert_EQ(CFE_Status_t, BPA_TABLEP_TableInit(), CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableInit_Error(void)
{
    uint8_t ErrorLoop;
    uint16_t ExpectedStubCount;
    CFE_Status_t Status;

    ExpectedStubCount = 0;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    for (ErrorLoop = 0; ErrorLoop < BPNODE_NUMBER_OF_TABLES; ErrorLoop++)
    {
        ExpectedStubCount += (ErrorLoop + 1);

        /* Reset return values so only the n-th call fails */
        UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);

        /* Make the n-th call fail */
        UT_SetDeferredRetcode(UT_KEY(CFE_TBL_Register), ErrorLoop + 1, CFE_TBL_ERR_DUPLICATE_DIFF_SIZE);

        /* Run the function under test */
        Status = BPA_TABLEP_TableInit();

        /* Verify the return code */
        UtAssert_EQ(CFE_Status_t, Status, CFE_TBL_ERR_DUPLICATE_DIFF_SIZE);

        /* Show that something is being called */
        UtAssert_STUB_COUNT(CFE_TBL_Register, ExpectedStubCount);

        /* Verify that the correct event was issued */
        BPNode_Test_Verify_Event(ErrorLoop, BPNODE_TBL_REG_ERR_EID, "Error Registering Table: %s, RC = 0x%08lX");
    }
}

void Test_BPA_TABLEP_SingleTableInit_Nominal(void)
{
    CFE_Status_t Status;
    CFE_TBL_Handle_t TestHandle;

    TestHandle = 0;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    /* Run the function under test */
    Status = BPA_TABLEP_SingleTableInit("Test", "TestFile", 1, NULL, NULL, &TestHandle);

    /* Verify the return code is as expected */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);
}

void Test_BPA_TABLEP_SingleTableInit_Register_Error(void)
{
    CFE_Status_t Status;
    CFE_TBL_Handle_t TestHandle;

    TestHandle = 0;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_TBL_ERR_INVALID_OPTIONS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    /* Run the function under test */
    Status = BPA_TABLEP_SingleTableInit("Test", "TestFile", 1, NULL, NULL, &TestHandle);

    /* Verify the return code is as expected */
    UtAssert_EQ(CFE_Status_t, Status, CFE_TBL_ERR_INVALID_OPTIONS);

    /* Verify the event issued is as expected */
    BPNode_Test_Verify_Event(0, BPNODE_TBL_REG_ERR_EID,
                                "Error Registering Table: %s, RC = 0x%08lX");

    /* Show that no other TBL function was run */
    UtAssert_STUB_COUNT(CFE_TBL_Load, 0);
    UtAssert_STUB_COUNT(CFE_TBL_GetAddress, 0);
}

void Test_BPA_TABLEP_SingleTableInit_Load_Error(void)
{
    CFE_Status_t Status;
    CFE_TBL_Handle_t TestHandle;

    TestHandle = 0;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_TBL_ERR_INVALID_OPTIONS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    /* Run the function under test */
    Status = BPA_TABLEP_SingleTableInit("Test", "TestFile", 1, NULL, NULL, &TestHandle);

    /* Verify the return code is as expected */
    UtAssert_EQ(CFE_Status_t, Status, CFE_TBL_ERR_INVALID_OPTIONS);

    /* Verify the event issued is as expected */
    BPNode_Test_Verify_Event(0, BPNODE_TBL_LD_ERR_EID,
                                "Error Loading Table: %s, RC = 0x%08lX");

    /* Show that no other TBL function was run */
    UtAssert_STUB_COUNT(CFE_TBL_GetAddress, 0);
}

void Test_BPA_TABLEP_SingleTableInit_GetAddress_Error(void)
{
    CFE_Status_t Status;
    CFE_TBL_Handle_t TestHandle;

    TestHandle = 0;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_Load), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_INVALID_OPTIONS);

    /* Run the function under test */
    Status = BPA_TABLEP_SingleTableInit("Test", "TestFile", 1, NULL, NULL, &TestHandle);

    /* Verify the return code is as expected */
    UtAssert_EQ(CFE_Status_t, Status, CFE_TBL_ERR_INVALID_OPTIONS);

    /* Verify the event issued is as expected */
    BPNode_Test_Verify_Event(0, BPNODE_TBL_ADDR_ERR_EID,
                                "Error Getting Table Address: %s, RC = 0x%08lX");
}

void Test_BPA_TABLEP_TableUpdate_InfoUpdated_Nominal(void)
{
    uint8 TableType;
    BPLib_Status_t Status;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(BPA_CFE_Status_Translate), BPLIB_TBL_UPDATED);

    for (TableType = CHANNEL_CONFIG; TableType <= ADU_PROXY_CONFIG; TableType++)
    {
        /* Run the function under test */
        Status = BPA_TABLEP_TableUpdate(TableType, NULL);

        /* Verify that the function returns the expected status */
        UtAssert_EQ(BPLib_Status_t, Status, BPLIB_TBL_UPDATED);

        /* Show that the translation function got the correct input */
        UtAssert_EQ(CFE_Status_t, Context_CFE_Status[TableType], CFE_TBL_INFO_UPDATED);
    }
}

void Test_BPA_TABLEP_TableUpdate_Success_Nominal(void)
{
    uint8 TableType;
    BPLib_Status_t Status;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    for (TableType = CHANNEL_CONFIG; TableType <= ADU_PROXY_CONFIG; TableType++)
    {
        Status = BPA_TABLEP_TableUpdate(TableType, NULL);

        UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);
    }
}

void Test_BPA_TABLEP_TableUpdate_Error(void)
{
    UtAssert_EQ(BPLib_Status_t, BPA_TABLEP_TableUpdate(BPNODE_NUMBER_OF_TABLES + 1, NULL), BPLIB_ERROR);
}

void Test_BPA_TABLEP_TableManage_InfoUpdated_Nominal(void)
{
    CFE_Status_t Status;

    /* Force the function under test to return a table update status */
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_INFO_UPDATED);
    UT_SetDefaultReturnValue(UT_KEY(BPA_CFE_Status_Translate), BPLIB_TBL_UPDATED);

    /* Run function under test */
    Status = BPA_TABLEP_TableManage("Test", NULL, 0);

    /* Verify return code */
    UtAssert_EQ(CFE_Status_t, Status, BPLIB_TBL_UPDATED);

    /* Verify the code given to the translation function */
    UtAssert_EQ(CFE_Status_t, Context_CFE_Status[0], CFE_TBL_INFO_UPDATED);
}

void Test_BPA_TABLEP_TableManage_Success_Nominal(void)
{
    CFE_Status_t Status;

    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);

    /* Run function under test */
    Status = BPA_TABLEP_TableManage("Test", NULL, 0);

    /* Verify return code */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);
}

void Test_BPA_TABLEP_TableManage_Error(void)
{
    BPLib_Status_t Status;

    /* Force function under test to return an error */
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_ES_ERR_RESOURCEID_NOT_VALID);
    UT_SetDefaultReturnValue(UT_KEY(BPA_CFE_Status_Translate), BPLIB_ERROR);

    /* Run the function under test */
    Status = BPA_TABLEP_TableManage("Test", NULL, 0);

    /* Verify the return code */
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_ERROR);

    /* Verify the code given to the translation function */
    UtAssert_EQ(CFE_Status_t, Context_CFE_Status[0], CFE_ES_ERR_RESOURCEID_NOT_VALID);

    /* Verify that the correct event was issued */
    BPNode_Test_Verify_Event(0, BPNODE_TBL_MNG_ERR_EID,
                                "Error managing the table: %s on wakeup, Status=0x%08X");
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
    ADD_TEST(Test_BPA_TABLEP_TableInit_InfoUpdated_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableInit_Success_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableInit_Error);
    ADD_TEST(Test_BPA_TABLEP_SingleTableInit_Nominal);
    ADD_TEST(Test_BPA_TABLEP_SingleTableInit_Register_Error);
    ADD_TEST(Test_BPA_TABLEP_SingleTableInit_Load_Error);
    ADD_TEST(Test_BPA_TABLEP_SingleTableInit_GetAddress_Error);
    ADD_TEST(Test_BPA_TABLEP_TableUpdate_InfoUpdated_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableUpdate_Success_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableUpdate_Error);
    ADD_TEST(Test_BPA_TABLEP_TableManage_InfoUpdated_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableManage_Success_Nominal);
    ADD_TEST(Test_BPA_TABLEP_TableManage_Error);
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