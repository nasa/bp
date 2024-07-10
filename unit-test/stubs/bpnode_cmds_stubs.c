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
 * Auto-Generated stub implementations for functions defined in bpnode_cmds header
 */

#include "bpnode_cmds.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NoopCmd()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_NoopCmd(const BPNode_NoopCmd_t *Msg)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NoopCmd, CFE_Status_t);

    UT_GenStub_AddParam(BPNode_NoopCmd, const BPNode_NoopCmd_t *, Msg);

    UT_GenStub_Execute(BPNode_NoopCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NoopCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_ResetCountersCmd()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_ResetCountersCmd(const BPNode_ResetCountersCmd_t *Msg)
{
    UT_GenStub_SetupReturnBuffer(BPNode_ResetCountersCmd, CFE_Status_t);

    UT_GenStub_AddParam(BPNode_ResetCountersCmd, const BPNode_ResetCountersCmd_t *, Msg);

    UT_GenStub_Execute(BPNode_ResetCountersCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_ResetCountersCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_SendHkCmd()
 * ----------------------------------------------------
 */
CFE_Status_t BPNode_SendHkCmd(const BPNode_SendHkCmd_t *Msg)
{
    UT_GenStub_SetupReturnBuffer(BPNode_SendHkCmd, CFE_Status_t);

    UT_GenStub_AddParam(BPNode_SendHkCmd, const BPNode_SendHkCmd_t *, Msg);

    UT_GenStub_Execute(BPNode_SendHkCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_SendHkCmd, CFE_Status_t);
}
