/*
 * NASA Docket No. GSC-19,559-1, and identified as "Delay/Disruption Tolerant Networking 
 * (DTN) Bundle Protocol (BP) v7 Core Flight System (cFS) Application Build 7.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software distributed under 
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF 
 * ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. The copyright notice to be 
 * included in the software is as follows: 
 *
 * Copyright 2025 United States Government as represented by the Administrator of the 
 * National Aeronautics and Space Administration. All Rights Reserved. */


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

/* TLMP-scoped packets to ease packet initialization overhead */
BPNode_NodeMibConfigHkTlm_t      NodeMibConfigHkTlm;      /** \brief Node MIB Config housekeeping packet */
BPNode_SourceMibConfigHkTlm_t    SourceMibConfigHkTlm;    /** \brief Per Source MIB Config housekeeping packet */
BPNode_NodeMibCountersHkTlm_t    NodeMibCountersHkTlm;    /** \brief Node MIB counter housekeeping packet */
BPNode_SourceMibCountersHkTlm_t  SourceMibCountersHkTlm;  /** \brief Per source MIB counters housekeeping packet */
BPNode_NodeMibReportsHkTlm_t     NodeMibReportsHkTlm;     /** \brief Node MIB reports housekeeping packet */
BPNode_ChannelContactStatHkTlm_t ChannelContactStatHkTlm; /** \brief Channel contact statistics housekeeping packet */
BPNode_StorageHkTlm_t            StorageHkTlm;            /** \brief Storage housekeeping packet */


/*
** Function Definitions
*/

void BPA_TLMP_Init(void)
{
    /* Inititalize node MIB configuration housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(NodeMibConfigHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_CONFIG_HK_TLM_MID),
                    sizeof(BPNode_NodeMibConfigHkTlm_t));

    /* Inititalize source MIB configuration housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(SourceMibConfigHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_CONFIG_HK_TLM_MID),
                    sizeof(BPNode_SourceMibConfigHkTlm_t));

    /* Inititalize node MIB counters housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(NodeMibCountersHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_COUNTERS_HK_TLM_MID),
                    sizeof(BPNode_NodeMibCountersHkTlm_t));

    /* Inititalize node MIB reports housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(NodeMibReportsHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_NODE_MIB_REPORTS_HK_TLM_MID),
                    sizeof(BPNode_NodeMibReportsHkTlm_t));                    

    /* Inititalize source MIB counters housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(SourceMibCountersHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_SOURCE_MIB_COUNTERS_HK_TLM_MID),
                    sizeof(BPNode_SourceMibCountersHkTlm_t));

    /* Inititalize channel contact statistics housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(ChannelContactStatHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_CHANNEL_CONTACT_STAT_HK_TLM_MID),
                    sizeof(BPNode_ChannelContactStatHkTlm_t));

    /* Inititalize storage housekeeping packet */
    CFE_MSG_Init(CFE_MSG_PTR(StorageHkTlm.TelemetryHeader),
                    CFE_SB_ValueToMsgId(BPNODE_STORAGE_HK_TLM_MID),
                    sizeof(BPNode_StorageHkTlm_t));
}

BPLib_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t* NodeMibConfigHkTlm_Payload)
{
    CFE_Status_t Status;

    NodeMibConfigHkTlm.Payload                   = *NodeMibConfigHkTlm_Payload;
    NodeMibConfigHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    NodeMibConfigHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(NodeMibConfigHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(NodeMibConfigHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t* SourceMibConfigHkTlm_Payload)
{
    CFE_Status_t Status;

    SourceMibConfigHkTlm.Payload                   = *SourceMibConfigHkTlm_Payload;
    SourceMibConfigHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    SourceMibConfigHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(SourceMibConfigHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(SourceMibConfigHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t* NodeMibCountersHkTlm_Payload)
{
    CFE_Status_t Status;
    
    NodeMibCountersHkTlm.Payload                   = *NodeMibCountersHkTlm_Payload;    
    NodeMibCountersHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    NodeMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(NodeMibCountersHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(NodeMibCountersHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t* SourceMibCountersHkTlm_Payload)
{
    CFE_Status_t Status;
    
    SourceMibCountersHkTlm.Payload                   = *SourceMibCountersHkTlm_Payload;
    SourceMibCountersHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    SourceMibCountersHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(SourceMibCountersHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(SourceMibCountersHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendNodeMibReportsPkt(BPLib_NodeMibReportsHkTlm_Payload_t* NodeMibReportsHkTlm_Payload)
{
    CFE_Status_t Status;
    
    NodeMibReportsHkTlm.Payload                    = *NodeMibReportsHkTlm_Payload;    
    NodeMibReportsHkTlm.Payload.MonotonicTime      = BPLib_TIME_GetMonotonicTime();
    NodeMibReportsHkTlm.Payload.CorrelationFactor  = BPLib_TIME_GetCorrelationFactor();
    
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(NodeMibReportsHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(NodeMibReportsHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t* ChannelContactStatHkTlm_Payloa)
{
    CFE_Status_t Status;
    
    ChannelContactStatHkTlm.Payload                   = *ChannelContactStatHkTlm_Payloa;
    ChannelContactStatHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    ChannelContactStatHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(ChannelContactStatHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(ChannelContactStatHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}

BPLib_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t* StorageHkTlm_Payload)
{
    CFE_Status_t Status;
    
    StorageHkTlm.Payload                   = *StorageHkTlm_Payload;
    StorageHkTlm.Payload.MonotonicTime     = BPLib_TIME_GetMonotonicTime();
    StorageHkTlm.Payload.CorrelationFactor = BPLib_TIME_GetCorrelationFactor();

    CFE_SB_TimeStampMsg(CFE_MSG_PTR(StorageHkTlm.TelemetryHeader));
    Status = CFE_SB_TransmitMsg(CFE_MSG_PTR(StorageHkTlm.TelemetryHeader), true);

    return BPA_CFE_Status_Translate(Status);
}
