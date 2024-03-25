/*
 * TODO: Fill in file header, if necessary.
 */

/************************************************
 * Includes
 ************************************************/

#include <assert.h>
#include "cfe.h"
#include "bp_eventids.h"
#include "bpl_evm_api.h"
#include "bpnode_evp_cfs.h"

/************************************************
 * Local Data
 ************************************************/

/*
** Limits:
** - Must contain less than or equal to CFE_PLATFORM_EVS_MAX_EVENT_FILTERS entries
*/
static const CFE_EVS_BinFilter_t BPNODE_EVP_CFS_EVENT_FILTER[] = {
    {BP_IO_SEND_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_IO_RECEIVE_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_LIB_PROC_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_LIB_LOAD_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_LIB_STORE_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_LIB_ACCEPT_ERR_EID, CFE_EVS_FIRST_8_STOP},
    {BP_FILE_ERR_EID, CFE_EVS_FIRST_8_STOP},
};

const size_t BPNODE_EVP_CFS_NUM_EVENT_FILTERS = sizeof(BPNODE_EVP_CFS_EVENT_FILTER) /
                                                sizeof(BPNODE_EVP_CFS_EVENT_FILTER[0]);

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BPNODE_EVP_Initialize_Impl
 *-----------------------------------------------*/
BPL_Status_t BPNODE_EVP_Initialize_Impl(void)
{
    BPL_Status_t ReturnStatus;
    CFE_Status_t CfeEvsRegisterStatus;

    CfeEvsRegisterStatus = CFE_EVS_Register(BPNODE_EVP_CFS_EVENT_FILTER,
                                            BPNODE_EVP_CFS_NUM_EVENT_FILTERS,
                                            CFE_EVS_EventFilter_BINARY);

    if (CfeEvsRegisterStatus != CFE_SUCCESS)
    {
        OS_printf("BPNODE_EVP_Initialize_Impl call to CFE_EVS_Register returned error: %u!\n",
            CfeEvsRegisterStatus);
        ReturnStatus.ReturnValue = BPL_STATUS_ERROR_PROXY_INIT;
    }
    else
    {
        ReturnStatus.ReturnValue = BPL_STATUS_SUCCESS;
    }

    return ReturnStatus;
}

/*-----------------------------------------------
 * BPNODE_EVP_TranslateTypeToHost
 *-----------------------------------------------*/
uint16_t BPNODE_EVP_TranslateTypeToHost(BPL_EVM_EventType_t EventType)
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
 * BPNODE_EVP_SendEvent_Impl
 *-----------------------------------------------*/
BPL_Status_t BPNODE_EVP_SendEvent_Impl(uint16_t EventID, BPL_EVM_EventType_t EventType,
    char const * EventText, va_list EventTextArgPtr)
{
    BPL_Status_t ReturnStatus;
    CFE_Status_t ProxyStatus;
    uint16_t HostEventType = BPNODE_EVP_TranslateTypeToHost(EventType);
    char ExpandedEventText[BPNODE_EVP_MAX_MESSAGE_LENGTH];
    int ExpandedLength;

    /*
    ** Due to how we truncate the message if its too long (as seen in code, below),
    ** we need to ensure that this buffer is at least 2 characters long.
    */
    assert(BPNODE_EVP_MAX_MESSAGE_LENGTH >= 2);
    assert(BPNODE_EVP_MAX_MESSAGE_LENGTH <= CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    memset(&ExpandedEventText, 0, sizeof(ExpandedEventText));
    ExpandedLength = vsnprintf((char *)ExpandedEventText, sizeof(ExpandedEventText),
        EventText, EventTextArgPtr);
    if (ExpandedLength >= (int)sizeof(ExpandedEventText))
    {
        /* Mark character before zero terminator to indicate truncation */
        ExpandedEventText[sizeof(ExpandedEventText) - 2u] = BPNODE_EVP_MSG_TRUNCATED;
        /*
        ** TODO: should we return an error here?
        ** Note: In the cFE implementation, they don't treat message truncation as an error.
        */
    }

    /* TODO: We'll probably want to remove this, or wrap it behind an "if debug" compiler flag. */
    OS_printf("BPNODE_EVP_SendEvent_Impl(%u, %s, %s)\n",
        EventID, BPL_EVM_EventTypeToString(EventType), ExpandedEventText);

    ProxyStatus = CFE_EVS_SendEvent(EventID, HostEventType, "%s", ExpandedEventText);

    if (ProxyStatus != CFE_SUCCESS)
    {
        OS_printf("BPNODE_EVP_SendEvent_Impl CFE_EVS_SendEvent returned error status: 0x%08X!\n",
            ProxyStatus);
        ReturnStatus.ReturnValue = BPL_STATUS_ERROR_GENERAL;
    }
    else
    {
        ReturnStatus.ReturnValue = BPL_STATUS_SUCCESS;
    }

    return ReturnStatus;
}
