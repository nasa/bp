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
 *   This file contains the source code for the FWP Time Proxy.
 */

/*
** Include Files
*/

#include "cfe.h"
#include "fwp_timep.h"


/*
** Function Definitions
*/

/* Returns current monotonic time */
int64_t BPA_TIMEP_GetMonotonicTime(void)
{
    OS_time_t TimeMsec;

    CFE_PSP_GetTime(&TimeMsec);

    return (int64_t) OS_TimeGetTotalMilliseconds(TimeMsec);
}

/* Returns host time epoch */
void BPA_TIMEP_GetHostEpoch(BPA_TIMEP_Epoch_t *Epoch)
{
    if (Epoch != NULL)
    {
        Epoch->Year   = CFE_MISSION_TIME_EPOCH_YEAR;
        Epoch->Day    = CFE_MISSION_TIME_EPOCH_DAY;
        Epoch->Hour   = CFE_MISSION_TIME_EPOCH_HOUR;
        Epoch->Minute = CFE_MISSION_TIME_EPOCH_MINUTE;
        Epoch->Second = CFE_MISSION_TIME_EPOCH_SECOND;
        Epoch->Msec   = CFE_MISSION_TIME_EPOCH_MICROS / 1000;
    }

    return;
}

/* Returns current host clock state */
BPA_TIMEP_ClockState_t BPA_TIMEP_GetHostClockState(void)
{
    CFE_TIME_ClockState_Enum_t HostClockState = CFE_TIME_GetClockState();

    if (HostClockState == CFE_TIME_ClockState_VALID)
    {
        return BPA_TIMEP_CLOCK_VALID;
    }
    
    return BPA_TIMEP_CLOCK_INVALID;
}

/* Returns current host time */
int64_t BPA_TIMEP_GetHostTime(void)
{
    CFE_TIME_SysTime_t HostTime;
    int64_t HostTimeMsec = 0;

    HostTime = CFE_TIME_GetTAI();

    HostTimeMsec += (int64_t) HostTime.Seconds * 1000;
    HostTimeMsec += (int64_t) CFE_TIME_Sub2MicroSecs(HostTime.Subseconds) / 1000;

    return HostTimeMsec;
}
