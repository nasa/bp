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
#include "bp_events.h"
#include "bp_io.h"
#include "bp_cfg.h"
#include "bp_global.h"

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
    unsigned char   PacketSpace[BP_MAX_PACKET_SIZE]; /* preallocated buffer holding current packet */
} BP_SbIOBuffer_t;

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

int32 BP_FlowHandle_ToIndex(BP_FlowHandle_t fh, uint32 *Idx)
{
    return CFE_ResourceId_ToIndex(CFE_RESOURCEID_UNWRAP(fh), BP_FLOW_HANDLE_BASE, BP_MAX_FLOWS, Idx);
}

static BP_FlowCtrlEntry_t *BP_LocateFlowEntryByHandle(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;
    uint32              Idx;

    if (BP_FlowHandle_ToIndex(fh, &Idx) == CFE_SUCCESS)
    {
        FlowPtr = &BP_GlobalData.FlowControl[Idx];
    }
    else
    {
        FlowPtr = NULL;
    }

    return FlowPtr;
}

static inline bool BP_FlowEntryIsMatch(const BP_FlowCtrlEntry_t *FlowPtr, BP_FlowHandle_t fh)
{
    return (FlowPtr != NULL && CFE_RESOURCEID_TEST_EQUAL(FlowPtr->Handle, fh));
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
static int32 BP_LoadFlowConfigTable(CFE_TBL_Handle_t handle, const char *filename, BP_FlowTbl_t **buffer)
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
 * process_incoming_bundles
 *-----------------------------------------------*/
static int process_incoming_bundles(BP_FlowCtrlEntry_t *flow, uint32_t *flags, int max_cycles)
{
    int cycles = 0;
    int lib_status;

    while (cycles < max_cycles)
    {
        /* Read next bundle */
        if (flow->CurrentBundleInSize == 0)
        {
            if (!BP_IOReadBundle(flow->IOD, &flow->CurrentBundleInPtr, &flow->CurrentBundleInSize))
            {
                break;
            }
        }

        /* Process next bundle */
        if (flow->CurrentBundleInSize != 0)
        {
            /* Process bundle */
            lib_status = bplib_process(flow->BPC, flow->CurrentBundleInPtr, flow->CurrentBundleInSize, BP_CHECK, flags);
            if (lib_status == BP_SUCCESS)
            {
                flow->CurrentBundleInSize = 0;
                cycles++;
            }
            else if (lib_status == BP_TIMEOUT)
            {
                break;
            }
            else
            {
                /* bundle channel statistics will show why it wasn't processed */
                flow->CurrentBundleInSize = 0;
                break;
            }
        }
    }

    return cycles;
}

/*-----------------------------------------------
 * store_incoming_data
 *-----------------------------------------------*/
static int store_incoming_data(BP_FlowCtrlEntry_t *flow, uint32_t *flags, int max_cycles)
{
    int            cycles     = 0;
    bool           read_data  = true;
    bool           store_data = false;
    CFE_Status_t   Status;
    CFE_SB_MsgId_t MsgId;

    while (read_data && (cycles < max_cycles))
    {
        /* Read Next Data */
        if (flow->CurrentChunkInSize == 0)
        {
            flow->CurrentChunkInPtr = NULL;
            Status                  = CFE_SB_ReceiveBuffer(&flow->CurrentSbMsgInPtr, flow->DataPipe, CFE_SB_POLL);
            if (Status == CFE_SUCCESS)
            {
                Status = CFE_MSG_GetSize(&flow->CurrentSbMsgInPtr->Msg, &flow->CurrentChunkInSize);
            }
            if (Status == CFE_SUCCESS)
            {
                Status = CFE_MSG_GetMsgId(&flow->CurrentSbMsgInPtr->Msg, &MsgId);
            }

            if (Status == CFE_SUCCESS)
            {
                if (flow->CurrentChunkInSize > BP_MAX_PACKET_SIZE)
                {
                    /* Msg is too big to be accepted into BP */
                    flow->DataInDropped++;
                    CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Flow %s - attempted to store packet that was too large: %lu", flow->Config.Name,
                                      (unsigned long)flow->CurrentChunkInSize);

                    Status                   = CFE_STATUS_WRONG_MSG_LENGTH;
                    flow->CurrentChunkInSize = 0;
                }
                else
                {
                    /* Assign Buffer */
                    flow->CurrentChunkInPtr = (const unsigned char *)flow->CurrentSbMsgInPtr;

                    /* Strip Header */
                    if (BP_STRIP_HDR_BYTES)
                    {
                        flow->CurrentChunkInSize -= sizeof(CFE_MSG_TelemetryHeader_t);
                        flow->CurrentChunkInPtr += sizeof(CFE_MSG_TelemetryHeader_t);
                        store_data = true; /* when stripping header, force one payload per bundle */
                    }

                    /* Throttle Incoming Data */
                    accumulate_throttling(MsgId);
                }
            }
            else
            {
                /* Stop Reading Data and Store Current Data In */
                read_data  = false;
                store_data = true;

                /* Generate Event Message if Software Bus Failure */
                if (Status != CFE_SB_NO_MESSAGE)
                {
                    flow->DataInDropped++;
                    CFE_EVS_SendEvent(BP_IO_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Failed to read data. Status = %x", (unsigned int)Status);
                }
            }
        }

        /* Buffer Data */
        if (flow->CurrentChunkInSize != 0)
        {
            if ((flow->CurrentChunkInSize + flow->CurrentDataAccumInSize) <= BP_MAX_BUNDLE_SIZE)
            {
                /* Copy Current Packet In into Current Data In */
                memcpy(&flow->CurrentDataAccumInBuf[flow->CurrentDataAccumInSize], flow->CurrentChunkInPtr,
                       flow->CurrentChunkInSize);
                flow->CurrentDataAccumInSize += flow->CurrentChunkInSize;
                flow->CurrentChunkInSize = 0;
                cycles++; /* count cycle */
            }
            else
            {
                /* Store Current Data In as Bundle (makes room for current packet) */
                store_data = true;
            }
        }

        /* Store Data */
        if (store_data && (flow->CurrentDataAccumInSize > 0))
        {
            int lib_status =
                bplib_store(flow->BPC, flow->CurrentDataAccumInBuf, flow->CurrentDataAccumInSize, BP_CHECK, flags);
            if (lib_status == BP_SUCCESS)
            {
                /* Successfully Stored Data */
                flow->CurrentDataAccumInSize = 0;
                store_data                   = false;
            }
            else
            {
                /* Stop Reading Data */
                read_data = false;

                /* Handle Failure to Store Data */
                if (lib_status != BP_TIMEOUT)
                {
                    flow->CurrentDataAccumInSize = 0;
                    flow->DataInDropped++;
                    CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Flow %s - Failed (%d) to store data [%08X]", flow->Config.Name, (int)lib_status,
                                      (unsigned int)*flags);
                }
            }
        }
    }

    return cycles;
}

/*-----------------------------------------------
 * load_outgoing_bundles
 *-----------------------------------------------*/
static int load_outgoing_bundles(BP_FlowCtrlEntry_t *flow, uint32_t *flags, int max_cycles)
{
    int cycles = 0;

    while (cycles < max_cycles)
    {
        /* Load next bundle */
        if (flow->CurrentBundleOutSize == 0)
        {
            int status =
                bplib_load(flow->BPC, &flow->CurrentBundleOutPtr, &flow->CurrentBundleOutSize, BP_CHECK, flags);
            if (status == BP_TIMEOUT)
            {
                break;
            }
            else if (status != BP_SUCCESS)
            {
                CFE_EVS_SendEvent(BP_LIB_LOAD_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to load bundle [%08X]", flow->Config.Name, status,
                                  (unsigned int)*flags);
                break;
            }
        }

        /* Write Bundle */
        if (flow->CurrentBundleOutSize > 0)
        {
            if (BP_IOWriteBundle(flow->IOD, flow->CurrentBundleOutPtr, flow->CurrentBundleOutSize))
            {
                /* Acknowledge bundle */
                bplib_ackbundle(flow->BPC, flow->CurrentBundleOutPtr);
                flow->CurrentBundleOutSize = 0;
                cycles++; /*count cycle */
            }
            else
            {
                /* Unable to Send */
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
    int          cycles = 0;
    size_t       OutMsgLen;
    CFE_Status_t Status;

    while (cycles < max_cycles)
    {
        /* Accept next data */
        if (flow->CurrentChunkOutSize == 0)
        {
            int status = bplib_accept(flow->BPC, (void **)&flow->CurrentChunkOutPtr, &flow->CurrentChunkOutSize,
                                      BP_CHECK, flags);
            if (status == BP_TIMEOUT)
            {
                break;
            }
            else if (status != BP_SUCCESS)
            {
                CFE_EVS_SendEvent(BP_LIB_ACCEPT_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Flow %s - Failed (%d) to accept data [%08X]", flow->Config.Name, status,
                                  (unsigned int)*flags);
                break;
            }
        }

        /* Write next data */
        if (flow->CurrentChunkOutSize != 0)
        {
            flow->CurrentSbMsgOutPtr = CFE_SB_AllocateMessageBuffer(flow->CurrentChunkOutSize);
            if (flow->CurrentSbMsgOutPtr != NULL)
            {
                memcpy(flow->CurrentSbMsgOutPtr, flow->CurrentChunkOutPtr, flow->CurrentChunkOutSize);
                Status = CFE_SUCCESS;
            }
            else
            {
                Status = CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
            }

            if (Status == CFE_SUCCESS)
            {
                /*
                 * Confirm that the conversion back to a SB message was valid.  Currently the only way to
                 * test this is by checking that the embedded size within the message matches the chunk size.
                 */
                Status = CFE_MSG_GetSize(&flow->CurrentSbMsgOutPtr->Msg, &OutMsgLen);
                if (Status == CFE_SUCCESS && OutMsgLen != flow->CurrentChunkOutSize)
                {
                    CFE_EVS_SendEvent(BP_IO_RECEIVE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Data size does not match header size: %lu != %lu", (unsigned long)OutMsgLen,
                                      (unsigned long)flow->CurrentChunkOutSize);

                    Status = CFE_STATUS_WRONG_MSG_LENGTH;
                }
            }

            if (Status == CFE_SUCCESS)
            {
                Status = CFE_SB_TransmitBuffer(flow->CurrentSbMsgOutPtr, false);
            }

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

            /* Always forget the pointer - it should have either been transmitted (and thereby no longer owned) or
             * released */
            flow->CurrentSbMsgOutPtr = NULL;

            /* Always acknowledge payload */
            bplib_ackpayload(flow->BPC, flow->CurrentChunkOutPtr);
            flow->CurrentChunkOutSize = 0;
            cycles++;
        }
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

    /* Initialize BP Library */
    if (bplib_init() != BP_SUCCESS)
    {
        return BP_LIB_INIT_ERR_EID;
    }

    /* Initialize Throttling */
    initialize_throttling();

    BP_GlobalData.LastFlowHandle = CFE_ResourceId_FromInteger(BP_FLOW_HANDLE_BASE);

    /* Register flow table with cFE */
    BP_GlobalData.FlowTableHandle = BP_INVALID_TABLE;
    cfe_status = CFE_TBL_Register(&BP_GlobalData.FlowTableHandle, BP_FLOW_TBL_NAME, sizeof(BP_FlowTbl_t),
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
int32 BP_FlowLoad(const char *flow_table_filename)
{
    int32               cfe_status;
    BP_FlowTbl_t       *StagedConfig;
    BP_FlowTblEntry_t  *StagingEntryPtr;
    BP_FlowCtrlEntry_t *FlowPtr;
    CFE_ResourceId_t    PendingFlowHandle;
    CFE_TBL_Info_t      tbl_info;
    int                 flow_idx, num_flows = 0;

    /* Load Flow Table */
    cfe_status = BP_LoadFlowConfigTable(BP_GlobalData.FlowTableHandle, flow_table_filename, &StagedConfig);
    if (cfe_status != CFE_SUCCESS)
        return cfe_status;

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
int32 BP_FlowEnable(BP_FlowHandle_t fh)
{
    int                 i;
    char                pipe_name[BP_PIPE_NAME_SIZE];
    int32               status = CFE_SUCCESS;
    BP_FlowCtrlEntry_t *FlowPtr;
    bp_route_t          route;
    bp_attr_t           attributes;
    const bp_store_t   *store_p;

    store_p = NULL;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled)
    {
        return CFE_SUCCESS; /* not an error */
    }

    /* Initialize Buffers */
    FlowPtr->CurrentDataAccumInSize = 0;
    FlowPtr->CurrentSbMsgInPtr      = NULL;
    FlowPtr->CurrentSbMsgOutPtr     = NULL;
    FlowPtr->CurrentChunkInPtr      = NULL;
    FlowPtr->CurrentChunkInSize     = 0;
    FlowPtr->CurrentChunkOutPtr     = NULL;
    FlowPtr->CurrentChunkOutSize    = 0;
    FlowPtr->CurrentBundleOutPtr    = NULL;
    FlowPtr->CurrentBundleOutSize   = 0;
    FlowPtr->CurrentBundleInPtr     = NULL;
    FlowPtr->CurrentBundleInSize    = 0;

    /* Locate storage service to use */
    FlowPtr->StorageHandle = BP_StorageService_FindByName(FlowPtr->Config.Store);
    store_p                = BP_StorageService_Get(FlowPtr->StorageHandle);

    /* Open BPLIB channel if a valid storage service exists */
    if (store_p)
    {
        /* Setup Route */
        route = (bp_route_t) {BP_SRC_NODE + CFE_PSP_GetProcessorId() - 1,
                              FlowPtr->Config.SrcServ,
                              FlowPtr->Config.DstNode,
                              FlowPtr->Config.DstServ,
                              0,
                              0};

        /* Initialize Attributes */
        bplib_attrinit(&attributes);
        attributes.lifetime             = FlowPtr->Config.Lifetime;
        attributes.timeout              = FlowPtr->Config.Timeout;
        attributes.request_custody      = attributes.timeout == 0 ? false : true;
        attributes.active_table_size    = FlowPtr->Config.MaxActive;
        attributes.max_fills_per_dacs   = 1;
        attributes.max_gaps_per_dacs    = 1;
        attributes.max_length           = BP_MAX_BUNDLE_SIZE;
        attributes.cid_reuse            = true;
        attributes.retransmit_order     = BP_RETX_OLDEST_BUNDLE;
        attributes.persistent_storage   = true;
        attributes.storage_service_parm = NULL;

        /* Open Channel */
        FlowPtr->BPC = bplib_open(route, *store_p, attributes);
    }

    /* Check status of opening library channel */
    if (store_p == NULL)
    {
        CFE_EVS_SendEvent(BP_LIB_INVALID_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Could not find storage store_index %s", FlowPtr->Config.Store);
        status = BP_LIB_INVALID_STORE_ERR_EID;
    }
    else if (FlowPtr->BPC == NULL)
    {
        CFE_EVS_SendEvent(BP_LIB_OPEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to open library channel for flow %s",
                          FlowPtr->Config.Name);
        status = BP_LIB_OPEN_ERR_EID;
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
                                  "Failed (%d) to subscribe to %04X on data pipe", (int)cfe_status,
                                  CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId));

                /* Mark Flow as Unhealthy */
                FlowPtr->Healthy = false;
            }
        }
    }

    /* Open an IO channel */
    if (status == CFE_SUCCESS)
    {
        status = BP_IOOpen(FlowPtr->Config.IOParm, &FlowPtr->IOD);
        if (status != CFE_SUCCESS)
        {
            FlowPtr->IOD = BP_IO_INVALID_DESCRIPTOR;
            CFE_EVS_SendEvent(BP_IO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Failed (%d) to open IO channel initialized from %s", (int)status,
                              FlowPtr->Config.IOParm);
        }
        else
        {
            /* Success */
            FlowPtr->Active  = false; /* Start Out Paused */
            FlowPtr->Enabled = true;
        }
    }

    /* Clean Up Failed Enable */
    if (status != CFE_SUCCESS)
    {
        /* Mark Flow as Unhealthy */
        FlowPtr->Healthy = false;

        /* Close IO channel */
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->IOD))
        {
            BP_IOClose(FlowPtr->IOD);
            FlowPtr->IOD = BP_IO_INVALID_DESCRIPTOR;
        }

        /* Delete data pipe
         *  if the pipe is a throttle pipe, then this is a case where it will get deleted */
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->DataPipe))
        {
            CFE_SB_DeletePipe(FlowPtr->DataPipe);
            FlowPtr->DataPipe = BP_INVALID_PIPE;
        }

        /* Close BPLIB channel */
        if (FlowPtr->BPC != NULL)
        {
            bplib_close(FlowPtr->BPC);
            FlowPtr->BPC = NULL;
        }
    }

    return status;
}

/*-----------------------------------------------
 * BP_FlowDisable
 *----------------------------------------------*/
int32 BP_FlowDisable(BP_FlowHandle_t fh)
{
    int                 i;
    int32               cfe_status;
    bool                throttle_pipe;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled)
    {
        FlowPtr->Enabled = false;
        FlowPtr->Active  = false;
        FlowPtr->Healthy = true;

        /* Close IO Channel */
        if (CFE_RESOURCEID_TEST_DEFINED(FlowPtr->IOD))
        {
            BP_IOClose(FlowPtr->IOD);
            FlowPtr->IOD = BP_IO_INVALID_DESCRIPTOR;
        }

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
                                      "Throttled stream ID detected (%04X)... preserving data pipe for flow %s",
                                      CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId), FlowPtr->Config.Name);
                    throttle_pipe = true;
                    continue;
                }

                /* Unsubscribe valid entry */
                cfe_status = CFE_SB_Unsubscribe(FlowPtr->Config.PktTbl[i].StreamId, FlowPtr->DataPipe);
                if (cfe_status != CFE_SUCCESS)
                {
                    CFE_EVS_SendEvent(BP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Failed (%d) to unsubscribe %04X from data pipe", (int)cfe_status,
                                      CFE_SB_MsgIdToValue(FlowPtr->Config.PktTbl[i].StreamId));
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
        if (FlowPtr->BPC != NULL)
        {
            /* Flush Channel - this is necessary given that current storage services
             *  are unable to recover bundles that are active at the time the channel
             *  is closed; future enhancements to bplib may support the release of
             *  resources associated with active bundles while still maintaining them
             *  for future recovery */
            bplib_flush(FlowPtr->BPC);

            /* Close Channel */
            bplib_close(FlowPtr->BPC);
            FlowPtr->BPC = NULL;
        }
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowSetTimeout
 *----------------------------------------------*/
int32 BP_FlowSetTimeout(BP_FlowHandle_t fh, int timeout)
{
    int                 optval = timeout;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot configure disabled flow: %s",
                          BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    if (FlowPtr->Config.Timeout == 0 && timeout != 0)
    {
        CFE_EVS_SendEvent(BP_PARM_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot set non-zero timeout on flow %s that does not request custody", BP_FlowGetName(fh));
        return BP_PARM_ERR_EID;
    }

    /* Set Timeout Configuration */
    if (bplib_config(FlowPtr->BPC, BP_OPT_MODE_WRITE, BP_OPT_TIMEOUT, &optval) != BP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_PARM_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Failed to configure bplib timeout of %d on flow %s", timeout, BP_FlowGetName(fh));
        return BP_PARM_ERR_EID;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowRevertTimeout
 *----------------------------------------------*/
int32 BP_FlowRevertTimeout(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    return BP_FlowSetTimeout(fh, FlowPtr->Config.Timeout);
}

/*-----------------------------------------------
 * BP_FlowSetPriority
 *----------------------------------------------*/
int32 BP_FlowSetPriority(BP_FlowHandle_t fh, int priority)
{
    BP_FlowCtrlEntry_t *FlowPtr;
    int                 optval = priority;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot configure disabled flow: %s",
                          BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Set Priority Configuration */
    if (bplib_config(FlowPtr->BPC, BP_OPT_MODE_WRITE, BP_OPT_CLASS_OF_SERVICE, &optval) != BP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_PARM_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Failed to configure bplib priority of %d on flow %s", priority, BP_FlowGetName(fh));
        return BP_PARM_ERR_EID;
    }

    FlowPtr->COS.Priority = optval;
    prioritize_flows(); /* Re-sort Flow Priorities */
    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowRevertPriority
 *----------------------------------------------*/
int32 BP_FlowRevertPriority(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    return BP_FlowSetPriority(fh, FlowPtr->Config.Priority);
}

/*-----------------------------------------------
 * BP_FlowIsEnabled
 *-----------------------------------------------*/
bool BP_FlowIsEnabled(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return false;
    }

    return FlowPtr->Enabled;
}

/*-----------------------------------------------
 * BP_FlowGetStats
 *-----------------------------------------------*/
int32 BP_FlowGetStats(BP_FlowHandle_t fh, BP_FlowStats_t *stat)
{
    int                 optval;
    bp_stats_t          tempstats;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    /* Get stats */
    if (stat)
    {
        memset(stat, 0, sizeof(BP_FlowStats_t));

        /* Initialize to Invalid Status */
        stat->Active                 = BP_INVALID_STATUS;
        stat->Healthy                = BP_INVALID_STATUS;
        stat->IOStats.SendHealthy    = BP_INVALID_STATUS;
        stat->IOStats.ReceiveHealthy = BP_INVALID_STATUS;

        /* Get Flow Information */
        strncpy(stat->Name, FlowPtr->Config.Name, BP_FLOW_NAME_SIZE);
        stat->Enabled  = FlowPtr->Enabled;
        stat->Timeout  = FlowPtr->Config.Timeout;
        stat->Priority = FlowPtr->COS.Priority;

        /* Get Enabled-Only Status */
        if (stat->Enabled)
        {
            /* Get Flow Status */
            stat->Healthy        = FlowPtr->Healthy;
            stat->DataInDropped  = FlowPtr->DataInDropped;
            stat->DataOutDropped = FlowPtr->DataOutDropped;
            stat->LibFlags       = FlowPtr->LibFlags;

            /* Get IO Module Status */
            stat->Active = FlowPtr->Active;
            BP_IOGetStats(FlowPtr->IOD, &stat->IOStats);

            /* Get Library Status */
            bplib_latchstats(FlowPtr->BPC, &tempstats);
            stat->LibStats.lost                  = tempstats.lost;
            stat->LibStats.expired               = tempstats.expired;
            stat->LibStats.unrecognized          = tempstats.unrecognized;
            stat->LibStats.transmitted_bundles   = tempstats.transmitted_bundles;
            stat->LibStats.transmitted_dacs      = tempstats.transmitted_dacs;
            stat->LibStats.retransmitted_bundles = tempstats.retransmitted_bundles;
            stat->LibStats.delivered_payloads    = tempstats.delivered_payloads;
            stat->LibStats.received_bundles      = tempstats.received_bundles;
            stat->LibStats.forwarded_bundles     = tempstats.forwarded_bundles;
            stat->LibStats.received_dacs         = tempstats.received_dacs;
            stat->LibStats.stored_bundles        = tempstats.stored_bundles;
            stat->LibStats.stored_payloads       = tempstats.stored_payloads;
            stat->LibStats.stored_dacs           = tempstats.stored_dacs;
            stat->LibStats.acknowledged_bundles  = tempstats.acknowledged_bundles;
            stat->LibStats.active_bundles        = tempstats.active_bundles;

            bplib_config(FlowPtr->BPC, BP_OPT_MODE_READ, BP_OPT_TIMEOUT, &optval);
            stat->Timeout = optval; /* overrides the table value set above */
        }
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowClearStats
 *-----------------------------------------------*/
int32 BP_FlowClearStats(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
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

    /* Clear IO stats */
    BP_IOClearStats(FlowPtr->IOD);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowGetHandle
 *-----------------------------------------------*/
BP_FlowHandle_t BP_FlowGetHandle(const char *name)
{
    int                 f;
    BP_FlowCtrlEntry_t *CheckPtr;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = NULL;
    for (f = 0; f < BP_MAX_FLOWS; f++)
    {
        CheckPtr = &BP_GlobalData.FlowControl[f];

        if (CFE_RESOURCEID_TEST_DEFINED(CheckPtr->Handle) &&
            strncmp(CheckPtr->Config.Name, name, BP_FLOW_NAME_SIZE) == 0)
        {
            FlowPtr = CheckPtr;
            break;
        }
    }

    if (FlowPtr == NULL)
    {
        CFE_EVS_SendEvent(BP_INVALID_FLOW_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid flow: %s", name);
        return BP_INVALID_FLOW;
    }

    return FlowPtr->Handle;
}

/*-----------------------------------------------
 * BP_FlowGetName
 *-----------------------------------------------*/
const char *BP_FlowGetName(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        /* do not return NULL, as this might be used in printf()-style call */
        return "INVALID";
    }

    return FlowPtr->Config.Name;
}

/*-----------------------------------------------
 * BP_FlowFlush
 *-----------------------------------------------*/
int32 BP_FlowFlush(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot flush disabled flow: %s",
                          BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Reset incoming data */
    if (FlowPtr->CurrentChunkOutSize > 0)
    {
        bplib_ackpayload(FlowPtr->BPC, FlowPtr->CurrentChunkOutPtr);
        FlowPtr->CurrentChunkOutSize = 0;
    }

    /* Reset outgoing bundle */
    if (FlowPtr->CurrentBundleOutSize > 0)
    {
        bplib_ackbundle(FlowPtr->BPC, FlowPtr->CurrentBundleOutPtr);
        FlowPtr->CurrentBundleOutSize = 0;
    }

    /* Flush I/O */
    BP_IOFlush(FlowPtr->IOD);

    /* Flush bplib channel */
    bplib_flush(FlowPtr->BPC);

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowPause
 *-----------------------------------------------*/
int32 BP_FlowPause(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot pause disabled flow: %s",
                          BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Pause flow */
    FlowPtr->Active = false;

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowResume
 *-----------------------------------------------*/
int32 BP_FlowResume(BP_FlowHandle_t fh)
{
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }
    else if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR, "Cannot resume disabled flow: %s",
                          BP_FlowGetName(fh));
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
    int cycles = 0; /* indicates output activity to detemine if fill needed */

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
                /* Perform Ingress Bundle Processing */
                process_incoming_bundles(FlowPtr, &FlowPtr->LibFlags, BP_IO_READ_LIMIT);
                store_incoming_data(FlowPtr, &FlowPtr->LibFlags, BP_APP_READ_LIMIT);

                /* Perform Egress Bundle Processing */
                if (FlowPtr->Active)
                {
                    cycles += load_outgoing_bundles(FlowPtr, &FlowPtr->LibFlags, BP_IO_WRITE_LIMIT);
                    accept_outgoing_data(FlowPtr, &FlowPtr->LibFlags, BP_APP_WRITE_LIMIT);
                }
            }

            /* Goto Next Flow */
            PrevFlowPtr = FlowPtr;
            FlowPtr     = BP_LocateFlowEntryByHandle(FlowPtr->COS.NextFlow);
        }

        /* Rotate Flows in Level */
        if (PrevFlowPtr != LevelPtr)
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

    /* Flush I/O Layer
     *      1. this is only performed if no output activity detected for this wakeup period
     *      2. this allows I/O layer to generate fill if necessary to push out frames
     *      3. the invalid descriptor is used to flush I/O */
    if (cycles == 0)
    {
        BP_IOFlush(BP_IO_INVALID_DESCRIPTOR);
    }

    /* Perform Throttling */
    disperse_throttling();

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowDirectStore
 *-----------------------------------------------*/
int32 BP_FlowDirectStore(BP_FlowHandle_t fh, uint8 *buffer, int len)
{
    int                 lib_status;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        FlowPtr->DataInDropped++;
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot directly store data to disabled flow: %s", BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Store data */
    lib_status = bplib_store(FlowPtr->BPC, buffer, len, BP_CHECK, &FlowPtr->LibFlags);
    if (lib_status != BP_SUCCESS)
    {
        FlowPtr->DataInDropped++;
        CFE_EVS_SendEvent(BP_LIB_STORE_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Flow %s - Failed (%d) to directly store data [%08X]", BP_FlowGetName(fh), (int)lib_status,
                          (unsigned int)FlowPtr->LibFlags);
        return BP_LIB_LOAD_ERR_EID;
    }

    return CFE_SUCCESS;
}

/*-----------------------------------------------
 * BP_FlowDirectConfig
 *-----------------------------------------------*/
int32 BP_FlowDirectConfig(BP_FlowHandle_t fh, int mode, int opt, int *val)
{
    int                 lib_status;
    BP_FlowCtrlEntry_t *FlowPtr;

    FlowPtr = BP_LocateFlowEntryByHandle(fh);

    /* Check flow */
    if (!BP_FlowEntryIsMatch(FlowPtr, fh))
    {
        return BP_INVALID_FLOW_ERR_EID;
    }

    if (val == NULL)
    {
        return BP_PARM_ERR_EID;
    }

    if (FlowPtr->Enabled == false)
    {
        CFE_EVS_SendEvent(BP_FLOW_DISABLED_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Cannot directly configure a disabled flow: %s", BP_FlowGetName(fh));
        return BP_FLOW_DISABLED_ERR_EID;
    }

    /* Configure channel */
    lib_status = bplib_config(FlowPtr->BPC, mode, opt, val);
    if (lib_status != BP_SUCCESS)
    {
        CFE_EVS_SendEvent(BP_LIB_CONFIG_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Flow %s - Failed (%d) to directly configure channel", BP_FlowGetName(fh), (int)lib_status);
        return BP_LIB_CONFIG_ERR_EID;
    }

    return CFE_SUCCESS;
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
    BP_FlowHandle_t fh;
    uint32          pos;

    for (pos = 0; pos < BP_MAX_FLOWS; ++pos)
    {
        fh = BP_GlobalData.FlowControl[pos].Handle;

        if (CFE_RESOURCEID_TEST_DEFINED(fh))
        {
            Func(fh, Arg);
        }
    }
}
