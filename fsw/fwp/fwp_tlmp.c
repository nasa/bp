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


/*
** \file
**   This file contains the source code for the FWP Telemetry Proxy.
*/

/*
** Include
*/

#include "fwp_tlmp.h"
#include "bpnode_utils.h"
#include "bpnode_app.h"

/* TLMP-scoped packet to ease packet initialization overhead */
CFE_MSG_Message_t* BPNode_TLMP_MsgPtr;

/*
** Function Definitions
*/

void BPA_TLMP_Init(void)
{
    CFE_MSG_Init(BPNode_TLMP_MsgPtr,
                    CFE_SB_ValueToMsgId(0),
                    sizeof(CFE_MSG_Message_t));
}

BPLib_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t* NodeMIBConfigTlmPayload)
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_NodeMibConfigHkTlm_t* Msg;

    Msg = (BPNode_NodeMibConfigHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_CONFIG_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *NodeMIBConfigTlmPayload;
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t* SrcMIBConfigTlmPayload)
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_SourceMibConfigHkTlm_t* Msg;

    Msg = (BPNode_SourceMibConfigHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *SrcMIBConfigTlmPayload;
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}

BPLib_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t* NodeMIBCounterTlmPayload)
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_NodeMibCountersHkTlm_t* Msg;

    Msg = (BPNode_NodeMibCountersHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *NodeMIBCounterTlmPayload;    
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t* SrcMIBCounterTlmPayload )
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_SourceMibCountersHkTlm_t* Msg;

    Msg = (BPNode_SourceMibCountersHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *SrcMIBCounterTlmPayload;
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}

BPLib_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t* ChannelContactTlmPayload)
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_ChannelContactStatHkTlm_t* Msg;

    Msg = (BPNode_ChannelContactStatHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *ChannelContactTlmPayload;
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}

BPLib_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t* StorTlmPayload)
{
    BPLib_Status_t Status;
    BPLib_TIME_MonotonicTime_t MonotonicTime;
    BPNode_StorageHkTlm_t* Msg;

    Msg = (BPNode_ChannelContactStatHkTlm_t*) BPNode_TLMP_MsgPtr;
    CFE_MSG_SetMsgId((CFE_MSG_Message_t*) Msg, CFE_SB_ValueToMsgId(BPNODE_STORAGE_HK_TLM_MID));

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    Msg->Payload                   = *StorTlmPayload;
    Msg->Payload.MonotonicTime     = MonotonicTime.Time;
    Msg->Payload.TimeBootEra       = MonotonicTime.BootEra;
    Msg->Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) Msg);
    Status = (BPLib_Status_t) CFE_SB_TransmitMsg((CFE_MSG_Message_t*) Msg, true);

    return Status;
}
