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
 *   This file contains the source code for the FWP ADU Proxy.
 */

/*
** Include Files
*/

#include "cfe.h"
#include "fwp_adup.h"
#include "bpnode_app.h"


/*
** Function Definitions
*/

/* Validate ADU Proxy table data */
CFE_Status_t BPA_ADUP_ValidateConfigTbl(void *TblData)
{
    BPA_ADUP_Table_t *TblDataPtr = (BPA_ADUP_Table_t *) TblData;
    uint32_t i, j;

    for (i = 0; i < BPLIB_MAX_NUM_CHANNELS; i++)
    {
        /*
        ** Validate array length and that message IDs are all valid
        */

        if (!CFE_SB_IsValidMsgId(TblDataPtr->Entries[i].SendToMsgId) ||
            TblDataPtr->Entries[i].NumRecvFrmMsgIds > BPNODE_MAX_CHAN_SUBSCRIPTION)
        {
            return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
        }

        for (j = 0; j < TblDataPtr->Entries[i].NumRecvFrmMsgIds; j++)
        {
            if (!CFE_SB_IsValidMsgId(TblDataPtr->Entries[i].RecvFrmMsgIds[j]))
            {
                return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
            }

            if (TblDataPtr->Entries[i].MsgLims[j] > BPNODE_ADU_PIPE_DEPTH)
            {
                return BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
            }
        }
    }

    return CFE_SUCCESS;
}

/* Ingest an ADU */
BPLib_Status_t BPA_ADUP_In(void *AduPtr, uint32_t ChanId, size_t *AduSize)
{
    BPLib_Status_t  Status = BPLIB_SUCCESS;
    CFE_SB_Buffer_t *Buf   = (CFE_SB_Buffer_t *) AduPtr;

    CFE_MSG_GetSize(&Buf->Msg, AduSize);

    /* Validate ADU is an acceptable size */
    if (*AduSize <= BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize)
    {
        /* Remove header from ADU */
        if (BPNode_AppData.AduInData[ChanId].AduUnwrapping == true)
        {
            /* TODO remove header */
        }

        /* Pass ADU to PI */
        Status = BPLib_PI_Ingress(&BPNode_AppData.BplibInst, ChanId, AduPtr, *AduSize);
    }
    else
    {
        Status = BPLIB_ERROR;

        BPLib_EM_SendEvent(BPNODE_ADU_IN_TOO_BIG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU In #%d]: Received an ADU too big to ingest, Size=%ld, MaxBundlePayloadSize=%d",
                            ChanId, *AduSize, BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize);
    }

    return Status;
}

/* Send out an ADU */
BPLib_Status_t BPA_ADUP_Out(uint32_t ChanId, uint32_t Timeout, size_t *AduSize)
{
    BPLib_Status_t Status;

    /* Get an ADU from PI */
    Status = BPLib_PI_Egress(&BPNode_AppData.BplibInst, ChanId,
                            (void *) &BPNode_AppData.AduOutData[ChanId].OutBuf.Payload,
                            AduSize, BPNODE_ADU_OUT_MAX_ADU_OUT_BYTES, Timeout);

    if (Status == BPLIB_SUCCESS)
    {
        /* AduSize is validated within PI Egress, no need to check */

        /* Add cFS header to ADU */
        if (BPNode_AppData.AduOutData[ChanId].AduWrapping == true)
        {
            CFE_MSG_SetMsgId(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader),
                                        BPNode_AppData.AduOutData[ChanId].SendToMsgId);
            CFE_MSG_SetSize(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader),
                                                *AduSize + sizeof(CFE_MSG_TelemetryHeader_t));

            /* Send wrapped ADU onto Software Bus */
            CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader), true);
        }
        /* Don't add cFS header, assume it has one already */
        else
        {
            /* Send ADU onto Software Bus */
            CFE_SB_TransmitMsg((CFE_MSG_Message_t *) &BPNode_AppData.AduOutData[ChanId].OutBuf.Payload, false);
        }     
    }
    /* Only report non-timeout errors */
    else if (Status != BPLIB_PI_TIMEOUT)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_OUT_PI_OUT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU Out #%d]: Failed to egress an ADU. Error = %d.",
                            ChanId, Status);
    }

    return Status;
}

/* Add a new application's configurations */
BPLib_Status_t BPA_ADUP_AddApplication(uint32_t ChanId)
{
    uint8_t i;

    /*
    ** Set ADU proxy configurations
    */

    BPLib_NC_ReaderLock();

    BPNode_AppData.AduOutData[ChanId].SendToMsgId = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].SendToMsgId;
    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].NumRecvFrmMsgIds;

    for (i = 0; i < BPNode_AppData.AduProxyTablePtr->Entries[ChanId].NumRecvFrmMsgIds; i++)
    {
        BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i] = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].RecvFrmMsgIds[i];
        BPNode_AppData.AduInData[ChanId].MsgLims[i] = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].MsgLims[i];
    }

    /*
    ** Set channel configurations
    */

    BPNode_AppData.AduState[ChanId].AddAutomatically = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AddAutomatically;

    BPNode_AppData.AduInData[ChanId].AduUnwrapping = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AduUnwrapping;
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].MaxBundlePayloadSize;

    BPNode_AppData.AduOutData[ChanId].AduWrapping = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AduWrapping;

    BPNode_AppData.AduOutData[ChanId].RateLimit = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].EgressBitsPerCycle;
    BPNode_AppData.AduInData[ChanId].RateLimit = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].IngressBitsPerCycle;

    BPLib_NC_ReaderUnlock();

    return BPLIB_SUCCESS;
}

/* Start an application */
BPLib_Status_t BPA_ADUP_StartApplication(uint32_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;

    /* Subscribe to all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_SubscribeEx(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                    BPNode_AppData.AduInData[ChanId].AduPipe,
                                    CFE_SB_DEFAULT_QOS, 
                                    BPNode_AppData.AduInData[ChanId].MsgLims[i]);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_START_SUB_DBG_EID, BPLib_EM_EventType_DEBUG,
                                "Error subscribing to ADU on channel #%d, Error = %d, MsgId = 0x%x",
                                ChanId, Status,
                                CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));

            return BPLIB_ERROR;
        }
    }

    return BPLIB_SUCCESS;
}

/* Stop an application */
BPLib_Status_t BPA_ADUP_StopApplication(uint32_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;

    /* Unsubscribe from all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_Unsubscribe(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                  BPNode_AppData.AduInData[ChanId].AduPipe);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_STOP_UNSUB_DBG_EID, BPLib_EM_EventType_DEBUG,
                                "Error unsubscribing from ADU on channel #%d, Error = %d, MsgId = 0x%x",
                                ChanId,
                                Status,
                                CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));

            return BPLIB_ERROR;
        }
    }

    /* Notify ADU In task to clear pipe */
    BPNode_AppData.AduInData[ChanId].ClearPipe = true;

    return BPLIB_SUCCESS;
}

/* Remove an application */
BPLib_Status_t BPA_ADUP_RemoveApplication(uint32_t ChanId)
{
    /* No cFS-specific operations needed */

    return BPLIB_SUCCESS;
}
