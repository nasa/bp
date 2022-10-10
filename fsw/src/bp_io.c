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

/************************************************
 * Defines
 ************************************************/

#define BP_IO_CFGSTR_MAX_SIZE    256
#define BP_IO_BUNDLE_PIPE_PREFIX "BP_BPIPE_"
#define BP_IO_OUT_BUNDLE_MID_VAR "OutMID"
#define BP_IO_IN_BUNDLE_MID_VAR  "InMID"
#define BP_IO_IN_PIPE_DEPTH_VAR  "InDepth"

/************************************************
 * Typedefs
 ************************************************/

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
 * parse_value
 *-----------------------------------------------*/
static bool parse_value(const char *str, unsigned long *val, int base)
{
    if (str == NULL)
        return false;
    char *endptr;
    errno                = 0;
    unsigned long result = strtoul(str, &endptr, base);
    if ((endptr == str) || ((result == ULONG_MAX || result == 0) && errno == ERANGE))
    {
        return false;
    }
    *val = result;
    return true;
}

/*-----------------------------------------------
 * parse_parameters
 *-----------------------------------------------*/
static bool parse_parameters(const char *cfgstr, BP_IOConfig_t *config)
{
    assert(config);

    char cfgbuf[BP_IO_CFGSTR_MAX_SIZE];

    /* Initialize Parameter Structure */
    config->BundleOutMID      = BP_INVALID_MID;
    config->BundleInMID       = BP_INVALID_MID;
    config->BundleInPipeDepth = 0;

    /* Get Length and Copy to Buffer */
    int len = strlen(cfgstr) + 1;
    len     = len < BP_IO_CFGSTR_MAX_SIZE ? len : BP_IO_CFGSTR_MAX_SIZE - 1;
    memset(cfgbuf, 0, BP_IO_CFGSTR_MAX_SIZE);
    strncpy(cfgbuf, cfgstr, len);

    /* Traverse Configuration Buffer */
    int idx = 0;
    while (idx < len)
    {
        /* Find Next Token */
        const char *token = &cfgbuf[idx];
        while (idx < len && cfgbuf[idx] != '\0' && cfgbuf[idx] != '=')
            idx++;
        cfgbuf[idx++] = '\0';

        /* Find Next Value */
        const char *value = &cfgbuf[idx];
        while (idx < len && cfgbuf[idx] != '\0' && cfgbuf[idx] != '&')
            idx++;
        cfgbuf[idx++] = '\0';

        /* Process Parameter */
        if (strcmp(token, BP_IO_OUT_BUNDLE_MID_VAR) == 0)
        {
            /* Set Output Bundle MID */
            unsigned long tmp;
            if (parse_value(value, &tmp, 16))
            {
                config->BundleOutMID = CFE_SB_ValueToMsgId(tmp);
            }
            else
            {
                return false;
            }
        }
        else if (strcmp(token, BP_IO_IN_BUNDLE_MID_VAR) == 0)
        {
            /* Set Input Bundle MID */
            unsigned long tmp;
            if (parse_value(value, &tmp, 16))
            {
                config->BundleInMID = CFE_SB_ValueToMsgId(tmp);
            }
            else
            {
                return false;
            }
        }
        else if (strcmp(token, BP_IO_IN_PIPE_DEPTH_VAR) == 0)
        {
            /* Set Input Pipe Depth */
            unsigned long tmp;
            if (parse_value(value, &tmp, 16))
            {
                config->BundleInPipeDepth = (int)tmp;
            }
            else
            {
                return false;
            }
        }
        else
        {
            /* Unrecognized Parameter */
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
int32 BP_IOOpen(const char *CfgStr, BP_IoHandle_t *Descriptor)
{
    BP_IOCtrl_t     *IoPtr;
    CFE_Status_t     cfe_status;
    char             pipe_name[BP_PIPE_NAME_SIZE];
    CFE_ResourceId_t PendingIoHandle;

    assert(Descriptor);

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
    if (!parse_parameters(CfgStr, &IoPtr->Config))
    {
        CFE_EVS_SendEvent(BP_IO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to parse IO configuration string: %s",
                          CfgStr);
        return -1;
    }

    /* Create Bundle Input Pipe */
    snprintf(pipe_name, BP_PIPE_NAME_SIZE, "%s%lx", BP_IO_BUNDLE_PIPE_PREFIX,
             CFE_ResourceId_ToInteger(PendingIoHandle));
    cfe_status = CFE_SB_CreatePipe(&IoPtr->BundleInPipe, IoPtr->Config.BundleInPipeDepth, pipe_name);
    if (cfe_status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%X) to create bundle pipe %s",
                          (int)cfe_status, pipe_name);
        return -1;
    }

    /* Subscribe to Bundle Packets on Software Bus */
    cfe_status = CFE_SB_SubscribeEx(IoPtr->Config.BundleInMID, IoPtr->BundleInPipe, CFE_SB_DEFAULT_QOS,
                                    IoPtr->Config.BundleInPipeDepth);
    if (cfe_status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%X) to subscribe to %04X on bundle pipe",
                          (int)cfe_status, (unsigned int)CFE_SB_MsgIdToValue(IoPtr->Config.BundleInMID));
        return -1;
    }

    /* Mark entry in use */
    IoPtr->Handle              = BP_IOHANDLE_C(PendingIoHandle);
    BP_GlobalData.LastIoHandle = PendingIoHandle;

    /* Set Descriptor */
    CFE_EVS_SendEvent(BP_IO_OPEN_INFO_EID, CFE_EVS_EventType_INFORMATION,
                      "Opened IO channel for using configuration: %s", CfgStr);
    *Descriptor = IoPtr->Handle;

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

    if (CFE_RESOURCEID_TEST_DEFINED(IoPtr->BundleInPipe))
    {
        CFE_SB_DeletePipe(IoPtr->BundleInPipe);
        IoPtr->BundleInPipe = BP_INVALID_PIPE;
    }

    IoPtr->Handle = BP_IO_INVALID_DESCRIPTOR;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_IOGetStats
 *-----------------------------------------------*/
int32 BP_IOGetStats(BP_IoHandle_t ioh, BP_IOStats_t *stats)
{
    BP_IOCtrl_t *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    assert(stats);

    /* Set Health Flags */
    stats->SendHealthy    = IoPtr->SendInError == false;
    stats->ReceiveHealthy = IoPtr->ReceiveInError == false;
    stats->SendNotReady   = IoPtr->SendNotReady;

    /* Calculate I/O Rates */
    CFE_TIME_SysTime_t sys_time        = CFE_TIME_GetMET();
    double             curr_seconds    = (double)sys_time.Seconds + ((double)sys_time.Subseconds / 4294967296.0);
    double             elapsed_seconds = curr_seconds - IoPtr->LastStatsSeconds;
    if (elapsed_seconds > 0)
    {
        stats->BytesSentPerSecond     = IoPtr->BytesSent / elapsed_seconds;
        stats->BytesReceivedPerSecond = IoPtr->BytesReceived / elapsed_seconds;
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
bool BP_IOReadBundle(BP_IoHandle_t ioh, void **buf, size_t *bufsize)
{
    CFE_Status_t   SbStatus;
    CFE_MSG_Type_t MsgType;
    size_t         MsgSize;
    unsigned char *DataPtr;
    BP_IOCtrl_t   *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    assert(buf);
    assert(bufsize);

    /* Read bundle from link */
    SbStatus = CFE_SB_ReceiveBuffer(&IoPtr->BundleInMsgBuf, IoPtr->BundleInPipe, CFE_SB_POLL);
    if (SbStatus == CFE_SUCCESS)
    {
        SbStatus = CFE_MSG_GetType(&IoPtr->BundleInMsgBuf->Msg, &MsgType);

        /* should confirm that the message actually has a CMD header */
        if (SbStatus == CFE_SUCCESS && MsgType != CFE_MSG_Type_Cmd)
        {
            SbStatus = CFE_SB_WRONG_MSG_TYPE;
        }
    }
    if (SbStatus == CFE_SUCCESS)
    {
        SbStatus = CFE_MSG_GetSize(&IoPtr->BundleInMsgBuf->Msg, &MsgSize);

        /* should confirm that the message size is nonzero (after header) */
        if (SbStatus == CFE_SUCCESS && MsgSize <= sizeof(CFE_MSG_CommandHeader_t))
        {
            SbStatus = CFE_STATUS_WRONG_MSG_LENGTH;
        }
    }

    if (SbStatus == CFE_SUCCESS)
    {
        /* Adjust size/ptr for removal of command header */
        DataPtr = (unsigned char *)IoPtr->BundleInMsgBuf;
        DataPtr += sizeof(CFE_MSG_CommandHeader_t);
        MsgSize -= sizeof(CFE_MSG_CommandHeader_t);

        /* Export to caller */
        *buf     = DataPtr;
        *bufsize = MsgSize;

        IoPtr->BytesReceived += MsgSize;
    }
    else if (SbStatus != CFE_SB_NO_MESSAGE)
    {
        if (!IoPtr->ReceiveInError)
        {
            CFE_EVS_SendEvent(BP_IO_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%X) to receive bundle",
                              (int)SbStatus);
            IoPtr->ReceiveInError = true;
        }
    }

    return (SbStatus == CFE_SUCCESS);
}

/*-----------------------------------------------
 * BP_IOWriteBundle
 *
 * Note: if buf is null or bufsize is zero then
 *       an idle encap packet is used to fill out
 *       the frame
 *-----------------------------------------------*/
bool BP_IOWriteBundle(BP_IoHandle_t ioh, const void *buf, size_t bufsize)
{
    CFE_SB_Buffer_t *MsgBuf;
    unsigned char   *DataPtr;
    size_t           ActualSize;
    CFE_Status_t     SbStatus;
    BP_IOCtrl_t     *IoPtr;

    IoPtr = BP_LocateIoEntryByHandle(ioh);
    if (!BP_IoEntryIsMatch(IoPtr, ioh))
    {
        return -1;
    }

    SbStatus = CFE_SB_BUFFER_INVALID;

    if (buf && bufsize > 0)
    {
        /* Get a buffer, accounting for the extra size of TLM header */
        ActualSize = bufsize + sizeof(CFE_MSG_TelemetryHeader_t);
        MsgBuf     = CFE_SB_AllocateMessageBuffer(ActualSize);
        if (MsgBuf == NULL)
        {
            SbStatus = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
        }
        else
        {
            /*
             * Init the message, but note this does not use the full size as it would (unnecessarily)
             * memset the whole thing to 0 - data is about to be memcpy'ed into it immediately after.
             */
            CFE_MSG_Init(&MsgBuf->Msg, IoPtr->Config.BundleOutMID, sizeof(CFE_MSG_TelemetryHeader_t));
            CFE_SB_TimeStampMsg(&MsgBuf->Msg);

            /* Copy payload in and set correct size */
            DataPtr = (unsigned char *)MsgBuf + sizeof(CFE_MSG_TelemetryHeader_t);
            memcpy(DataPtr, buf, bufsize);
            CFE_MSG_SetSize(&MsgBuf->Msg, ActualSize);

            /* Send Bundle */
            SbStatus = CFE_SB_TransmitBuffer(MsgBuf, true);
            if (SbStatus == CFE_SUCCESS)
            {
                IoPtr->BytesSent += bufsize;
            }
            else
            {
                /* Be sure to release the buffer because SB did not take ownership */
                CFE_SB_ReleaseMessageBuffer(MsgBuf);
            }
        }

        if (SbStatus != CFE_SUCCESS && !IoPtr->SendInError)
        {
            CFE_EVS_SendEvent(BP_IO_SEND_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%X) to send bundle", (int)SbStatus);
            IoPtr->SendInError = true;
        }
    }

    return (SbStatus == CFE_SUCCESS);
}
