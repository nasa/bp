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

/*
** Function Definitions
*/


void BPA_TLMP_Init(void)
{
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.NodeMibConfigHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_CONFIG_HK_TLM_MID), sizeof(BPNode_NodeMibConfigHkTlm_t));    
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.SrcMibConfigHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID), sizeof(BPNode_SourceMibConfigHkTlm_t));
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID), sizeof(BPNode_NodeMibCountersHkTlm_t));
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.SrcMibCountersHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID), sizeof(BPNode_SourceMibCountersHkTlm_t));
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.StorageHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_STORAGE_HK_TLM_MID), sizeof(BPNode_StorageHkTlm_t));
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.ChannelContactHkPkt.TelemetryHeader), 
            CFE_SB_ValueToMsgId(BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID), sizeof(BPNode_ChannelContactStatHkTlm_t));
    
}

CFE_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t* NodeMIBConfigTlmPayload)
{
    CFE_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.NodeMibConfigHkPkt.Payload = *NodeMIBConfigTlmPayload;
    BPNode_AppData.NodeMibConfigHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibConfigHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibConfigHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibConfigHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibConfigHkPkt.TelemetryHeader), true);
    return Status;
}

CFE_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t* SrcMIBConfigTlmPayload)
{
    CFE_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.SrcMibConfigHkPkt.Payload = *SrcMIBConfigTlmPayload;
    BPNode_AppData.SrcMibConfigHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.SrcMibConfigHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.SrcMibConfigHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.SrcMibConfigHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.SrcMibConfigHkPkt.TelemetryHeader), true);
    return Status;
}

CFE_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t* NodeMIBCounterTlmPayload)
{
    CFE_Status_t Status = CFE_SUCCESS;    
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.NodeMibCountersHkPkt.Payload = *NodeMIBCounterTlmPayload;    
    BPNode_AppData.NodeMibCountersHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.NodeMibCountersHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.NodeMibCountersHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.NodeMibCountersHkPkt.TelemetryHeader), true);
    return Status;
}

CFE_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t* SrcMIBCounterTlmPayload )
{
    CFE_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.SrcMibCountersHkPkt.Payload = *SrcMIBCounterTlmPayload;
    BPNode_AppData.SrcMibCountersHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.SrcMibCountersHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.SrcMibCountersHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.SrcMibCountersHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.SrcMibCountersHkPkt.TelemetryHeader), true);
    return Status;
}

CFE_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t* ChannelContactTlmPayload)
{
    CFE_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.ChannelContactHkPkt.Payload = *ChannelContactTlmPayload;
    BPNode_AppData.ChannelContactHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.ChannelContactHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.ChannelContactHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.ChannelContactHkPkt.TelemetryHeader), true);
    return Status;
}

CFE_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t* StorTlmPayload)
{
    CFE_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.StorageHkPkt.Payload = *StorTlmPayload;
    BPNode_AppData.StorageHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.StorageHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.StorageHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.StorageHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.StorageHkPkt.TelemetryHeader), true);
    return Status;
}
