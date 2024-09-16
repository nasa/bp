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
#include "fwp_tablep.h"
#include "bpnode_app.h"


/*
** Function Definitions
*/

/* Ingest an ADU */
BPLib_Status_t BPA_ADUP_In(void *AduPtr)
{
    BPNode_AppData.NodeMibCountersHkTlm.Payload.AduCountReceived++;
    return BPLIB_SUCCESS;
}

/* Send out an ADU */
BPLib_Status_t BPA_ADUP_Out(void *AduPtr)
{
    return BPLIB_SUCCESS;
}

/* Add a new application's configurations */
BPLib_Status_t BPA_ADUP_AddApplication(uint8_t ChanId)
{
    BPA_ADUP_Config_t *AduConfigs = &BPNode_AppData.TblNameParamsArr[BPNODE_ADU_TBL_IDX][ChanId];
    BPNode_ChannelSet_t  *ChanConfigs = &BPNode_AppData.TblNameParamsArr[BPNODE_CHAN_TBL_IDX][ChanId];

    /*
    ** Set ADU proxy configurations
    */

    BPNode_AppData.AduOutData[ChanId].SendToMsgId = AduConfigs->SendToMsgId;
    BPNode_AppData.AduInData[ChanId].NumRecvFromMsgIds = AduConfigs->NumRecvFrmMsgIds;

    for (i = 0; i < AduConfigs->NumRecvFrmMsgIds)
    {
        BPNode_AppData.AduInData[ChanId].RecvFromMsgIds[i] = AduConfigs->RecvFrmMsgIds[i];
    }

    /*
    ** Set channel configurations
    */

    BPNode_AppData.AduConfigs[ChanId].AddAutomatically = ChanConfigs->AddAutomatically;

    BPNode_AppData.AduInData[ChanId].AduWrapping = ChanConfigs->AduWrapping;
    BPNode_AppData.AduInData[ChanId].RecvBytesPerCycle = ChanConfigs->RecvBytesPerCycle;
    BPNode_AppData.AduInData[ChanId].MaxBundlePayloadSize = ChanConfigs->MaxBundlePayloadSize;

    BPNode_AppData.AduOutData[ChanId].AduUnwrapping = ChanConfigs->AduUnwrapping;
    BPNode_AppData.AduOutData[ChanId].SendBytesPerCycle = ChanConfigs->SendBytesPerCycle;
    
    return BPLIB_SUCCESS;
}

/* Start an application */
BPLib_Status_t BPA_ADUP_StartApplication(uint8_t ChanId)
{
    CFE_Status_t Status;
    uint8 i;

    /* Subscribe to all related message IDs */
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
    uint8 i;

    /* Unsubscribe from all related message IDs */
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

    /* TODO empty pipe */

    return BPLIB_SUCCESS;
}
