/*
 * TODO: Fill in file header, if necessary.
 */

#ifndef BPNODE_EVT_H
#define BPNODE_EVT_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"

/************************************************
 * Typedefs
 ************************************************/

typedef struct
{
    uint32_t ReturnValue;
} BPNODE_Status_t;

/************************************************
 * Exported Functions
 ************************************************/

BPNODE_Status_t BPNODE_EVT_Initialize_Impl(void);
BPNODE_Status_t BPNODE_EVT_SendEvent_Impl(uint32_t EventID);

#endif /* BPNODE_EVT_H */
