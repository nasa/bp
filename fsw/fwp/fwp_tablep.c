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
** \file
**   This file contains the source code for the FWP Table Proxy.
*/

/*
** Include
*/

#include "bplib.h"
#include "fwp_tablep.h"
#include "fwp_adup.h"
#include "bpnode_utils.h"

/*
** Function Definitions
*/

/* Validate Channel Config table data */
CFE_Status_t BPNode_ChannelConfigTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPNode_ChannelTable_t *TblDataPtr = (BPNode_ChannelTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr[0].ChannelSet->HopLimit <= 0)
    {
        /* element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* Validate Contacts table data */
CFE_Status_t BPNode_ContactsTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPLib_ContactsTable_t *TblDataPtr = (BPLib_ContactsTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr[0].ContactSet->PortNum <= 0)
    {
        /* element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* Validate CRS table data */
CFE_Status_t BPNode_CRSTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPNode_CRSTable_t *TblDataPtr = (BPNode_CRSTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr[0].CRS_Set->SizeTrigger <= 0)
    {
        /* element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* Validate Custodian Auth table data */
CFE_Status_t BPNode_CustodianAuthTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    /* Validate data values are within allowed range */

    return ReturnCode;
}

/* Validate Custody Auth table data */
CFE_Status_t BPNode_CustodyAuthTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    /* Validate data values are within allowed range */

    return ReturnCode;
}

/* Validate MIB Config PN table data */
CFE_Status_t BPNode_MIBConfigPNTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPNode_MIBConfigPNTable_t *TblDataPtr = (BPNode_MIBConfigPNTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr->BundleAgentNum <= 0)
    {
        /* element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* Validate MIB Config PS table data */
CFE_Status_t BPNode_MIBConfigPSTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPNode_MIBConfigPSTable_t *TblDataPtr = (BPNode_MIBConfigPSTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr[0].MIB_PS_Set->ParamSetMaxLifetime <= 0)
    {
        /* element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* Validate Report-To Auth table data */
CFE_Status_t BPNode_ReportToAuthTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    return ReturnCode;
}

/* Validate Source Auth table data */
CFE_Status_t BPNode_SrcAuthTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    return ReturnCode;
}

/* Validate Source Latency table data */
CFE_Status_t BPNode_SrcLatencyTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    return ReturnCode;
}

/* Validate Storage table data */
CFE_Status_t BPNode_StorageTblValidateFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    return ReturnCode;
}

BPNode_TblNameParams_t TblNameParamsArr0[] = 
{
    {"ADUProxyTable",      ADUP_CONFIG_TABLE_FILE,        0, sizeof(BPA_ADUP_Table_t),            NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_ADUP_ValidateConfigTbl},
    {"ChannelTable",       CHANNEL_TABLE_FILE,            0, sizeof(BPNode_ChannelTable_t),       NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_ChannelConfigTblValidateFunc},
    {"ContactsTable",      CONTACTS_TABLE_FILE,           0, sizeof(BPLib_ContactsTable_t),       NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_ContactsTblValidateFunc},
    {"CRSTable",           CRS_TABLE_FILE,                0, sizeof(BPNode_CRSTable_t),           NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_CRSTblValidateFunc},
    {"CustodianTable",     CUSTODIAN_TABLE_FILE,          0, sizeof(BPNode_CustodianTable_t),     NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_CustodianAuthTblValidateFunc},
    {"CustodyTable",       CUSTODY_TABLE_FILE,            0, sizeof(BPNode_CustodyTable_t),       NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_CustodyAuthTblValidateFunc},
    {"MIBConfigPNTable",   MIB_CONFIG_PN_TABLE_FILE,      0, sizeof(BPNode_MIBConfigPNTable_t),   NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_MIBConfigPNTblValidateFunc},
    {"MIBConfigPSTable",   MIB_CONFIG_PS_TABLE_FILE,      0, sizeof(BPNode_MIBConfigPSTable_t),   NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_MIBConfigPSTblValidateFunc},
    {"ReportToTable",      REPORTTO_TABLE_FILE,           0, sizeof(BPNode_ReportToTable_t),      NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_ReportToAuthTblValidateFunc},
    {"SrcAuthTable",       SRC_AUTH_TABLE_FILE,           0, sizeof(BPNode_SrcAuthTable_t),       NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_SrcAuthTblValidateFunc},
    {"SrcLatencyTable",    SRC_LATENCY_TABLE_FILE,        0, sizeof(BPNode_SrcLatencyTable_t),    NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_SrcLatencyTblValidateFunc},
    {"StorageTable",       STORAGE_TABLE_FILE,            0, sizeof(BPNode_StorageTable_t),       NULL, (CFE_TBL_CallbackFuncPtr_t)BPNode_StorageTblValidateFunc}
};


/*Initialize table proxy, load default tables*/
CFE_Status_t BPA_TABLEP_TableInit(void)
{
    CFE_Status_t Status;
     
    BPNode_AppData.TblNameParamsArr = TblNameParamsArr0;        
    
    for (int i = 0; i < BPNODE_NUMBER_OF_TABLES; i++)
    {
        Status = CFE_TBL_Register(&BPNode_AppData.TblNameParamsArr[i].TableHandle, BPNode_AppData.TblNameParamsArr[i].TableName, 
                BPNode_AppData.TblNameParamsArr[i].TableSize, CFE_TBL_OPT_DEFAULT, BPNode_AppData.TblNameParamsArr[i].TblValidationFuncPtr);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_REG_ERR_EID, BPLib_EM_EventType_ERROR,
                    "Error Registering Table: %s, RC = 0x%08lX", BPNode_AppData.TblNameParamsArr[i].TableName, (unsigned long)Status);
            return Status;
        }
        /* Load table */
        Status = CFE_TBL_Load(BPNode_AppData.TblNameParamsArr[i].TableHandle, CFE_TBL_SRC_FILE, BPNode_AppData.TblNameParamsArr[i].TableFileName);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_LD_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error Loading Table : %s, RC = 0x%08lX", BPNode_AppData.TblNameParamsArr[i].TableName, (unsigned long)Status);
            return Status;
        }
        /* Get table address */
        Status = CFE_TBL_GetAddress((void *) &BPNode_AppData.TblNameParamsArr[i].TablePtr,
                                            BPNode_AppData.TblNameParamsArr[i].TableHandle);  
        if (Status != CFE_TBL_INFO_UPDATED)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error Getting Table: %s Address, RC = 0x%08lX", BPNode_AppData.TblNameParamsArr[i].TableName, (unsigned long)Status);
            return Status;
        }   
        
        /* Set Table Handle to Node Configuration Here*/

    }

    /* Set shortcut pointers for easier usability of table data */
    BPNode_AppData.AduTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_ADU_TBL_IDX].TablePtr;
    BPNode_AppData.ChanTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CHAN_TBL_IDX].TablePtr;
    BPNode_AppData.ContactsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CON_TBL_IDX].TablePtr;
    BPNode_AppData.CrsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CRS_TBL_IDX].TablePtr;
    BPNode_AppData.CustodianTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CSTDN_TBL_IDX].TablePtr;
    BPNode_AppData.CustodyTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CSTDY_TBL_IDX].TablePtr;
    BPNode_AppData.MibPnTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_MIBN_TBL_IDX].TablePtr;
    BPNode_AppData.MibPsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_MIBS_TBL_IDX].TablePtr;
    BPNode_AppData.ReportTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_REP_TBL_IDX].TablePtr;
    BPNode_AppData.AuthTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_AUTH_TBL_IDX].TablePtr;
    BPNode_AppData.LatTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_LATE_TBL_IDX].TablePtr;
    BPNode_AppData.StorTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_STOR_TBL_IDX].TablePtr;

    return CFE_SUCCESS;  
}

CFE_Status_t BPA_TABLEP_TableUpdate(void)
{
    CFE_Status_t Status;
    
    for (int i = 0; i < BPNODE_NUMBER_OF_TABLES; i++)
    {
        /* Manage any pending table loads, validations, etc. */
        (void) CFE_TBL_ReleaseAddress(BPNode_AppData.TblNameParamsArr[i].TableHandle);

        (void) CFE_TBL_Manage(BPNode_AppData.TblNameParamsArr[i].TableHandle);

        Status = CFE_TBL_GetAddress((void *) &BPNode_AppData.TblNameParamsArr[i].TablePtr, 
                                              BPNode_AppData.TblNameParamsArr[i].TableHandle);

        if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_MNG_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error managing the table: %s on wakeup, Status=0x%08X", BPNode_AppData.TblNameParamsArr[i].TableName, Status);
            return Status;
        }
        
        /* Set Table Handle to Node Configuration Here*/
        
    }

    /* Set shortcut pointers for easier usability of table data */
    BPNode_AppData.AduTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_ADU_TBL_IDX].TablePtr;
    BPNode_AppData.ChanTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CHAN_TBL_IDX].TablePtr;
    BPNode_AppData.ContactsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CON_TBL_IDX].TablePtr;
    BPNode_AppData.CrsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CRS_TBL_IDX].TablePtr;
    BPNode_AppData.CustodianTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CSTDN_TBL_IDX].TablePtr;
    BPNode_AppData.CustodyTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_CSTDY_TBL_IDX].TablePtr;
    BPNode_AppData.MibPnTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_MIBN_TBL_IDX].TablePtr;
    BPNode_AppData.MibPsTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_MIBS_TBL_IDX].TablePtr;
    BPNode_AppData.ReportTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_REP_TBL_IDX].TablePtr;
    BPNode_AppData.AuthTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_AUTH_TBL_IDX].TablePtr;
    BPNode_AppData.LatTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_LATE_TBL_IDX].TablePtr;
    BPNode_AppData.StorTblPtr = BPNode_AppData.TblNameParamsArr[BPNODE_STOR_TBL_IDX].TablePtr;

    
    return CFE_SUCCESS;
}

CFE_Status_t BPA_TABLEP_SingleTableUpdate(CFE_TBL_Handle_t TblHandle)
{    
    return CFE_TBL_Modified(TblHandle);
}