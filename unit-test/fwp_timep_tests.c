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
** File: fwp_timep_tests.c
**
** Purpose:
** Coverage Unit Test cases for the Framework Proxy
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode's Framework Proxy
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */
#include "fwp_timep.h"
#include "bpnode_test_utils.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_BPA_TIMEP_GetMonotonicTime(void)
{
    /*
     * Test Case For:
     * int64_t BPA_TIMEP_GetMonotonicTime(void)
     */

    OS_time_t TimeMsec;
    int64 TestTime = 0;

    UT_SetDataBuffer(UT_KEY(CFE_PSP_GetTime), &TimeMsec, sizeof(TimeMsec), false);

    // TODO figure out how to pass in return value to OS_TimeGetTotalMilliseconds
    UtAssert_INT32_EQ((int32) BPA_TIMEP_GetMonotonicTime(), (int32) TestTime);
}

void Test_BPA_TIMEP_GetHostEpoch(void)
{
    /*
     * Test Case For:
     * void BPA_TIMEP_GetHostEpoch(BPA_TIMEP_Epoch_t *Epoch)
     */

    BPA_TIMEP_Epoch_t Epoch;

    BPA_TIMEP_GetHostEpoch(&Epoch);

    UtAssert_UINT32_EQ(Epoch.Year, CFE_MISSION_TIME_EPOCH_YEAR);
    UtAssert_UINT32_EQ(Epoch.Day, CFE_MISSION_TIME_EPOCH_DAY);
    UtAssert_UINT32_EQ(Epoch.Hour, CFE_MISSION_TIME_EPOCH_HOUR);
    UtAssert_UINT32_EQ(Epoch.Minute, CFE_MISSION_TIME_EPOCH_MINUTE);
    UtAssert_UINT32_EQ(Epoch.Second, CFE_MISSION_TIME_EPOCH_SECOND);
    UtAssert_UINT32_EQ(Epoch.Msec, CFE_MISSION_TIME_EPOCH_MICROS / 1000);
}

void Test_BPA_TIMEP_GetHostClockState(void)
{
    /*
     * Test Case For:
     * BPA_TIMEP_ClockState_t BPA_TIMEP_GetHostClockState(void)
     */

    /* Valid clock state */
    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_GetClockState), 1, CFE_TIME_ClockState_VALID);

    UtAssert_UINT32_EQ(BPA_TIMEP_GetHostClockState(), BPA_TIMEP_CLOCK_VALID);

    /* Invalid clock state (ex: flywheeling) */
    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_GetClockState), 1, CFE_TIME_ClockState_FLYWHEEL);

    UtAssert_UINT32_EQ(BPA_TIMEP_GetHostClockState(), BPA_TIMEP_CLOCK_INVALID);
}

void Test_BPA_TIMEP_GetHostTime(void)
{
    /*
     * Test Case For:
     * int64_t BPA_TIMEP_GetHostTime(void)
     */
    // CFE_TIME_SysTime_t HostTime;
    uint32 MicroSecs = 5678000;
    int64_t ExpectedTime;
    
    // TODO Figure out how to set return value of CFE_TIME_GetTAI
    // HostTime.Seconds = 12340;
    // ExpectedTime = (HostTime.Seconds * 1000) + (MicroSecs / 1000);
    ExpectedTime = MicroSecs / 1000;


    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_Sub2MicroSecs), 1, MicroSecs);

    UtAssert_INT32_EQ(BPA_TIMEP_GetHostTime(), ExpectedTime);
    UtAssert_STUB_COUNT(CFE_TIME_GetTAI, 1);

}


/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(BPA_TIMEP_GetMonotonicTime);
    ADD_TEST(BPA_TIMEP_GetHostEpoch);
    ADD_TEST(BPA_TIMEP_GetHostClockState);
    ADD_TEST(BPA_TIMEP_GetHostTime);
}
