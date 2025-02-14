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
                                        (void*) BPNode_AppData.AduProxyTablePtr,
                                        BPNode_AppData.TableHandles[BPNODE_ADU_TBL_IDX]);

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PI channel configuration table */
        Status = BPA_TABLEP_SingleTableInit("ChannelTable",
                                            sizeof(BPLib_PI_ChannelTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PI_ValidateConfigs,
                                            (void*) BPLib_FWP_ConfigPtrs.ChanTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_CHAN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the CLA contacts table */
        Status = BPA_TABLEP_SingleTableInit("ContactsTable",
                                            sizeof(BPLib_CLA_ContactsTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_CLA_ContactsTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.ContactsTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_CON_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the ARP CRS table */
        Status = BPA_TABLEP_SingleTableInit("CRSTable",
                                            sizeof(BPLib_ARP_CRSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_ARP_CRSTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CrsTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_CRS_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custodian table */
        Status = BPA_TABLEP_SingleTableInit("CustodianTable",
                                            sizeof(BPLib_PDB_CustodianTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodianAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CustodianTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_CSTDN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custody table */
        Status = BPA_TABLEP_SingleTableInit("CustodyTable",
                                            sizeof(BPLib_PDB_CustodyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_CustodyAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.CustodyTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_CSTDY_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per node table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPNTable",
                                            sizeof(BPLib_NC_MIBConfigPNTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPNTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.MibPnTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_MIBN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per source table */
        Status = BPA_TABLEP_SingleTableInit("MIBConfigPSTable",
                                            sizeof(BPLib_NC_MIBConfigPSTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_NC_MIBConfigPSTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.MibPsTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_MIBS_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB report to table */
        Status = BPA_TABLEP_SingleTableInit("ReportToTable",
                                            sizeof(BPLib_PDB_ReportToTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_ReportToAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.ReportTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_REP_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB authorized sources table */
        Status = BPA_TABLEP_SingleTableInit("SrcAuthTable",
                                            sizeof(BPLib_PDB_SrcAuthTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcAuthTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.AuthTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_AUTH_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Authorize the PDB source latency table */
        Status = BPA_TABLEP_SingleTableInit("SrcLatencyTable",
                                            sizeof(BPLib_PDB_SrcLatencyTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_PDB_SrcLatencyTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.LatTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_LATE_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the STOR storage table */
        Status = BPA_TABLEP_SingleTableInit("StorageTable",
                                            sizeof(BPLib_STOR_StorageTable_t),
                                            (CFE_TBL_CallbackFuncPtr_t) BPA_TABLEP_STOR_StorageTblValidateFunc,
                                            (void*) BPLib_FWP_ConfigPtrs.StorTblPtr,
                                            BPNode_AppData.TableHandles[BPNODE_STOR_TBL_IDX]);
    }

    return Status;
}

CFE_Status_t BPA_TABLEP_SingleTableInit(const char* TableName, size_t Size, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr, void* TablePtr, CFE_TBL_Handle_t TableHandle)
{
    CFE_Status_t Status;

    /* Register table */
    Status = CFE_TBL_Register(&TableHandle, TableName, Size,
                                CFE_TBL_OPT_DEFAULT, TblValidationFuncPtr);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_REG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error Registering Table: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
    }
    else
    {
        /* Load table */
        Status = CFE_TBL_Load(TableHandle, CFE_TBL_SRC_FILE, TableName);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_TBL_LD_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error Loading Table: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
        }
        else
        {
            /* Get table address */
            Status = CFE_TBL_GetAddress((void **) &TablePtr, TableHandle);

            if (Status != CFE_TBL_INFO_UPDATED)
            {
                BPLib_EM_SendEvent(BPNODE_TBL_ADDR_ERR_EID, BPLib_EM_EventType_ERROR,
                                    "Error Getting Table Address: %s, RC = 0x%08lX", TableName, (unsigned long)Status);
            }
        }
    }

    return Status;
}

CFE_Status_t BPA_TABLEP_TableUpdate()
{
    CFE_Status_t Status;

    Status = CFE_SUCCESS;

    /* Initialize the ADU proxy table */
    Status = BPA_TABLEP_TableManage("ADUProxyTable",
                                    (void*) BPNode_AppData.AduProxyTablePtr,
                                    BPNode_AppData.TableHandles[BPNODE_ADU_TBL_IDX]);

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PI channel configuration table */
        Status = BPA_TABLEP_TableManage("ChannelTable",
                                        (void*) BPLib_FWP_ConfigPtrs.ChanTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_CHAN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the CLA contacts table */
        Status = BPA_TABLEP_TableManage("ContactsTable",
                                        (void*) BPLib_FWP_ConfigPtrs.ContactsTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_CON_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the ARP CRS table */
        Status = BPA_TABLEP_TableManage("CRSTable",
                                        (void*) BPLib_FWP_ConfigPtrs.CrsTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_CRS_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custodian table */
        Status = BPA_TABLEP_TableManage("CustodianTable",
                                        (void*) BPLib_FWP_ConfigPtrs.CustodianTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_CSTDN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB custody table */
        Status = BPA_TABLEP_TableManage("CustodyTable",
                                        (void*) BPLib_FWP_ConfigPtrs.CustodyTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_CSTDY_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per node table */
        Status = BPA_TABLEP_TableManage("MIBConfigPNTable",
                                        (void*) BPLib_FWP_ConfigPtrs.MibPnTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_MIBN_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the NC MIB config per source table */
        Status = BPA_TABLEP_TableManage("MIBConfigPSTable",
                                        (void*) BPLib_FWP_ConfigPtrs.MibPsTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_MIBS_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB report to table */
        Status = BPA_TABLEP_TableManage("ReportToTable",
                                        (void*) BPLib_FWP_ConfigPtrs.ReportTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_REP_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the PDB authorized sources table */
        Status = BPA_TABLEP_TableManage("SrcAuthTable",
                                        (void*) BPLib_FWP_ConfigPtrs.AuthTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_AUTH_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Authorize the PDB source latency table */
        Status = BPA_TABLEP_TableManage("SrcLatencyTable",
                                        (void*) BPLib_FWP_ConfigPtrs.LatTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_LATE_TBL_IDX]);
    }

    if (Status == CFE_SUCCESS || Status == CFE_TBL_INFO_UPDATED)
    {
        /* Initialize the STOR storage table */
        Status = BPA_TABLEP_TableManage("StorageTable",
                                        (void*) BPLib_FWP_ConfigPtrs.StorTblPtr,
                                        BPNode_AppData.TableHandles[BPNODE_STOR_TBL_IDX]);
    }

    /* TODO: Set Table Handle to Node Configuration */

    return Status;
}

CFE_Status_t BPA_TABLEP_TableManage(const char* TableName, void* TablePtr, CFE_TBL_Handle_t TableHandle)
{
    CFE_Status_t Status;

    (void) CFE_TBL_ReleaseAddress(TableHandle);
    (void) CFE_TBL_Manage(TableHandle);
    Status = CFE_TBL_GetAddress((void **) &TablePtr, TableHandle);

    if (Status != CFE_SUCCESS && Status != CFE_TBL_INFO_UPDATED)
    {
        BPLib_EM_SendEvent(BPNODE_TBL_MNG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error managing the table: %s on wakeup, Status=0x%08X", TableName, Status);
    }

    return Status;
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