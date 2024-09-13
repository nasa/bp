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
 *   This file contains the source code for the BPNode CLA IO functions
 */


/*
** Include Files
*/
#include <ctype.h>
#include "bpnode_cla_bundle_io.h"
#include "bplib_api_types.h"
#include "bplib.h"
#include "bpnode_eventids.h"

#define BPNODE_CLA_PSP_DRIVER_NAME          "unsock_intf"
#define BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL    0
#define BPNODE_CLA_PSP_INPUT_SUBCHANNEL     1
#define BPNODE_CLA_PSP_BUFFER_SIZE          3072



/*
** Global Data Section
*/
typedef struct
{
    bool                            EgressServiceEnabled;
    bool                            IngressServiceEnabled;
    BPLib_Handle_t                  ClaIntfId;
    uint32                          ContactsID;
    uint32                          RecvBundleCounts;
    uint32                          RecvMsgCounts;

    BPNODE_CLA_BundleFlowEntry_t    Egress;
    BPNODE_CLA_BundleFlowEntry_t    Ingress;
    
    BPLib_ContactsTable_t           ContactsTbl;

} BPNODE_CLA_GlobalData_t;

BPNODE_CLA_GlobalData_t BPNODE_CLA_Global;

/*
** Prototypes Section
*/
CFE_Status_t BPNODE_CLA_ProcessBundleOutput(BPNODE_CLA_BundleFlowEntry_t *Entry);
CFE_Status_t BPNODE_CLA_ProcessBundleInput(BPNODE_CLA_BundleFlowEntry_t *Entry);

/* BPNODE_CLA_OutputTask() -- Application entry point and main process loop */
void BPNODE_CLA_OutputTask(void)
{
    CFE_Status_t status;

    /*
    ** Egress RunLoop
    */
    while (BPNODE_CLA_Global.EgressServiceEnabled)
    {
        status = BPNODE_CLA_ProcessBundleOutput(&BPNODE_CLA_Global.Egress);
        if (status != CFE_SUCCESS)
        {
            OS_TaskDelay(250);
        }
    }

}


/* BPNODE_CLA_InputTask() -- entry point for input process (child task)  */
void BPNODE_CLA_InputTask(void)
{
    CFE_Status_t status;

    /*
    ** Ingress RunLoop
    */
    while (BPNODE_CLA_Global.IngressServiceEnabled)
    {
        status = BPNODE_CLA_ProcessBundleInput(&BPNODE_CLA_Global.Ingress);
        if (status != CFE_SUCCESS)
        {
            OS_TaskDelay(250);
        }
    }

} 

/* BPNODE_CLA_InitOutput -- Initialize output task  */
CFE_Status_t BPNODE_CLA_InitOutput(BPNODE_CLA_BundleFlowEntry_t *Entry)
{
    int32        PspStatus;
    CFE_Status_t CfeStatus;

    /* OUTPUT instance number should be 1 on processor 1, 2 on processor 2 */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME, &Entry->PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: CFE_PSP_IODriver_FindByName status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    Entry->PspLocation.SubsystemId  = 2 - (CFE_PSP_GetProcessorId() & 1);
    Entry->PspLocation.SubchannelId = BPNODE_CLA_PSP_OUTPUT_SUBCHANNEL;

    PspStatus = CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_DIRECTION,
                                         CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_OUTPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_DIRECTION (output) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    PspStatus =
        CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_RUNNING (output) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    CfeStatus = CFE_ES_CreateChildTask(&Entry->ServiceTask, "BPNODE_CLA_OUT", BPNODE_CLA_OutputTask, CFE_ES_TASK_STACK_ALLOCATE,
                                       32768, 100, 0);
    if (CfeStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: Start Child Task (output) status %x",
                          (unsigned int)CfeStatus);
        return CfeStatus;
    }

    CFE_EVS_SendEvent(BPNODE_CLA_INFO_EID, CFE_EVS_EventType_INFORMATION, "BPNODE_CLA: Started Output task ID %lx -> socket",
                      CFE_RESOURCEID_TO_ULONG(Entry->ServiceTask));

    return CFE_SUCCESS;
}

/* BPNODE_CLA_InitInput -- Initialize input task  */
CFE_Status_t BPNODE_CLA_InitInput(BPNODE_CLA_BundleFlowEntry_t *Entry)
{
    int32        PspStatus;
    CFE_Status_t CfeStatus;

    /* INPUT instance number should be 2 on processor 1, 1 on processor 2 */
    PspStatus = CFE_PSP_IODriver_FindByName(BPNODE_CLA_PSP_DRIVER_NAME, &Entry->PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: CFE_PSP_IODriver_FindByName status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    Entry->PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
    Entry->PspLocation.SubchannelId = BPNODE_CLA_PSP_INPUT_SUBCHANNEL;

    PspStatus = CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_DIRECTION,
                                         CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_DIRECTION (input) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    PspStatus =
        CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_RUNNING (input) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    CfeStatus = CFE_ES_CreateChildTask(&Entry->ServiceTask, "BPNODE_CLA_IN", BPNODE_CLA_InputTask, CFE_ES_TASK_STACK_ALLOCATE,
                                       32768, 100, 0);
    if (CfeStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BPNODE_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: Start Child Task (input) status %x\n",
                          (unsigned int)CfeStatus);
        return CfeStatus;
    }

    CFE_EVS_SendEvent(BPNODE_CLA_INFO_EID, CFE_EVS_EventType_INFORMATION, "BPNODE_CLA: Started Input task ID %lx <- socket",
                      CFE_RESOURCEID_TO_ULONG(Entry->ServiceTask));

    return CFE_SUCCESS;
}

int32 BPNODE_CLA_SetupLibIntf(void)
{
    /* Create bplib CLA and default route */

    return CFE_SUCCESS;
}

/* BPNODE_CLA_Init() -- SGW initialization   */
CFE_Status_t BPNODE_CLA_Init(void)
{
    CFE_Status_t status;

    memset(&BPNODE_CLA_Global, 0, sizeof(BPNODE_CLA_Global));

    status = BPNODE_CLA_SetupLibIntf();
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    /* Create Message Pipe if configured for output */
    BPNODE_CLA_Global.EgressServiceEnabled = true;
    status                             = BPNODE_CLA_InitOutput(&BPNODE_CLA_Global.Egress);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    BPNODE_CLA_Global.IngressServiceEnabled = true;
    status                              = BPNODE_CLA_InitInput(&BPNODE_CLA_Global.Ingress);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    CFE_EVS_SendEvent(BPNODE_CLA_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "BPNODE_CLA Initialized");

    return CFE_SUCCESS;
} /* End of BPNODE_CLA_Init() */


/* BPNODE_CLA_ProcessBundleInput() -- Receive bundles from network CL and Forward ingress bundles to CLA  */
CFE_Status_t BPNODE_CLA_ProcessBundleInput(BPNODE_CLA_BundleFlowEntry_t *Entry)
{
    CFE_PSP_IODriver_ReadPacketBuffer_t RdBuf;
    int32                               Status;

    if (Entry->CurrentBufferSize == 0)
    {
        /* Read next bundle */
        RdBuf.BufferSize = sizeof(Entry->BundleBuffer);
        RdBuf.BufferMem  = Entry->BundleBuffer;

        Status = CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_PACKET_IO_READ,
                                          CFE_PSP_IODriver_VPARG(&RdBuf));

        if (Status == CFE_PSP_SUCCESS)
        {
            Entry->CurrentBufferSize = RdBuf.BufferSize;
            Status                   = CFE_SUCCESS;
        }
        else
        {
            Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
        }
    }

    if (Entry->CurrentBufferSize != 0)
    {
        Status = BPLib_CLA_Ingress(BPNODE_CLA_Global.ContactsTbl, BPNODE_CLA_Global.ClaIntfId, Entry->BundleBuffer,
                                   Entry->CurrentBufferSize, 0);
        if (Status != BPLIB_TIMEOUT)
        {
            Entry->CurrentBufferSize = 0;
            if (Status == BPLIB_SUCCESS)
            {
                Status = CFE_SUCCESS;
            }
            else
            {
                CFE_EVS_SendEvent(BPNODE_LIB_PROC_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to process bundle", "cla_ingress", (int)Status);
                Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
            }
        }
    }

    return Status;
}

/* BPNODE_CLA_ProcessBundleOutput() -- Receive bundles from CLA and Send egress bundles to Network CL  */
CFE_Status_t BPNODE_CLA_ProcessBundleOutput(BPNODE_CLA_BundleFlowEntry_t *Entry)
{
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;
    int32                                Status;
    size_t                               TempSize;

    if (Entry->CurrentBufferSize == 0)
    {
        /* Load next bundle */
        TempSize = sizeof(Entry->BundleBuffer);
        Status = BPLib_CLA_Egress(BPNODE_CLA_Global.ContactsTbl, BPNODE_CLA_Global.ClaIntfId, Entry->BundleBuffer, &TempSize, 100);
        if (Status == BPLIB_SUCCESS)
        {
            Entry->CurrentBufferSize = TempSize;
        }
        else if (Status != BPLIB_TIMEOUT)
        {
            CFE_EVS_SendEvent(BPNODE_LIB_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Flow %s - Failed (%d) to load bundle",
                              "cla_egress", (int)Status);
            return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
        }
    }

    if (Entry->CurrentBufferSize != 0)
    {
        /* Read next bundle */
        WrBuf.OutputSize = Entry->CurrentBufferSize;
        WrBuf.BufferMem  = Entry->BundleBuffer;

        /* this does not check return code here, it is "best effort" at this stage.
         * bplib should retry based on custody signals if this does not work. */
        CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_PACKET_IO_WRITE, CFE_PSP_IODriver_VPARG(&WrBuf));

        Entry->CurrentBufferSize = 0;
    }

    return CFE_SUCCESS;

} /* End of BPNODE_CLA_ProcessBundleOutput() */