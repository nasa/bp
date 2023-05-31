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

/************************************************
 * Includes
 ************************************************/

#include "bplib.h"

#include "bp_flow.h"
#include "bp_eventids.h"
#include "bp_io.h"
#include "bp_platform_cfg.h"
#include "bp_global.h"
#include "bplib_routing.h"

#include "cfe_resourceid_basevalue.h"

/************************************************
 * Typedefs
 ************************************************/

/*
 * Base for unique-ish BP Flow ID handles. This is just a random
 * offset from OS_OBJECT_ID_USER.
 *
 * This is chosen to not overlap with default CFE resource IDs, but
 * even if there is overlap, or if another CFS app chooses the same
 * random value, it is not going to break anything because type
 * checking still applies.  So an overlap/collision just slightly weakens
 * the protection offered here, but it doesn't cause a problem.
 */
enum
{
    BP_FLOW_HANDLE_BASE = CFE_RESOURCEID_MAKE_BASE(OS_OBJECT_TYPE_USER + 19)
};

typedef union
{
    CFE_SB_Buffer_t Buffer;

    /* This member exists to make sure that this object is sized appropriately
     * to hold the largest possible packet.  It is not accessed directly. */
    /* cppcheck-suppress unusedStructMember */
    unsigned char PacketSpace[BP_MAX_PACKET_SIZE];
} BP_SbIOBuffer_t;

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

int32 BP_FlowHandle_ToIndex(BP_FlowHandle_t Flow, uint32 *Idx)
{
    return CFE_ResourceId_ToIndex(CFE_RESOURCEID_UNWRAP(Flow), BP_FLOW_HANDLE_BASE, BP_MAX_FLOWS, Idx);
}

static BP_FlowCtrlEntry_t *BP_LocateFlowEntryByHandle(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;
    uint32              Idx;

    if (BP_FlowHandle_ToIndex(Flow, &Idx) == CFE_SUCCESS)
    {
        FlowPtr = &BP_GlobalData.FlowControl[Idx];
    }
    else
    {
        FlowPtr = NULL;
    }

    return FlowPtr;
}

static inline bool BP_FlowEntryIsMatch(const BP_FlowCtrlEntry_t *FlowPtr, BP_FlowHandle_t Flow)
{
    return (FlowPtr != NULL && CFE_RESOURCEID_TEST_EQUAL(FlowPtr->Handle, Flow));
}

/*----------------------------------------------------------------
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool BP_CheckFlowSlotUsed(CFE_ResourceId_t CheckId)
{
    BP_FlowCtrlEntry_t *FlowPtr;
    /*
     * Note - The pointer here should never be NULL because the ID should always be
     * within the expected range, but if it ever is NULL, this should return true
     * such that the caller will _not_ attempt to use the record.
     */
    FlowPtr = BP_LocateFlowEntryByHandle(BP_FLOWHANDLE_C(CheckId));
    return (FlowPtr == NULL || CFE_RESOURCEID_TEST_DEFINED(FlowPtr->Handle));
}

/*-----------------------------------------------
 * initialize_throttling
 *-----------------------------------------------*/
static void initialize_throttling(void)
{
    int index = 0;
    for (index = 0; index < BP_NUM_SEM_THROTTLES; index++)
    {
        if (OS_CountSemGetIdByName(&BP_GlobalData.Throttles[index].Sem, BP_GlobalData.Throttles[index].Name) ==
            CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BP_SEM_INFO_EID, CFE_EVS_EventType_INFORMATION, "Found semaphore: %s",
                              BP_GlobalData.Throttles[index].Name);
        }
        else if (OS_CountSemCreate(&BP_GlobalData.Throttles[index].Sem, BP_GlobalData.Throttles[index].Name,
                                   BP_GlobalData.Throttles[index].Max, 0) == CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BP_SEM_INFO_EID, CFE_EVS_EventType_INFORMATION, "Created semaphore: %s",
                              BP_GlobalData.Throttles[index].Name);
        }
        else
        {
            CFE_EVS_SendEvent(BP_SEM_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to create semaphore: %s",
                              BP_GlobalData.Throttles[index].Name);
        }
    }
}

/*-----------------------------------------------
 * accumulate_throttling
 *-----------------------------------------------*/
static void accumulate_throttling(CFE_SB_MsgId_t MsgId)
{
    int index = 0;

    for (index = 0; index < BP_NUM_SEM_THROTTLES; index++)
    {
        if (CFE_SB_MsgId_Equal(MsgId, BP_GlobalData.Throttles[index].MsgId))
        {
            if (OS_ObjectIdDefined(BP_GlobalData.Throttles[index].Sem))
            {
                if (BP_GlobalData.Throttles[index].Throttle < BP_GlobalData.Throttles[index].Max)
                {
                    BP_GlobalData.Throttles[index].Throttle++;
                }
                else
                {
                    CFE_EVS_SendEvent(BP_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Unexpected traffic being throttled by: %s", BP_GlobalData.Throttles[index].Name);
                }
            }
        }
    }
}

/*-----------------------------------------------
 * disperse_throttling
 *-----------------------------------------------*/
static void disperse_throttling(void)
{
    int index, count;
    for (index = 0; index < BP_NUM_SEM_THROTTLES; index++)
    {
        {
            for (count = 0; count < BP_GlobalData.Throttles[index].Throttle; count++)
            {
                OS_CountSemGive(BP_GlobalData.Throttles[index].Sem);
            }
        }

        /* Reset Throttle */
        BP_GlobalData.Throttles[index].Throttle = 0;
    }
}

/*-----------------------------------------------
 * check_throttling
 *-----------------------------------------------*/
static bool check_throttling(CFE_SB_MsgId_t MsgId)
{
    int index = 0;

    for (index = 0; index < BP_NUM_SEM_THROTTLES; index++)
    {
        if (CFE_SB_MsgId_Equal(MsgId, BP_GlobalData.Throttles[index].MsgId))
        {
            if (OS_ObjectIdDefined(BP_GlobalData.Throttles[index].Sem))
            {
                return true;
            }
        }
    }
    return false;
}

/*-----------------------------------------------
 *
 *  note: the higher the priority value, the higher
 *  the priority of the flow (i.e. it will get
 *  precedence)
 *-----------------------------------------------*/
static void prioritize_flows(void)
{
    int flow_idx;
    /* BP_FlowHandle_t level, prev_level; */
    BP_FlowCtrlEntry_t *FlowPtr;
    BP_FlowCtrlEntry_t *LevelPtr;
    BP_FlowCtrlEntry_t *PrevLevelPtr;
    BP_FlowCtrlEntry_t *CurrPtr;

    /* Initialize Empty Prioritization */
    BP_GlobalData.FlowPriorityHead = BP_INVALID_FLOW;
    for (flow_idx = 0; flow_idx < BP_MAX_FLOWS; flow_idx++)
    {
        FlowPtr = &BP_GlobalData.FlowControl[flow_idx];

        FlowPtr->COS.NextFlow  = BP_INVALID_FLOW;
        FlowPtr->COS.NextLevel = BP_INVALID_FLOW;
    }

    /* Sort Each Flow into Priority Levels */
    for (flow_idx = 0; flow_idx < BP_MAX_FLOWS; flow_idx++)
    {
        FlowPtr = &BP_GlobalData.FlowControl[flow_idx];

        /* Check Flow Priority */
        if (FlowPtr->COS.Priority == BP_INVALID_PRIORITY)
            continue;

        /* Find Level */
        PrevLevelPtr = NULL;
        LevelPtr     = BP_LocateFlowEntryByHandle(BP_GlobalData.FlowPriorityHead);
        while (LevelPtr != NULL)
        {
            if (FlowPtr->COS.Priority >= LevelPtr->COS.Priority)
            {
                break;
            }

            /* Goto Next Level (lower priority) */
            PrevLevelPtr = LevelPtr;
            LevelPtr     = BP_LocateFlowEntryByHandle(LevelPtr->COS.NextLevel);
        }

        /* Insert Flow */
        if (LevelPtr == NULL) /* new lowest priority */
        {
            if (PrevLevelPtr == NULL)
            {
                BP_GlobalData.FlowPriorityHead = FlowPtr->Handle;
            }
            else
            {
                PrevLevelPtr->COS.NextLevel = FlowPtr->Handle;
            }
        }
        else if (FlowPtr->COS.Priority > LevelPtr->COS.Priority) /* insert right before level */
        {
            FlowPtr->COS.NextLevel = LevelPtr->Handle;

            if (PrevLevelPtr == NULL)
            {
                BP_GlobalData.FlowPriorityHead = FlowPtr->Handle;
            }
            else
            {
                PrevLevelPtr->COS.NextLevel = FlowPtr->Handle;
            }
        }
        else /* insert at level */
        {
            /* Goto End of Level */
            PrevLevelPtr = LevelPtr;
            while (true)
            {
                CurrPtr = BP_LocateFlowEntryByHandle(PrevLevelPtr->COS.NextFlow);
                if (CurrPtr == NULL)
                {
                    break;
                }

                PrevLevelPtr = CurrPtr;
            }

            /* Insert at End of Level */
            PrevLevelPtr->COS.NextFlow  = FlowPtr->Handle;
            PrevLevelPtr->COS.NextLevel = LevelPtr->COS.NextLevel;
        }
    }
}

/*-----------------------------------------------
 * load_table
 *-----------------------------------------------*/
static int32 BP_LoadFlowConfigTable(CFE_TBL_Handle_t handle, const char *filename, BP_FlowTable_t **buffer)
{
    int32 cfe_status;
    void *table_ptr;

    /* Load table using cFE */
    if (filename != NULL)
    {
        cfe_status = CFE_TBL_Load(handle, CFE_TBL_SRC_FILE, filename);
        if (cfe_status < CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(BP_TBL_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%lX) to load table %s",
                              (unsigned long)cfe_status, filename);
            return BP_TBL_LOAD_ERR_EID;
        }
    }

    /* Allow cFE an opportunity to make table updates */
    cfe_status = CFE_TBL_Manage(handle);
    if (cfe_status < CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_TBL_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%lX) to manage table %s",
                          (unsigned long)cfe_status, filename);
        return BP_TBL_LOAD_ERR_EID;
    }

    /* Copy Table Over */
    cfe_status = CFE_TBL_GetAddress(&table_ptr, handle);
    if (cfe_status < CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_TBL_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%lX) to populate table %s",
                          (unsigned long)cfe_status, filename);
        return BP_TBL_LOAD_ERR_EID;
    }

    *buffer = table_ptr;
    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * store_incoming_data
 *-----------------------------------------------*/
static int store_incoming_data(BP_FlowCtrlEntry_t *flow, uint32_t *flags, int max_cycles)
{
    int          cycles = 0;
    int          lib_status;
    CFE_Status_t Status;

    while (cycles < max_cycles)
    {
        /* Read Next Data */
        if (flow->CurrentChunkInSize == 0)
        {
            flow->CurrentChunkInPtr = NULL;
            Status                  = CFE_SB_ReceiveBuffer(&flow->CurrentSbMsgInPtr, flow->DataPipe, CFE_SB_POLL);
            if (Status == CFE_SUCCESS)
            {
                Status = CFE_MSG_GetMsgId(&flow->CurrentSbMsgInPtr->Msg, &flow->CurrentSbMsgInId);
            }
            if (Status == CFE_SUCCESS)
            {
                Status = CFE_MSG_GetSize(&flow->CurrentSbMsgInPtr->Msg, &flow->CurrentChunkInSize);
            }

            if (Status != CFE_SUCCESS)
            {
                /* Generate Event Message if Software Bus Failure */
                if (Status != CFE_SB_NO_MESSAGE)
                {
                    flow->DataInDropped++;
                    CFE_EVS_SendEvent(BP_IO_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Failed to read data. Status = %x", (unsigned int)Status);
                }
                flow->CurrentChunkInSize = 0;
                break;
            }

            if (flow->CurrentChunkInSize > BP_MAX_PACKET_SIZE)
            {
                /* Msg is too big to be accepted into BP */
                flow->DataInDropped++;
                CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - attempted to store packet that was too large: %lu", flow->Config.Name,
                                  (unsigned long)flow->CurrentChunkInSize);

                flow->CurrentChunkInSize = 0;
            }

            /* Assign Buffer */
            flow->CurrentChunkInSize = CFE_SB_GetUserDataLength(&flow->CurrentSbMsgInPtr->Msg);
            flow->CurrentChunkInPtr  = CFE_SB_GetUserData(&flow->CurrentSbMsgInPtr->Msg);

            /* Throttle Incoming Data */
            accumulate_throttling(flow->CurrentSbMsgInId);
        }

        /* Store Data */
        if (flow->CurrentChunkInSize > 0)
        {
            lib_status = bplib_send(flow->BPS, flow->CurrentChunkInPtr, flow->CurrentChunkInSize, 0);
            if (lib_status == BP_TIMEOUT)
            {
                /* temporary error; silently retry it next time */
                break;
            }

            flow->CurrentChunkInSize = 0;

            if (lib_status != BP_SUCCESS)
            {
                flow->DataInDropped++;
                CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to store data [%08X]", flow->Config.Name, (int)lib_status,
                                  (unsigned int)*flags);
                break;
            }
        }
    }

    return cycles;
}

/*-----------------------------------------------
 * accept_outgoing_data
 *-----------------------------------------------*/
static int accept_outgoing_data(BP_FlowCtrlEntry_t *flow, uint32_t *flags, int max_cycles)
{
    int                         cycles = 0;
    int                         lib_status;
    CFE_Status_t                Status;
    static const CFE_MSG_Size_t MSG_MAX_SIZE = 1024;

    while (cycles < max_cycles)
    {
        /* Accept next data */
        if (flow->CurrentSbMsgOutPtr == NULL)
        {
            flow->CurrentSbMsgOutPtr = CFE_SB_AllocateMessageBuffer(MSG_MAX_SIZE);
            if (flow->CurrentSbMsgOutPtr != NULL)
            {
                CFE_MSG_SetMsgId(&flow->CurrentSbMsgOutPtr->Msg, flow->Config.RecvStreamId);
                CFE_MSG_SetSize(&flow->CurrentSbMsgOutPtr->Msg, MSG_MAX_SIZE);
            }
        }

        if (flow->CurrentSbMsgOutPtr != NULL)
        {
            flow->CurrentChunkOutPtr  = CFE_SB_GetUserData(&flow->CurrentSbMsgOutPtr->Msg);
            flow->CurrentChunkOutSize = CFE_SB_GetUserDataLength(&flow->CurrentSbMsgOutPtr->Msg);

            lib_status = bplib_recv(flow->BPS, flow->CurrentChunkOutPtr, &flow->CurrentChunkOutSize, BP_CHECK);
            if (lib_status == BP_TIMEOUT)
            {
                break;
            }
            else if (lib_status != BP_SUCCESS)
            {
                CFE_EVS_SendEvent(BP_LIB_ACCEPT_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to accept data", flow->Config.Name, lib_status);
                break;
            }

            /* Update the header to the actual size */
            CFE_SB_SetUserDataLength(&flow->CurrentSbMsgOutPtr->Msg, flow->CurrentChunkOutSize);

            Status = CFE_SB_TransmitBuffer(flow->CurrentSbMsgOutPtr, false);

            /*
             * Wrap-up: If anything did not work, increment drop counter.
             */
            if (Status != CFE_SUCCESS)
            {
                flow->DataOutDropped++;
                CFE_EVS_SendEvent(BP_IO_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Failed to forward data onto software bus: %lX", (unsigned long)Status);

                if (flow->CurrentSbMsgOutPtr != NULL)
                {
                    /*
                     * Be sure to release the buffer if the transmit did not happen.
                     *
                     * Note that CFE_SB_TransmitBuffer takes ownership but only if it returns CFE_SUCCESS.
                     * If for any reason CFE_SB_TransmitBuffer was not called or did not succeed, the buffer
                     * must be released here to avoid leaking.
                     */
                    CFE_SB_ReleaseMessageBuffer(flow->CurrentSbMsgOutPtr);
                }
            }

            flow->CurrentSbMsgOutPtr  = NULL;
            flow->CurrentChunkOutPtr  = NULL;
            flow->CurrentChunkOutSize = 0;
        }

        cycles++;
    }

    return cycles;
}

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BP_FlowInit
 *-----------------------------------------------*/
int32 BP_FlowInit(const char *AppName)
{
    int32 cfe_status;

    /* Initialize Throttling */
    initialize_throttling();

    BP_GlobalData.LastFlowHandle = CFE_ResourceId_FromInteger(BP_FLOW_HANDLE_BASE);

    /* Register flow table with cFE */
    BP_GlobalData.FlowTableHandle = BP_INVALID_TABLE;
    cfe_status = CFE_TBL_Register(&BP_GlobalData.FlowTableHandle, BP_FLOW_TBL_NAME, sizeof(BP_FlowTable_t),
                                  CFE_TBL_OPT_DEFAULT, NULL);
    if (cfe_status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_TBL_LOAD_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%d) to register flow table %s",
                          (int)cfe_status, BP_FLOW_TBL_NAME);
        return BP_TBL_LOAD_ERR_EID;
    }

    snprintf(BP_GlobalData.ConfigTableName, sizeof(BP_GlobalData.ConfigTableName), "%s.%s", BP_GlobalData.AppName,
             BP_FLOW_TBL_NAME);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowLoad
 *-----------------------------------------------*/
int32 BP_FlowLoad(const char *FlowTableFileName)
{
    int32               cfe_status;
    BP_FlowTable_t       *StagedConfig;
    BP_FlowTblEntry_t  *StagingEntryPtr;
    BP_FlowCtrlEntry_t *FlowPtr;
    CFE_ResourceId_t    PendingFlowHandle;
    CFE_TBL_Info_t      tbl_info;
    bp_ipn_addr_t       storage_addr;
    int                 flow_idx, num_flows = 0;

    /* Load Flow Table */
    cfe_status = BP_LoadFlowConfigTable(BP_GlobalData.FlowTableHandle, FlowTableFileName, &StagedConfig);
    if (cfe_status != CFE_SUCCESS)
        return cfe_status;

    BP_GlobalData.LocalNodeNumber = StagedConfig->LocalNodeIpn;
    BP_GlobalData.BaseIntfId      = bplib_create_node_intf(BP_GlobalData.RouteTbl, BP_GlobalData.LocalNodeNumber);
    if (!bp_handle_is_valid(BP_GlobalData.BaseIntfId))
    {
        fprintf(stderr, "%s(): bplib_create_node_intf failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }
    if (bplib_route_intf_set_flags(BP_GlobalData.RouteTbl, BP_GlobalData.BaseIntfId,
                                   BPLIB_INTF_STATE_ADMIN_UP | BPLIB_INTF_STATE_OPER_UP) < 0)
    {
        fprintf(stderr, "%s(): bplib_route_intf_set_flags failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* this currently assumes service number 10 for storage, should be configurable */
    storage_addr            = (bp_ipn_addr_t) {BP_GlobalData.LocalNodeNumber, 10};
    BP_GlobalData.StorageId = bplib_create_file_storage(BP_GlobalData.RouteTbl, &storage_addr);
    if (!bp_handle_is_valid(BP_GlobalData.StorageId))
    {
        fprintf(stderr, "%s(): bplib_create_file_storage failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }
    if (bplib_route_intf_set_flags(BP_GlobalData.RouteTbl, BP_GlobalData.StorageId,
                                   BPLIB_INTF_STATE_ADMIN_UP | BPLIB_INTF_STATE_OPER_UP) < 0)
    {
        fprintf(stderr, "%s(): bplib_route_intf_set_flags storage failed\n", __func__);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Remove Any Open Throttle Pipes */
    for (flow_idx = 0; flow_idx < BP_MAX_FLOWS; flow_idx++)
    {
        FlowPtr = &BP_GlobalData.FlowControl[flow_idx];

        if (!CFE_RESOURCEID_TEST_DEFINED(FlowPtr->Handle))
        {
            /* Not configured; move on */
            continue;
        }

        /* Check for Open Throttle Pipe */
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->DataPipe))
        {
            CFE_SB_DeletePipe(FlowPtr->DataPipe);
            FlowPtr->DataPipe = CFE_SB_INVALID_PIPE;
            CFE_EVS_SendEvent(BP_PIPE_INFO_EID, CFE_EVS_EventType_INFORMATION,
                              "Deleting data pipe for flow handle = %lx", CFE_RESOURCEID_TO_ULONG(FlowPtr->Handle));
        }

        /* Wipe the structure */
        memset(FlowPtr, 0, sizeof(*FlowPtr));
    }

    /* Create Flow Control Structure for each valid Config Table Entry */
    for (flow_idx = 0; flow_idx < BP_MAX_FLOWS; flow_idx++)
    {
        StagingEntryPtr = &StagedConfig->Flows[flow_idx];

        /* Check If Flow Populated */
        if (StagingEntryPtr->Name[0] != '\0')
        {
            PendingFlowHandle =
                CFE_ResourceId_FindNext(BP_GlobalData.LastFlowHandle, BP_MAX_FLOWS, BP_CheckFlowSlotUsed);
            FlowPtr = BP_LocateFlowEntryByHandle(BP_FLOWHANDLE_C(PendingFlowHandle));

            if (FlowPtr != NULL)
            {
                FlowPtr->Handle              = BP_FLOWHANDLE_C(PendingFlowHandle);
                BP_GlobalData.LastFlowHandle = PendingFlowHandle;

                /* Set Config Table */
                memcpy(&FlowPtr->Config, StagingEntryPtr, sizeof(BP_FlowTblEntry_t));

                /* Set Flow Priority
                 *  note: the uint16 config priority should
                 *  never overflow the int COS priority */
                FlowPtr->COS.Priority = (int)FlowPtr->Config.Priority;

                /* Enable Flow (if defaulted that way) */
                if (FlowPtr->Config.Enabled)
                {
                    if (BP_FlowEnable(FlowPtr->Handle) == CFE_SUCCESS)
                    {
                        num_flows++;
                    }
                }
            }
        }
    }

    /* need to release the address obtained from BP_LoadFlowConfigTable */
    CFE_TBL_ReleaseAddress(BP_GlobalData.FlowTableHandle);

    /* Build Flow Priority Order */
    prioritize_flows();

    /* Get Table Information */
    cfe_status = CFE_TBL_GetInfo(&tbl_info, BP_GlobalData.ConfigTableName);
    if (cfe_status == CFE_SUCCESS)
    {
        /* Generate Event Message with Information */
        CFE_EVS_SendEvent(BP_TBL_LOAD_INFO_EID, CFE_EVS_EventType_INFORMATION,
                          "Loaded flow table %s (size: %d, crc: %04X), %d flows enabled", tbl_info.LastFileLoaded,
                          (int)tbl_info.Size, (unsigned int)tbl_info.Crc, num_flows);
    }
    else
    {
        /* Generate Event Message without Information */
        CFE_EVS_SendEvent(BP_TBL_LOAD_INFO_EID, CFE_EVS_EventType_INFORMATION,
                          "Table information unknown, %d flows enabled", num_flows);
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowEnable
 *----------------------------------------------*/
int32 BP_FlowEnable(BP_FlowHandle_t Flow)
{
    int                 i;
    char                pipe_name[BP_PIPE_NAME_SIZE];
    int32               status = CFE_SUCCESS;
    BP_FlowCtrlEntry_t *FlowPtr;
    bp_ipn_addr_t       ipn_addr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled)
    {
        return CFE_SUCCESS; /* not an error */
    }

    /* Initialize Buffers */
    FlowPtr->CurrentSbMsgInId   = CFE_SB_INVALID_MSG_ID;
    FlowPtr->CurrentSbMsgInPtr  = NULL;
    FlowPtr->CurrentSbMsgOutPtr = NULL;
    FlowPtr->CurrentChunkInPtr  = NULL;
    FlowPtr->CurrentChunkInSize = 0;
    FlowPtr->DataInDropped      = 0;
    FlowPtr->DataOutDropped     = 0;
    FlowPtr->LibFlags           = 0;

    /* Create bplib application data socket */
    FlowPtr->BPS = bplib_create_socket(BP_GlobalData.RouteTbl);
    if (FlowPtr->BPS == NULL)
    {
        CFE_EVS_SendEvent(BP_LIB_OPEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to open library channel for flow %s",
                          FlowPtr->Config.Name);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    ipn_addr = (bp_ipn_addr_t) {BP_GlobalData.LocalNodeNumber, FlowPtr->Config.SrcServ};
    if (bplib_bind_socket(FlowPtr->BPS, &ipn_addr) < 0)
    {
        fprintf(stderr, "Failed bplib_bind_socket()... exiting\n");
        bplib_close_socket(FlowPtr->BPS);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    ipn_addr = (bp_ipn_addr_t) {FlowPtr->Config.DstNode, FlowPtr->Config.DstServ};
    if (bplib_connect_socket(FlowPtr->BPS, &ipn_addr) < 0)
    {
        fprintf(stderr, "Failed bplib_connect_socket()... exiting\n");
        bplib_close_socket(FlowPtr->BPS);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    /* Create data pipe */
    if (status == CFE_SUCCESS)
    {
        if (!CFE_RESOURCEID_TEST_DEFINED(FlowPtr->DataPipe))
        {
            snprintf(pipe_name, BP_PIPE_NAME_SIZE, "%s%s", BP_DATA_PIPE_PREFIX, FlowPtr->Config.Name);
            int32 cfe_status = CFE_SB_CreatePipe(&FlowPtr->DataPipe, FlowPtr->Config.PipeDepth, pipe_name);
            if (cfe_status != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR, "Failed (%d) to create data pipe %s",
                                  (int)cfe_status, pipe_name);
                status = BP_PIPE_ERR_EID;
            }
        }
        else
        {
            CFE_EVS_SendEvent(BP_PIPE_INFO_EID, CFE_EVS_EventType_INFORMATION, "Reusing existing data pipe for flow %s",
                              FlowPtr->Config.Name);
        }
    }

    /* Subscribe to telemetry packets on software bus */
    if (status == CFE_SUCCESS)
    {
        for (i = 0; i < BP_PKTTBL_MAX_ROWS; i++)
        {
            /* Skip invalid entries */
            if (FlowPtr->Config.PktTbl[i].BuffLim == 0)
            {
                continue;
            }

            /* Subscribe valid entry */
            CFE_SB_Qos_t Quality    = {FlowPtr->Config.PktTbl[i].Priority, FlowPtr->Config.PktTbl[i].Reliability};
            int32        cfe_status = CFE_SB_SubscribeEx(FlowPtr->Config.PktTbl[i].StreamId, FlowPtr->DataPipe, Quality,
                                                         FlowPtr->Config.PktTbl[i].BuffLim);
            if (cfe_status != CFE_SUCCESS)
            {
                /* Report Failures and Continue (do not mark status) */
                CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Failed (%d) to subscribe to %04lX on data pipe", (int)cfe_status,
                                  (unsigned long)CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId));

                /* Mark Flow as Unhealthy */
                FlowPtr->Healthy = false;
            }
            else
            {
                CFE_ES_WriteToSysLog("BP_FLOW: Subscribed to %04lx\n",
                                     (unsigned long)CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId));
            }
        }

        /* Success */
        FlowPtr->Active  = false; /* Start Out Paused */
        FlowPtr->Enabled = true;
    }
    else
    {
        /* Mark Flow as Unhealthy */
        FlowPtr->Healthy = false;

        /* Delete data pipe
         *  if the pipe is a throttle pipe, then this is a case where it will get deleted */
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->DataPipe))
        {
            CFE_SB_DeletePipe(FlowPtr->DataPipe);
            FlowPtr->DataPipe = BP_INVALID_PIPE;
        }

        /* Close BPLIB channel */
        if (FlowPtr->BPS != NULL)
        {
            bplib_close_socket(FlowPtr->BPS);
            FlowPtr->BPS = NULL;
        }
    }

    return status;
}

/*-----------------------------------------------
 * BP_FlowDisable
 *----------------------------------------------*/
int32 BP_FlowDisable(BP_FlowHandle_t Flow)
{
    int                 i;
    int32               cfe_status;
    bool                throttle_pipe;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled)
    {
        FlowPtr->Enabled = false;
        FlowPtr->Active  = false;
        FlowPtr->Healthy = true;

        /* Delete Pipe */
        throttle_pipe = false;
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->DataPipe))
        {
            /* Unsubscribe to telemetry packets on software bus */
            for (i = 0; i < BP_PKTTBL_MAX_ROWS; i++)
            {
                /* Skip invalid entries */
                if (FlowPtr->Config.PktTbl[i].BuffLim == 0)
                {
                    continue;
                }

                /* Check for throttle entry */
                if (check_throttling(FlowPtr->Config.PktTbl[i].StreamId))
                {
                    CFE_EVS_SendEvent(BP_PIPE_INFO_EID, CFE_EVS_EventType_INFORMATION,
                                      "Throttled stream ID detected (%04lX)... preserving data pipe for flow %s",
                                      (unsigned long)CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId),
                                      FlowPtr->Config.Name);
                    throttle_pipe = true;
                    continue;
                }

                /* Unsubscribe valid entry */
                cfe_status = CFE_SB_Unsubscribe(FlowPtr->Config.PktTbl[i].StreamId, FlowPtr->DataPipe);
                if (cfe_status != CFE_SUCCESS)
                {
                    CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Failed (%d) to unsubscribe %04lX from data pipe", (int)cfe_status,
                                      (unsigned long)CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId));
                }
            }

            /* Delete Pipe */
            if (!throttle_pipe)
            {
                CFE_SB_DeletePipe(FlowPtr->DataPipe);
                FlowPtr->DataPipe = BP_INVALID_PIPE;
            }
        }

        /* Close BPLIB Channel */
        if (FlowPtr->BPS != NULL)
        {
            bplib_close_socket(FlowPtr->BPS);
            FlowPtr->BPS = NULL;
        }
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowSetTimeout
 *----------------------------------------------*/
int32 BP_FlowSetTimeout(BP_FlowHandle_t Flow, int Timeout)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot configure disabled flow: %s",
                          BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    if (FlowPtr->Config.Timeout == 0 && Timeout != 0)
    {
        CFE_EVS_SendEvent(BP_PARM_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot set non-zero timeout on flow %s that does not request custody", BP_FlowGetName(Flow));
        return BP_PARM_ERR_EID;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowRevertTimeout
 *----------------------------------------------*/
int32 BP_FlowRevertTimeout(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    return BP_FlowSetTimeout(Flow, FlowPtr->Config.Timeout);
}

/*-----------------------------------------------
 * BP_FlowSetPriority
 *----------------------------------------------*/
int32 BP_FlowSetPriority(BP_FlowHandle_t Flow, int Priority)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot configure disabled flow: %s",
                          BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    FlowPtr->COS.Priority = Priority;
    prioritize_flows(); /* Re-sort Flow Priorities */
    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowRevertPriority
 *----------------------------------------------*/
int32 BP_FlowRevertPriority(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    return BP_FlowSetPriority(Flow, FlowPtr->Config.Priority);
}

/*-----------------------------------------------
 * BP_FlowIsEnabled
 *-----------------------------------------------*/
bool BP_FlowIsEnabled(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return false;
    }

    return FlowPtr->Enabled;
}

/*-----------------------------------------------
 * BP_FlowGetStats
 *-----------------------------------------------*/
int32 BP_FlowGetStats(BP_FlowHandle_t Flow, BP_FlowStats_t *Stats)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    /* Get stats */
    if (Stats)
    {
        memset(Stats, 0, sizeof(BP_FlowStats_t));

        /* Initialize to Invalid Status */
        Stats->Active                 = BP_INVALID_STATUS;
        Stats->Healthy                = BP_INVALID_STATUS;
        Stats->IOStats.SendHealthy    = BP_INVALID_STATUS;
        Stats->IOStats.ReceiveHealthy = BP_INVALID_STATUS;

        /* Get Flow Information */
        strncpy(Stats->Name, FlowPtr->Config.Name, BP_FLOW_NAME_SIZE);
        Stats->Enabled  = FlowPtr->Enabled;
        Stats->Timeout  = FlowPtr->Config.Timeout;
        Stats->Priority = FlowPtr->COS.Priority;

        /* Get Enabled-Only Status */
        if (Stats->Enabled)
        {
            /* Get Flow Status */
            Stats->Healthy        = FlowPtr->Healthy;
            Stats->DataInDropped  = FlowPtr->DataInDropped;
            Stats->DataOutDropped = FlowPtr->DataOutDropped;
            Stats->LibFlags       = FlowPtr->LibFlags;

            /* Get IO Module Status */
            Stats->Active = FlowPtr->Active;
        }
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowClearStats
 *-----------------------------------------------*/
int32 BP_FlowClearStats(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        return CFE_SUCCESS; /* not an error */
    }

    /* Clear flow stats */
    FlowPtr->DataInDropped  = 0;
    FlowPtr->DataOutDropped = 0;

    /* Clear library stats */
    FlowPtr->LibFlags = 0;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowGetHandle
 *-----------------------------------------------*/
BP_FlowHandle_t BP_FlowGetHandle(const char *Name)
{
    int                 f;
    BP_FlowCtrlEntry_t *CheckPtr;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = NULL;
    for (f = 0; f < BP_MAX_FLOWS; f++)
    {
        CheckPtr = &BP_GlobalData.FlowControl[f];

        if (CFE_RESOURCEID_TEST_DEFINED(CheckPtr->Handle) &&
            strncmp(CheckPtr->Config.Name, Name, BP_FLOW_NAME_SIZE) == 0)
        {
            FlowPtr = CheckPtr;
            break;
        }
    }

    if (FlowPtr == NULL)
    {
        CFE_EVS_SendEvent(BP_INVALID_FLOW_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid flow: %s", Name);
        return BP_INVALID_FLOW;
    }

    return FlowPtr->Handle;
}

/*-----------------------------------------------
 * BP_FlowGetName
 *-----------------------------------------------*/
const char *BP_FlowGetName(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        /* do not return NULL, as this might be used in printf()-style call */
        return "INVALID";
    }

    return FlowPtr->Config.Name;
}

/*-----------------------------------------------
 * BP_FlowFlush
 *-----------------------------------------------*/
int32 BP_FlowFlush(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot flush disabled flow: %s",
                          BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowPause
 *-----------------------------------------------*/
int32 BP_FlowPause(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot pause disabled flow: %s",
                          BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Pause flow */
    FlowPtr->Active = false;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowResume
 *-----------------------------------------------*/
int32 BP_FlowResume(BP_FlowHandle_t Flow)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot resume disabled flow: %s",
                          BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Resume flow */
    FlowPtr->Active = true;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowProcess
 *-----------------------------------------------*/
int32 BP_FlowProcess(void)
{
    BP_FlowCtrlEntry_t *FlowPtr;
    BP_FlowCtrlEntry_t *LevelPtr;
    BP_FlowCtrlEntry_t *PrevFlowPtr;
    BP_FlowCtrlEntry_t *PrevLevelPtr;

    /* Cycle Flows in Order of Priority Levels */
    LevelPtr     = BP_LocateFlowEntryByHandle(BP_GlobalData.FlowPriorityHead);
    PrevLevelPtr = NULL;
    while (LevelPtr != NULL)
    {
        /* Reset Cycle */
        PrevFlowPtr = NULL;
        FlowPtr     = LevelPtr;

        /* Round Robin within Priority Level */
        while (FlowPtr != NULL)
        {
            /* Cycle Flow */
            if (FlowPtr->Enabled)
            {
                /* Perform Ingress Processing */
                store_incoming_data(FlowPtr, &FlowPtr->LibFlags, BP_APP_READ_LIMIT);

                /* Perform Egress Processing */
                if (FlowPtr->Active)
                {
                    accept_outgoing_data(FlowPtr, &FlowPtr->LibFlags, BP_APP_WRITE_LIMIT);
                }
            }

            /* Goto Next Flow */
            PrevFlowPtr = FlowPtr;
            FlowPtr     = BP_LocateFlowEntryByHandle(FlowPtr->COS.NextFlow);
        }

        /* Rotate Flows in Level */
        if (PrevFlowPtr != LevelPtr && PrevFlowPtr != NULL)
        {
            /* Update Flow Representing the Level
             *  note - [level].NextFlow cannot be invalid due to check that
             *  previous flow is not equal to level. */
            BP_FlowHandle_t ulevel    = LevelPtr->COS.NextFlow;
            LevelPtr->COS.NextFlow    = BP_INVALID_FLOW;
            PrevFlowPtr->COS.NextFlow = LevelPtr->Handle;
            if (PrevLevelPtr != NULL)
            {
                PrevLevelPtr->COS.NextLevel = ulevel;
            }
            else /* highest level */
            {
                BP_GlobalData.FlowPriorityHead = ulevel;
            }

            /* Set Previous Level to Updated Current Level */
            PrevLevelPtr = BP_LocateFlowEntryByHandle(ulevel);
        }
        else
        {
            /* Set Previous Level to Current Level */
            PrevLevelPtr = LevelPtr;
        }

        /* Goto Next Priority Level */
        LevelPtr = BP_LocateFlowEntryByHandle(LevelPtr->COS.NextLevel);
    }

    /* Perform Throttling */
    disperse_throttling();

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowDirectStore
 *-----------------------------------------------*/
int32 BP_FlowDirectStore(BP_FlowHandle_t Flow, uint8 *Buffer, int Len)
{
    int                 lib_status;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        FlowPtr->DataInDropped++;
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot directly store data to disabled flow: %s", BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Store data */
    lib_status = bplib_send(FlowPtr->BPS, Buffer, Len, BP_CHECK);
    if (lib_status != BP_SUCCESS)
    {
        FlowPtr->DataInDropped++;
        CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Flow %s - Failed (%d) to directly store data [%08X]", BP_FlowGetName(Flow), (int)lib_status,
                          (unsigned int)FlowPtr->LibFlags);
        return BP_LIB_LOAD_ERR_EID;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowDirectConfig
 *-----------------------------------------------*/
int32 BP_FlowDirectConfig(BP_FlowHandle_t Flow, int Mode, int Option, int *Value)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(Flow);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, Flow))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot directly configure a disabled flow: %s", BP_FlowGetName(Flow));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* This call is not yet implemented */
    return BP_LIB_CONFIG_ERR_EID;
}

/*-----------------------------------------------
 * BP_ForEachFlow
 *-----------------------------------------------*/
void BP_ForEachFlow(void (*Func)(BP_FlowHandle_t, void *Arg), void *Arg)
{
    /*
     * This recomputes the bitmask for TLM purposes, by setting
     * a bit inside a uint32 for each flow that is enabled
     */
    BP_FlowHandle_t Flow;
    uint32          pos;

    for (pos = 0; pos < BP_MAX_FLOWS; ++pos)
    {
        Flow = BP_GlobalData.FlowControl[pos].Handle;

        if (CFE_RESOURCEID_TEST_DEFINED(Flow))
        {
            Func(Flow, Arg);
        }
    }
}
