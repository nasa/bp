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
**   This file contains the source code for the FWP Table Proxy.
*/

/*
** Include
*/

#include "fwp_tlmp.h"
#include "bpnode_utils.h"

/*
** Function Definitions
*/


BPLib_Status_t BPA_TLMP_Init(void)
{
    BPLib_Status_t Status = CFE_SUCCESS;
    
    /* Initialize Software Bus HK Telemetry Pipe */
    Status = CFE_SB_CreatePipe(&BPNode_AppData.TlmConfigData.tlmPipe, BPNODE_TLM_PIPE_DEPTH, BPNODE_TLM_PIPE_NAME);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_PIPE_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error creating SB TLM Pipe, Error = %d", Status);
        return Status;
    }
        
    /* Subscribe to BP Send MIB Per Node Configs Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_CONFIG_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_NODE_CONFIG_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Node MIB Config, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_CONFIG_HK_TLM_MID), sizeof(BPNode_NodeMibConfigHkTlm_t));
    
    /* Subscribe to BP Send MIB Per Source Configs Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_SRC_CONFIG_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Source MIB Config, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID), sizeof(BPNode_SourceMibConfigHkTlm_t));

    /* Subscribe to BP Send MIB Per Node Counters Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_NODE_COUNTER_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Node MIB Counters, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID), sizeof(BPNode_NodeMibCountersHkTlm_t));

    /* Subscribe to BP Send MIB Per Source Counters Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_SRC_COUNTER_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Source MIB Counters, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID), sizeof(BPNode_SourceMibCountersHkTlm_t));

    /* Subscribe to BP Send Storage Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_STORAGE_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_STORAGE_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Storage, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.StorageHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_STORAGE_HK_TLM_MID), sizeof(BPNode_StorageHkTlm_t));

    /* Subscribe to BP Send Channel Contact Housekeeping Requests */
    Status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID), BPNode_AppData.TlmConfigData.tlmPipe);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_CHANNEL_CONTACT_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error subscribe to SB TLM Channel Contact, Error = %d", Status);
        return Status;
    }
    /* Initialize Messages*/
    CFE_MSG_Init(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.ChannelContactHkPkt.TelemetryHeader), CFE_SB_ValueToMsgId(BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID), sizeof(BPNode_ChannelContactStatHkTlm_t));
    
    return Status;    
}

BPLib_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t* NodeMIBConfigTlmPayload)
{
    BPLib_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.Payload = *NodeMIBConfigTlmPayload;
    BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibConfigHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_NODE_CONFIG_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Node MIB Config Tlm packet, Error = %d", Status);
    }    
    return Status;
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t* SrcMIBConfigTlmPayload)
{
    BPLib_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.Payload = *SrcMIBConfigTlmPayload;
    BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibConfigHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_SRC_CONFIG_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Source MIB Config Tlm packet, Error = %d", Status);
    }
    return Status;
}

BPLib_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t* NodeMIBCounterTlmPayload)
{
    BPLib_Status_t Status = CFE_SUCCESS;    
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.Payload = *NodeMIBCounterTlmPayload;    
    BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.NodeMibCountersHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_NODE_COUNTERS_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Node MIB Counters Tlm packet, Error = %d", Status);
    }
    return Status;
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t* SrcMIBCounterTlmPayload )
{
    BPLib_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.Payload = *SrcMIBCounterTlmPayload;
    BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.SrcMibCountersHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_SRC_COUNTERS_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Source MIB Counters Tlm packet, Error = %d", Status);
    }
    return Status;
}

BPLib_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t* ChannelContactTlmPayload)
{
    BPLib_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.ChannelContactHkPkt.Payload = *ChannelContactTlmPayload;
    BPNode_AppData.TlmConfigData.ChannelContactHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.ChannelContactHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.ChannelContactHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.ChannelContactHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.ChannelContactHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_CHANNEL_CONTACT_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Channel Contact Tlm packet, Error = %d", Status);
    }
    return Status;
}

BPLib_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t* StorTlmPayload)
{
    BPLib_Status_t Status = CFE_SUCCESS;
    BPLib_TIME_MonotonicTime_t MonotonicTime;

    BPLib_TIME_GetMonotonicTime(&MonotonicTime);
    BPNode_AppData.TlmConfigData.StorageHkPkt.Payload = *StorTlmPayload;
    BPNode_AppData.TlmConfigData.StorageHkPkt.Payload.MonotonicTime = MonotonicTime.Time;
    BPNode_AppData.TlmConfigData.StorageHkPkt.Payload.TimeBootEra = MonotonicTime.BootEra;
    BPNode_AppData.TlmConfigData.StorageHkPkt.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.StorageHkPkt.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(BPNode_AppData.TlmConfigData.StorageHkPkt.TelemetryHeader), true);
    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_TLM_SB_TRANS_STORAGE_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Error SB Transmit Storage Tlm packet, Error = %d", Status);
    }
    return Status;
}
