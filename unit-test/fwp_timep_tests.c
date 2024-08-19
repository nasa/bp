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
 *  Unit tests for fwp_timep.c
 */

/*
** Include Files
*/

#include "fwp_timep.h"
#include "bpnode_test_utils.h"


/*
** Function Definitions
*/

/* Handler to set seconds value for CFE_TIME_GetTAI to provided value */
void Test_CFE_TIME_GetTAI_Handler(void *UserObj, UT_EntryKey_t FuncKey, 
                                                        const UT_StubContext_t *Context)
{
    CFE_TIME_SysTime_t Time;
    uint32 *SecsPtr = UserObj;

    Time.Seconds = *SecsPtr;
    Time.Subseconds = 0;

    UT_Stub_SetReturnValue(UT_KEY(CFE_TIME_GetTAI), Time);
}

/* Test BPA_TIMEP_GetMonotonicTime */
void Test_BPA_TIMEP_GetMonotonicTime(void)
{
    int64 TimeTicks = 123450000;
    int64 TimeMsec = 12345;

    UT_SetDataBuffer(UT_KEY(CFE_PSP_GetTime), (OS_time_t *) &TimeTicks, 
                                                            sizeof(TimeTicks), false);

    UtAssert_INT32_EQ((int32) BPA_TIMEP_GetMonotonicTime(), (int32) TimeMsec);
}

/* Test BPA_TIMEP_GetHostEpoch */
void Test_BPA_TIMEP_GetHostEpoch(void)
{
    BPA_TIMEP_Epoch_t Epoch;

    BPA_TIMEP_GetHostEpoch(&Epoch);

    UtAssert_UINT32_EQ(Epoch.Year, CFE_MISSION_TIME_EPOCH_YEAR);
    UtAssert_UINT32_EQ(Epoch.Day, CFE_MISSION_TIME_EPOCH_DAY);
    UtAssert_UINT32_EQ(Epoch.Hour, CFE_MISSION_TIME_EPOCH_HOUR);
    UtAssert_UINT32_EQ(Epoch.Minute, CFE_MISSION_TIME_EPOCH_MINUTE);
    UtAssert_UINT32_EQ(Epoch.Second, CFE_MISSION_TIME_EPOCH_SECOND);
    UtAssert_UINT32_EQ(Epoch.Msec, CFE_MISSION_TIME_EPOCH_MICROS / 1000);
}

/* Test BPA_TIMEP_GetHostClockState */
void Test_BPA_TIMEP_GetHostClockState(void)
{
    /* Valid clock state */
    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_GetClockState), 1, CFE_TIME_ClockState_VALID);

    UtAssert_UINT32_EQ(BPA_TIMEP_GetHostClockState(), BPA_TIMEP_CLOCK_VALID);

    /* Invalid clock state (ex: flywheeling) */
    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_GetClockState), 1, CFE_TIME_ClockState_FLYWHEEL);

    UtAssert_UINT32_EQ(BPA_TIMEP_GetHostClockState(), BPA_TIMEP_CLOCK_INVALID);
}

/* Test BPA_TIMEP_GetHostTime */
void Test_BPA_TIMEP_GetHostTime(void)
{
    uint32 Secs = 1234;
    uint32 MicroSecs = 567000;
    int64_t ExpectedTime;

    ExpectedTime = (Secs * 1000) + (MicroSecs / 1000);

    UT_SetHandlerFunction(UT_KEY(CFE_TIME_GetTAI), &Test_CFE_TIME_GetTAI_Handler, &Secs);
    UT_SetDeferredRetcode(UT_KEY(CFE_TIME_Sub2MicroSecs), 1, MicroSecs);

    UtAssert_INT32_EQ(BPA_TIMEP_GetHostTime(), ExpectedTime);
    UtAssert_STUB_COUNT(CFE_TIME_GetTAI, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_TIMEP_GetMonotonicTime);
    ADD_TEST(Test_BPA_TIMEP_GetHostEpoch);
    ADD_TEST(Test_BPA_TIMEP_GetHostClockState);
    ADD_TEST(Test_BPA_TIMEP_GetHostTime);
}
