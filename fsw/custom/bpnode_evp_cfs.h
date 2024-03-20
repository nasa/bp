/*
 * TODO: Fill in file header, if necessary.
 */

#ifndef BPNODE_EVP_CFS_H
#define BPNODE_EVP_CFS_H

/************************************************
 * Includes
 ************************************************/

#include "cfe.h"
#include "bpl_evm_api.h"

/************************************************
 * Typedefs
 ************************************************/

/*
** Defines the character used when truncating event strings that are too long
** Match what cFE uses (CFE_EVS_MSG_TRUNCATED)
*/
#define BPNODE_EVP_MSG_TRUNCATED ('$')

/*
** Defines the max length of the expanded event string
**
** Limits:
** - must be greater than or equal to 2
** - must be less than or equal to host limit (CFE_MISSION_EVS_MAX_MESSAGE_LENGTH)
*/
#define BPNODE_EVP_MAX_MESSAGE_LENGTH (CFE_MISSION_EVS_MAX_MESSAGE_LENGTH)

/************************************************
 * Exported Functions
 ************************************************/

BPL_Status_t BPNODE_EVP_Initialize_Impl(void);
BPL_Status_t BPNODE_EVP_SendEvent_Impl(uint16_t EventID, BPL_EVM_EventType_t EventType,
    char const * EventText, va_list EventTextArgPtr);

#endif /* BPNODE_EVP_CFS_H */
