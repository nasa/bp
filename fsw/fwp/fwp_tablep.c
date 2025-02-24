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

/* ========*/
/* Includes */
/* ======== */

#include "bplib.h"
#include "fwp_tablep.h"
#include "fwp_adup.h"
#include "bpnode_utils.h"
#include "bpnode_app.h"

/* ======= */
/* Globals */
/* ======= */

BPLib_NC_ConfigPtrs_t BPNode_ConfigPtrs;

/* ==================== */
/* Function Definitions */
/* ==================== */

/* Initialize table proxy, load default tables */
CFE_Status_t BPA_TABLEP_TableInit(void)
{
    CFE_Status_t Status;

    /* Initialize the ADU proxy table */
    Status = BPA_TABLEP_SingleTableInit("ADUProxyTable",
                                        ADUP_CONFIG_TABLE_FILE,
                                        sizeof(BPA_ADUP_Table_t),
                                        (CFE_TBL_CallbackFuncPtr_t) BPA_ADUP_ValidateConfigTbl,
                                        (void**) &BPNode_AppData.AduProxyTablePtr,
                                        &BPNode_AppData.TableHandles[ADU_PROXY_CONFIG]);

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PI channel configuration table */
        Status = BPA_TABLEP_SingleTableInit("ChannelTable",
                                            CHANNEL_TABLE_FILE,
                                            sizeof(BPLib_PI_ChannelTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PI_ValidateConfigs,
                                            (void**) &BPNode_ConfigPtrs.ChanTblPtr,
                                            &BPNode_AppData.TableHandles[CHANNEL_CONFIG]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the CLA contacts table */
        Status = BPA_TABLEP_SingleTableInit("ContactsTable",
                                            CONTACTS_TABLE_FILE,
                                            sizeof(BPLib_CLA_ContactsTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_CLA_ContactsTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.ContactsTblPtr,
                                            &BPNode_AppData.TableHandles[CONTACTS]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the ARP CRS table */
        Status = BPA_TABLEP_SingleTableInit("CRSTable",
                                            CRS_TABLE_FILE,
                                            sizeof(BPLib_ARP_CRSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_ARP_CRSTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.CrsTblPtr,
                                            &BPNode_AppData.TableHandles[COMPRESSED_REPORTING]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custodian table */
        Status = BPA_TABLEP_SingleTableInit("CustodianTable",
                                            CUSTODIAN_TABLE_FILE,
                                            sizeof(BPLib_PDB_CustodianTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodianAuthTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.CustodianTblPtr,
                                            &BPNode_AppData.TableHandles[CUSTODIAN_AUTH_POLICY]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custody table */
        Status = BPA_TABLEP_SingleTableInit("CustodyTable",
                                            CUSTODY_TABLE_FILE,
                                            sizeof(BPLib_PDB_CustodyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodyAuthTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.CustodyTblPtr,
                                            &BPNode_AppData.TableHandles[CUSTODY_AUTH_POLICY]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per node table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPNTable",
                                            MIB_CONFIG_PN_TABLE_FILE,
                                            sizeof(BPLib_NC_MIBConfigPNTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPNTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.MibPnTblPtr,
                                            &BPNode_AppData.TableHandles[MIB_CONFIG_PER_NODE]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per source table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPSTable",
                                            MIB_CONFIG_PS_TABLE_FILE,
                                            sizeof(BPLib_NC_MIBConfigPSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPSTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.MibPsTblPtr,
                                            &BPNode_AppData.TableHandles[MIB_CONFIG_PER_SRC]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB report to table */
        Status = BPA_TABLEP_SingleTableInit("ReportToTable",
                                            REPORTTO_TABLE_FILE,
                                            sizeof(BPLib_PDB_ReportToTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_ReportToAuthTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.ReportTblPtr,
                                            &BPNode_AppData.TableHandles[REPORT_TO_EID_AUTH_POLICY]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB authorized sources table */
        Status = BPA_TABLEP_SingleTableInit("SrcAuthTable",
                                            SRC_AUTH_TABLE_FILE,
                                            sizeof(BPLib_PDB_SrcAuthTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcAuthTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.AuthTblPtr,
                                            &BPNode_AppData.TableHandles[SRC_AUTH_POLICY]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Authorize the PDB source latency table */
        Status = BPA_TABLEP_SingleTableInit("SrcLatencyTable",
                                            SRC_LATENCY_TABLE_FILE,
                                            sizeof(BPLib_PDB_SrcLatencyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcLatencyTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.LatTblPtr,
                                            &BPNode_AppData.TableHandles[SRC_LATENCY_POLICY]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the STOR storage table */
        Status = BPA_TABLEP_SingleTableInit("StorageTable",
                                            STORAGE_TABLE_FILE,
                                            sizeof(BPLib_STOR_StorageTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_STOR_StorageTblValidateFunc,
                                            (void**) &BPNode_ConfigPtrs.StorTblPtr,
                                            &BPNode_AppData.TableHandles[STORAGE]);
    }

    if (Status == CFE_TBL_INFO_UPDATED)
    {
        Status = CFE_SUCCESS;
    }

    return Status;
}

CFE_Status_t BPA_TABLEP_SingleTableInit(const char* TableName, const char* TableFileName, size_t Size, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr, void** TablePtr, CFE_TBL_Handle_t* TableHandle)
{
    CFE_Status_t Status;

    /* Register table */
    Status = CFE_TBL_Register(TableHandle, TableName, Size,
                                CFE_TBL_OPT_DEFAULT, TblValidationFuncPtr);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_REG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error Registering Table: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
    }
    else
    {
        /* Load table */
        Status = CFE_TBL_Load(*TableHandle, CFE_TBL_SRC_FILE, TableFileName);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_LD_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error Loading Table: %s, RC = 0x%08lX", TableFileName, (unsigned long)Status);
        }
        else
        {
            /* Get table address */
            Status = CFE_TBL_GetAddress(TablePtr, *TableHandle);

            if (Status != CFE_TBL_INFO_UPDATED && Status != CFE_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error Getting Table Address: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
            }
        }
    }

    return Status;
}

BPLib_Status_t BPA_TABLEP_TableUpdate(uint8 TableType, void** TblPtr)
{
    BPLib_Status_t Status;

    Status = BPLIB_ERROR;

    switch (TableType)
    {
        case CHANNEL_CONFIG:
            Status = BPA_TABLEP_TableManage("ChannelTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[CHANNEL_CONFIG]);

            break;
        case CONTACTS:
            Status = BPA_TABLEP_TableManage("ContactsTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[CONTACTS]);

            break;
        case COMPRESSED_REPORTING:
            Status = BPA_TABLEP_TableManage("CRSTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[COMPRESSED_REPORTING]);

            break;
        case CUSTODIAN_AUTH_POLICY:
            Status = BPA_TABLEP_TableManage("CustodianTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[CUSTODIAN_AUTH_POLICY]);

            break;
        case CUSTODY_AUTH_POLICY:
            Status = BPA_TABLEP_TableManage("CustodyTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[CUSTODY_AUTH_POLICY]);

            break;
        case MIB_CONFIG_PER_NODE:
            Status = BPA_TABLEP_TableManage("MIBConfigPNTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[MIB_CONFIG_PER_NODE]);

            break;
        case MIB_CONFIG_PER_SRC:
            Status = BPA_TABLEP_TableManage("MIBConfigPSTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[MIB_CONFIG_PER_SRC]);

            break;
        case REPORT_TO_EID_AUTH_POLICY:
            Status = BPA_TABLEP_TableManage("ReportToTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[REPORT_TO_EID_AUTH_POLICY]);

            break;
        case SRC_AUTH_POLICY:
            Status = BPA_TABLEP_TableManage("SrcAuthTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[SRC_AUTH_POLICY]);

            break;
        case SRC_LATENCY_POLICY:
            Status = BPA_TABLEP_TableManage("SrcLatencyTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[SRC_LATENCY_POLICY]);

            break;
        case STORAGE:
            Status = BPA_TABLEP_TableManage("StorageTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[STORAGE]);

            break;
        case ADU_PROXY_CONFIG:
            Status = BPA_TABLEP_TableManage("ADUProxyTable",
                                            TblPtr,
                                            BPNode_AppData.TableHandles[ADU_PROXY_CONFIG]);

            break;
    }

    return Status;
}

BPLib_Status_t BPA_TABLEP_TableManage(const char* TableName, void** TablePtr, CFE_TBL_Handle_t TableHandle)
{
    CFE_Status_t Status;

    (void) CFE_TBL_ReleaseAddress(TableHandle);
    (void) CFE_TBL_Manage(TableHandle);
    Status = CFE_TBL_GetAddress(TablePtr, TableHandle);

    if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_MNG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error managing the table: %s on wakeup, Status=0x%08X", TableName, Status);
    }

    return BPA_CFE_Status_Translate(Status);
}

CFE_Status_t BPA_TABLEP_PI_ValidateConfigs(void* TblData)                { return BPA_BPLib_Status_Translate(BPLib_PI_ValidateConfigs(TblData)); }
CFE_Status_t BPA_TABLEP_CLA_ContactsTblValidateFunc(void* TblData)       { return BPA_BPLib_Status_Translate(BPLib_CLA_ContactsTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_ARP_CRSTblValidateFunc(void* TblData)            { return BPA_BPLib_Status_Translate(BPLib_ARP_CRSTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_PDB_CustodianAuthTblValidateFunc(void* TblData)  { return BPA_BPLib_Status_Translate(BPLib_PDB_CustodianAuthTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_PDB_CustodyAuthTblValidateFunc(void* TblData)    { return BPA_BPLib_Status_Translate(BPLib_PDB_CustodyAuthTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_NC_MIBConfigPNTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_NC_MIBConfigPNTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_NC_MIBConfigPSTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_NC_MIBConfigPSTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_PDB_ReportToAuthTblValidateFunc(void* TblData)   { return BPA_BPLib_Status_Translate(BPLib_PDB_ReportToAuthTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_PDB_SrcAuthTblValidateFunc(void* TblData)        { return BPA_BPLib_Status_Translate(BPLib_PDB_SrcAuthTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_PDB_SrcLatencyTblValidateFunc(void* TblData)     { return BPA_BPLib_Status_Translate(BPLib_PDB_SrcLatencyTblValidateFunc(TblData)); }
CFE_Status_t BPA_TABLEP_STOR_StorageTblValidateFunc(void* TblData)       { return BPA_BPLib_Status_Translate(BPLib_STOR_StorageTblValidateFunc(TblData)); }