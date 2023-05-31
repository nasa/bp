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
#include "iodriver_base.h"
#include "bp_msg.h"

/************************************************
 * Defines
 ************************************************/

#define BP_IOHANDLE_C(x)         ((BP_IoHandle_t)CFE_RESOURCEID_WRAP(x))
#define BP_IO_INVALID_DESCRIPTOR BP_IOHANDLE_C(CFE_RESOURCEID_UNDEFINED)

/************************************************
 * Typedefs
 ************************************************/

typedef struct
{
    void  *BaseMem;
    size_t MaxSize;
    size_t CurrentSize;
} BP_BundleBuffer_t;

typedef CFE_RESOURCEID_BASE_TYPE BP_IoHandle_t;

/* IO Control Structure */
typedef struct
{
    BP_IoHandle_t Handle;

    CFE_PSP_IODriver_Location_t Location;

    int    SendInError;    /* bool */
    int    ReceiveInError; /* bool */
    uint32 SendNotReady;
    uint32 BytesSent;
    uint32 BytesReceived;
    double LastStatsSeconds;
} BP_IOCtrl_t;

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * Must be called before any other call
 *----------------------------------------------*/
int32 BP_IOInit(void);

/*-----------------------------------------------
 * Returns a decriptor to an IO control block
 *----------------------------------------------*/
int32 BP_IOOpen(const char *IOParm, BP_BundleBuffer_t *Buffer, BP_IoHandle_t *ioh);

/*-----------------------------------------------
 * Close an IO control block
 *----------------------------------------------*/
int32 BP_IOClose(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * Gets counters and status flags.
 *----------------------------------------------*/
int32 BP_IOGetStats(BP_IoHandle_t ioh, BP_IOStats_t *Stats);

/*-----------------------------------------------
 * Reset counters and status flags to a known reset state.
 *
 * Notes:
 *   1. Any counter or variable that is reported in HK telemetry
 *      that doesn't change the functional behavior is reset.
 *----------------------------------------------*/
int32 BP_IOClearStats(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * Clear out any pending packets
 *----------------------------------------------*/
int32 BP_IOFlush(BP_IoHandle_t ioh);

/*-----------------------------------------------
 * Read bundles to be processed
 *----------------------------------------------*/
bool BP_IOReadBundle(BP_IoHandle_t ioh, BP_BundleBuffer_t *InBuf);

/*-----------------------------------------------
 * Write bundles
 *----------------------------------------------*/
bool BP_IOWriteBundle(BP_IoHandle_t ioh, const BP_BundleBuffer_t *OutBuf);

#endif /* BP_IO_H */
