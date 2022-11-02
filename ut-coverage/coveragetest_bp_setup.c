/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
 * Includes
 */
#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

#include "cfe.h"
#include "bp_app.h"
#include "bplib_routing.h"


/*----------------------------------------------------------------
 *
 * Function: UT_AltHandler_GenericPointerReturn
 *
 * A simple handler that can be used for any stub that returns a pointer.
 * it just forces the return value to be the object passed in as UserObj.
 *
 *-----------------------------------------------------------------*/
void UT_AltHandler_GenericPointerReturn(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_Stub_SetReturnValue(FuncKey, UserObj);
}

/*
 * Setup function prior to every test
 */
void BP_UT_Setup(void)
{
    UT_ResetState(0);
}

void TEST_BP_AppMain(void)
{
    /* Make bplib_route_alloc_table return NULL */
    UT_SetHandlerFunction(UT_KEY(bplib_route_alloc_table), UT_AltHandler_GenericPointerReturn, NULL);
    UtAssert_VOIDCALL(BP_AppMain());
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UtTest_Add(TEST_BP_AppMain, BP_UT_Setup, NULL, "BP_AppMain");
}
