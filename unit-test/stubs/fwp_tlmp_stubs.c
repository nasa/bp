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

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in fwp_tlmp header
 */

#include "fwp_tlmp.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_Init()
 * ----------------------------------------------------
 */
void BPA_TLMP_Init(void)
{

    UT_GenStub_Execute(BPA_TLMP_Init, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendChannelContactPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t *ChannelContactTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendChannelContactPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendChannelContactPkt, BPLib_ChannelContactStatHkTlm_Payload_t *,
                        ChannelContactTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendChannelContactPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendChannelContactPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendNodeMibConfigPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t *NodeMIBConfigTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendNodeMibConfigPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendNodeMibConfigPkt, BPLib_NodeMibConfigHkTlm_Payload_t *, NodeMIBConfigTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendNodeMibConfigPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendNodeMibConfigPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendNodeMibCounterPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t *NodeMIBCounterTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendNodeMibCounterPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendNodeMibCounterPkt, BPLib_NodeMibCountersHkTlm_Payload_t *,
                        NodeMIBCounterTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendNodeMibCounterPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendNodeMibCounterPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendNodeMibReportsPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendNodeMibReportsPkt(BPLib_NodeMibReportsHkTlm_Payload_t *NodeMibReportsHkTlm_Payload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendNodeMibReportsPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendNodeMibReportsPkt, BPLib_NodeMibReportsHkTlm_Payload_t *,
                        NodeMibReportsHkTlm_Payload);

    UT_GenStub_Execute(BPA_TLMP_SendNodeMibReportsPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendNodeMibReportsPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendPerSourceMibConfigPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t *SrcMIBConfigTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendPerSourceMibConfigPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendPerSourceMibConfigPkt, BPLib_SourceMibConfigHkTlm_Payload_t *,
                        SrcMIBConfigTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendPerSourceMibConfigPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendPerSourceMibConfigPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendPerSourceMibCounterPkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t *SrcMIBCounterTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendPerSourceMibCounterPkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendPerSourceMibCounterPkt, BPLib_SourceMibCountersHkTlm_Payload_t *,
                        SrcMIBCounterTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendPerSourceMibCounterPkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendPerSourceMibCounterPkt, BPLib_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TLMP_SendStoragePkt()
 * ----------------------------------------------------
 */
BPLib_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t *StorTlmPayload)
{
    UT_GenStub_SetupReturnBuffer(BPA_TLMP_SendStoragePkt, BPLib_Status_t);

    UT_GenStub_AddParam(BPA_TLMP_SendStoragePkt, BPLib_StorageHkTlm_Payload_t *, StorTlmPayload);

    UT_GenStub_Execute(BPA_TLMP_SendStoragePkt, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TLMP_SendStoragePkt, BPLib_Status_t);
}
