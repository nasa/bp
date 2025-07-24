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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in bpnode_cla_in header
 */

#include "bpnode_cla_in.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaInCreateTasks()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ClaInCreateTasks(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaInCreateTasks, CFE_Status_t);

    UT_GenStub_Execute(BPNode_ClaInCreateTasks, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaInCreateTasks, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_AppMain()
 * ----------------------------------------------------
 */
void BPNode_ClaIn_AppMain(void)
{

    UT_GenStub_Execute(BPNode_ClaIn_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_ProcessBundleInput()
 * ----------------------------------------------------
 */
int32 BPNode_ClaIn_ProcessBundleInput(uint32 ContId, size_t *BundleSize)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaIn_ProcessBundleInput, int32);

    UT_GenStub_AddParam(BPNode_ClaIn_ProcessBundleInput, uint32, ContId);
    UT_GenStub_AddParam(BPNode_ClaIn_ProcessBundleInput, size_t *, BundleSize);

    UT_GenStub_Execute(BPNode_ClaIn_ProcessBundleInput, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaIn_ProcessBundleInput, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_Setup()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaIn_Setup(uint32 ContactId, int32 PortNum, const char *IpAddr)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaIn_Setup, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaIn_Setup, uint32, ContactId);
    UT_GenStub_AddParam(BPNode_ClaIn_Setup, int32, PortNum);
    UT_GenStub_AddParam(BPNode_ClaIn_Setup, const char *, IpAddr);

    UT_GenStub_Execute(BPNode_ClaIn_Setup, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaIn_Setup, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_Start()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaIn_Start(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaIn_Start, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaIn_Start, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaIn_Start, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaIn_Start, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_Stop()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaIn_Stop(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaIn_Stop, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaIn_Stop, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaIn_Stop, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaIn_Stop, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_TaskExit()
 * ----------------------------------------------------
 */
void BPNode_ClaIn_TaskExit(uint32 ContactId)
{
    UT_GenStub_AddParam(BPNode_ClaIn_TaskExit, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaIn_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_TaskInit()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ClaIn_TaskInit(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaIn_TaskInit, CFE_Status_t);

    UT_GenStub_AddParam(BPNode_ClaIn_TaskInit, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaIn_TaskInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaIn_TaskInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaIn_Teardown()
 * ----------------------------------------------------
 */
void BPNode_ClaIn_Teardown(uint32 ContactId)
{
    UT_GenStub_AddParam(BPNode_ClaIn_Teardown, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaIn_Teardown, Basic, NULL);
}
