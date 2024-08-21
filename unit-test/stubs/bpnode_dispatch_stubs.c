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
 * Auto-Generated stub implementations for functions defined in bpnode_dispatch header
 */

#include "bpnode_dispatch.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ProcessGroundCommand()
 * ----------------------------------------------------
 */
void BPNode_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    UT_GenStub_AddParam(BPNode_ProcessGroundCommand, const CFE_SB_Buffer_t *, SBBufPtr);

    UT_GenStub_Execute(BPNode_ProcessGroundCommand, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_TaskPipe()
 * ----------------------------------------------------
 */
void BPNode_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    UT_GenStub_AddParam(BPNode_TaskPipe, const CFE_SB_Buffer_t *, SBBufPtr);

    UT_GenStub_Execute(BPNode_TaskPipe, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_VerifyCmdLength()
 * ----------------------------------------------------
 */
bool BPNode_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    UT_GenStub_SetupReturnBuffer(BPNode_VerifyCmdLength, bool);

    UT_GenStub_AddParam(BPNode_VerifyCmdLength, const CFE_MSG_Message_t *, MsgPtr);
    UT_GenStub_AddParam(BPNode_VerifyCmdLength, size_t, ExpectedLength);

    UT_GenStub_Execute(BPNode_VerifyCmdLength, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_VerifyCmdLength, bool);
}
