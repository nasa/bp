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
 * @file
 *
 * BPNode Application Mission Configuration Header File
 *
 * This is a compatibility header for the "mission_cfg.h" file that has
 * traditionally provided public config definitions for each cFS app.
 *
 * @note This file may be overridden/superceded by mission-provided definitions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */

#ifndef BPNODE_MISSION_CFG_H
#define BPNODE_MISSION_CFG_H

#include "bpnode_interface_cfg.h"

#define BPNODE_CLA_UDP_DRIVER

#ifdef BPNODE_CLA_UDP_DRIVER
    #define BPNODE_CLA_PSP_DRIVER_NAME          "udpsock_intf"       /*IODriver udpsock_intf driver name*/
#else
    #define BPNODE_CLA_PSP_DRIVER_NAME          "unsock_intf"       /*IODriver unsock_intf driver name*/
#endif

/**
 * \brief Default EID Service Number to egress bundles over channel 0
 */
#define BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_0 42

/**
 * \brief Default EID Service Number to egress bundles over channel 1
 */
#define BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_1 53

/**
 * \brief Default EID Node Number to egress bundles to contact 0
 */
#define BPNODE_EID_NODE_NUM_FOR_CONTACT_0 200

/**
 * \brief Default EID Service Number to egress bundles to contact 0
 */
#define BPNODE_EID_SERVICE_NUM_FOR_CONTACT_0 64

/**
 * \brief Default EID Node Number to egress bundles to contact 1
 */
#define BPNODE_EID_NODE_NUM_FOR_CONTACT_1 100

/**
 * \brief Default EID Service Number to egress bundles to contact 1
 */
#define BPNODE_EID_SERVICE_NUM_FOR_CONTACT_1 42

#endif /* BPNODE_MISSION_CFG_H */
