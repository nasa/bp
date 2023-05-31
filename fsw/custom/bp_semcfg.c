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

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "bp_internal_cfg.h"
#include "bp_flow.h"

/************************************************************************
** Defines
*************************************************************************/

/* The CF task uses semaphores to throttle how many CFDP PDUs it posts
 * to the software bus.  Nominally, the semaphore count below should
 * match the pipe depth and the buffer limit of the CFDP PDU CCSDS packet
 * software bus queue.  See the flow table's PipeDepth and PktTbl fields. */
#define BP_CF_SEM_MAX_COUNT BP_APP_READ_LIMIT

/*
 * Use default values (empty string) if no semaphores were specified.
 * This just simplifies when building out of the box from the source.
 */
#ifndef CF_CHANNEL_0_SEM_NAME
#define CF_CHANNEL_0_SEM_NAME "BP0_tsem"
#endif

#ifndef CF_CHANNEL_1_SEM_NAME
#define CF_CHANNEL_1_SEM_NAME "BP1_tsem"
#endif

#ifndef CF_SPACE_TO_GND_PDU_MID0
#define CF_SPACE_TO_GND_PDU_MID0 CFE_SB_MSGID_RESERVED
#endif

#ifndef CF_SPACE_TO_GND_PDU_MID1
#define CF_SPACE_TO_GND_PDU_MID1 CFE_SB_MSGID_RESERVED
#endif

/************************************************************************
** Data
*************************************************************************/

/*
 * NOTE: The actual semaphore ID is not included here, as it is a runtime value.
 * The compiler will zero-fill the structure, which is OK/safe.
 */

BP_Throttle_t BP_Throttles[BP_NUM_SEM_THROTTLES] = {
    {CF_CHANNEL_0_SEM_NAME, CF_SPACE_TO_GND_PDU_MID0, BP_CF_SEM_MAX_COUNT, 0},
    {CF_CHANNEL_1_SEM_NAME, CF_SPACE_TO_GND_PDU_MID1, BP_CF_SEM_MAX_COUNT, 0}};
