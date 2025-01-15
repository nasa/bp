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
#include "bpnode_app.h"

/*
** Function Definitions
*/

BPNode_TblNameParams_t TblNameParamsArr0[] = 
{
    {"ADUProxyTable",      ADUP_CONFIG_TABLE_FILE,        0, sizeof(BPA_ADUP_Table_t),            NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_ADUP_ValidateConfigTbl},
    {"ChannelTable",       CHANNEL_TABLE_FILE,            0, sizeof(BPLib_PI_ChannelTable_t),     NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PI_ValidateConfigs},
    {"ContactsTable",      CONTACTS_TABLE_FILE,           0, sizeof(BPLib_CLA_ContactsTable_t),   NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_CLA_ContactsTblValidateFunc},
    {"CRSTable",           CRS_TABLE_FILE,                0, sizeof(BPLib_ARP_CRSTable_t),        NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_ARP_CRSTblValidateFunc},
    {"CustodianTable",     CUSTODIAN_TABLE_FILE,          0, sizeof(BPLib_PDB_CustodianTable_t),  NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PDB_CustodianAuthTblValidateFunc},
    {"CustodyTable",       CUSTODY_TABLE_FILE,            0, sizeof(BPLib_PDB_CustodyTable_t),    NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PDB_CustodyAuthTblValidateFunc},
    {"MIBConfigPNTable",   MIB_CONFIG_PN_TABLE_FILE,      0, sizeof(BPLib_NC_MIBConfigPNTable_t), NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_NC_MIBConfigPNTblValidateFunc},
    {"MIBConfigPSTable",   MIB_CONFIG_PS_TABLE_FILE,      0, sizeof(BPLib_NC_MIBConfigPSTable_t), NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_NC_MIBConfigPSTblValidateFunc},
    {"ReportToTable",      REPORTTO_TABLE_FILE,           0, sizeof(BPLib_PDB_ReportToTable_t),   NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PDB_ReportToAuthTblValidateFunc},
    {"SrcAuthTable",       SRC_AUTH_TABLE_FILE,           0, sizeof(BPLib_PDB_SrcAuthTable_t),    NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PDB_SrcAuthTblValidateFunc},
    {"SrcLatencyTable",    SRC_LATENCY_TABLE_FILE,        0, sizeof(BPLib_PDB_SrcLatencyTable_t), NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_PDB_SrcLatencyTblValidateFunc},
    {"StorageTable",       STORAGE_TABLE_FILE,            0, sizeof(BPLib_STOR_StorageTable_t),   NULL, (CFE_TBL_CallbackFuncPtr_t)BPA_TABLEP_STOR_StorageTblValidateFunc}
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

inline CFE_Status_t BPA_TABLEP_PI_ValidateConfigs(void* TblData)                { return BPA_BPLib_Status_Translate(BPLib_PI_ValidateConfigs(TblData)); }
inline CFE_Status_t BPA_TABLEP_CLA_ContactsTblValidateFunc(void* TblData)       { return BPA_BPLib_Status_Translate(BPLib_CLA_ContactsTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_ARP_CRSTblValidateFunc(void* TblData)            { return BPA_BPLib_Status_Translate(BPLib_ARP_CRSTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_PDB_CustodianAuthTblValidateFunc(void* TblData)  { return BPA_BPLib_Status_Translate(BPLib_PDB_CustodianAuthTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_PDB_CustodyAuthTblValidateFunc(void* TblData)    { return BPA_BPLib_Status_Translate(BPLib_PDB_CustodyAuthTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_NC_MIBConfigPNTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_NC_MIBConfigPNTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_NC_MIBConfigPSTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_NC_MIBConfigPSTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_PDB_ReportToAuthTblValidateFunc(void* TblData)   { return BPA_BPLib_Status_Translate(BPLib_PDB_ReportToAuthTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_PDB_SrcAuthTblValidateFunc(void* TblData)        { return BPA_BPLib_Status_Translate(BPLib_PDB_SrcAuthTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_PDB_SrcLatencyTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_PDB_SrcLatencyTblValidateFunc(TblData)); }
inline CFE_Status_t BPA_TABLEP_STOR_StorageTblValidateFunc(void* TblData)       { return BPA_BPLib_Status_Translate(BPLib_STOR_StorageTblValidateFunc(TblData)); }