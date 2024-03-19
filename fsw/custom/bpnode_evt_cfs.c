/*
 * TODO: Fill in file header, if necessary.
 */

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bpl_evm_api.h"
#include "bpnode_evt_cfs.h"

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BPNODE_EVT_Initialize_Impl
 *-----------------------------------------------*/
BPL_Status_t BPNODE_EVT_Initialize_Impl(void)
{
    BPL_Status_t ReturnStatus = { .ReturnValue = BPL_STATUS_SUCCESS };
    OS_printf("BPNODE_EVT_Initialize_Impl called!\n");
    return ReturnStatus;
}

/*-----------------------------------------------
 * BPNODE_EVT_TranslateTypeToHost
 *-----------------------------------------------*/
uint16_t BPNODE_EVT_TranslateTypeToHost(BPL_EVM_EventType_t EventType)
{
    uint16_t HostEventType;
    switch (EventType)
    {
        case BPL_EVM_EventType_DEBUG:
            HostEventType = CFE_EVS_EventType_DEBUG;
            break;
        case BPL_EVM_EventType_INFO:
            HostEventType = CFE_EVS_EventType_INFORMATION;
            break;
        case BPL_EVM_EventType_WARNING:
            HostEventType = CFE_EVS_EventType_ERROR;
            break;
        case BPL_EVM_EventType_ERROR:
            HostEventType = CFE_EVS_EventType_ERROR;
            break;
        case BPL_EVM_EventType_CRITICAL:
            HostEventType = CFE_EVS_EventType_CRITICAL;
            break;
        default:
            /* This default case also captures the BPL_EVM_EventType_UNKNOWN case. */
            HostEventType = CFE_EVS_EventType_ERROR;
            break;
    }
    return HostEventType;
}

/*-----------------------------------------------
 * BPNODE_EVT_SendEvent_Impl
 *-----------------------------------------------*/
BPL_Status_t BPNODE_EVT_SendEvent_Impl(uint16_t EventID, BPL_EVM_EventType_t EventType)
{
    BPL_Status_t ReturnStatus;
    CFE_Status_t ProxyStatus;
    uint16_t HostEventType = BPNODE_EVT_TranslateTypeToHost(EventType);

    ProxyStatus = CFE_EVS_SendEvent(EventID, HostEventType,
                    "TODO: Replace this static string!");

    if (ProxyStatus != CFE_SUCCESS)
    {
        OS_printf("BPNODE_EVT_SendEvent_Impl CFE_EVS_SendEvent returned error status: 0x%08X!\n",
            ProxyStatus);
        ReturnStatus.ReturnValue = BPL_STATUS_ERROR_GENERAL;
    }
    else
    {
        OS_printf("BPNODE_EVT_SendEvent_Impl called with ID 0x%08X and Type %u!\n",
            EventID, (unsigned int) EventType);
        ReturnStatus.ReturnValue = BPL_STATUS_SUCCESS;
    }

    return ReturnStatus;
}
