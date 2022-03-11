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

#ifndef BP_PLATFORM_CFG_H
#define BP_PLATFORM_CFG_H

#include "bp_perfids.h"

/* By default override the semaphore array size to match the number of
 * CFDP channels. The out-of-the-box behavior of the BP app is to integrate
 * with the CF task and throttle CFDP. */
#define BP_NUM_SEM_THROTTLES 2

/* Depending on the epoch being used by time services, this value needs to
 * be changed to reflect the number of seconds from the epoch to J2000. */
#define BP_CFE_SECS_AT_2000 630720013

/* Name of the flash storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_FLASH_STORE "FLASH"

/* Name of the ram storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_RAM_STORE "RAM"

/* Name of the file storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_FILE_STORE "FILE"

#endif /* BP_PLATFORM_CFG_H */
