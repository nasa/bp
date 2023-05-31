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

#ifndef BP_FLOW_H
#define BP_FLOW_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bp_platform_cfg.h"
#include "bp_tbl.h"
#include "bplib.h"
#include "bp_io.h"
#include "bp_storage.h"

/************************************************
 * Defines
 ************************************************/

#define BP_FLOWHANDLE_C(x)  ((BP_FlowHandle_t)CFE_RESOURCEID_WRAP(x))
#define BP_INVALID_FLOW     BP_FLOWHANDLE_C(CFE_RESOURCEID_UNDEFINED)
#define BP_INVALID_PRIORITY (-1)
#define BP_INVALID_STATUS   (-1)
#define BP_FLOW_TBL_NAME    "FlowTable"

/************************************************
 * Typedefs
 ************************************************/

typedef CFE_RESOURCEID_BASE_TYPE BP_FlowHandle_t;

/* Throttling (Semaphores) */

typedef struct
{
    const char     Name[BP_SEM_NAME_SIZE];
    CFE_SB_MsgId_t MsgId;
    uint32         Max;
    uint32         Throttle;
    osal_id_t      Sem;
} BP_Throttle_t;

extern BP_Throttle_t BP_Throttles[];

typedef struct
{
    int             Priority;
    BP_FlowHandle_t NextFlow;
    BP_FlowHandle_t NextLevel;
} BP_FlowPriority_t;

typedef struct
{
    BP_FlowHandle_t    Handle;
    BP_FlowTblEntry_t  Config;
    bool               Enabled;
    bool               Active;
    bool               Healthy;
    BP_StorageHandle_t StorageHandle;
    BP_FlowPriority_t  COS; /* class of service */
    bp_socket_t       *BPS; /* bundle protocol library socket */
    CFE_SB_PipeId_t    DataPipe;

    CFE_SB_Buffer_t *CurrentSbMsgInPtr;
    CFE_SB_Buffer_t *CurrentSbMsgOutPtr;

    CFE_SB_MsgId_t       CurrentSbMsgInId;
    const unsigned char *CurrentChunkInPtr;
    size_t               CurrentChunkInSize;

    unsigned char *CurrentChunkOutPtr;
    size_t         CurrentChunkOutSize;

    int      DataInDropped;
    int      DataOutDropped;
    uint32_t LibFlags;
} BP_FlowCtrlEntry_t;

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BP_FlowInit
 *
 * Initializes a flow module.  Called once at startup
 *----------------------------------------------*/
int32 BP_FlowInit(const char *AppName);

/*-----------------------------------------------
 * BP_FlowLoad
 *
 * Initializes all flows in the flow table
 *----------------------------------------------*/
int32 BP_FlowLoad(const char *FlowTableFileName);

/*-----------------------------------------------
 * BP_FlowEnable
 *
 * Enable the provided flow
 *----------------------------------------------*/
int32 BP_FlowEnable(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowDisable
 *
 * Disable provided flow
 *----------------------------------------------*/
int32 BP_FlowDisable(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowSetTimeout
 *
 * Sets Timeout on provided flow
 *----------------------------------------------*/
int32 BP_FlowSetTimeout(BP_FlowHandle_t Flow, int Timeout);

/*-----------------------------------------------
 * BP_FlowRevertTimeout
 *
 * Sets Timeout on provided flow
 *----------------------------------------------*/
int32 BP_FlowRevertTimeout(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowSetPriority
 *
 * Sets priority on provided flow
 *----------------------------------------------*/
int32 BP_FlowSetPriority(BP_FlowHandle_t Flow, int Priority);

/*-----------------------------------------------
 * BP_FlowRevertPriority
 *
 * Sets Timeout on provided flow
 *----------------------------------------------*/
int32 BP_FlowRevertPriority(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowIsEnabled
 *
 * Returns true if the flow is enabled
 *----------------------------------------------*/
bool BP_FlowIsEnabled(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowGetStats
 *
 * Gets counters and status flags from flow  on provided flow
 *----------------------------------------------*/
int32 BP_FlowGetStats(BP_FlowHandle_t Flow, BP_FlowStats_t *Stats);

/*-----------------------------------------------
 * BP_FlowClearStats
 *
 * Reset counters and status flags to a known reset state
 * on provided flow
 *----------------------------------------------*/
int32 BP_FlowClearStats(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowGetHandle
 *
 * Given a flow name, returns the flow handle
 *----------------------------------------------*/
BP_FlowHandle_t BP_FlowGetHandle(const char *Name);

/*-----------------------------------------------
 * BP_FlowGetName
 *
 * Returns pointer to the name of the flow
 *----------------------------------------------*/
const char *BP_FlowGetName(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowFlush
 *
 * Clear out any pending packets and stored bundles
 * on provided flow
 *----------------------------------------------*/
int32 BP_FlowFlush(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowPause
 *
 * Stops the receipt of and transmission of data and bundles
 *----------------------------------------------*/
int32 BP_FlowPause(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowResume
 *
 * Starts the receipt of and transmission of data and bundles
 *----------------------------------------------*/
int32 BP_FlowResume(BP_FlowHandle_t Flow);

/*-----------------------------------------------
 * BP_FlowProcess
 *
 * Read packets from software bus and send them as bundles on IO
 * Read bundles from IO and send them as packets on software bus
 *----------------------------------------------*/
int32 BP_FlowProcess(void);

/*-----------------------------------------------
 * BP_FlowDirectStore
 *
 * Pass through to directly store data on flow
 *----------------------------------------------*/
int32 BP_FlowDirectStore(BP_FlowHandle_t Flow, uint8 *Buffer, int Len);

/*-----------------------------------------------
 * BP_FlowDirectConfig
 *
 * Pass through to directly configure the flow's bplib channel
 *----------------------------------------------*/
int32 BP_FlowDirectConfig(BP_FlowHandle_t Flow, int Mode, int Option, int *Value);

int32 BP_FlowHandle_ToIndex(BP_FlowHandle_t Flow, uint32 *Idx);

/*-----------------------------------------------
 * BP_ForEachFlow
 *
 * Call a sub-function for each flow that is enabled
 *----------------------------------------------*/
void BP_ForEachFlow(void (*Func)(BP_FlowHandle_t, void *Arg), void *Arg);

#endif /* BP_FLOW_H */
