/*
 * TODO: Fill in file header, if necessary.
 */

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bpnode_evt_cfs.h"

/************************************************
 * Exported Functions
 ************************************************/

/*-----------------------------------------------
 * BPNODE_EVT_Initialize_Impl
 *-----------------------------------------------*/
BPNODE_Status_t BPNODE_EVT_Initialize_Impl(void)
{
    BPNODE_Status_t ReturnStatus = { .ReturnValue = 0 };
    OS_printf("BPNODE_EVT_Initialize_Impl called!\n");
    return ReturnStatus;
}

/*-----------------------------------------------
 * BPNODE_EVT_SendEvent_Impl
 *-----------------------------------------------*/
BPNODE_Status_t BPNODE_EVT_SendEvent_Impl(uint32_t EventID)
{
    BPNODE_Status_t ReturnStatus = { .ReturnValue = 0 };
    OS_printf("BPNODE_EVT_SendEvent_Impl called with arg 0x%08X!\n", EventID);
    return ReturnStatus;
}
