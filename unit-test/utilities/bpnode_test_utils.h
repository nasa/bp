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
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"
#include "bpnode_utils.h"
#include "bpnode_msgids.h"
#include "bpnode_msg.h"
#include "bpnode_tbl.h"

/*
** Macro Definitions 
*/

/* Macro to add a test case to the list of tests to execute */
#define ADD_TEST(test) UtTest_Add(test, BPNode_UT_Setup, BPNode_UT_TearDown, #test)

/* Macro to get expected event name */
#define UT_CHECKEVENT_SETUP(Evt, ExpectedEvent, ExpectedFormat) \
    UT_CheckEvent_Setup_Impl(Evt, ExpectedEvent, #ExpectedEvent, ExpectedFormat)


/*
** Type Definitions
*/

/* Unit test check event hook information */
typedef struct
{
    uint16      ExpectedEvent;
    uint32      MatchCount;
    const char *ExpectedFormat;
} UT_CheckEvent_t;


/*
** Exported Functions
*/

/*
** Helper function to set up for event checking
** This attaches the hook function to CFE_EVS_SendEvent
*/
void UT_CheckEvent_Setup_Impl(UT_CheckEvent_t *Evt, uint16 ExpectedEvent, const char *EventName,
                              const char *ExpectedFormat);

/* Unit test case set up */
void BPNode_UT_Setup(void);

/* Unit test case teardown */
void BPNode_UT_TearDown(void);

#endif /* BPNODE_TEST_UTILS_H */
