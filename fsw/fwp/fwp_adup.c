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

/* Ingest an ADU */
BPLib_Status_t BPA_ADUP_In(void *AduPtr, uint8_t ChanId)
{
    CFE_SB_Buffer_t *Buf = (CFE_SB_Buffer_t *) AduPtr;
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

        BPNode_AppData.AduInData[ChanId].AduCountReceived++;

        /* TODO pass to PI */
    }
    else 
    {
        CFE_EVS_SendEvent(BPNODE_ADU_TOO_BIG_ERR_EID, CFE_EVS_EventType_ERROR,
                    "[ADU In #%d]: Received an ADU too big to ingest, Size=%ld, MaxBundlePayloadSize=%d",
                    ChanId, Size, BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize);
    }
    
    return BPLIB_SUCCESS;
}

/* Send out an ADU */
BPLib_Status_t BPA_ADUP_Out(void *AduPtr, uint8_t ChanId)
{
    /* Add cFS header to ADU */
    if (BPNode_AppData.AduOutData[ChanId].AduWrapping == true)
    {
        /* TODO add header */
    }

    /* Send ADU onto Software Bus */
    CFE_SB_TransmitMsg((CFE_MSG_Message_t *) AduPtr, false);

    BPNode_AppData.AduOutData[ChanId].AduCountDelivered++;

    return BPLIB_SUCCESS;
}

/* Add a new application's configurations */
BPLib_Status_t BPA_ADUP_AddApplication(uint8_t ChanId)
{
    uint8_t i;

    /* Check for channel ID validity */
    if (ChanId >= BPNODE_MAX_NUM_CHANNELS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_ADD_CHAN_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with add-application directive, invalid ChanId=%d", ChanId);
        return BPLIB_ERROR;
    }

    /* App state must be either stopped or added */
    if (BPNode_AppData.AduConfigs[ChanId].AppState == BPA_ADUP_APP_STARTED)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_ADD_STAT_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with add-application directive, invalid AppState=%d for ChanId=%d", 
            BPNode_AppData.AduConfigs[ChanId].AppState, ChanId);
        return BPLIB_ERROR;
    }

    /*
    ** Set ADU proxy configurations
    */

    BPNode_AppData.AduOutData[ChanId].SendToMsgId = BPNode_AppData.AduTblPtr->Entries[ChanId].SendToMsgId;
    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = BPNode_AppData.AduTblPtr->Entries[ChanId].NumRecvFrmMsgIds;

    for (i = 0; i < BPNode_AppData.AduTblPtr->Entries[ChanId].NumRecvFrmMsgIds; i++)
    {
        BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i] = BPNode_AppData.AduTblPtr->Entries[ChanId].RecvFrmMsgIds[i];
    }

    /*
    ** Set channel configurations
    */

    BPNode_AppData.AduConfigs[ChanId].AddAutomatically = BPNode_AppData.ChanTblPtr->ChannelSet[ChanId].AddAutomatically;

    BPNode_AppData.AduInData[ChanId].AduUnwrapping = BPNode_AppData.ChanTblPtr->ChannelSet[ChanId].AduUnwrapping;
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = BPNode_AppData.ChanTblPtr->ChannelSet[ChanId].MaxBundlePayloadSize;

    BPNode_AppData.AduOutData[ChanId].AduWrapping = BPNode_AppData.ChanTblPtr->ChannelSet[ChanId].AduWrapping;
    
    /* Set app state to added */
    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_ADDED;

    return BPLIB_SUCCESS;
}

/* Start an application */
BPLib_Status_t BPA_ADUP_StartApplication(uint8_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;

    /* Check for channel ID validity */
    if (ChanId >= BPNODE_MAX_NUM_CHANNELS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_STRT_CHAN_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with start-application directive, invalid ChanId=%d", ChanId);
        return BPLIB_ERROR;
    }

    /* App state must be added */
    if (BPNode_AppData.AduConfigs[ChanId].AppState != BPA_ADUP_APP_ADDED)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_STRT_STAT_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with start-application directive, invalid AppState=%d for ChanId=%d", 
            BPNode_AppData.AduConfigs[ChanId].AppState, ChanId);
        return BPLIB_ERROR;
    }

    /* Subscribe to all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_Subscribe(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                  BPNode_AppData.AduInData[ChanId].AduPipe);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_SUB_ERR_EID, CFE_EVS_EventType_ERROR,
                             "Error subscribing to ADU on channel #%d, Error = %d, MsgId = 0x%x", 
                             ChanId, Status, 
                             CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));
            return BPLIB_ERROR;
        }
    } 

    /* Set app state to started */
    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STARTED;

    return BPLIB_SUCCESS;
}

/* Stop an application */
BPLib_Status_t BPA_ADUP_StopApplication(uint8_t ChanId)
{
    CFE_Status_t Status;
    uint8_t i;

    /* Check for channel ID validity */
    if (ChanId >= BPNODE_MAX_NUM_CHANNELS)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_STOP_CHAN_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with stop-application directive, invalid ChanId=%d", ChanId);
        return BPLIB_ERROR;
    }

    /* App state must be started */
    if (BPNode_AppData.AduConfigs[ChanId].AppState != BPA_ADUP_APP_STARTED)
    {
        CFE_EVS_SendEvent(BPNODE_ADU_STOP_STAT_ERR_EID, CFE_EVS_EventType_ERROR,
            "Error with stop-application directive, invalid AppState=%d for ChanId=%d", 
            BPNode_AppData.AduConfigs[ChanId].AppState, ChanId);
        return BPLIB_ERROR;
    }

    /* Unsubscribe from all configured message IDs */
    for(i = 0; i < BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds; i++)
    {
        Status = CFE_SB_Unsubscribe(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i],
                                  BPNode_AppData.AduInData[ChanId].AduPipe);
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BPNODE_ADU_UNSUB_ERR_EID, CFE_EVS_EventType_ERROR,
                             "Error unsubscribing from ADU on channel #%d, Error = %d, MsgId = 0x%x", 
                             ChanId, Status, 
                             CFE_SB_MsgIdToValue(BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i]));
            
            return BPLIB_ERROR;
        }
    } 

    /* Set app state to stopped */
    BPNode_AppData.AduConfigs[ChanId].AppState = BPA_ADUP_APP_STOPPED;

    /* Notify ADU In task to clear pipe */
    BPNode_AppData.AduInData[ChanId].ClearPipe = true;

    return BPLIB_SUCCESS;
}
