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
 * BPNODE_EVT_SendEvent_Impl
 *-----------------------------------------------*/
BPL_Status_t BPNODE_EVT_SendEvent_Impl(uint16_t EventID)
{
    BPL_Status_t ReturnStatus = { .ReturnValue = BPL_STATUS_SUCCESS };
    OS_printf("BPNODE_EVT_SendEvent_Impl called with arg 0x%08X!\n", EventID);

    CFE_EVS_SendEvent(EventID, CFE_EVS_EventType_INFORMATION,
                    "TODO: Replace this static string!");

    return ReturnStatus;
}
