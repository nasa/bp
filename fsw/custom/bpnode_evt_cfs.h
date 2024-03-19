/*
 * TODO: Fill in file header, if necessary.
 */

#ifndef BPNODE_EVT_H
#define BPNODE_EVT_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bpl_evm_api.h"

/************************************************
 * Typedefs
 ************************************************/

/************************************************
 * Exported Functions
 ************************************************/

BPL_Status_t BPNODE_EVT_Initialize_Impl(void);
BPL_Status_t BPNODE_EVT_SendEvent_Impl(uint32_t EventID);

#endif /* BPNODE_EVT_H */
