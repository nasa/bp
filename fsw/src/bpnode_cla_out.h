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
 *   This file contains the function definitions for the CLA Out Task(s)
 */


#ifndef BPNODE_CLA_OUT_H
#define BPNODE_CLA_OUT_H

/*
** Include Files
*/

#include "cfe.h"
#include "iodriver_base.h"
#include "iodriver_packet_io.h"
#include "bplib.h"
#include "bpnode_platform_cfg.h"


/*
** Macro Definitions
*/

#define BPNODE_CLA_OUT_SEM_BASE_NAME          "BPN_CLA_OUT"    /** \brief Semaphore base name */
#define BPNODE_CLA_OUT_BASE_NAME              "BPNODE.CLA_OUT" /** \brief Task base name */
#define BPNODE_CLA_OUT_SLEEP_MSEC             (1000u)          /** \brief Sleep time */
#define BPNODE_CLA_OUT_PROC_BUNDLE_SLEEP_MSEC (250u)           /** \brief Bundle processing Sleep time */
#define BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL      (0u)             /** \brief IODriver Output subchannel*/
#define BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE     (4096)           /** \brief IODriver output buffer size*/
#define BPNODE_CLA_OUT_QUEUE_PEND_TIME        (100u)           /** \brief Time to pend on bundle queue */
#define BPNODE_CLA_OUT_SEM_INIT_WAIT_MSEC     (2000u)          /** \brief Wait time for init semaphore take, in milliseconds */
#define BPNODE_CLA_OUT_SEM_WAKEUP_WAIT_MSEC   (1100u)          /** \brief Wait time for wakeup semaphore take, in milliseconds */


/*
** Type Definitions
*/

/**
** \brief CLA Out Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t                 TaskId;
    osal_id_t                       InitSemId;
    osal_id_t                       WakeupSemId;
    osal_id_t                       ExitSemId;
    uint32                          PerfId;
    bool                            EgressServiceEnabled;

    /* IODriver usock_intf related*/
    CFE_PSP_IODriver_Direction_t    Dir;
    CFE_PSP_IODriver_Location_t     PspLocation;
    size_t                          CurrentBufferSize;
    uint8_t                         BundleBuffer[BPNODE_CLA_PSP_OUTPUT_BUFFER_SIZE];
} BPNode_ClaOutData_t;


/*
** Exported Functions
*/

/**
 * \brief Process Bundle Output to CLA
 *
 *  \par Description
 *       Receive the bundle from Bundle Interface and send the bundle to CL.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ContId Contact ID
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 BPNode_ClaOut_ProcessBundleOutput(uint8 ContId);

/**
  * \brief     Initialize a CLA Out task
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_INIT_SEM_ERROR: Initialization semaphore either wasn't
  *                                      created or the timed take timed out
  * \retval    BPLIB_CLA_WAKEUP_SEM_ERROR: Wake up semaphore wasn't created
  * \retval    BPLIB_CLA_EXIT_SEM_ERROR: Exit semaphore wasn't created
  * \retval    BPLIB_CLA_TASK_CREATE_ERROR: Task wasn't created
  */
BPLib_Status_t BPNode_ClaOutCreateTasks(void);

/**
  * \brief     Set up a CLA out task
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \param[in] PortNum (int32) If the task is using UDP, this is the port number
  *                            gathered from the Contacts Configuration
  * \param[in] IpAddr (char*) If the task is using UDP, this is the IP address
  *                           gathered from the Contacts Configuration
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: A I/O driver API call failed operation
  * \retval    BPLIB_CLA_INIT_SEM_ERROR: Unsuccessful call to OS_BinSemGive
  */
 BPLib_Status_t BPNode_ClaOut_Setup(uint32_t ContactId, int32 PortNum, char* IpAddr);

/**
  * \brief     Start up a CLA Out task
  * \note      Create a CLA Out child tasks and signal to the main task that
  *            the task is running
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: UDP conntection couldn't be set to running
  */
BPLib_Status_t BPNode_ClaOut_Start(uint32_t ContactId);

/**
  * \brief     Stop a CLA Out task
  * \note      Gracefully stops contact
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    void
  */
void BPNode_ClaOut_Stop(uint32_t ContactId);

/**
  * \brief     Teardown a CLA Out task
  * \param[in] ContactId (uint32_t) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    void
  */
void BPNode_ClaOut_Teardown(uint32_t ContactId);

/**
 * \brief CLA Out Main Task
 *
 *  \par Description
 *       CLA Out task main loop. Pull bundle from BI queue and send to CL through UNIX socket.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_ClaOut_AppMain(void);

#endif /* BPNODE_CLA_OUT_H */