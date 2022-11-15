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

/************************************************
 * Includes
 ************************************************/

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "cfe.h"
#include "bp_events.h"
#include "bp_cfg.h"
#include "bp_io.h"
#include "bp_global.h"
#include "cfe_resourceid_basevalue.h"

#include "iodriver_base.h"
#include "iodriver_packet_io.h"

/************************************************
 * Defines
 ************************************************/

#define BP_IO_CFGSTR_MAX_SIZE 256
#define BP_IO_PSP_MODULE_NAME "ModName"
#define BP_IO_PSP_BUFFER_SIZE "BufSize"

/************************************************
 * Typedefs
 ************************************************/

typedef struct
{
    char   ModuleName[32];
    size_t BundleBufferSize;
} BP_PspIOConfig_t;

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

/*
 * Base for unique-ish BP Io ID handles. This is just a random
 * offset from OS_OBJECT_ID_USER.
 *
 * This is chosen to not overlap with default CFE resource IDs, but
 * even if there is overlap, or if another CFS app chooses the same
 * random value, it is not going to break anything because type
 * checking still applies.  So an overlap/collision just slightly weakens
 * the protection offered here, but it doesn't cause a problem.
 */
enum
{
    BP_IO_HANDLE_BASE = CFE_RESOURCEID_MAKE_BASE(OS_OBJECT_TYPE_USER + 20)
};

static int32 BP_IoHandle_ToIndex(BP_IoHandle_t fh, uint32 *Idx)
{
    return CFE_ResourceId_ToIndex(CFE_RESOURCEID_UNWRAP(fh), BP_IO_HANDLE_BASE, BP_MAX_IO_DESC, Idx);
}

static BP_IOCtrl_t *BP_LocateIoEntryByHandle(BP_IoHandle_t fh)
{
    BP_IOCtrl_t *IoPtr;
    uint32       Idx;

    if (BP_IoHandle_ToIndex(fh, &Idx) == CFE_SUCCESS)
    {
        IoPtr = &BP_GlobalData.IOControl[Idx];
    }
    else
    {
        IoPtr = NULL;
    }

    return IoPtr;
}

static inline bool BP_IoEntryIsMatch(const BP_IOCtrl_t *IoPtr, BP_IoHandle_t fh)
{
    return (IoPtr != NULL && CFE_RESOURCEID_TEST_EQUAL(IoPtr->Handle, fh));
}

/*----------------------------------------------------------------
 *
 * Function: CFE_ES_CheckAppIdSlotUsed
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool BP_CheckIoSlotUsed(CFE_ResourceId_t CheckId)
{
    BP_IOCtrl_t *IoPtr;
    /*
     * Note - The pointer here should never be NULL because the ID should always be
     * within the expected range, but if it ever is NULL, this should return true
     * such that the caller will _not_ attempt to use the record.
     */
    IoPtr = BP_LocateIoEntryByHandle(BP_IOHANDLE_C(CheckId));
    return (IoPtr == NULL || CFE_RESOURCEID_TEST_DEFINED(IoPtr->Handle));
}

/*-----------------------------------------------
 * parse_parameters
 *-----------------------------------------------*/
static bool parse_parameters(const char *cfgstr, BP_PspIOConfig_t *config)
{
    size_t      len;
    const char *NextPos;
    const char *CurrPos;
    char       *ValPos;
    char        TempBuf[64];

    assert(config);

    /* Initialize Parameter Structure */
    config->ModuleName[0]    = 0;
    config->BundleBufferSize = 0;

    /* Get Length and Copy to Buffer */
    NextPos = cfgstr;
    while (NextPos != NULL && *NextPos != 0)
    {
        CurrPos = NextPos;
        NextPos = strchr(CurrPos, '&');
        if (NextPos == NULL)
        {
            len = strlen(CurrPos);
        }
        else
        {
            len = NextPos - CurrPos;
            ++NextPos;
        }
        if (len >= sizeof(TempBuf))
        {
            len = sizeof(TempBuf) - 1;
        }
        strncpy(TempBuf, CurrPos, len);
        TempBuf[len] = 0;
        ValPos       = strchr(TempBuf, '=');
        if (ValPos == NULL)
        {
            return false;
        }

        *ValPos = 0;
        ++ValPos;

        /* Process Parameter */
        if (strcmp(TempBuf, BP_IO_PSP_MODULE_NAME) == 0)
        {
            strncpy(config->ModuleName, ValPos, sizeof(config->ModuleName) - 1);
            config->ModuleName[sizeof(config->ModuleName) - 1] = 0;
        }
        else if (strcmp(TempBuf, BP_IO_PSP_BUFFER_SIZE) == 0)
        {
            config->BundleBufferSize = strtoul(ValPos, NULL, 0);
        }
        else
        {
            return false;
        }
    }

    return true;
}

/************************************************
 * Exported Functions
 ************************************************/
/*-----------------------------------------------
 * BP_IOInit
 *-----------------------------------------------*/
int32 BP_IOInit(void)
{
    BP_GlobalData.LastIoHandle = CFE_ResourceId_FromInteger(BP_IO_HANDLE_BASE);
    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOOpen
 *-----------------------------------------------*/
int32 BP_IOOpen(const char *IOParm, BP_BundleBuffer_t *Buffer, BP_IoHandle_t *ioh)
{
    int32            status;
    BP_IOCtrl_t     *IoPtr;
    CFE_ResourceId_t PendingIoHandle;
    BP_PspIOConfig_t IOConfig;
    uint32           PspModuleId;

    /* Find Open Descriptor */
    PendingIoHandle = CFE_ResourceId_FindNext(BP_GlobalData.LastIoHandle, BP_MAX_IO_DESC, BP_CheckIoSlotUsed);
    IoPtr           = BP_LocateIoEntryByHandle(BP_IOHANDLE_C(PendingIoHandle));

    if (IoPtr == NULL)
    {
        /* No more room */
        return -1;
    }

    /* Initialize I/O Control Structure */
    memset(IoPtr, 0, sizeof(*IoPtr));

    /* Parse Configuration String */
    if (!parse_parameters(IOParm, &IOConfig))
    {
        CFE_EVS_SendEvent(BP_IO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to parse IO configuration string: %s",
                          IOParm);
        return -1;
    }

    status = CFE_PSP_IODriver_FindByName(IOConfig.ModuleName, &PspModuleId);
    if (status != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_IO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "BP IO: CFE_PSP_IODriver_FindByName(%s) status %x", IOConfig.ModuleName,
                          (unsigned int)status);
        return -1;
    }

    Buffer->BaseMem = malloc(IOConfig.BundleBufferSize);
    if (Buffer->BaseMem == NULL)
    {
        CFE_EVS_SendEvent(BP_IO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR, "BP IO: Cannot allocate buffer of size %zu",
                          IOConfig.BundleBufferSize);
        return -1;
    }
    Buffer->MaxSize = IOConfig.BundleBufferSize;

    /* Mark entry in use */
    IoPtr->Handle              = BP_IOHANDLE_C(PendingIoHandle);
    BP_GlobalData.LastIoHandle = PendingIoHandle;

    /* Set Descriptor */
    CFE_EVS_SendEvent(BP_IO_OPEN_INFO_EID, CFE_EVS_EventType_INFORMATION,
                      "Opened IO channel for using configuration: %s", IOParm);
    *ioh = IoPtr->Handle;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOClose
 *-----------------------------------------------*/
int32 BP_IOClose(BP_IoHandle_t ioh)
{
    BP_IOCtrl_t *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    IoPtr->Handle = BP_IO_INVALID_DESCRIPTOR;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOGetStats
 *-----------------------------------------------*/
int32 BP_IOGetStats(BP_IoHandle_t ioh, BP_IOStats_t *Stats)
{
    BP_IOCtrl_t *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    /* Set Health Flags */
    Stats->SendHealthy    = IoPtr->SendInError == false;
    Stats->ReceiveHealthy = IoPtr->ReceiveInError == false;
    Stats->SendNotReady   = IoPtr->SendNotReady;

    /* Calculate I/O Rates */
    CFE_TIME_SysTime_t sys_time        = CFE_TIME_GetMET();
    double             curr_seconds    = (double)sys_time.Seconds + ((double)sys_time.Subseconds / 4294967296.0);
    double             elapsed_seconds = curr_seconds - IoPtr->LastStatsSeconds;
    if (elapsed_seconds > 0)
    {
        Stats->BytesSentPerSecond     = IoPtr->BytesSent / elapsed_seconds;
        Stats->BytesReceivedPerSecond = IoPtr->BytesReceived / elapsed_seconds;
        IoPtr->BytesSent              = 0;
        IoPtr->BytesReceived          = 0;
    }
    else
    {
        CFE_EVS_SendEvent(BP_IO_RATES_NOT_UPDATED_INFO_EID, CFE_EVS_EventType_INFORMATION,
                          "I/O rates not updated curr=%lf, last=%lf", curr_seconds, IoPtr->LastStatsSeconds);
    }

    IoPtr->LastStatsSeconds = curr_seconds;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOClearStats
 *-----------------------------------------------*/
int32 BP_IOClearStats(BP_IoHandle_t ioh)
{
    BP_IOCtrl_t *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    IoPtr->SendInError    = false;
    IoPtr->ReceiveInError = false;
    IoPtr->SendNotReady   = 0;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOFlush
 *-----------------------------------------------*/
int32 BP_IOFlush(BP_IoHandle_t ioh)
{
    (void)ioh;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOReadBundle
 *-----------------------------------------------*/
bool BP_IOReadBundle(BP_IoHandle_t ioh, BP_BundleBuffer_t *InBuf)
{
    BP_IOCtrl_t                        *IoPtr;
    int32                               PspStatus;
    CFE_PSP_IODriver_ReadPacketBuffer_t RdBuf;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return false;
    }

    assert(InBuf != NULL);
    assert(InBuf->MaxSize > 0);

    RdBuf.BufferSize = InBuf->MaxSize;
    RdBuf.BufferMem  = InBuf->BaseMem;

    PspStatus =
        CFE_PSP_IODriver_Command(&IoPtr->Location, CFE_PSP_IODRIVER_PACKET_IO_READ, CFE_PSP_IODRIVER_VPARG(&RdBuf));

    return (PspStatus == CFE_PSP_SUCCESS);
}

/*-----------------------------------------------
 * BP_IOWriteBundle
 *
 * Note: if buf is null or bufsize is zero then
 *       an idle encap packet is used to fill out
 *       the frame
 *-----------------------------------------------*/
bool BP_IOWriteBundle(BP_IoHandle_t ioh, const BP_BundleBuffer_t *OutBuf)
{
    int32                                PspStatus;
    BP_IOCtrl_t                         *IoPtr;
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return false;
    }

    assert(OutBuf != NULL);
    assert(OutBuf->CurrentSize > 0);

    WrBuf.BufferMem  = OutBuf->BaseMem;
    WrBuf.OutputSize = OutBuf->CurrentSize;

    PspStatus =
        CFE_PSP_IODriver_Command(&IoPtr->Location, CFE_PSP_IODRIVER_PACKET_IO_WRITE, CFE_PSP_IODRIVER_VPARG(&WrBuf));

    return (PspStatus == CFE_PSP_SUCCESS);
}
