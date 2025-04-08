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
 *  Utility function definitions for BPNode unit tests
 * 
 */


/*
** Include Files
*/

#include "common_types.h"
#include "cfe_evs.h"

#include "bpnode_test_utils.h"
#include "bplib.h"

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

/*
** Global Data
*/

/* Test configuration data */
BPA_ADUP_Table_t            TestAduTbl;
BPLib_PI_ChannelTable_t     TestChanTbl;
BPLib_CLA_ContactsTable_t   TestContactsTbl;
BPLib_ARP_CRSTable_t        TestCrsTbl;
BPLib_PDB_CustodianTable_t  TestCustodianTbl;
BPLib_PDB_CustodyTable_t    TestCustodyTbl;
BPLib_NC_MIBConfigPNTable_t TestMibPnTbl;
BPLib_NC_MIBConfigPSTable_t TestMibPsTbl;
BPLib_PDB_ReportToTable_t   TestReportTbl;
BPLib_PDB_SrcAuthTable_t    TestAuthTbl;
BPLib_PDB_SrcLatencyTable_t TestLatencyTbl;
BPLib_STOR_StorageTable_t   TestStorTbl;

#define UT_MAX_CFE_SENDEVENT_DEPTH 10
#define UT_MAX_TRANSLATE_DEPTH     13

CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[UT_MAX_CFE_SENDEVENT_DEPTH];
BPLib_Status_t Context_BPLib_Status[UT_MAX_TRANSLATE_DEPTH];
CFE_Status_t Context_CFE_Status[UT_MAX_TRANSLATE_DEPTH];

/*
** Function Definitions
*/

void UT_Handler_CFE_EVS_SendEvent(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint16 CallCount;
    uint16 idx;

    CallCount = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    if (CallCount > (sizeof(context_CFE_EVS_SendEvent) / sizeof(context_CFE_EVS_SendEvent[0])))
    {
        UtAssert_Failed("CFE_EVS_SendEvent UT depth %u exceeded: %u, increase UT_MAX_SENDEVENT_DEPTH",
                        UT_MAX_SENDEVENT_DEPTH, CallCount);
    }
    else
    {
        idx                                      = CallCount - 1;
        context_CFE_EVS_SendEvent[idx].EventID   = UT_Hook_GetArgValueByName(Context, "EventID", uint16);
        context_CFE_EVS_SendEvent[idx].EventType = UT_Hook_GetArgValueByName(Context, "EventType", uint16);

        strncpy(context_CFE_EVS_SendEvent[idx].Spec, UT_Hook_GetArgValueByName(Context, "Spec", const char *),
                CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);
        context_CFE_EVS_SendEvent[idx].Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH - 1] = '\0';
    }
}

void UT_Handler_BPA_BPLib_Status_Translate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint16 CallCount;
    uint16 idx;

    CallCount = UT_GetStubCount(UT_KEY(BPA_BPLib_Status_Translate));

    if (CallCount > UT_MAX_TRANSLATE_DEPTH)
    {
        UtAssert_Failed("BPA_BPLib_Status_Translate UT depth %u exceeded: %u, increase UT_MAX_TRANSLATE_DEPTH",
                        UT_MAX_TRANSLATE_DEPTH, CallCount);
    }
    else
    {
        idx = CallCount - 1;
        Context_BPLib_Status[idx] = UT_Hook_GetArgValueByName(Context, "BPLib_Status", BPLib_Status_t);
    }
}

void UT_Handler_BPA_CFE_Status_Translate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint16 CallCount;
    uint16 idx;

    CallCount = UT_GetStubCount(UT_KEY(BPA_CFE_Status_Translate));

    if (CallCount > UT_MAX_TRANSLATE_DEPTH)
    {
        UtAssert_Failed("BPA_CFE_Status_Translate UT depth %u exceeded: %u, increase UT_MAX_TRANSLATE_DEPTH",
                        UT_MAX_TRANSLATE_DEPTH, CallCount);
    }
    else
    {
        idx = CallCount - 1;
        Context_CFE_Status[idx] = UT_Hook_GetArgValueByName(Context, "CFE_Status", CFE_Status_t);
    }
}

void UT_Handler_BPLib_PI_Egress(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    size_t *AduSize = UT_Hook_GetArgValueByName(Context, "AduSize", size_t *);
    int32 Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status >= 0)
    {
        UT_Stub_CopyToLocal(UT_KEY(BPLib_PI_Egress), AduSize, sizeof(size_t));
    }
}

void BPNode_Test_Verify_Event(uint16_t EventNum, int32_t EventID, const char* EventText)
{
    /* Check the string */
    UtAssert_INT32_EQ(context_BPLib_EM_SendEvent[EventNum].EventID, EventID);
    UtAssert_STRINGBUF_EQ(EventText, BPLIB_EM_EXPANDED_EVENT_SIZE,
                            context_BPLib_EM_SendEvent[EventNum].Spec, BPLIB_EM_EXPANDED_EVENT_SIZE);
}

void Test_FWP_ADUP_VerifyIncrement(BPLib_EID_t EID, BPLib_AS_Counter_t Counter, uint32_t Amount, int16_t CallNum)
{
    UtAssert_STUB_COUNT(BPLib_AS_Increment, CallNum);

    UtAssert_EQ(uint64_t, EID.Scheme,       Context_BPLib_AS_Increment[CallNum - 1].EID.Scheme);
    UtAssert_EQ(uint64_t, EID.IpnSspFormat, Context_BPLib_AS_Increment[CallNum - 1].EID.IpnSspFormat);
    UtAssert_EQ(uint64_t, EID.Allocator,    Context_BPLib_AS_Increment[CallNum - 1].EID.Allocator);
    UtAssert_EQ(uint64_t, EID.Node,         Context_BPLib_AS_Increment[CallNum - 1].EID.Node);
    UtAssert_EQ(uint64_t, EID.Service,      Context_BPLib_AS_Increment[CallNum - 1].EID.Service);

    if (Counter != (BPLib_AS_Counter_t) -1)
    {
        UtAssert_EQ(BPLib_AS_Counter_t, Counter, Context_BPLib_AS_Increment[CallNum - 1].Counter);
    }

    if (Amount != (uint32_t) -1)
    {
        UtAssert_EQ(uint32_t, Amount, Context_BPLib_AS_Increment[CallNum - 1].Amount);
    }
}

void Test_FWP_ADUP_VerifyDecrement(BPLib_EID_t EID, BPLib_AS_Counter_t Counter, uint32_t Amount, int16_t CallNum)
{
    UtAssert_STUB_COUNT(BPLib_AS_Decrement, CallNum);

    UtAssert_EQ(uint64_t, EID.Scheme,       Context_BPLib_AS_Increment[CallNum - 1].EID.Scheme);
    UtAssert_EQ(uint64_t, EID.IpnSspFormat, Context_BPLib_AS_Increment[CallNum - 1].EID.IpnSspFormat);
    UtAssert_EQ(uint64_t, EID.Allocator,    Context_BPLib_AS_Increment[CallNum - 1].EID.Allocator);
    UtAssert_EQ(uint64_t, EID.Node,         Context_BPLib_AS_Increment[CallNum - 1].EID.Node);
    UtAssert_EQ(uint64_t, EID.Service,      Context_BPLib_AS_Increment[CallNum - 1].EID.Service);

    if (Counter != (BPLib_AS_Counter_t) -1)
    {
        UtAssert_EQ(BPLib_AS_Counter_t, Counter, Context_BPLib_AS_Decrement[CallNum - 1].Counter);
    }

    if (Amount != (uint32_t) -1)
    {
        UtAssert_EQ(uint32_t, Amount, Context_BPLib_AS_Decrement[CallNum - 1].Amount);
    }
}

/* Setup function prior to every test */
void BPNode_UT_Setup(void)
{
    UT_ResetState(0);

    memset((void*) &BPNode_AppData, 0, sizeof(BPNode_AppData_t));
    memset((void*) &BPNode_AppData.ConfigPtrs, 0, sizeof(BPLib_NC_ConfigPtrs_t));

    memset(context_BPLib_EM_SendEvent, 0, sizeof(BPLib_EM_SendEvent_context_t) * UT_MAX_SENDEVENT_DEPTH);
    memset(context_CFE_EVS_SendEvent, 0, sizeof(context_CFE_EVS_SendEvent));
    memset(Context_BPLib_Status, 0, sizeof(BPLib_Status_t) * UT_MAX_TRANSLATE_DEPTH);
    memset(Context_CFE_Status, 0, sizeof(CFE_Status_t) * UT_MAX_TRANSLATE_DEPTH);

    memset(&TestAduTbl, 0, sizeof(BPA_ADUP_Table_t));
    memset(&TestChanTbl, 0, sizeof(BPLib_PI_ChannelTable_t));
    memset(&TestContactsTbl, 0, sizeof(BPLib_CLA_ContactsTable_t));
    memset(&TestCrsTbl, 0, sizeof(BPLib_ARP_CRSTable_t));
    memset(&TestCustodianTbl, 0, sizeof(BPLib_PDB_CustodianTable_t));
    memset(&TestCustodyTbl, 0, sizeof(BPLib_PDB_CustodyTable_t));
    memset(&TestMibPnTbl, 0, sizeof(BPLib_NC_MIBConfigPNTable_t));
    memset(&TestMibPsTbl, 0, sizeof(BPLib_NC_MIBConfigPSTable_t));
    memset(&TestReportTbl, 0, sizeof(BPLib_PDB_ReportToTable_t));
    memset(&TestAuthTbl, 0, sizeof(BPLib_PDB_SrcAuthTable_t));
    memset(&TestLatencyTbl, 0, sizeof(BPLib_PDB_SrcLatencyTable_t));
    memset(&TestStorTbl, 0, sizeof(BPLib_STOR_StorageTable_t));

    UT_SetHandlerFunction(UT_KEY(BPLib_EM_SendEvent), UT_Handler_BPLib_EM_SendEvent, NULL);
    UT_SetHandlerFunction(UT_KEY(CFE_EVS_SendEvent), UT_Handler_CFE_EVS_SendEvent, NULL);
    UT_SetHandlerFunction(UT_KEY(BPA_BPLib_Status_Translate), UT_Handler_BPA_BPLib_Status_Translate, NULL);
    UT_SetHandlerFunction(UT_KEY(BPA_CFE_Status_Translate), UT_Handler_BPA_CFE_Status_Translate, NULL);
    UT_SetHandlerFunction(UT_KEY(BPLib_AS_Increment), UT_Handler_BPLib_AS_Increment, NULL);
    UT_SetHandlerFunction(UT_KEY(BPLib_AS_Decrement), UT_Handler_BPLib_AS_Decrement, NULL);
    UT_SetHandlerFunction(UT_KEY(BPLib_PI_Egress), UT_Handler_BPLib_PI_Egress, NULL);
    UT_SetHandlerFunction(UT_KEY(BPLib_CLA_GetContactRunState), UT_Handler_BPLib_CLA_GetContactRunState, NULL);

    BPNode_AppData.AduProxyTablePtr              = &TestAduTbl;
    BPNode_AppData.ConfigPtrs.AuthConfigPtr      = &TestAuthTbl;
    BPNode_AppData.ConfigPtrs.ChanConfigPtr      = &TestChanTbl;
    BPNode_AppData.ConfigPtrs.ContactsConfigPtr  = &TestContactsTbl;
    BPNode_AppData.ConfigPtrs.CrsConfigPtr       = &TestCrsTbl;
    BPNode_AppData.ConfigPtrs.CustodianConfigPtr = &TestCustodianTbl;
    BPNode_AppData.ConfigPtrs.CustodyConfigPtr   = &TestCustodyTbl;
    BPNode_AppData.ConfigPtrs.LatConfigPtr       = &TestLatencyTbl;
    BPNode_AppData.ConfigPtrs.MibPnConfigPtr     = &TestMibPnTbl;
    BPNode_AppData.ConfigPtrs.MibPsConfigPtr     = &TestMibPsTbl;
    BPNode_AppData.ConfigPtrs.ReportConfigPtr    = &TestReportTbl;
    BPNode_AppData.ConfigPtrs.StorConfigPtr      = &TestStorTbl;
}

/* Teardown function after every test */
void BPNode_UT_TearDown(void) {
    /* Clean up test environment */
}
