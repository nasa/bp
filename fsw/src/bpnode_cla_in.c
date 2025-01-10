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
 *   This file contains the source code for the BPNode CLA In Child Task(s)
 */

/*
** Include Files
*/
#include "bpnode_app.h"
#include "bpnode_cla_in.h"

/*
** Function Definitions
*/

/* Create all CLA In child task(s) */
int32 BPNode_ClaInCreateTasks(void)
{
    int32  Status;
    uint8  i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;
    
    /* Create all of the CLA In task(s) */
    for (i = 0; i < BPLIB_MAX_NUM_CONTACTS; i++)
    { 
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaInData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_INIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Failed to create the CLA In #%d task init semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create wakeup semaphore so main task can control workflow */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaInData[i].WakeupSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_ADU_OUT_WAKEUP_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                                "Failed to create the CLA In #%d task wakeup semaphore, %s. Error = %d.", 
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create exit semaphore so main task knows when child finished shutdown */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_IN_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaInData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_EXIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Failed to create the CLA In #%d task exit semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }
        
        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_CLA_IN_BASE_NAME, i);
        TaskPriority = BPNODE_CLA_IN_PRIORITY_BASE + i;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.ClaInData[i].TaskId,
                                NameBuff, BPNode_ClaIn_AppMain,
                                0, BPNODE_CLA_IN_STACK_SIZE, TaskPriority, 0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Failed to create the CLA In #%d child task. Error = %d.", 
                            i, Status);
            return Status;
        }

        /* Enable ingress */
        BPNode_AppData.ClaInData[i].IngressServiceEnabled = true;

        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.ClaInData[i].InitSemId, 
                                                                BPNODE_SEM_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "CLA In #%d task not running. Init Sem Error = %d.", 
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for CLA In task(s) */
int32 BPNode_ClaIn_TaskInit(uint8 *ContId)
{
    CFE_ES_TaskId_t TaskId;
    int32           Status;
    int32           PspStatus;
    uint8           i;
#ifdef BPNODE_CLA_UDP_DRIVER    
    int32           PortNum;
    char            Str[100];
#endif

    /* Get the task ID of currently running child task */
    Status = CFE_ES_GetTaskID(&TaskId);

    if (Status != CFE_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_NO_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA In #?]: Failed to get task ID. Error = %d", Status);
        return Status;        
    }

    /* Map this task's ID to a contact ID */
    for (i = 0; i < BPLIB_MAX_NUM_CONTACTS; i++)
    { 
        if (TaskId == BPNode_AppData.ClaInData[i].TaskId)
        {
            *ContId = i;
        }
    }

    if (*ContId == BPLIB_MAX_NUM_CONTACTS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA In #?]: Task ID does not match any known task IDs. ID = %d", 
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.ClaInData[*ContId].PerfId = BPNODE_CLA_IN_PERF_ID_BASE + *ContId;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME, 
                                &BPNode_AppData.ClaInData[*ContId].PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA In #%d]: Couldn't find I/O driver. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    BPNode_AppData.ClaInData[*ContId].PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
    BPNode_AppData.ClaInData[*ContId].PspLocation.SubchannelId = BPNODE_CLA_PSP_INPUT_SUBCHANNEL;

#ifdef BPNODE_CLA_UDP_DRIVER
    /* Configure Port Number */
    PortNum = BPNODE_CLA_IN_PORT + CFE_PSP_GetProcessorId() - 1;
    snprintf(Str, sizeof(Str), "port=%d", PortNum);

    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_CONFIGURATION,
                                            CFE_PSP_IODriver_CONST_STR(Str));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA In #%d]: Couldn't set port number configuration. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }        

    /* Configure IP Address */
    snprintf(Str, sizeof(Str), "IpAddr=%s", BPNODE_CLA_IN_IP);
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_CONFIGURATION,
                                            CFE_PSP_IODriver_CONST_STR(Str));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_IP_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA In #%d]: Couldn't set IP address configuration. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    OS_printf("[CLA In #%d]: Receiving on %s:%d\n", *ContId, BPNODE_CLA_IN_IP, PortNum);
#endif

    /* Set direction to input only */
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_DIRECTION,
                                            CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Couldn't set I/O direction to input. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Set I/O to running */
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[*ContId].PspLocation, 
                            CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_CFG_SET_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA In #%d]: Couldn't set I/O state to running. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Start performance log */
    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[*ContId].PerfId);

    /* Notify main task that child task is running */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[*ContId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.ClaInData[*ContId].InitSemId);
    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[*ContId].PerfId);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_IN_INIT_SEM_TK_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA In #%d]: Failed to give init semaphore. Error = %d", 
                          *ContId, Status);
        return Status;
    }

    BPNode_AppData.ClaInData[*ContId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    BPLib_EM_SendEvent(BPNODE_CLA_IN_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                      "[CLA In #%d]: Child Task Initialized.", *ContId);

    return CFE_SUCCESS;
}

/* Receive bundles from network CL and forward ingress bundles to CLA  */
int32 BPNode_ClaIn_ProcessBundleInput(uint8 ContId)
{
    CFE_PSP_IODriver_ReadPacketBuffer_t RdBuf;
    int32                               Status = CFE_PSP_SUCCESS;
    BPLib_Status_t                      BpStatus;

    /* Read next bundle from CL */
    if (BPNode_AppData.ClaInData[ContId].CurrentBufferSize == 0)
    {
        RdBuf.BufferSize = sizeof(BPNode_AppData.ClaInData[ContId].BundleBuffer);
        RdBuf.BufferMem  = BPNode_AppData.ClaInData[ContId].BundleBuffer;

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);

        Status = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContId].PspLocation, 
                                          CFE_PSP_IODriver_PACKET_IO_READ,
                                          CFE_PSP_IODriver_VPARG(&RdBuf));

        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);
        
        if (Status == CFE_PSP_SUCCESS)
        {
            BPNode_AppData.ClaInData[ContId].CurrentBufferSize = RdBuf.BufferSize;
            
            Status = CFE_SUCCESS;
        }

    }

    /* Ingress received bundle to bplib CLA */
    if (Status == CFE_SUCCESS && BPNode_AppData.ClaInData[ContId].CurrentBufferSize != 0)
    {
        BPLib_AS_Increment(0, BUNDLE_COUNT_RECEIVED, 1);

        /* Temporarily pass ingress bundle to egress thread for proof-of-concept */
        if (BPNode_AppData.ClaOutData[ContId].CurrentBufferSize == 0)
        {
            memcpy(BPNode_AppData.ClaOutData[ContId].BundleBuffer, 
                   BPNode_AppData.ClaInData[ContId].BundleBuffer,
                   BPNode_AppData.ClaInData[ContId].CurrentBufferSize);
            BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = BPNode_AppData.ClaInData[ContId].CurrentBufferSize;
        }

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);
        
        BpStatus = BPLib_CLA_Ingress(ContId, BPNode_AppData.ClaInData[ContId].BundleBuffer,
                                   BPNode_AppData.ClaInData[ContId].CurrentBufferSize, 0);
        
        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);

        /* If CLA did not timeout during ingress, zero out current buffer size */
        if (BpStatus != BPLIB_CLA_TIMEOUT)
        {
            BPNode_AppData.ClaInData[ContId].CurrentBufferSize = 0;
            if (BpStatus != BPLIB_SUCCESS)
            {
                BPLib_EM_SendEvent(BPNODE_CLA_IN_LIB_PROC_ERR_EID, BPLib_EM_EventType_ERROR,
                                  "[CLA In #%d]: Failed to ingress bundle. Error = %d", 
                                  ContId, Status);
                Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
            }
        }
    }

    return Status;
}

/* Main loop for CLA In task(s) */
void BPNode_ClaIn_AppMain(void)
{
    int32 Status;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS; /* Set to garbage value */

    /* Perform task-specific initialization */
    Status = BPNode_ClaIn_TaskInit(&ContId);

    if (Status != CFE_SUCCESS)
    {
        /* Contact ID can't be determined, shut down immediately */
        if (ContId == BPLIB_MAX_NUM_CONTACTS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "Terminating Unknown CLA In Task.");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("Terminating Unknown CLA In Task.\n");

            CFE_ES_ExitChildTask();

            return;
        }
        /* If contact ID can be determined, ready normal shutdown */
        else
        {
            BPNode_AppData.ClaInData[ContId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* CLA In task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.ClaInData[ContId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        /* Attempt to take the wakeup semaphore */
        Status = OS_BinSemTimedWait(BPNode_AppData.ClaInData[ContId].WakeupSemId, BPNODE_SEM_WAIT_MSEC);
        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_IN_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Failed to take wakeup semaphore for CLA In Task #%d, RC = %d",
                                ContId,
                                Status);
        }

        if (BPNode_AppData.ClaInData[ContId].IngressServiceEnabled)
        {
            Status = BPNode_ClaIn_ProcessBundleInput(ContId);
            if (Status != CFE_SUCCESS)
            {
                BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);
                BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);
            }
        }
        else 
        {
            BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);
            BPLib_PL_PerfLogEntry(BPNode_AppData.ClaInData[ContId].PerfId);
        }
    }

    /* Exit gracefully */
    BPNode_ClaIn_TaskExit(ContId);

    return;
}

/* Exit child task */
void BPNode_ClaIn_TaskExit(uint8 ContId)
{
    /* Set I/O to stop running */
    (void) CFE_PSP_IODriver_Command(&BPNode_AppData.ClaInData[ContId].PspLocation, 
                            CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(false));

    BPLib_EM_SendEvent(BPNODE_CLA_IN_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "[CLA In #%d]: Terminating Task. RunStatus = %d.",
                      ContId, BPNode_AppData.ClaInData[ContId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[CLA In #%d]: Terminating Task. RunStatus = %d.\n",
                         ContId, BPNode_AppData.ClaInData[ContId].RunStatus);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaInData[ContId].PerfId);

    /* Signal to the main task that the child task has exited */
    (void) OS_BinSemGive(BPNode_AppData.ClaInData[ContId].ExitSemId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}

