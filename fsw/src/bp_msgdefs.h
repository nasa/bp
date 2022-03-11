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

#ifndef BP_MSGDEFS_H
#define BP_MSGDEFS_H

/*************************************************************************
 * Includes
 **************************************************************************/

#include "cfe.h"

/*************************************************************************
 * Macros
 **************************************************************************/

/* BP_CMD_MID */
#define BP_NOOP_CC                      0
#define BP_RESET_CC                     1
#define BP_RELOAD_FLOW_TABLE_CC         2
#define BP_ENABLE_FLOW_CC               3
#define BP_DISABLE_FLOW_CC              4
#define BP_FLUSH_FLOW_CC                5
#define BP_PAUSE_FLOW_CC                6
#define BP_RESUME_FLOW_CC               7
#define BP_OVERRIDE_TIMEOUT_CC          8
#define BP_DISABLE_OVERRIDE_TIMEOUT_CC  9
#define BP_OVERRIDE_PRIORITY_CC         10
#define BP_DISABLE_OVERRIDE_PRIORITY_CC 11

/* BP_SEND_HK_MID */
#define BP_SEND_APP_CC      0
#define BP_SEND_ENABLED_CC  1
#define BP_SEND_DISABLED_CC 2

/* BP_WAKEUP_MID */
#define BP_WAKEUP_CC         0
#define BP_WAKEUP_PROCESS_CC 1

#endif /* BP_MSGDEFS_H */
