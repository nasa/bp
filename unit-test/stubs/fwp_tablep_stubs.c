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
