/*
 * NASA Docket No. GSC-18,587-1 and identified as “The Bundle Protocol Core
 * Flight System Application (BP) v6.5”
 *
 * Copyright © 2020 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration. All Rights Reserved.
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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in
 * bpnode_cla_bundle_io header
 */

#include "bpnode_cla_bundle_io.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_Init()
 * ----------------------------------------------------
 */
CFE_Status_t BPNODE_CLA_Init(void) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_Init, CFE_Status_t);

  UT_GenStub_Execute(BPNODE_CLA_Init, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_Init, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_InitInput()
 * ----------------------------------------------------
 */
CFE_Status_t BPNODE_CLA_InitInput(BPNODE_CLA_BundleFlowEntry_t *Entry) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_InitInput, CFE_Status_t);

  UT_GenStub_AddParam(BPNODE_CLA_InitInput, BPNODE_CLA_BundleFlowEntry_t *,
                      Entry);

  UT_GenStub_Execute(BPNODE_CLA_InitInput, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_InitInput, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_InitOutput()
 * ----------------------------------------------------
 */
CFE_Status_t BPNODE_CLA_InitOutput(BPNODE_CLA_BundleFlowEntry_t *Entry) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_InitOutput, CFE_Status_t);

  UT_GenStub_AddParam(BPNODE_CLA_InitOutput, BPNODE_CLA_BundleFlowEntry_t *,
                      Entry);

  UT_GenStub_Execute(BPNODE_CLA_InitOutput, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_InitOutput, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_InputTask()
 * ----------------------------------------------------
 */
void BPNODE_CLA_InputTask(void) 
{

  UT_GenStub_Execute(BPNODE_CLA_InputTask, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_OutputTask()
 * ----------------------------------------------------
 */
void BPNODE_CLA_OutputTask(void) 
{

  UT_GenStub_Execute(BPNODE_CLA_OutputTask, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_ProcessBundleInput()
 * ----------------------------------------------------
 */
CFE_Status_t BPNODE_CLA_ProcessBundleInput(BPNODE_CLA_BundleFlowEntry_t *Entry) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_ProcessBundleInput, CFE_Status_t);

  UT_GenStub_AddParam(BPNODE_CLA_ProcessBundleInput,
                      BPNODE_CLA_BundleFlowEntry_t *, Entry);

  UT_GenStub_Execute(BPNODE_CLA_ProcessBundleInput, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_ProcessBundleInput, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_ProcessBundleOutput()
 * ----------------------------------------------------
 */
CFE_Status_t BPNODE_CLA_ProcessBundleOutput(BPNODE_CLA_BundleFlowEntry_t *Entry) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_ProcessBundleOutput, CFE_Status_t);

  UT_GenStub_AddParam(BPNODE_CLA_ProcessBundleOutput,
                      BPNODE_CLA_BundleFlowEntry_t *, Entry);

  UT_GenStub_Execute(BPNODE_CLA_ProcessBundleOutput, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_ProcessBundleOutput,
                                   CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNODE_CLA_SetupLibIntf()
 * ----------------------------------------------------
 */
int32 BPNODE_CLA_SetupLibIntf(void) 
{
  UT_GenStub_SetupReturnBuffer(BPNODE_CLA_SetupLibIntf, int32);

  UT_GenStub_Execute(BPNODE_CLA_SetupLibIntf, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNODE_CLA_SetupLibIntf, int32);
}
