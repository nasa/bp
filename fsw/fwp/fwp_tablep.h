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
** @file
**   FWP's Table Proxy (TABLEP) header definitions
*/

#ifndef FWP_TABLEP_H
#define FWP_TABLEP_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "bpnode_eventids.h"
#include "bpnode_tbl.h"

/* ====== */
/* Macros */
/* ====== */

#define BPNODE_ADU_TBL_IDX      0
#define BPNODE_CHAN_TBL_IDX     1
#define BPNODE_CON_TBL_IDX      2
#define BPNODE_CRS_TBL_IDX      3
#define BPNODE_CSTDN_TBL_IDX    4
#define BPNODE_CSTDY_TBL_IDX    5
#define BPNODE_MIBN_TBL_IDX     6
#define BPNODE_MIBS_TBL_IDX     7
#define BPNODE_REP_TBL_IDX      8
#define BPNODE_AUTH_TBL_IDX     9
#define BPNODE_LATE_TBL_IDX    10
#define BPNODE_STOR_TBL_IDX    11

/* =========== */
/* Global Data */
/* =========== */

extern BPLib_FWP_ConfigPtrs_t BPNode_ConfigPtrs;

/* =================== */
/* Function Prototypes */
/* =================== */

CFE_Status_t BPA_TABLEP_TableInit(void);

CFE_Status_t BPA_TABLEP_SingleTableInit(const char* TableName, const char* TableFileName, size_t Size, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr, void** TablePtr, CFE_TBL_Handle_t* TableHandle);

BPLib_Status_t BPA_TABLEP_TableUpdate(BPLib_NC_TableType_t TableType, void** TblPtr);

BPLib_Status_t BPA_TABLEP_TableManage(const char* TableName, void** TablePtr, CFE_TBL_Handle_t TableHandle);

BPLib_Status_t BPA_TABLEP_SingleTableUpdate(CFE_TBL_Handle_t TblHandle);

/**
 * \brief     Wrapper to translate return from BPLib_PI_ValidateConfigs into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PI_ValidateConfigs()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PI_ValidateConfigs
 */
CFE_Status_t BPA_TABLEP_PI_ValidateConfigs(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_CLA_ContactsTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_CLA_ContactsTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_CLA_ContactsTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_CLA_ContactsTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return BPLibABLEP_ARP_CRSTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_ARP_CRSTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_ARP_CRSTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_ARP_CRSTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_PDB_CustodianAuthTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PDB_CustodianAuthTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PDB_CustodianAuthTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_PDB_CustodianAuthTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_PDB_CustodyAuthTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PDB_CustodyAuthTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PDB_CustodyAuthTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_PDB_CustodyAuthTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_NC_MIBConfigPNTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_NC_MIBConfigPNTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_NC_MIBConfigPNTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_NC_MIBConfigPNTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_NC_MIBConfigPSTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_NC_MIBConfigPSTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_NC_MIBConfigPSTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_NC_MIBConfigPSTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_PDB_ReportToAuthTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PDB_ReportToAuthTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PDB_ReportToAuthTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_PDB_ReportToAuthTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_PDB_SrcAuthTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PDB_SrcAuthTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PDB_SrcAuthTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_PDB_SrcAuthTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_PDB_SrcLatencyTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_PDB_SrcLatencyTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_PDB_SrcLatencyTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_PDB_SrcLatencyTblValidateFunc(void* TblData);

/**
 * \brief     Wrapper to translate return from BPLib_STOR_StorageTblValidateFunc() into a CFE_Statuts_t type
 * \details   Translate the BPLib_Status_t-type return status from BPLib_STOR_StorageTblValidateFunc()
 *            into a CFE_Status_t-type for CFE_TBL_Register()
 * \note      A pointer to this function will be used in TblNameParamsArr0
 * \param[in] TblData (void*) Table to validate
 * \return    Execution status translated into a CFE_Status_t type
 * \anchor    BPA_TABLEP_STOR_StorageTblValidateFunc
 */
CFE_Status_t BPA_TABLEP_STOR_StorageTblValidateFunc(void* TblData);

#endif /* FWP_TABLEP_H */