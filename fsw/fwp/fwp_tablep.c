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

/* Initialize table proxy, load default tables */
CFE_Status_t BPA_TABLEP_TableInit(void)
{
    CFE_Status_t Status;

    /* Initialize the ADU proxy table */
    Status = BPA_TABLEP_SingleTableInit("ADUProxyTable",
                                        sizeof(BPA_ADUP_Table_t),
                                        (CFE_TBL_CallbackFuncPtr_t) BPA_ADUP_ValidateConfigTbl,
                                        (void*) BPNode_AppData.AduProxyTablePtr);

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the PI channel configuration table */
        Status = BPA_TABLEP_SingleTableInit("ChannelTable",
                                            sizeof(BPLib_PI_ChannelTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PI_ValidateConfigs,
                                            (void*) BPLib_FWP_ConfigPtrs.ChanTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the CLA contacts table */
        Status = BPA_TABLEP_SingleTableInit("ContactsTable",
                                            sizeof(BPLib_CLA_ContactsTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_CLA_ContactsTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.ContactsTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the ARP CRS table */
        Status = BPA_TABLEP_SingleTableInit("CRSTable",
                                            sizeof(BPLib_ARP_CRSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_ARP_CRSTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CrsTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the PDB custodian table */
        Status = BPA_TABLEP_SingleTableInit("CustodianTable",
                                            sizeof(BPLib_PDB_CustodianTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodianAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CustodianTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the PDB custody table */
        Status = BPA_TABLEP_SingleTableInit("CustodyTable",
                                            sizeof(BPLib_PDB_CustodyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodyAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CustodyTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the NC MIB config per node table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPNTable",
                                            sizeof(BPLib_NC_MIBConfigPNTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPNTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.MibPnTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the NC MIB config per source table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPSTable",
                                            sizeof(BPLib_NC_MIBConfigPSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPSTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.MibPsTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the PDB report to table */
        Status = BPA_TABLEP_SingleTableInit("ReportToTable",
                                            sizeof(BPLib_PDB_ReportToTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_ReportToAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.ReportTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the PDB authorized sources table */
        Status = BPA_TABLEP_SingleTableInit("SrcAuthTable",
                                            sizeof(BPLib_PDB_SrcAuthTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.AuthTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Authorize the PDB source latency table */
        Status = BPA_TABLEP_SingleTableInit("SrcLatencyTable",
                                            sizeof(BPLib_PDB_SrcLatencyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcLatencyTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.LatTblPtr);
    }

    if (Status == CFE_SUCCESS)
    {
        /* Initialize the STOR storage table */
        Status = BPA_TABLEP_SingleTableInit("StorageTable",
                                            sizeof(BPLib_STOR_StorageTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_STOR_StorageTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.StorTblPtr);
    }

    return Status;
}

CFE_Status_t BPA_TABLEP_SingleTableInit(char* TableName, size_t Size, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr, void* TablePtr)
{
    CFE_Status_t Status;
    CFE_TBL_Handle_t InitHandle;

    /* Register table */
    Status = CFE_TBL_Register(&InitHandle, TableName, Size,
                                CFE_TBL_OPT_DEFAULT, TblValidationFuncPtr);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_REG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error Registering Table: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
    }
    else
    {
        /* Load table */
        Status = CFE_TBL_Load(InitHandle, CFE_TBL_SRC_FILE, TableName);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_LD_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error Loading Table: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
        }
        else
        {
            /* Get table address */
            Status = CFE_TBL_GetAddress((void **) &TablePtr, InitHandle);

            if (Status != CFE_TBL_INFO_UPDATED)
            {
                BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error Getting Table Address: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
            }
        }
    }

    return Status;
}

CFE_Status_t BPA_TABLEP_TableUpdate(BPNode_TblNameParams_t* Tbl)
{
    CFE_Status_t Status;

    /* Manage any pending table loads, validations, etc. */
    (void) CFE_TBL_ReleaseAddress(Tbl->TableHandle);

    (void) CFE_TBL_Manage(Tbl->TableHandle);

    Status = CFE_TBL_GetAddress((void *) &Tbl->TablePtr, Tbl->TableHandle);

    if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_MNG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error managing the table: %s on wakeup, Status=0x%08X", Tbl->TableName, Status);
        return Status;
    }

    /* TODO: Set Table Handle to Node Configuration */

    return CFE_SUCCESS;
}

BPLib_Status_t BPA_TABLEP_SingleTableUpdate(CFE_TBL_Handle_t TblHandle) { return BPA_CFE_Status_Translate(CFE_TBL_Modified(TblHandle)); }

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