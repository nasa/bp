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
 * National Aeronautics and Space Administration. All Rights Reserved.
 *
 */

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in bpnode_app header
 */

#include "bpnode_app.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AppExit()
 * ----------------------------------------------------
 */
void BPNode_AppExit(void)
{
    UT_GenStub_Execute(BPNode_AppExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AppInit()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_AppInit(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_AppInit, CFE_Status_t);

    UT_GenStub_Execute(BPNode_AppInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_AppInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_AppMain()
 * ----------------------------------------------------
 */
void BPNode_AppMain(void)
{

    UT_GenStub_Execute(BPNode_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_WakeupProcess()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_WakeupProcess(void)
{
    UT_GenStub_SetupReturnBuffer(BPNode_WakeupProcess, CFE_Status_t);

    UT_GenStub_Execute(BPNode_WakeupProcess, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_WakeupProcess, CFE_Status_t);
}
