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

#ifndef BP_INTERFACE_CFG_H
#define BP_INTERFACE_CFG_H

/* Name of the flash storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_FLASH_STORE "FLASH"

/* Name of the ram storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_RAM_STORE "RAM"

/* Name of the file storage service used in the flow table and in the
 * custome storage configuration module */
#define BP_FILE_STORE "FILE"

/*
 * Maximum size of a flow name in table and message structs
 */

#define BP_FLOW_NAME_SIZE 8

/*
 * Maximum size of a storage service name in table and message structs
 */
#define BP_STORE_NAME_SIZE 8

/*
 * Maximum number of statically allocated flow control structures
 */
#define BP_MAX_FLOWS 8

/*
 * Maximum number of statically allocated I/O control structures
 */
#define BP_MAX_IO_DESC BP_MAX_FLOWS

/*
 * Maximum size of the bundle supported by the application
 */
#define BP_MAX_BUNDLE_SIZE 4096

/*
 * Maximum size of CCSDS packet supported by the application
 */
#define BP_MAX_PACKET_SIZE 4096

/*
 * Maximum string length of per flow software bus pipe name
 */
#define BP_PIPE_NAME_SIZE CFE_MISSION_MAX_API_LEN

/*
 * Maximum string length of throttle semaphore name
 */
#define BP_SEM_NAME_SIZE CFE_MISSION_MAX_API_LEN

/*
 * Maximum number of packets per flow that can be subscribed to
 */
#define BP_PKTTBL_MAX_ROWS 4

/*
 * CFDP class 1 entity id used for direct file transfer
 */
#define BP_FILE_CFDP_ENTITY_ID 0x0101

/*
 * CCSDS message id used to encapsulate CFDP class 1 PDUs
 */
#define BP_FILE_CFDP_TLM_MID 0x0B26

#endif /* BP_INTERFACE_CFG_H */
