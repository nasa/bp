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
** @file
**   FWP's Telemetry Proxy (TLMP) header definitions
*/

#ifndef FWP_TLMP_H
#define FWP_TLMP_H

#include "cfe.h"
#include "cfe_config.h"
#include "bplib.h"
#include "bpnode_msgstruct.h"
#include "fwp_utils.h"

/**
 * \brief FWP Telemetry Proxy Initialization
 *
 *  \par Description
 *       Telemetry Proxy Init, create SB pipe and subscribe messages
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] none
 *
 *  \return None
 */
void BPA_TLMP_Init(void);

/**
 * \brief FWP Telemetry Proxy Send Node MIB Configuration HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Node MIB Configuration HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Node MIB Configurations payload data 
 *
 *  \return execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendNodeMibConfigPkt(BPLib_NodeMibConfigHkTlm_Payload_t* NodeMIBConfigTlmPayload);

/**
 * \brief FWP Telemetry Proxy Send Per Source MIB Configuration HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Per Source MIB Configuration HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Per Source MIB Configurations payload data 
 *
 *  \return execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendPerSourceMibConfigPkt(BPLib_SourceMibConfigHkTlm_Payload_t* SrcMIBConfigTlmPayload);

/**
 * \brief FWP Telemetry Proxy Send Node MIB Counters HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Node MIB Counters HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Per Node MIB Counters payload data 
 *
 *  \return execution status
 *  \retval CFE_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendNodeMibCounterPkt(BPLib_NodeMibCountersHkTlm_Payload_t* NodeMIBCounterTlmPayload);

/**
 * \brief FWP Telemetry Proxy Send Per Source MIB Counters HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Per Source MIB Counters HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Per Source MIB Counters payload data 
 *
 *  \return execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendPerSourceMibCounterPkt(BPLib_SourceMibCountersHkTlm_Payload_t* SrcMIBCounterTlmPayload);

/**
 * \brief FWP Telemetry Proxy Send Channel/Contact status HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Channel/Contact status HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Channel contact status payload data 
 *
 *  \return execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendChannelContactPkt(BPLib_ChannelContactStatHkTlm_Payload_t* ChannelContactTlmPayload);

/**
 * \brief FWP Telemetry Proxy Send Storage status HK packet
 *
 *  \par Description
 *       Telemetry Proxy, Send Storage status HK packet
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Storage status payload data 
 *
 *  \return execution status
 *  \retval BPLIB_SUCCESS Output was successful
 */
BPLib_Status_t BPA_TLMP_SendStoragePkt(BPLib_StorageHkTlm_Payload_t* StorTlmPayload);

#endif /* FWP_TLMP_H */