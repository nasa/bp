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
 *   This file contains the header definitions for the FWP Time Proxy.
 */


#ifndef FWP_TIMEP_H
#define FWP_TIMEP_H

/*
** Include Files
*/

#include <stdint.h>
#include "bplib_time.h"


/*
** Exported Functions
*/

/**
 * \brief FWP Time Proxy Get Monotonic Time
 *
 *  \par Description
 *       Returns the current monotonic time in milliseconds
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \return Monotonic time in milliseconds
 */
int64_t BPA_TIMEP_GetMonotonicTime(void);


/**
 * \brief FWP Time Proxy Get Host Epoch
 *
 *  \par Description
 *       Sets the provided pointer to the host epoch
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] Epoch Pointer to host epoch
 */
void BPA_TIMEP_GetHostEpoch(BPA_TIMEP_Epoch_t *Epoch);

/**
 * \brief FWP Time Proxy Get Host Clock State
 *
 *  \par Description
 *       Returns the current host clock state
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \return Host clock state
 *  \retval BPA_TIMEP_CLOCK_INVALID
 *  \retval BPA_TIMEP_CLOCK_VALID
 */
BPA_TIMEP_ClockState_t BPA_TIMEP_GetHostClockState(void);

/**
 * \brief FWP Time Proxy Get Host Time
 *
 *  \par Description
 *       Returns the current host time in milliseconds
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \return Host time in milliseconds
 */
int64_t BPA_TIMEP_GetHostTime(void);

#endif /* FWP_TIMEP_H */
