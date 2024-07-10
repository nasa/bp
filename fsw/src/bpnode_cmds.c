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
 * \file
 *   This file contains the source code for the BPNode command-handling functions
 */

/*
** Include Files:
*/
#include "bpnode_app.h"
#include "bpnode_cmds.h"
#include "bpnode_msgids.h"
#include "bpnode_eventids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"
#include "bpnode_msg.h"

#include "bplib.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t BPNode_SendHkCmd(const BPNode_SendHkCmd_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    BPNode_AppData.HkTlm.Payload.CommandErrorCounter = BPNode_AppData.ErrCounter;
    BPNode_AppData.HkTlm.Payload.CommandCounter      = BPNode_AppData.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < BPNODE_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(BPNode_AppData.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* BPNode Noop command                                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t BPNode_NoopCmd(const BPNode_NoopCmd_t *Msg)
{
    BPNode_AppData.CmdCounter++;

    CFE_EVS_SendEvent(BPNODE_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "Noop command %s",
                      BPNODE_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t BPNode_ResetCountersCmd(const BPNode_ResetCountersCmd_t *Msg)
{
    BPNode_AppData.CmdCounter = 0;
    BPNode_AppData.ErrCounter = 0;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "Reset counters command");

    return CFE_SUCCESS;
}
