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

#ifndef BP_IO_H
#define BP_IO_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"

/************************************************
 * Defines
 ************************************************/

#define BP_IOHANDLE_C(x)         ((BP_IoHandle_t)CFE_RESOURCEID_WRAP(x))
#define BP_IO_INVALID_DESCRIPTOR BP_IOHANDLE_C(CFE_RESOURCEID_UNDEFINED)

/************************************************
 * Typedefs
 ************************************************/

typedef CFE_RESOURCEID_BASE_TYPE BP_IoHandle_t;

typedef struct
{
    int8   SendHealthy;
    int8   ReceiveHealthy;
    uint16 SendNotReady;
    uint32 BytesSentPerSecond;
    uint32 BytesReceivedPerSecond;
} BP_IOStats_t;

/*
 * IO Parameter Structure
 */
typedef struct
{
    CFE_SB_MsgId_t BundleOutMID;
    CFE_SB_MsgId_t BundleInMID;
    int            BundleInPipeDepth;
} BP_IOConfig_t;

/* IO Control Structure */
typedef struct
{
    BP_IoHandle_t    Handle;
    BP_IOConfig_t    Config;
    CFE_SB_PipeId_t  BundleInPipe;
    CFE_SB_Buffer_t *BundleInMsgBuf;
    int              SendInError;    /* bool */
    int              ReceiveInError; /* bool */
    uint32           SendNotReady;
    uint32           BytesSent;
    uint32           BytesReceived;
    double           LastStatsSeconds;
} BP_IOCtrl_t;

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BP_IOInit
 *
 * Must be called before any other call
 *----------------------------------------------*/
int32 BP_IOInit(void);

/*-----------------------------------------------
 * BP_IOOpen
 *
 * Returns a decriptor to an IO control block
 *----------------------------------------------*/
int32 BP_IOOpen(const char *IOParm, BP_IoHandle_t *ioh);

/*-----------------------------------------------
 * BP_IOClose
 *
 * Close an IO control block
 *----------------------------------------------*/
int32 BP_IOClose(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * BP_IOGetStats
 *
 * Gets counters and status flags.
 *----------------------------------------------*/
int32 BP_IOGetStats(BP_IoHandle_t ioh, BP_IOStats_t *Stats);

/*-----------------------------------------------
 * BP_IOClearStats
 *
 * Reset counters and status flags to a known reset state.
 *
 * Notes:
 *   1. Any counter or variable that is reported in HK telemetry
 *      that doesn't change the functional behavior is reset.
 *----------------------------------------------*/
int32 BP_IOClearStats(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * BP_IOFlush
 *
 * Clear out any pending packets
 *----------------------------------------------*/
int32 BP_IOFlush(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * BP_IOReadBundle
 *
 * Read bundles to be processed
 *----------------------------------------------*/
bool BP_IOReadBundle(BP_IoHandle_t ioh, void **InBuf, size_t *InBufSize);

/*-----------------------------------------------
 * BP_IOWriteBundle
 *
 * Write bundles
 *----------------------------------------------*/
bool BP_IOWriteBundle(BP_IoHandle_t ioh, const void *OutBuf, size_t OutBufSize);

#endif /* BP_IO_H */
