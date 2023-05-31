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

#ifndef BP_INTERNAL_CFG_H
#define BP_INTERNAL_CFG_H

/* By default override the semaphore array size to match the number of
 * CFDP channels. The out-of-the-box behavior of the BP app is to integrate
 * with the CF task and throttle CFDP. */
#define BP_NUM_SEM_THROTTLES 2

/* Depending on the epoch being used by time services, this value needs to
 * be changed to reflect the number of seconds from the epoch to J2000. */
#define BP_CFE_SECS_AT_2000 630720013

/*
 * Mission specific application revision (displayed at startup and from noop)
 */
#define BP_MISSION_REV 0

/*
 * Maximum number of ground and wakeup commands that can be queued
 */
#define BP_APP_PIPE_DEPTH 32

/*
 * Name of the application pipe
 */
#define BP_APP_PIPE_NAME "BP_CMD_PIPE"

/*
 * Used to create the per flow data pipes
 */
#define BP_DATA_PIPE_PREFIX "BP_PIPE_"

/*
 * Main loop software bus timeout
 */
#define BP_SB_TIMEOUT_MS 1000

/*
 * Maximum number of incoming bundles to read per wakeup cycle
 */
#define BP_IO_READ_LIMIT 64

/*
 * Maximum number of outgoing bundles to write per wakeup cycle
 */
#define BP_IO_WRITE_LIMIT 64

/*
 * Maximum number of incoming payloads to read/bundle per wakeup
 *
 * Note: With default flow config, this value is also used to configure
 * the depth of the input pipe, and therefore must be less than OS_QUEUE_MAX_DEPTH.
 */
#define BP_APP_READ_LIMIT 10

/*
 * Maximum number of outgoing payloads to write/unbundle per wakeup
 */
#define BP_APP_WRITE_LIMIT 10

/*
 * Maximum string length of I/O parameter
 */
#define BP_IO_PARM_SIZE 128

/*
 * BP application performance ID
 */
#define BP_APPMAIN_PERF_ID 82

/*
 * Default flow table file name, loaded on startup
 */
#define BP_DEFAULT_FLOW_TBL "/cf/bp_flowtable.tbl"

/*
 * Whether to strip the CCSDS header bytes from payload prior to bundling
 */
#define BP_STRIP_HDR_BYTES true

/*
 * Special value that indicates an invalid message id
 */
#define BP_INVALID_MID CFE_SB_INVALID_MSG_ID

/*
 * Special value that indicates an invalid software bus pipe
 */
#define BP_INVALID_PIPE CFE_SB_INVALID_PIPE

/*
 * Special value that indicates an invalid cFE table
 */
#define BP_INVALID_TABLE (-1)

/*
 * Number of available storage services
 */
#define BP_NUM_STORAGE_SERVICES 3

/*
 * Local directory to use for bplib file storage service
 */
#define BP_FILE_STORE_ROOT "/cf/dtn"

/*
 * Number of bundles to cache in ram in bplib file storage service
 */
#define BP_FILE_CACHE_SIZE 16384

#endif /* BP_CFG_H */
