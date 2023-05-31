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

#include <ctype.h>

#include "cfe.h"

#include "bp_global.h"
#include "bp_app.h"
#include "bp_eventids.h"

#include "bplib.h"
#include "bplib_routing.h"

#include "iodriver_base.h"
#include "iodriver_packet_io.h"

#define BP_CLA_PSP_DRIVER_NAME       "unsock_intf"
#define BP_CLA_PSP_OUTPUT_SUBCHANNEL 0
#define BP_CLA_PSP_INPUT_SUBCHANNEL  1
#define BP_CLA_PSP_BUFFER_SIZE       3072

typedef struct
{
    CFE_PSP_IODriver_Direction_t Dir;
    CFE_ES_TaskId_t              ServiceTask;
    CFE_PSP_IODriver_Location_t  PspLocation;
    size_t                       CurrentBufferSize;
    uint8_t                      BundleBuffer[BP_CLA_PSP_BUFFER_SIZE];

} BP_CLA_BundleFlowEntry_t;

/*
** Global Data Section
*/
typedef struct
{
    bool        EgressServiceEnabled;
    bool        IngressServiceEnabled;
    bp_handle_t ClaIntfId;

    BP_CLA_BundleFlowEntry_t Egress;
    BP_CLA_BundleFlowEntry_t Ingress;

} BP_CLA_GlobalData_t;

BP_CLA_GlobalData_t BP_CLA_Global;

/*
** Prototypes Section
*/
CFE_Status_t BP_CLA_ProcessBundleOutput(BP_CLA_BundleFlowEntry_t *Entry);
CFE_Status_t BP_CLA_ProcessBundleInput(BP_CLA_BundleFlowEntry_t *Entry);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                   */
/* BP_CLA_OutputTask() -- Application entry point and main process loop */
/*                                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BP_CLA_OutputTask(void)
{
    CFE_Status_t status;

    /*
    ** Egress RunLoop
    */
    while (BP_CLA_Global.EgressServiceEnabled)
    {
        status = BP_CLA_ProcessBundleOutput(&BP_CLA_Global.Egress);
        if (status != CFE_SUCCESS)
        {
            OS_TaskDelay(250);
        }
    }

} /* End of BP_CLA_OutputTask() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                   */
/* BP_CLA_InputTask() -- entry point for input process (child task)     */
/*                                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BP_CLA_InputTask(void)
{
    CFE_Status_t status;

    /*
    ** Ingress RunLoop
    */
    while (BP_CLA_Global.IngressServiceEnabled)
    {
        status = BP_CLA_ProcessBundleInput(&BP_CLA_Global.Ingress);
        if (status != CFE_SUCCESS)
        {
            OS_TaskDelay(250);
        }
    }

} /* End of BP_CLA_InputTask() */

CFE_Status_t BP_CLA_InitOutput(BP_CLA_BundleFlowEntry_t *Entry)
{
    int32        PspStatus;
    CFE_Status_t CfeStatus;

    /* OUTPUT instance number should be 1 on processor 1, 2 on processor 2 */
    PspStatus = CFE_PSP_IODriver_FindByName(BP_CLA_PSP_DRIVER_NAME, &Entry->PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: CFE_PSP_IODriver_FindByName status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    Entry->PspLocation.SubsystemId  = 2 - (CFE_PSP_GetProcessorId() & 1);
    Entry->PspLocation.SubchannelId = BP_CLA_PSP_OUTPUT_SUBCHANNEL;

    PspStatus = CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_DIRECTION,
                                         CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_OUTPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_DIRECTION (output) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    PspStatus =
        CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_RUNNING (output) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    CfeStatus = CFE_ES_CreateChildTask(&Entry->ServiceTask, "BP_CLA_OUT", BP_CLA_OutputTask, CFE_ES_TASK_STACK_ALLOCATE,
                                       32768, 100, 0);
    if (CfeStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: Start Child Task (output) status %x",
                          (unsigned int)CfeStatus);
        return CfeStatus;
    }

    CFE_EVS_SendEvent(BP_CLA_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP_CLA: Started Output task ID %lx -> socket",
                      CFE_RESOURCEID_TO_ULONG(Entry->ServiceTask));

    return CFE_SUCCESS;
}

CFE_Status_t BP_CLA_InitInput(BP_CLA_BundleFlowEntry_t *Entry)
{
    int32        PspStatus;
    CFE_Status_t CfeStatus;

    /* INPUT instance number should be 2 on processor 1, 1 on processor 2 */
    PspStatus = CFE_PSP_IODriver_FindByName(BP_CLA_PSP_DRIVER_NAME, &Entry->PspLocation.PspModuleId);
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: CFE_PSP_IODriver_FindByName status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    Entry->PspLocation.SubsystemId  = 1 + (CFE_PSP_GetProcessorId() & 1);
    Entry->PspLocation.SubchannelId = BP_CLA_PSP_INPUT_SUBCHANNEL;

    PspStatus = CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_DIRECTION,
                                         CFE_PSP_IODriver_U32ARG(CFE_PSP_IODriver_Direction_INPUT_ONLY));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_DIRECTION (input) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    PspStatus =
        CFE_PSP_IODriver_Command(&Entry->PspLocation, CFE_PSP_IODriver_SET_RUNNING, CFE_PSP_IODriver_U32ARG(true));
    if (PspStatus != CFE_PSP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR,
                          "SGW Error: CFE_PSP_IODriver_Command CFE_PSP_IODriver_SET_RUNNING (input) status %x",
                          (unsigned int)PspStatus);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    CfeStatus = CFE_ES_CreateChildTask(&Entry->ServiceTask, "BP_CLA_IN", BP_CLA_InputTask, CFE_ES_TASK_STACK_ALLOCATE,
                                       32768, 100, 0);
    if (CfeStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_CLA_ERR_EID, CFE_EVS_EventType_ERROR, "SGW Error: Start Child Task (input) status %x\n",
                          (unsigned int)CfeStatus);
        return CfeStatus;
    }

    CFE_EVS_SendEvent(BP_CLA_INFO_EID, CFE_EVS_EventType_INFORMATION, "BP_CLA: Started Input task ID %lx <- socket",
                      CFE_RESOURCEID_TO_ULONG(Entry->ServiceTask));

    return CFE_SUCCESS;
}

int32 BP_CLA_SetupLibIntf(void)
{
    /* Create bplib CLA and default route */
    BP_CLA_Global.ClaIntfId = bplib_create_cla_intf(BP_GlobalData.RouteTbl);
    if (!bp_handle_is_valid(BP_CLA_Global.ClaIntfId))
    {
        fprintf(stderr, "%s(): bplib_create_cla_intf failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    if (bplib_route_add(BP_GlobalData.RouteTbl, 0, 0, BP_CLA_Global.ClaIntfId) < 0)
    {
        fprintf(stderr, "%s(): bplib_route_add cla failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    if (bplib_route_intf_set_flags(BP_GlobalData.RouteTbl, BP_CLA_Global.ClaIntfId,
                                   BPLIB_INTF_STATE_ADMIN_UP | BPLIB_INTF_STATE_OPER_UP) < 0)
    {
        fprintf(stderr, "%s(): bplib_route_intf_set_flags cla failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* BP_CLA_Init() -- SGW initialization                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t BP_CLA_Init(void)
{
    CFE_Status_t status;

    memset(&BP_CLA_Global, 0, sizeof(BP_CLA_Global));

    status = BP_CLA_SetupLibIntf();
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    /* Create Message Pipe if configured for output */
    BP_CLA_Global.EgressServiceEnabled = true;
    status                             = BP_CLA_InitOutput(&BP_CLA_Global.Egress);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    BP_CLA_Global.IngressServiceEnabled = true;
    status                              = BP_CLA_InitInput(&BP_CLA_Global.Ingress);
    if (status != CFE_SUCCESS)
    {
        return status;
    }

    CFE_EVS_SendEvent(BP_CLA_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "BP_CLA Initialized");

    return CFE_SUCCESS;
} /* End of BP_CLA_Init() */

void BP_CLA_DebugPrint(const char *Tag, const void *Addr, size_t Sz)
{
    const uint8 *Data = Addr;
    char         HexBuf[50];
    char         ChBuf[20];
    size_t       i;
    size_t       pos;

    OS_printf("Packet %s:\n", Tag);
    for (i = 0; i < Sz; ++i)
    {
        pos = i & 0xF;
        snprintf(&HexBuf[pos * 3], 4, "%02x ", Data[i]);
        if (isprint((unsigned char)Data[i]))
        {
            ChBuf[pos] = Data[i];
        }
        else
        {
            ChBuf[pos] = '.';
        }

        if (pos == 15)
        {
            HexBuf[48] = 0;
            ChBuf[16]  = 0;

            OS_printf("%04lx: %-50s %20s\n", (unsigned long)i & ~0x0F, HexBuf, ChBuf);
        }
    }
    pos             = i & 0xF;
    HexBuf[pos * 3] = 0;
    ChBuf[pos]      = 0;
    if (HexBuf[0] != 0)
    {
        OS_printf("%04lx: %-50s %20s\n", (unsigned long)i & ~0x0F, HexBuf, ChBuf);
    }
    OS_printf("\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* BP_CLA_ProcessBundleInput() -- Forward ingress bundles         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CFE_Status_t BP_CLA_ProcessBundleInput(BP_CLA_BundleFlowEntry_t *Entry)
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
            BP_CLA_DebugPrint("Ingress", RdBuf.BufferMem, RdBuf.BufferSize);
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
        Status = bplib_cla_ingress(BP_GlobalData.RouteTbl, BP_CLA_Global.ClaIntfId, Entry->BundleBuffer,
                                   Entry->CurrentBufferSize, 0);
        if (Status != BP_TIMEOUT)
        {
            Entry->CurrentBufferSize = 0;
            if (Status == BP_SUCCESS)
            {
                Status = CFE_SUCCESS;
            }
            else
            {
                CFE_EVS_SendEvent(BP_LIB_PROC_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to process bundle", "cla_ingress", (int)Status);
                Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
            }
        }
    }

    return Status;
}

CFE_Status_t BP_CLA_ProcessBundleOutput(BP_CLA_BundleFlowEntry_t *Entry)
{
    CFE_PSP_IODriver_WritePacketBuffer_t WrBuf;
    int32                                Status;
    size_t                               TempSize;

    if (Entry->CurrentBufferSize == 0)
    {
        /* Load next bundle */
        TempSize = sizeof(Entry->BundleBuffer);
        Status = bplib_cla_egress(BP_GlobalData.RouteTbl, BP_CLA_Global.ClaIntfId, Entry->BundleBuffer, &TempSize, 100);
        if (Status == BP_SUCCESS)
        {
            Entry->CurrentBufferSize = TempSize;
        }
        else if (Status != BP_TIMEOUT)
        {
            CFE_EVS_SendEvent(BP_LIB_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Flow %s - Failed (%d) to load bundle",
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

        BP_CLA_DebugPrint("Egress", WrBuf.BufferMem, WrBuf.OutputSize);
        Entry->CurrentBufferSize = 0;
    }

    return CFE_SUCCESS;

} /* End of BP_CLA_ProcessBundleOutput() */
