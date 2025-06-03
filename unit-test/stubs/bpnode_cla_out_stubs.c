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
 * Auto-Generated stub implementations for functions defined in bpnode_cla_out header
 */

#include "bpnode_cla_out.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOutCreateTasks()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ClaOutCreateTasks(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOutCreateTasks, CFE_Status_t);

    UT_GenStub_Execute(BPNode_ClaOutCreateTasks, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOutCreateTasks, CFE_Status_t);
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
 * Generated stub function for BPNode_ClaOut_DeleteSems()
 * ----------------------------------------------------
 */
void BPNode_ClaOut_DeleteSems(uint32 ContactId)
{
    UT_GenStub_AddParam(BPNode_ClaOut_DeleteSems, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_DeleteSems, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_ProcessBundleOutput()
 * ----------------------------------------------------
 */
int32 BPNode_ClaOut_ProcessBundleOutput(uint32 ContId, size_t *MsgSize)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_ProcessBundleOutput, int32);

    UT_GenStub_AddParam(BPNode_ClaOut_ProcessBundleOutput, uint32, ContId);
    UT_GenStub_AddParam(BPNode_ClaOut_ProcessBundleOutput, size_t *, MsgSize);

    UT_GenStub_Execute(BPNode_ClaOut_ProcessBundleOutput, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOut_ProcessBundleOutput, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_Setup()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaOut_Setup(uint32 ContactId, int32 PortNum, char *IpAddr)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_Setup, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaOut_Setup, uint32, ContactId);
    UT_GenStub_AddParam(BPNode_ClaOut_Setup, int32, PortNum);
    UT_GenStub_AddParam(BPNode_ClaOut_Setup, char *, IpAddr);

    UT_GenStub_Execute(BPNode_ClaOut_Setup, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOut_Setup, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_Start()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaOut_Start(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_Start, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaOut_Start, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_Start, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOut_Start, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_Stop()
 * ----------------------------------------------------
 */
BPLib_Status_t BPNode_ClaOut_Stop(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_Stop, BPLib_Status_t);

    UT_GenStub_AddParam(BPNode_ClaOut_Stop, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_Stop, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOut_Stop, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_TaskExit()
 * ----------------------------------------------------
 */
void BPNode_ClaOut_TaskExit(uint32 ContactId)
{
    UT_GenStub_AddParam(BPNode_ClaOut_TaskExit, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_TaskInit()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ClaOut_TaskInit(uint32 ContactId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ClaOut_TaskInit, CFE_Status_t);

    UT_GenStub_AddParam(BPNode_ClaOut_TaskInit, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_TaskInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ClaOut_TaskInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ClaOut_Teardown()
 * ----------------------------------------------------
 */
void BPNode_ClaOut_Teardown(uint32 ContactId)
{
    UT_GenStub_AddParam(BPNode_ClaOut_Teardown, uint32, ContactId);

    UT_GenStub_Execute(BPNode_ClaOut_Teardown, Basic, NULL);
}
