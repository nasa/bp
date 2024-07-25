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
** @file
**   FWP's Time Proxy (TIMEP) header definitions
*/
#ifndef FWP_TIMEP_H
#define FWP_TIMEP_H


/*
** Type Definitions
*/

/* Time epoch */
typedef struct
{
    uint32_t Year;
    uint32_t Day;
    uint32_t Hour;
    uint32_t Minute;
    uint32_t Second;
    uint32_t Msec;

} BPA_TIMEP_Epoch_t;

/* Host clock state */
typedef enum 
{
    BPA_TIMEP_CLOCK_INVALID = 0,
    BPA_TIMEP_CLOCK_VALID   = 1

} BPA_TIMEP_ClockState_t;


/*
** Exported Functions
*/

/*
** Name:  BPA_TIMEP_GetMonotonicTime
**
** Purpose:
**     This function returns the current monotonic time in milliseconds
**
** Returns:
**      int64_t MonotonicTime: Current monotonic time in milliseconds
*/
int64_t BPA_TIMEP_GetMonotonicTime(void);

/*
** Name:  BPA_TIMEP_GetHostEpoch
**
** Purpose:
**     This function populates the provided pointer with the host epoch
**
** Parameters:
**      BPA_TIMEP_Epoch_t *Epoch: Host epoch
*/
void BPA_TIMEP_GetHostEpoch(BPA_TIMEP_Epoch_t *Epoch);

/*
** Name:  BPA_TIMEP_GetHostClockState
**
** Purpose:
**     This function returns the current host clock state
**
** Returns:
**      BPA_TIMEP_ClockState_t State: Host clock state (valid or invalid)
*/
BPA_TIMEP_ClockState_t BPA_TIMEP_GetHostClockState(void);

/*
** Name:  BPA_TIMEP_GetHostTime
**
** Purpose:
**     This function returns the current host time in  milliseconds
**
** Returns:
**      int64_t Time: Current host time in milliseconds
*/
int64_t BPA_TIMEP_GetHostTime(void);

#endif /* FWP_TIMEP_H */
