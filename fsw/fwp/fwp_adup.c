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
    uint8_t i, j;

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
        }
    }

    return CFE_SUCCESS;
}

/* Ingest an ADU */
BPLib_Status_t BPA_ADUP_In(void *AduPtr, uint8_t ChanId)
{
    BPLib_Status_t  Status = BPLIB_SUCCESS;
    CFE_SB_Buffer_t *Buf   = (CFE_SB_Buffer_t *) AduPtr;
    CFE_MSG_Size_t   Size;

    CFE_MSG_GetSize(&Buf->Msg, &Size);

    /* Validate ADU is an acceptable size */
    if (Size <= BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize)
    {
        /* Remove header from ADU */
        if (BPNode_AppData.AduInData[ChanId].AduUnwrapping == true)
        {
            /* TODO remove header */
        }

        BPLib_AS_Increment(BPLIB_EID_INSTANCE, ADU_COUNT_RECEIVED, 1);

        /* Pass ADU to PI */
        Status = BPLib_PI_Ingress(&BPNode_AppData.BplibInst, ChanId, AduPtr, Size);

        if (Status != BPLIB_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_PI_IN_ERR_EID, BPLib_EM_EventType_ERROR,
                                "[ADU In #%d]: Failed to ingress an ADU. Error = %d.",
                                ChanId, Status);
        }
    }
    else
    {
        Status = BPLIB_ERROR;

        BPLib_EM_SendEvent(BPNODE_ADU_IN_TOO_BIG_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[ADU In #%d]: Received an ADU too big to ingest, Size=%ld, MaxBundlePayloadSize=%d",
                            ChanId, Size, BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize);
    }

    return Status;
}

/* Send out an ADU */
BPLib_Status_t BPA_ADUP_Out(uint8_t ChanId, uint32_t Timeout)
{
    BPLib_Status_t Status;
    size_t         AduSize;

    /* Get an ADU from PI */
    Status = BPLib_PI_Egress(&BPNode_AppData.BplibInst, ChanId,
                            (void *) &BPNode_AppData.AduOutData[ChanId].OutBuf.Payload,
                            &AduSize, BPNODE_ADU_OUT_MAX_ADU_OUT_BYTES, Timeout);

    if (Status == BPLIB_SUCCESS)
    {
        /* AduSize seems wrong, return length error */
        if (AduSize == 0 || AduSize > BPNODE_ADU_OUT_MAX_ADU_OUT_BYTES)
        {
            Status = BPLIB_BUF_LEN_ERROR;
        }        
        /* Add cFS header to ADU */
        else if (BPNode_AppData.AduOutData[ChanId].AduWrapping == true)
        {
            CFE_MSG_SetMsgId(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader),
                                        BPNode_AppData.AduOutData[ChanId].SendToMsgId);
            CFE_MSG_SetSize(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader),
                                                AduSize + sizeof(CFE_MSG_TelemetryHeader_t));

            /* Send wrapped ADU onto Software Bus */
            CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.AduOutData[ChanId].OutBuf.TelemetryHeader), true);
            BPLib_AS_Increment(BPLIB_EID_INSTANCE, ADU_COUNT_DELIVERED, 1);
        }
        /* Don't add cFS header, assume it has one already */
        else
        {
            /* Send ADU onto Software Bus */
            CFE_SB_TransmitMsg((CFE_MSG_Message_t *) &BPNode_AppData.AduOutData[ChanId].OutBuf.Payload, false);
            BPLib_AS_Increment(BPLIB_EID_INSTANCE, ADU_COUNT_DELIVERED, 1);
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
BPLib_Status_t BPA_ADUP_AddApplication(uint8_t ChanId)
{
    uint8_t i;
    BPLib_NC_ApplicationState_t AppState;

    /* Check for channel ID validity */
    if (ChanId >= BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_ADD_CHAN_ERR_EID, BPLib_EM_EventType_DEBUG,
                            "Error with add-application directive, invalid ChanId=%d",
                            ChanId);

        return BPLIB_ERROR;
    }

    /* App state must be either stopped or added */
    AppState = BPLib_NC_GetAppState(ChanId);
    if (AppState == BPLIB_NC_APP_STATE_STARTED)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_ADD_STAT_ERR_EID, BPLib_EM_EventType_DEBUG,
                            "Error with add-application directive, invalid AppState=%d for ChanId=%d",
                            AppState,
                            ChanId);

        return BPLIB_ERROR;
    }

    /*
    ** Set ADU proxy configurations
    */

    BPNode_AppData.AduOutData[ChanId].SendToMsgId = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].SendToMsgId;
    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].NumRecvFrmMsgIds;

    for (i = 0; i < BPNode_AppData.AduProxyTablePtr->Entries[ChanId].NumRecvFrmMsgIds; i++)
    {
        BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i] = BPNode_AppData.AduProxyTablePtr->Entries[ChanId].RecvFrmMsgIds[i];
    }

    /*
    ** Set channel configurations
    */

    BPNode_AppData.AduState[ChanId].AddAutomatically = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AddAutomatically;

    BPNode_AppData.AduInData[ChanId].AduUnwrapping = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AduUnwrapping;
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].MaxBundlePayloadSize;

    BPNode_AppData.AduOutData[ChanId].AduWrapping = BPNode_AppData.ConfigPtrs.ChanConfigPtr->Configs[ChanId].AduWrapping;

    /* Set app state to added */
    BPLib_NC_SetAppState(ChanId, BPLIB_NC_APP_STATE_ADDED);

    return BPLIB_SUCCESS;
}

/* Start an application */
BPLib_Status_t BPA_ADUP_StartApplication(uint8_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;
    BPLib_NC_ApplicationState_t AppState;

    /* Check for channel ID validity */
    if (ChanId >= BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_START_CHAN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error with start-application directive, invalid ChanId=%d",
                            ChanId);

        return BPLIB_ERROR;
    }

    /* App state must be added */
    AppState = BPLib_NC_GetAppState(ChanId);
    if (AppState != BPLIB_NC_APP_STATE_ADDED &&
        AppState != BPLIB_NC_APP_STATE_STOPPED)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_START_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error with start-application directive, invalid AppState=%d for ChanId=%d",
                            AppState,
                            ChanId);

        return BPLIB_ERROR;
    }

    /* Subscribe to all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_Subscribe(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                  BPNode_AppData.AduInData[ChanId].AduPipe);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_START_SUB_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error subscribing to ADU on channel #%d, Error = %d, MsgId = 0x%x",
                                ChanId,
                                Status,
                                CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));

            return BPLIB_ERROR;
        }
    }

    /* Set app state to started */
    BPLib_NC_SetAppState(ChanId, BPLIB_NC_APP_STATE_STARTED);

    return BPLIB_SUCCESS;
}

/* Stop an application */
BPLib_Status_t BPA_ADUP_StopApplication(uint8_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;
    BPLib_NC_ApplicationState_t AppState;

    /* Check for channel ID validity */
    if (ChanId >= BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_STOP_CHAN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error with stop-application directive, invalid ChanId=%d",
                            ChanId);

        return BPLIB_ERROR;
    }

    /* App state must be started */
    AppState = BPLib_NC_GetAppState(ChanId);
    if (AppState != BPLIB_NC_APP_STATE_STARTED)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_STOP_STAT_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Error with stop-application directive, invalid AppState=%d for ChanId=%d",
                            AppState,
                            ChanId);

        return BPLIB_ERROR;
    }

    /* Unsubscribe from all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_Unsubscribe(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                  BPNode_AppData.AduInData[ChanId].AduPipe);
        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_STOP_UNSUB_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Error unsubscribing from ADU on channel #%d, Error = %d, MsgId = 0x%x",
                                ChanId,
                                Status,
                                CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));

            return BPLIB_ERROR;
        }
    }

    /* Set app state to stopped */
    BPLib_NC_SetAppState(ChanId, BPLIB_NC_APP_STATE_STOPPED);

    /* Notify ADU In task to clear pipe */
    BPNode_AppData.AduInData[ChanId].ClearPipe = true;

    return BPLIB_SUCCESS;
}

/* Remove an application */
BPLib_Status_t BPA_ADUP_RemoveApplication(uint8_t ChanId)
{
    BPLib_NC_ApplicationState_t AppState;

    /* Check for channel ID validity */
    if (ChanId >= BPLIB_MAX_NUM_CHANNELS)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_REM_CHAN_ERR_EID, BPLib_EM_EventType_DEBUG,
                            "Error with remove-application directive, invalid ChanId=%d",
                            ChanId);

        return BPLIB_ERROR;
    }

    /* App state must be stopped or added */
    AppState = BPLib_NC_GetAppState(ChanId);
    if (AppState != BPLIB_NC_APP_STATE_ADDED &&
        AppState != BPLIB_NC_APP_STATE_STOPPED)
    {
        BPLib_EM_SendEvent(BPNODE_ADU_REM_STAT_ERR_EID, BPLib_EM_EventType_DEBUG,
                            "Error with remove-application directive, invalid AppState=%d for ChanId=%d",
                            AppState,
                            ChanId);

        return BPLIB_ERROR;
    }

    /* TODO PI cleanup */

    /* Set app state to removed */
    BPLib_NC_SetAppState(ChanId, BPLIB_NC_APP_STATE_REMOVED);

    return BPLIB_SUCCESS;
}
