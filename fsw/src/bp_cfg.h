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

#ifndef BP_CFG_H
#define BP_CFG_H

#include "cfe.h"
#include "bp_platform_cfg.h"

/*************************************************************************
 * Macros
 *************************************************************************/

/*
 * Mission specific application revision (displayed at startup and from noop)
 */
#ifndef BP_MISSION_REV
#define BP_MISSION_REV 0
#endif

/*
 * Maximum number of ground and wakeup commands that can be queued
 */
#ifndef BP_APP_PIPE_DEPTH
#define BP_APP_PIPE_DEPTH 32
#endif

/*
 * Name of the application pipe
 */
#ifndef BP_APP_PIPE_NAME
#define BP_APP_PIPE_NAME "BP_CMD_PIPE"
#endif

/*
 * Used to create the per flow data pipes
 */
#ifndef BP_DATA_PIPE_PREFIX
#define BP_DATA_PIPE_PREFIX "BP_PIPE_"
#endif

/*
 * Main loop software bus timeout
 */
#ifndef BP_SB_TIMEOUT_MS
#define BP_SB_TIMEOUT_MS 1000
#endif

/*
 * Maximum number of incoming bundles to read per wakeup cycle
 */
#ifndef BP_IO_READ_LIMIT
#define BP_IO_READ_LIMIT 64
#endif

/*
 * Maximum number of outgoing bundles to write per wakeup cycle
 */
#ifndef BP_IO_WRITE_LIMIT
#define BP_IO_WRITE_LIMIT 64
#endif

/*
 * Maximum number of incoming payloads to read/bundle per wakeup
 *
 * Note: With default flow config, this value is also used to configure
 * the depth of the input pipe, and therefore must be less than OS_QUEUE_MAX_DEPTH.
 */
#ifndef BP_APP_READ_LIMIT
#define BP_APP_READ_LIMIT 10
#endif

/*
 * Maximum number of outgoing payloads to write/unbundle per wakeup
 */
#ifndef BP_APP_WRITE_LIMIT
#define BP_APP_WRITE_LIMIT 10
#endif

/*
 * Maximum string length of I/O parameter
 */
#ifndef BP_IO_PARM_SIZE
#define BP_IO_PARM_SIZE 128
#endif

/*
 * BP application performance ID
 */
#ifndef BP_APPMAIN_PERF_ID
#define BP_APPMAIN_PERF_ID 82
#endif

/*
 * Maximum number of statically allocated flow control structures
 */
#ifndef BP_MAX_FLOWS
#define BP_MAX_FLOWS 8
#endif

/*
 * Maximum number of statically allocated I/O control structures
 */
#ifndef BP_MAX_IO_DESC
#define BP_MAX_IO_DESC BP_MAX_FLOWS
#endif

/*
 * Default flow table file name, loaded on startup
 */
#ifndef BP_DEFAULT_FLOW_TBL
#define BP_DEFAULT_FLOW_TBL "/cf/bp_flowtable.tbl"
#endif

/*
 * Whether to strip the CCSDS header bytes from payload prior to bundling
 */
#ifndef BP_STRIP_HDR_BYTES
#define BP_STRIP_HDR_BYTES true
#endif

/*
 * Maximum size of the bundle supported by the application
 */
#ifndef BP_MAX_BUNDLE_SIZE
#define BP_MAX_BUNDLE_SIZE 4096
#endif

/*
 * Maximum size of CCSDS packet supported by the application
 */
#ifndef BP_MAX_PACKET_SIZE
#define BP_MAX_PACKET_SIZE 4096
#endif

/*
 * Special value that indicates an invalid message id
 */
#ifndef BP_INVALID_MID
#define BP_INVALID_MID CFE_SB_INVALID_MSG_ID
#endif

/*
 * Special value that indicates an invalid software bus pipe
 */
#ifndef BP_INVALID_PIPE
#define BP_INVALID_PIPE CFE_SB_INVALID_PIPE
#endif

/*
 * Special value that indicates an invalid cFE table
 */
#ifndef BP_INVALID_TABLE
#define BP_INVALID_TABLE (-1)
#endif

/*
 * Maximum string length of per flow software bus pipe name
 */
#ifndef BP_PIPE_NAME_SIZE
#define BP_PIPE_NAME_SIZE OS_MAX_API_NAME
#endif

/*
 * Maximum number of packets per flow that can be subscribed to
 */
#ifndef BP_PKTTBL_MAX_ROWS
#define BP_PKTTBL_MAX_ROWS 4
#endif

/*
 * Number of throttling semaphores (used for cFE CF 2.0 backpressure)
 */
#ifndef BP_NUM_SEM_THROTTLES
#define BP_NUM_SEM_THROTTLES 0
#endif

/*
 * Number of available storage services
 */
#ifndef BP_NUM_STORAGE_SERVICES
#define BP_NUM_STORAGE_SERVICES 3
#endif

/*
 * Local directory to use for bplib file storage service
 */
#ifndef BP_FILE_STORE_ROOT
#define BP_FILE_STORE_ROOT "/cf/dtn"
#endif

/*
 * Number of bundles to cache in ram in bplib file storage service
 */
#ifndef BP_FILE_CACHE_SIZE
#define BP_FILE_CACHE_SIZE 16384
#endif

/*
 * CFDP class 1 entity id used for direct file transfer
 */
#ifndef BP_FILE_CFDP_ENTITY_ID
#define BP_FILE_CFDP_ENTITY_ID 0x0101
#endif

/*
 * CCSDS message id used to encapsulate CFDP class 1 PDUs
 */
#ifndef BP_FILE_CFDP_TLM_MID
#define BP_FILE_CFDP_TLM_MID 0x0B26
#endif

#endif /* BP_CFG_H */
