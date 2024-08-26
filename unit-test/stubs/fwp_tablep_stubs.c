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
**   This file contains the source code for the Table Proxy stubs
*/

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in fwp_tablep
 * header
 */

#include "fwp_tablep.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TableP_TableInit()
 * ----------------------------------------------------
 */
CFE_Status_t BPA_TableP_TableInit(void) {
  UT_GenStub_SetupReturnBuffer(BPA_TableP_TableInit, CFE_Status_t);

  UT_GenStub_Execute(BPA_TableP_TableInit, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPA_TableP_TableInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TableP_TableUpdate()
 * ----------------------------------------------------
 */
CFE_Status_t BPA_TableP_TableUpdate(void) {
  UT_GenStub_SetupReturnBuffer(BPA_TableP_TableUpdate, CFE_Status_t);

  UT_GenStub_Execute(BPA_TableP_TableUpdate, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPA_TableP_TableUpdate, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TableP_SingleTableUpdate()
 * ----------------------------------------------------
 */
CFE_Status_t BPA_TableP_SingleTableUpdate(CFE_TBL_Handle_t TblHandle) {
  UT_GenStub_SetupReturnBuffer(BPA_TableP_SingleTableUpdate, CFE_Status_t);

  UT_GenStub_AddParam(BPA_TableP_SingleTableUpdate, CFE_TBL_Handle_t,
                      TblHandle);

  UT_GenStub_Execute(BPA_TableP_SingleTableUpdate, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPA_TableP_SingleTableUpdate, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ADUPTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ADUPTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_ADUPTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_ADUPTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_ADUPTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ADUPTblValidateFunc, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_CRSTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_CRSTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_CRSTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_CRSTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_CRSTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_CRSTblValidateFunc, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ChannelConfigTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ChannelConfigTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_ChannelConfigTblValidateFunc,
                               CFE_Status_t);

  UT_GenStub_AddParam(BPNode_ChannelConfigTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_ChannelConfigTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ChannelConfigTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ContactsTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ContactsTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_ContactsTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_ContactsTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_ContactsTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ContactsTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_CustodianAuthTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_CustodianAuthTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_CustodianAuthTblValidateFunc,
                               CFE_Status_t);

  UT_GenStub_AddParam(BPNode_CustodianAuthTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_CustodianAuthTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_CustodianAuthTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_CustodyAuthTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_CustodyAuthTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_CustodyAuthTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_CustodyAuthTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_CustodyAuthTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_CustodyAuthTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_MIBConfigPNTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_MIBConfigPNTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_MIBConfigPNTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_MIBConfigPNTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_MIBConfigPNTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_MIBConfigPNTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_MIBConfigPSTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_MIBConfigPSTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_MIBConfigPSTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_MIBConfigPSTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_MIBConfigPSTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_MIBConfigPSTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ReportToAuthTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ReportToAuthTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_ReportToAuthTblValidateFunc,
                               CFE_Status_t);

  UT_GenStub_AddParam(BPNode_ReportToAuthTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_ReportToAuthTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ReportToAuthTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_SrcAuthTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_SrcAuthTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_SrcAuthTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_SrcAuthTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_SrcAuthTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_SrcAuthTblValidateFunc, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_SrcLatencyTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_SrcLatencyTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_SrcLatencyTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_SrcLatencyTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_SrcLatencyTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_SrcLatencyTblValidateFunc,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_StorageTblValidateFunc()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_StorageTblValidateFunc(void *TblData) {
  UT_GenStub_SetupReturnBuffer(BPNode_StorageTblValidateFunc, CFE_Status_t);

  UT_GenStub_AddParam(BPNode_StorageTblValidateFunc, void *, TblData);

  UT_GenStub_Execute(BPNode_StorageTblValidateFunc, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_StorageTblValidateFunc, CFE_Status_t);
}