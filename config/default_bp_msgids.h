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

#ifndef BP_MSGIDS_H
#define BP_MSGIDS_H

/* should define CFE_PLATFORM_CMD_MID_BASE/CFE_PLATFORM_TLM_MID_BASE */
#include "cfe_msgids.h"
#include "bp_topicids.h"

/*
 * BP application command message id (ground)
 */
#define BP_CMD_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_BP_CMD_MSG

/*
 * BP send housekeeping message id (internal)
 */
#define BP_SEND_HK_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_BP_SEND_HK_MSG

/*
 * BP wakeup message id (internal)
 */
#define BP_WAKEUP_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_BP_WAKEUP_MSG

/*
 * BP application telemetry message id (ground)
 */
#define BP_HK_TLM_MID CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_BP_HK_TLM_MSG

/*
 * BP per flow application telemetry message id (ground)
 */
#define BP_FLOW_HK_TLM_MID CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_BP_FLOW_HK_TLM_MSG

#endif /* !BP_MSGIDS_H */
