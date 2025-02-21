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
 *  Utility function declarations for BPNode unit tests
 */

#ifndef BPNODE_TEST_UTILS_H
#define BPNODE_TEST_UTILS_H

/*
** Include Files
*/

#include "common_types.h"
#include "cfe_evs.h"

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

#include "cfe.h"
#include "bpnode_eventids.h"
#include "bpnode_app.h"
#include "bpnode_utils.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"
#include "bpnode_tbl.h"

#include "fwp_dp.h"
#include "bplib_em_handlers.h"
#include "bplib_as_handlers.h"
#include "fwp_tablep.h"

/*
** Type Definitions
*/

/* CFE_EVS_SendEvent handler information */
typedef struct
{
    uint16 EventID;
    uint16 EventType;
    char   Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
} CFE_EVS_SendEvent_context_t;

/*
** Global Data
*/

extern CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[];
extern BPLib_Status_t Context_BPLib_Status[];
extern CFE_Status_t Context_CFE_Status[];

extern BPA_ADUP_Table_t            TestAduTbl;
extern BPLib_PI_ChannelTable_t     TestChanTbl;
extern BPLib_CLA_ContactsTable_t   TestContactsTbl;
extern BPLib_ARP_CRSTable_t        TestCrsTbl;
extern BPLib_PDB_CustodianTable_t  TestCustodianTbl;
extern BPLib_PDB_CustodyTable_t    TestCustodyTbl;
extern BPLib_NC_MIBConfigPNTable_t TestMibPnTbl;
extern BPLib_NC_MIBConfigPSTable_t TestMibPsTbl;
extern BPLib_PDB_ReportToTable_t   TestReportTbl;
extern BPLib_PDB_SrcAuthTable_t    TestAuthTbl;
extern BPLib_PDB_SrcLatencyTable_t TestLatencyTbl;
extern BPLib_STOR_StorageTable_t   TestStorTbl;

/*
** Macro Definitions 
*/

/* Macro to add a test case to the list of tests to execute */
#define ADD_TEST(test) UtTest_Add(test, BPNode_UT_Setup, BPNode_UT_TearDown, #test)


/*
** Exported Functions
*/

/* Handler to capture arguments passed to CFE_EVS_SendEvent */
void UT_Handler_CFE_EVS_SendEvent(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context);

/* Handler to capture arguments passed to BPA_BPLib_Status_Translate */
void UT_Handler_BPA_BPLib_Status_Translate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context);

/* Handler to capture arguments passed to BPA_CFE_Status_Translate */
void UT_Handler_BPA_CFE_Status_Translate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context);

/* Verify values given when BPLib_EM_SendEvent is called */
void BPNode_Test_Verify_Event(uint16_t EventNum, int32_t EventID, const char* EventText);

/* Verify values given when BPLib_AS_Increment is called, match what's expected */
void Test_FWP_ADUP_VerifyIncrement(BPLib_EID_t EID, BPLib_AS_Counter_t Counter, uint32_t Amount, int16_t CallNum);

/* Verify values given when BPLib_AS_Decrement is called, match what's expected */
void Test_FWP_ADUP_VerifyDecrement(BPLib_EID_t EID, BPLib_AS_Counter_t Counter, uint32_t Amount, int16_t CallNum);

/* Unit test case set up */
void BPNode_UT_Setup(void);

/* Unit test case teardown */
void BPNode_UT_TearDown(void);

#endif /* BPNODE_TEST_UTILS_H */
