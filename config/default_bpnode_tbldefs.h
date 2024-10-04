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
 * @file
 *   Specification for the BPNode table related
 *   constant definitions.
 *
 * @note
 *   These Macro definitions have been put in this file (instead of
 *   bpnode_tbl.h). DO NOT PUT ANY TYPEDEFS OR
 *   STRUCTURE DEFINITIONS IN THIS FILE!
 *   ADD THEM TO bpnode_tbl.h IF NEEDED!
 */

#ifndef BPNODE_TBLDEFS_H
#define BPNODE_TBLDEFS_H

#include "common_types.h"
#include "bpnode_platform_cfg.h"

/************************************************************************
 * Macro Definitions
 ************************************************************************/
#define BPNODE_MAX_AUTH_SOURCES         10
#define BPNODE_MAX_NUM_LATENCY_SRC      10
#define BPNODE_MAX_NUM_STORE_SET        10
#define BPNODE_MAX_NUM_MIB_PS           10
#define BPNODE_MAX_MUN_CRS              10

/*
** Type Definitions
*/

/*
** CRS Table
*/
typedef struct
{
    char        DestEID[BPLIB_MAX_EID_LENGTH];
    uint32      TimeTrigger;
    uint32      SizeTrigger;
} BPNode_CRSSet_t;

typedef struct
{
    BPNode_CRSSet_t CRS_Set[BPNODE_MAX_MUN_CRS];
} BPNode_CRSTable_t;

/*
** Contacts Table
*/
typedef enum 
{
    UDPType,
    TCPType,
    EPPType,
    LTPType,
}CLAType_t;

typedef struct
{
    uint32      ContactID;
    char        DestEIDs[BPLIB_MAX_EID_LENGTH];
    uint32      CLAType;  
    char        CLAddr[BPLIB_MAX_EID_LENGTH];
    int32       PortNum;
    uint32      DestLTPEngineID;
    uint32      SendBytePerCycle;
    uint32      ReceiveBytePerCycle;
    uint32      RetransmitTimeout;
    uint32      CSTimeTrigger;
    uint32      CSSizeTrigger;
} BPNode_ContactsSet_t;

typedef struct
{
    BPNode_ContactsSet_t ContactSet[BPNODE_MAX_NUM_CONTACTS];
} BPNode_ContactsTable_t;

/*
** Custodian Authorization Policy Table
*/
typedef struct
{
    char AuthCustodians[BPNODE_MAX_AUTH_SOURCES][BPLIB_MAX_EID_LENGTH];
} BPNode_CustodianTable_t;

/*
** Custody Authorization Policy Table
*/
typedef struct
{
    char AuthCustodySrc[BPNODE_MAX_AUTH_SOURCES][BPLIB_MAX_EID_LENGTH];
} BPNode_CustodyTable_t;

/*
** Report-To Authorization EID Table
*/
typedef struct
{
    char AuthReportToEIDs[BPNODE_MAX_AUTH_SOURCES][BPLIB_MAX_EID_LENGTH];
} BPNode_ReportToTable_t;


/*
** Authorized Source EID Table
*/
typedef struct
{
    char AuthSrcEIDs[BPNODE_MAX_AUTH_SOURCES][BPLIB_MAX_EID_LENGTH];
} BPNode_SrcAuthTable_t;

/*
** Source Latency Table
*/
typedef enum
{
    Low,
    Med,
    High,
} SrcLatency;

typedef struct
{
    char    SrcEID[BPLIB_MAX_EID_LENGTH];
    uint32  Latency;
    uint32  Spare;    
}BPNode_SrcLatencySet_t;

typedef struct
{
    BPNode_SrcLatencySet_t LatencySet[BPNODE_MAX_NUM_LATENCY_SRC];
} BPNode_SrcLatencyTable_t;

/*
** Storage Policy Table
*/
typedef struct
{
    char SrcEID[BPLIB_MAX_EID_LENGTH];
    uint32 StorageSize;
} BPNode_StorageSet_t;

typedef struct
{
    BPNode_StorageSet_t StorageSet[BPNODE_MAX_NUM_STORE_SET];
} BPNode_StorageTable_t;

/*
** MIB Config Per Node Policy Table
*/
typedef struct
{
    char        SysNodeName[BPLIB_MAX_EID_LENGTH];
    char        SysNodeOwner[BPLIB_MAX_EID_LENGTH];
    char        SysSoftExec[BPLIB_MAX_EID_LENGTH];
    char        SysSoftExecVer[BPLIB_MAX_EID_LENGTH];
    uint32      BundleAgentNum;
    char        BundleAgentVer[BPLIB_MAX_EID_LENGTH];
    char        BundleAgentOpState[BPLIB_MAX_EID_LENGTH];
    char        BundleAgentConfig[BPLIB_MAX_EID_LENGTH];
    uint32      BundleAgentAvailStorage;
    uint32      BundleAgentCtdbSize;
    uint32      BundleSizeNoFrag;
    uint32      ParamSetMaxSeqenceNum;
    uint32      ParamSetMaxPayloadLength;
    uint32      ParamSetMaxBundleLength;
    uint32      ParamSetMaxTransmissionID;
    uint32      ParamSetMaxForwardingRetries;
    uint32      ParamSetMaxSimultaneousContacts;
    uint32      ParamSetBehaviorEventReporting;
    bool        ParamSetBehaviorReturnToSender;
    bool        ParamSetBehaviorTransIDReuse;
    bool        ParamSetBehaviorTransIDRollover;
    bool        ParamSetBehaviorWrapResponse;
    bool        ParamSupportBIBE;
    bool        ParamSupportCustody;
    bool        ParamSupportConfidentiality;
    bool        Spare;

} BPNode_MIBConfigPNTable_t;


/*
** MIB Config Per Source Policy Table
*/
typedef struct
{
    char        SrcEID[BPLIB_MAX_EID_LENGTH];
    char        EIDPattern[BPLIB_MAX_EID_LENGTH];
    uint32      ParamSetMaxLifetime;
    uint32      ParamSetMaxBSRGenerationRate;
    uint32      ParamSetMaxCBRGenerationRate;
    uint8       BundleSetBehaviorReceivedBSRGenerate;
    uint8       BundleSetBehaviorAcceptedBSRGenerate;
    uint8       BundleSetBehaviorForwardedBSRGenerate;
    uint8       BundleSetBehaviorDeliveredBSRGenerate;
    uint8       BundleSetBehaviorDeletedBSRGenerate;
    uint8       BundleSetBehaviorReceivedCBRGenerate;
    uint8       BundleSetBehaviorAcceptedCBRGenerate;
    uint8       BundleSetBehaviorForwardedCBRGenerate;
    uint8       BundleSetBehaviorDeliveredCBRGenerate;
    uint8       BundleSetBehaviorDeletedCBRGenerate;  
    uint16      Spare;
} BPNode_MIBConfigPSSet_t;

typedef struct
{
    BPNode_MIBConfigPSSet_t MIB_PS_Set[BPNODE_MAX_NUM_MIB_PS];
} BPNode_MIBConfigPSTable_t;

#endif /* BPNODE_TBLDEFS_H */
