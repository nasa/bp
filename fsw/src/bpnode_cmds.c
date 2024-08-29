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
 * \file
 *   This file contains the source code for the BPNode command-handling functions
 */


/*
** Include Files
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


/*
** Function Definitions
*/

/* Send Node MIB Counters housekeeping command */
CFE_Status_t BPNode_SendNodeMibCountersHkCmd(const BPNode_SendNodeMibCountersHkCmd_t *Msg)
{
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);

    BPNode_AppData.NodeMibCountersHkTlm.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkTlm.TelemetryHeader), true);

    return CFE_SUCCESS;
}


/* Noop command */
CFE_Status_t BPNode_NoopCmd(const BPNode_NoopCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount++;

    CFE_EVS_SendEvent(BPNODE_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, 
                        "No-op command. Version %d.%d.%d.",
                        BPNODE_MAJOR_VERSION, BPNODE_MINOR_VERSION, BPNODE_REVISION);

    return CFE_SUCCESS;
}


/* Reset all counters command */
CFE_Status_t BPNode_ResetAllCountersCmd(const BPNode_ResetAllCountersCmd_t *Msg)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AcceptedDirectiveCount = 0;
    BPNode_AppData.NodeMibCountersHkTlm.Payload.RejectedDirectiveCount = 0;

    CFE_EVS_SendEvent(BPNODE_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, 
                    "Reset all counters command");

    return CFE_SUCCESS;
}
