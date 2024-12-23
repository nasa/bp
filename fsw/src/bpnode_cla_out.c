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
 *   This file contains the source code for the BPNode CLA Out Child Task(s)
 */

/*
** Include Files
*/

#include "bpnode_app.h"
#include "bpnode_cla_out.h"


/*
** Function Definitions
*/

/* Create all CLA Out child task(s) */
int32 BPNode_ClaOutCreateTasks(void)
{
    int32  Status;
    uint8  i;
    char   NameBuff[OS_MAX_API_NAME];
    uint16 TaskPriority;
    
    /* Create all of the CLA Out task(s) */
    for (i = 0; i < BPLIB_MAX_NUM_CONTACTS; i++)
    {
        /* Create init semaphore so main task knows when child initialized */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_INIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[i].InitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Failed to create the CLA Out #%d task init semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create wakeup semaphore so main task can control CLA Out tasks */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_WAKE_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[i].WakeupSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Failed to create the CLA Out #%d task wakeup semaphore, %s. Error = %d.",
                                i,
                                NameBuff,
                                Status);

            return Status;
        }

        /* Create exit semaphore so main task knows when child finished shutdown */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_EXIT_%d", BPNODE_CLA_OUT_SEM_BASE_NAME, i);
        Status = OS_BinSemCreate(&BPNode_AppData.ClaOutData[i].ExitSemId, NameBuff, 0, 0);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_SEM_ERR_EID, BPLib_EM_EventType_ERROR,
                        "Failed to create the CLA Out #%d task exit semaphore. Error = %d.", 
                        i, Status);
            return Status;
        }

        /* Create child task */
        snprintf(NameBuff, OS_MAX_API_NAME, "%s_%d", BPNODE_CLA_OUT_BASE_NAME, i);
        TaskPriority = BPNODE_CLA_IN_PRIORITY_BASE + i;

        Status = CFE_ES_CreateChildTask(&BPNode_AppData.ClaOutData[i].TaskId,
                                NameBuff, BPNode_ClaOut_AppMain,
                                0, BPNODE_CLA_OUT_STACK_SIZE, TaskPriority, 0);

        if (Status != CFE_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_CREATE_ERR_EID, BPLib_EM_EventType_ERROR,
                            "Failed to create the CLA Out #%d child task. Error = %d.", 
                            i, Status);
            return Status;
        }

        /* Enable egress */
        BPNode_AppData.ClaOutData[i].EgressServiceEnabled = true;

        /* Verify initialization by trying to take the init semaphore */
        BPLib_PL_PerfLogExit(BPNODE_PERF_ID);
        Status = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[i].InitSemId, 
                                                                BPNODE_SEM_WAIT_MSEC);
        BPLib_PL_PerfLogEntry(BPNODE_PERF_ID);

        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "CLA Out #%d task not running. Init Sem Error = %d.", 
                            i, Status);
            return Status;
        }
    }

    return CFE_SUCCESS;
}

/* Task initialization for CLA Out task(s) */
int32 BPNode_ClaOut_TaskInit(uint8 *ContId)
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
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_NO_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA Out #?]: Failed to get task ID. Error = %d", Status);
        return Status;        
    }

    /* Map this task's ID to a contact ID */
    for (i = 0; i < BPLIB_MAX_NUM_CONTACTS; i++)
    {
        if (TaskId == BPNode_AppData.ClaOutData[i].TaskId)
        {
            *ContId = i;
        }
    }

    if (*ContId == BPLIB_MAX_NUM_CONTACTS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_INV_ID_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA Out #?]: Task ID does not match any known task IDs. ID = %d", 
                          TaskId);
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    BPNode_AppData.ClaOutData[*ContId].PerfId = BPNODE_CLA_OUT_PERF_ID_BASE + *ContId;

    /* Get PSP module ID for either the Unix or UDP socket driver */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME, 
                                &BPNode_AppData.ClaOutData[*ContId].PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_FIND_NAME_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA Out #%d]: Couldn't find I/O driver. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    BPNode_AppData.ClaOutData[*ContId].PspLocation.SubsystemId  = 2 - (CFE_PSP_GetProcessorId() & 1);
    BPNode_AppData.ClaOutData[*ContId].PspLocation.SubchannelId = BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL;

#ifdef BPNODE_CLA_UDP_DRIVER
    /* Configure Port Number */
    PortNum = BPNODE_CLA_OUT_PORT + CFE_PSP_GetProcessorId() - 1;
    snprintf(Str, sizeof(Str), "port=%d", PortNum);

    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_CONFIGURATION,
                                            CFE_PSP_IODriver_CONST_STR(Str));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_PORT_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA Out #%d]: Couldn't set port number configuration. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }  

    /* Configure IP Address */
    snprintf(Str, sizeof(Str), "IpAddr=%s", BPNODE_CLA_OUT_IP);
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_CONFIGURATION,
                                            CFE_PSP_IODriver_CONST_STR(Str));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_IP_ERR_EID, BPLib_EM_EventType_ERROR, 
                            "[CLA Out #%d]: Couldn't set IP address configuration. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    OS_printf("[CLA Out #%d]: Receiving on %s:%d\n", *ContId, BPNODE_CLA_OUT_IP, PortNum);
#endif

    /* Set direction to output only */
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[*ContId].PspLocation, 
                                            CFE_PSP_IODriver_SET_DIRECTION,
                                            CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_OUTPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_DIR_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA Out #%d]: Couldn't set I/O direction to output. Error = %d",
                            (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Set I/O to running */
    PspStatus = CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[*ContId].PspLocation, 
                            CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_CFG_SET_RUN_ERR_EID, BPLib_EM_EventType_ERROR,
                            "[CLA Out #%d]: Couldn't set I/O state to running. Error = %d",
                            *ContId, PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Start performance log */
    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[*ContId].PerfId);

    /* Notify main task that child task is running */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[*ContId].PerfId);
    Status = OS_BinSemGive(BPNode_AppData.ClaOutData[*ContId].InitSemId);
    BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[*ContId].PerfId);

    if (Status != OS_SUCCESS)
    {
        BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_SEM_TK_ERR_EID, BPLib_EM_EventType_ERROR,
                          "[CLA Out #%d]: Failed to give init semaphore. Error = %d", 
                          *ContId, Status);
        return Status;
    }

    BPNode_AppData.ClaOutData[*ContId].RunStatus = CFE_ES_RunStatus_APP_RUN;

    BPLib_EM_SendEvent(BPNODE_CLA_OUT_INIT_INF_EID, BPLib_EM_EventType_INFORMATION,
                      "[CLA Out #%d]: Child Task Initialized.", *ContId);

    return CFE_SUCCESS;
}

/* Receive bundles from CLA and send egress bundles to network CL */
int32 BPNode_ClaOut_ProcessBundleOutput(uint8 ContId)
{
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;
    int32                                Status = CFE_PSP_SUCCESS;
    BPLib_Status_t                       BpStatus;

    /* Get next bundle from CLA */
    if (BPNode_AppData.ClaOutData[ContId].CurrentBufferSize == 0)
    {
        BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

        BpStatus = BPLib_CLA_Egress(ContId, BPNode_AppData.ClaOutData[ContId].BundleBuffer, 
                                    &BPNode_AppData.ClaOutData[ContId].CurrentBufferSize, 
                                    BPNODE_CLA_OUT_QUEUE_PEND_TIME);
        
        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);

        if (BpStatus != BPLIB_SUCCESS && BpStatus != BPLIB_CLA_TIMEOUT)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_LIB_LOAD_ERR_EID, BPLib_EM_EventType_ERROR, 
                               "[CLA Out #%d]: Failed to get bundle for egress. Error = %d",
                               ContId, Status);
            return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
        }
    }

    /* Send egress bundle onto CL */
    if (BPNode_AppData.ClaOutData[ContId].CurrentBufferSize != 0)
    {
        WrBuf.OutputSize = BPNode_AppData.ClaOutData[ContId].CurrentBufferSize;
        WrBuf.BufferMem  = BPNode_AppData.ClaOutData[ContId].BundleBuffer;

        BPLib_AS_Increment(0, BUNDLE_COUNT_DELIVERED, 1);

        BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

        /* This does not check return code here, it is "best effort" at this stage.
         * bplib should retry based on custody signals if this does not work. */
        CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContId].PspLocation, 
                                    CFE_PSP_IODriver_PACKET_IO_WRITE, 
                                    CFE_PSP_IODriver_VPARG(&WrBuf));
        
        BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);

        BPNode_AppData.ClaOutData[ContId].CurrentBufferSize = 0;
    }

    return CFE_SUCCESS;
}

/* Main loop for CLA Out task(s) */
void BPNode_ClaOut_AppMain(void)
{
    int32 Status;
    uint8 ContId = BPLIB_MAX_NUM_CONTACTS; /* Set to garbage value */

    /* Perform task-specific initialization */
    Status = BPNode_ClaOut_TaskInit(&ContId);

    if (Status != CFE_SUCCESS)
    {
        /* Contact ID can't be determined, shut down immediately */
        if (ContId == BPLIB_MAX_NUM_CONTACTS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_UNK_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "Terminating Unknown CLA Out Task.");

            /* In case event services is not working, add a message to the system log */
            CFE_ES_WriteToSysLog("Terminating Unknown CLA Out Task.\n");

            CFE_ES_ExitChildTask();

            return;
        }
        /* If contact ID can be determined, ready normal shutdown */
        else
        {
            BPNode_AppData.ClaOutData[ContId].RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /* CLA Out task loop */
    while (CFE_ES_RunLoop(&BPNode_AppData.ClaOutData[ContId].RunStatus) == CFE_ES_RunStatus_APP_RUN)
    {
        Status = OS_BinSemTimedWait(BPNode_AppData.ClaOutData[ContId].WakeupSemId, BPNODE_SEM_WAIT_MSEC);
        if (Status != OS_SUCCESS)
        {
            BPLib_EM_SendEvent(BPNODE_CLA_OUT_WAKEUP_SEM_ERR_EID,
                                BPLib_EM_EventType_ERROR,
                                "Failed to take wakeup semaphore for CLA Out Task #%d, RC = %d",
                                ContId,
                                Status);
        }

        if (BPNode_AppData.ClaOutData[ContId].EgressServiceEnabled)
        {
            Status = BPNode_ClaOut_ProcessBundleOutput(ContId);
            if (Status != CFE_SUCCESS)
            {
                BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);
                (void) OS_TaskDelay(BPNODE_CLA_OUT_PROC_BUNDLE_SLEEP_MSEC);
                BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);
            }
        }
        else 
        {
            BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);
            (void) OS_TaskDelay(BPNODE_CLA_OUT_SLEEP_MSEC);
            BPLib_PL_PerfLogEntry(BPNode_AppData.ClaOutData[ContId].PerfId);
        }
    }

    /* Exit gracefully */
    BPNode_ClaOut_TaskExit(ContId);

    return;
}


/* Exit child task */
void BPNode_ClaOut_TaskExit(uint8 ContId)
{
    /* Set I/O to stop running */
    (void) CFE_PSP_IODriver_Command(&BPNode_AppData.ClaOutData[ContId].PspLocation, 
                            CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(false));

    BPLib_EM_SendEvent(BPNODE_CLA_OUT_EXIT_CRIT_EID, BPLib_EM_EventType_CRITICAL,
                      "[CLA Out #%d]: Terminating Task. RunStatus = %d.",
                      ContId, BPNode_AppData.ClaOutData[ContId].RunStatus);

    /* In case event services is not working, add a message to the system log */
    CFE_ES_WriteToSysLog("[CLA Out #%d]: Terminating Task. RunStatus = %d.\n",
                         ContId, BPNode_AppData.ClaOutData[ContId].RunStatus);

    /* Exit the perf log */
    BPLib_PL_PerfLogExit(BPNode_AppData.ClaOutData[ContId].PerfId);

    /* Signal to the main task that the child task has exited */
    (void) OS_BinSemGive(BPNode_AppData.ClaOutData[ContId].ExitSemId);

    /* Stop execution */
    CFE_ES_ExitChildTask();

    return;
}

