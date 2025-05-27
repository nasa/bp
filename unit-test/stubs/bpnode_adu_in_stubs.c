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
 * Auto-Generated stub implementations for functions defined in bpnode_adu_in header
 */

#include "bpnode_adu_in.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduInCreateTasks()
 * ----------------------------------------------------
 */
int32 BPNode_AduInCreateTasks(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_AduInCreateTasks, int32);

    UT_GenStub_Execute(BPNode_AduInCreateTasks, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_AduInCreateTasks, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduIn_AppMain()
 * ----------------------------------------------------
 */
void BPNode_AduIn_AppMain(void)
{

    UT_GenStub_Execute(BPNode_AduIn_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduIn_TaskExit()
 * ----------------------------------------------------
 */
void BPNode_AduIn_TaskExit(uint32 ChanId)
{
    UT_GenStub_AddParam(BPNode_AduIn_TaskExit, uint32, ChanId);

    UT_GenStub_Execute(BPNode_AduIn_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduIn_TaskInit()
 * ----------------------------------------------------
 */
int32 BPNode_AduIn_TaskInit(uint32 *ChanId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_AduIn_TaskInit, int32);

    UT_GenStub_AddParam(BPNode_AduIn_TaskInit, uint32 *, ChanId);

    UT_GenStub_Execute(BPNode_AduIn_TaskInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_AduIn_TaskInit, int32);
}
