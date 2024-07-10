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
 *   This file contains the source code for the BPNode App.
 */

/*
** Include Files:
*/
#include "bpnode_app.h"
#include "bpnode_cmds.h"
#include "bpnode_utils.h"
#include "bpnode_eventids.h"
#include "bpnode_dispatch.h"
#include "bpnode_tbl.h"
#include "bpnode_version.h"

/*
** global data
*/
BPNode_AppData_t BPNode_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void BPNode_AppMain(void)
{
    CFE_Status_t     Status;
    CFE_SB_Buffer_t *SBBufPtr;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(BPNODE_PERF_ID);

    /*
    ** Perform application-specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    Status = BPNode_AppInit();
    if (Status != CFE_SUCCESS)
    {
        BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** BPNode Runloop
    */
    while (CFE_ES_RunLoop(&BPNode_AppData.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(BPNODE_PERF_ID);

        /* Pend on receipt of command packet */
        Status = CFE_SB_ReceiveBuffer(&SBBufPtr, BPNode_AppData.CommandPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(BPNODE_PERF_ID);

        if (Status == CFE_SUCCESS)
        {
            BPNode_TaskPipe(SBBufPtr);
        }
        else
        {
            CFE_EVS_SendEvent(BPNODE_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "SB Pipe Read Error, App Will Exit");

            BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(BPNODE_PERF_ID);

    CFE_ES_ExitApp(BPNode_AppData.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t BPNode_AppInit(void)
{
    CFE_Status_t Status;
    char         VersionString[BPNODE_CFG_MAX_VERSION_STR_LEN];

    /* Zero out the global data structure */
    memset(&BPNode_AppData, 0, sizeof(BPNode_AppData));

    BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app configuration data
    */
    BPNode_AppData.PipeDepth = BPNODE_PIPE_DEPTH;

    strncpy(BPNode_AppData.PipeName, "BPNODE_CMD_PIPE", sizeof(BPNode_AppData.PipeName));
    BPNode_AppData.PipeName[sizeof(BPNode_AppData.PipeName) - 1] = 0;

    /*
    ** Register the events
    */
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BPNode: Error Registering Events, RC = 0x%08lX\n", (unsigned long)Status);
    }
    else
    {
        /*
         ** Initialize housekeeping packet (clear user data area).
         */
        CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_HK_TLM_MID),
                     sizeof(BPNode_AppData.HkTlm));

        /*
         ** Create Software Bus message pipe.
         */
        Status = CFE_SB_CreatePipe(&BPNode_AppData.CommandPipe, BPNode_AppData.PipeDepth, BPNode_AppData.PipeName);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_CR_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Error creating SB Command Pipe, RC = 0x%08lX", (unsigned long)Status);
        }
    }

    if (Status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to Housekeeping request commands
        */
        Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_SEND_HK_MID), BPNode_AppData.CommandPipe);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_SUB_HK_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)Status);
        }
    }

    if (Status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to ground command packets
        */
        Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_CMD_MID), BPNode_AppData.CommandPipe);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_SUB_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Error Subscribing to Commands, RC = 0x%08lX", (unsigned long)Status);
        }
    }

    if (Status == CFE_SUCCESS)
    {
        /*
        ** Register Example Table(s)
        */
        Status = CFE_TBL_Register(&BPNode_AppData.TblHandles[0], "ExampleTable", sizeof(BPNode_ExampleTable_t),
                                  CFE_TBL_OPT_DEFAULT, BPNode_TblValidationFunc);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_TABLE_REG_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Error Registering Example Table, RC = 0x%08lX", (unsigned long)Status);
        }
        else
        {
            Status = CFE_TBL_Load(BPNode_AppData.TblHandles[0], CFE_TBL_SRC_FILE, BPNODE_TABLE_FILE);
        }

        CFE_Config_GetVersionString(VersionString, BPNODE_CFG_MAX_VERSION_STR_LEN, "BPNode", BPNODE_VERSION,
                                    BPNODE_BUILD_CODENAME, BPNODE_LAST_OFFICIAL);

        CFE_EVS_SendEvent(BPNODE_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "BPNode App Initialized.%s",
                          VersionString);
    }

    return Status;
}
