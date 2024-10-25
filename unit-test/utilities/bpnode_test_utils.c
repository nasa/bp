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

/* Test table data */
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


/* An example hook function to check for a specific event */
static int32 UT_CheckEvent_Hook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context,
                                      va_list va)
{
    UT_CheckEvent_t *State = UserObj;
    uint16           EventId;
    const char *     Spec;

    /*
     * The BPLib_EM_SendEvent stub passes the EventID as the
     * first context argument.
     */
    if (Context->ArgCount > 0)
    {
        EventId = UT_Hook_GetArgValueByName(Context, "EventID", uint16);
        if (EventId == State->ExpectedEvent)
        {
            if (State->ExpectedFormat != NULL)
            {
                Spec = UT_Hook_GetArgValueByName(Context, "Spec", const char *);
                if (Spec != NULL)
                {
                    /*
                     * Example of how to validate the full argument set.
                     * ------------------------------------------------
                     *
                     * If really desired one can call something like:
                     *
                     * char TestText[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
                     * vsnprintf(TestText, sizeof(TestText), Spec, va);
                     *
                     * And then compare the output (TestText) to the expected fully-rendered string.
                     *
                     * NOTE: While this can be done, use with discretion - This isn't really
                     * verifying that the FSW code unit generated the correct event text,
                     * rather it is validating what the system snprintf() library function
                     * produces when passed the format string and args.
                     *
                     * This type of check has been demonstrated to make tests very fragile,
                     * because it is influenced by many factors outside the control of the
                     * test case.
                     *
                     * __This derived string is not an actual output of the unit under test__
                     */
                    if (strcmp(Spec, State->ExpectedFormat) == 0)
                    {
                        ++State->MatchCount;
                    }
                    else // Print mismatched expected vs. actual event format strings.
                    {
                        UtPrintf("CheckEvent: Mismatched event format string.");
                        UtPrintf("CheckEvent: Expected - [%s]", State->ExpectedFormat);
                        UtPrintf("CheckEvent: Actual - [%s]", Spec);
                    }
                }
                else
                {
                    UtPrintf(" <!> Spec is NULL");
                }
            }
            else
            {
                ++State->MatchCount;
            }
        }
        else
        {
            UtPrintf(" <!> Expected event ID of %d, received %d", State->ExpectedEvent, EventId);
        }
    }

    return 0;
}

/*
 * Helper function to set up for event checking
 * This attaches the hook function to BPLib_EM_SendEvent
 */
void UT_CheckEvent_Setup_Impl(UT_CheckEvent_t *Evt, uint16 ExpectedEvent, const char *EventName,
                              const char *ExpectedFormat)
{
    if (ExpectedFormat == NULL)
    {
        UtPrintf("CheckEvent will match: %s(%u)", EventName, ExpectedEvent);
    }
    else
    {
        UtPrintf("CheckEvent will match: %s(%u), \"%s\"", EventName, ExpectedEvent, ExpectedFormat);
    }
    memset(Evt, 0, sizeof(*Evt));
    Evt->ExpectedEvent  = ExpectedEvent;
    Evt->ExpectedFormat = ExpectedFormat;
    UT_SetVaHookFunction(UT_KEY(BPLib_EM_SendEvent), UT_CheckEvent_Hook, Evt);
    UT_SetVaHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_CheckEvent_Hook, Evt);
}

/* Setup function prior to every test */
void BPNode_UT_Setup(void)
{
    UT_ResetState(0);

    memset(&BPNode_AppData, 0, sizeof(BPNode_AppData_t));

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
}

/* Teardown function after every test */
void BPNode_UT_TearDown(void) {
    /* Clean up test environment */
}
