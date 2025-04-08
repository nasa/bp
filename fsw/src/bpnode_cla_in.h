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
 *   Header file for CLA Input.
 */

#ifndef BPNODE_CLA_IN_H
#define BPNODE_CLA_IN_H

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

#define BPNODE_CLA_IN_SEM_BASE_NAME          "BPN_CLA_IN"    /** \brief Initialization semaphore base name */
#define BPNODE_CLA_IN_BASE_NAME              "BPNODE.CLA_IN" /** \brief Task base name */
#define BPNODE_CLA_IN_SLEEP_MSEC             (1000u)         /** \brief Sleep time */
#define BPNODE_CLA_IN_BUNDLE_PROC_SLEEP_MSEC (250u)          /** \brief Bundle processing Sleep time */
#define BPNODE_CLA_PSP_INPUT_BUFFER_SIZE     (4096)          /** \brief IODriver buffer size*/
#define BPNODE_CLA_IN_SEM_INIT_WAIT_MSEC     (2000u)         /** \brief Wait time for init semaphore take, in milliseconds */
#define BPNODE_CLA_IN_SEM_WAKEUP_WAIT_MSEC   (1100u)         /** \brief Wait time for wakeup semaphore take, in milliseconds */


/*
** Type Definitions
*/

/**
** \brief CLA In Task Data
*/
typedef struct
{
    CFE_ES_TaskId_t              TaskId;
    osal_id_t                    InitSemId;
    osal_id_t                    WakeupSemId;
    osal_id_t                    ExitSemId;
    uint32                       PerfId;

    /* IODriver usock_intf related*/
    CFE_PSP_IODriver_Direction_t Dir;
    CFE_PSP_IODriver_Location_t  PspLocation;
    size_t                       CurrentBufferSize;
    uint8                        BundleBuffer[BPNODE_CLA_PSP_INPUT_BUFFER_SIZE];

} BPNode_ClaInData_t;


/*
** Exported Functions
*/

/**
 * \brief Process Bundle Input from CLA
 *
 *  \par Description
 *       Receive and process candidate bundle (bundle or control message) from CLA
 *       and pass the bundle to Bundle Interface.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] ContId Contact ID
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 BPNode_ClaIn_ProcessBundleInput(uint32 ContId);

/**
  * \brief     Create all CLA In tasks
  * \return    Execution status
  * \retval    CFE_SUCCESS: Successful execution
  * \retval    OS errors from OS_BinSemCreate
  * \retval    OS errors from OS_BinSemTimedWait
  * \retval    CFE errors from CFE_ES_CreateChildTask
  */
CFE_Status_t BPNode_ClaInCreateTasks(void);

/**
  * \brief     Initialize a CLA In task
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    Execution status
  * \retval    CFE_SUCCESS: Successful execution
  * \retval    PSP errors from CFE_PSP_IODriver_FindByName
  * \retval    PSP errors from CFE_PSP_IODriver_Command
  * \retval    OS errors from OS_BinSemGive
  */
CFE_Status_t BPNode_ClaIn_TaskInit(uint32 ContactId);

/**
  * \brief     Set up a CLA In task
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \param[in] PortNum (int32) If the task is using UDP, this is the port number
  *                            gathered from the Contacts Configuration
  * \param[in] IpAddr (char*) If the task is using UDP, this is the IP address
  *                           gathered from the Contacts Configuration
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: A I/O driver API call failed operation
  */
BPLib_Status_t BPNode_ClaIn_Setup(uint32 ContactId, int32 PortNum, char* IpAddr);

/**
  * \brief     Start up a CLA In task
  * \note      Create a CLA In child tasks and signal to the main task that
  *            the task is running
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                               arrays that corresponds to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: UDP connection couldn't be set to running
  */
BPLib_Status_t BPNode_ClaIn_Start(uint32 ContactId);

/**
  * \brief     Stop a CLA In task
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                               arrays that corresponds to that contact's info
  * \return    Execution status
  * \retval    BPLIB_SUCCESS: Successful execution
  * \retval    BPLIB_CLA_IO_ERROR: Something went wrong while running CFE_PSP_IODriver_Command
  */
BPLib_Status_t BPNode_ClaIn_Stop(uint32 ContactId);

/**
  * \brief     Teardown a CLA In task
  * \note      Nothing is implemented as of right now
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                               arrays that corresponds to that contact's info
  * \return    void
  */
void BPNode_ClaIn_Teardown(uint32 ContactId);

/**
 * \brief CLA In Main Task
 *
 *  \par Description
 *       CLA In task main loop. Receive bundles from CLs and pass them to Bundle Interface.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 */
void BPNode_ClaIn_AppMain(void);

/** \brief Exit provided CLA In task
 *
 *  \par Description
 *       Exit CLA In task gracefully
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 * 
 *  \param[in] ContactId Contacts ID for this task
 */
void BPNode_ClaIn_TaskExit(uint32 ContactId);

/**
  * \brief     Delete semaphores associated with the task with the given contact ID
  * \param[in] ContactId (uint32) Index into the various contact info tracking
  *                                 arrays that corresponds to that contact's info
  * \return    void
  */
 void BPNode_ClaIn_DeleteSems(uint32 ContactId);

#endif /* BPNODE_CLA_IN_H */