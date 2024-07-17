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
** Global data
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
    CFE_SB_Buffer_t *BufPtr;

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

        /* Pend on receipt of wakeup message */
        Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.WakeupPipe, 
                                                BPNODE_WAKEUP_PIPE_TIMEOUT);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(BPNODE_PERF_ID);

        if (Status == CFE_SUCCESS)
        {
            /* Process wakeup tasks */
            Status = BPNode_ProcessMain();
        }
        
        if (Status != CFE_SUCCESS)
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

CFE_Status_t BPNode_ProcessMain(void)
{
    CFE_Status_t     Status = CFE_SUCCESS;
    CFE_SB_Buffer_t *BufPtr = NULL;

    do
    {
        Status = CFE_SB_ReceiveBuffer(&BufPtr, BPNode_AppData.CommandPipe, CFE_SB_POLL);

        if (Status == CFE_SUCCESS && BufPtr != NULL)
        {
            BPNode_TaskPipe(BufPtr);
        }
    } while (Status == CFE_SUCCESS);

    /* Not an error case */
    if (Status == CFE_SB_NO_MESSAGE)
    {
        Status = CFE_SUCCESS;
    }


    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t BPNode_AppInit(void)
{
    CFE_Status_t Status;

    /* Zero out the global data structure */
    memset(&BPNode_AppData, 0, sizeof(BPNode_AppData));

    BPNode_AppData.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Register the events
    */
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("BPNode: Error Registering Events, RC = 0x%08lX\n", 
                                                                (unsigned long)Status);
        return Status;
    }
    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.HkTlm.TelemetryHeader), 
                CFE_SB_ValueToMsgId(BPNODE_HK_TLM_MID), sizeof(BPNode_AppData.HkTlm));

    /*
    ** Create command pipe.
    */
    Status = CFE_SB_CreatePipe(&BPNode_AppData.CommandPipe, BPNODE_CMD_PIPE_DEPTH, 
                                            "BPNODE_CMD_PIPE");
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CR_CMD_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                "Error creating SB Command Pipe, RC = 0x%08lX", (unsigned long)Status);

        return Status;
    }

    /*
    ** Create wakeup pipe.
    */
    Status = CFE_SB_CreatePipe(&BPNode_AppData.WakeupPipe, BPNODE_WAKEUP_PIPE_DEPTH, 
                                            "BPNODE_WAKEUP_PIPE");
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CR_WKP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                    "Error creating SB Wakeup Pipe, RC = 0x%08lX", (unsigned long)Status);

        return Status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_SEND_HK_MID), 
                                                        BPNode_AppData.CommandPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_SUB_HK_ERR_EID, CFE_EVS_EventType_ERROR,
                "Error Subscribing to HK request, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    /*
    ** Subscribe to ground command packets
    */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_CMD_MID), 
                                                        BPNode_AppData.CommandPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_SUB_CMD_ERR_EID, CFE_EVS_EventType_ERROR,
                    "Error Subscribing to Commands, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    /*
    ** Subscribe to wakeup messages on the wakeup pipe
    */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_WAKEUP_MID), 
                                                        BPNode_AppData.WakeupPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_SUB_WKP_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error Subscribing to wakeup messages, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    /*
    ** Register and load table
    */
    Status = CFE_TBL_Register(&BPNode_AppData.TblHandles[0], "ExampleTable", 
            sizeof(BPNode_ExampleTable_t), CFE_TBL_OPT_DEFAULT, BPNode_TblValidationFunc);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_TBL_REG_ERR_EID, CFE_EVS_EventType_ERROR,
                "Error Registering Example Table, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    Status = CFE_TBL_Load(BPNode_AppData.TblHandles[0], CFE_TBL_SRC_FILE, BPNODE_TABLE_FILE);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_TBL_LD_ERR_EID, CFE_EVS_EventType_ERROR,
                    "Error Loading Example Table, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    Status = CFE_TBL_GetAddress((void *) &BPNode_AppData.ExampleTblPtr,
                                        BPNode_AppData.TblHandles[0]);

    if (Status != CFE_TBL_INFO_UPDATED)
    {
        CFE_EVS_SendEvent(BPNODE_TBL_ADDR_ERR_EID, CFE_EVS_EventType_ERROR,
                    "Error Getting Example Table Address, RC = 0x%08lX", (unsigned long)Status);
        return Status;
    }

    CFE_EVS_SendEvent(BPNODE_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, 
                            "BPNode App Initialized. Version %d.%d.%d.", 
                            BPNODE_MAJOR_VERSION, BPNODE_MINOR_VERSION, BPNODE_REVISION);

    return CFE_SUCCESS;
}
