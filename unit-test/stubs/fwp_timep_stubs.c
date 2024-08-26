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

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in fwp_timep header
 */

#include "fwp_timep.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TIMEP_GetMonotonicTime()
 * ----------------------------------------------------
 */
int64_t BPA_TIMEP_GetMonotonicTime(void)
{
    UT_GenStub_SetupReturnBuffer(BPA_TIMEP_GetMonotonicTime, int64_t);

    UT_GenStub_Execute(BPA_TIMEP_GetMonotonicTime, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TIMEP_GetMonotonicTime, int64_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TIMEP_GetHostEpoch()
 * ----------------------------------------------------
 */
void BPA_TIMEP_GetHostEpoch(BPLib_TIME_Epoch_t *Epoch)
{
    UT_GenStub_AddParam(BPA_TIMEP_GetHostEpoch, BPLib_TIME_Epoch_t *, Epoch);

    UT_GenStub_Execute(BPA_TIMEP_GetHostEpoch, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TIMEP_GetHostClockState()
 * ----------------------------------------------------
 */
BPLib_TIME_ClockState_t BPA_TIMEP_GetHostClockState(void)
{
    UT_GenStub_SetupReturnBuffer(BPA_TIMEP_GetHostClockState, BPLib_TIME_ClockState_t);

    UT_GenStub_Execute(BPA_TIMEP_GetHostClockState, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TIMEP_GetHostClockState, BPLib_TIME_ClockState_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_TIMEP_GetHostTime()
 * ----------------------------------------------------
 */
int64_t BPA_TIMEP_GetHostTime(void)
{
    UT_GenStub_SetupReturnBuffer(BPA_TIMEP_GetHostTime, int64_t);

    UT_GenStub_Execute(BPA_TIMEP_GetHostTime, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_TIMEP_GetHostTime, int64_t);
}
