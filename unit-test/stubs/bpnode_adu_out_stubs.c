/*
 * NASA Docket No. GSC-19,559-1, and identified as "Delay/Disruption Tolerant Networking 
 * (DTN) Bundle Protocol (BP) v7 Core Flight System (cFS) Application Build 7.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software distributed under 
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF 
 * ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. The copyright notice to be 
 * included in the software is as follows: 
 *
 * Copyright 2025 United States Government as represented by the Administrator of the 
 * National Aeronautics and Space Administration. All Rights Reserved. */

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in bpnode_adu_out header
 */

#include "bpnode_adu_out.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduOutCreateTasks()
 * ----------------------------------------------------
 */
int32 BPNode_AduOutCreateTasks(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_AduOutCreateTasks, int32);

    UT_GenStub_Execute(BPNode_AduOutCreateTasks, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_AduOutCreateTasks, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduOut_AppMain()
 * ----------------------------------------------------
 */
void BPNode_AduOut_AppMain(void)
{

    UT_GenStub_Execute(BPNode_AduOut_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduOut_TaskExit()
 * ----------------------------------------------------
 */
void BPNode_AduOut_TaskExit(uint32 ChanId)
{
    UT_GenStub_AddParam(BPNode_AduOut_TaskExit, uint32, ChanId);

    UT_GenStub_Execute(BPNode_AduOut_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AduOut_TaskInit()
 * ----------------------------------------------------
 */
int32 BPNode_AduOut_TaskInit(uint32 *ChanId)
{
    UT_GenStub_SetupReturnBuffer(BPNode_AduOut_TaskInit, int32);

    UT_GenStub_AddParam(BPNode_AduOut_TaskInit, uint32 *, ChanId);

    UT_GenStub_Execute(BPNode_AduOut_TaskInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_AduOut_TaskInit, int32);
}
