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
 * Auto-Generated stub implementations for functions defined in bpnode_cla_out
 * header
 */

#include "bpnode_cla_out.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_CLA_ProcessBundleOutput()
 * ----------------------------------------------------
 */
int32 BPNode_CLA_ProcessBundleOutput(BPNode_ClaOutData_t *CLAEgress,
                                            uint8 ContId) 
{
  UT_GenStub_SetupReturnBuffer(BPNode_CLA_ProcessBundleOutput, int32);

  UT_GenStub_AddParam(BPNode_CLA_ProcessBundleOutput, BPNode_ClaOutData_t *,
                      CLAEgress);
  UT_GenStub_AddParam(BPNode_CLA_ProcessBundleOutput, uint8, ContId);

  UT_GenStub_Execute(BPNode_CLA_ProcessBundleOutput, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_CLA_ProcessBundleOutput,
                                   int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOutCreateTasks()
 * ----------------------------------------------------
 */
int32 BPNode_ClaOutCreateTasks(void) 
{
  UT_GenStub_SetupReturnBuffer(BPNode_ClaOutCreateTasks, int32);

  UT_GenStub_Execute(BPNode_ClaOutCreateTasks, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ClaOutCreateTasks, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_AppMain()
 * ----------------------------------------------------
 */
void BPNode_ClaOut_AppMain(void) 
{

  UT_GenStub_Execute(BPNode_ClaOut_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_TaskExit()
 * ----------------------------------------------------
 */
void BPNode_ClaOut_TaskExit(uint8 ContId) 
{
  UT_GenStub_AddParam(BPNode_ClaOut_TaskExit, uint8, ContId);

  UT_GenStub_Execute(BPNode_ClaOut_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_TaskInit()
 * ----------------------------------------------------
 */
int32 BPNode_ClaOut_TaskInit(uint8 *ContId) 
{
  UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_TaskInit, int32);

  UT_GenStub_AddParam(BPNode_ClaOut_TaskInit, uint8 *, ContId);

  UT_GenStub_Execute(BPNode_ClaOut_TaskInit, Basic, NULL);

  return UT_GenStub_GetReturnValue(BPNode_ClaOut_TaskInit, int32);
}
