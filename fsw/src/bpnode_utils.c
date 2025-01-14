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

/**
 * \file
 *   This file contains the source code for the BPNode utility functions
 */

/*
** Include Files
*/

#include "bpnode_utils.h"


/*
** Function Definitions
*/

/* Validate example table data */
CFE_Status_t BPNode_TblValidationFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;

    return ReturnCode;
}

BPLib_Status_t BPNode_CFE_Status_To_BPLib_Status(CFE_Status_t CFE_Status) 
{
    BPLib_Status_t BPLib_Status;

    if (CFE_Status & CFE_SEVERITY_SUCCESS)
    { /* cFE success-type return code */
        BPLib_Status = BPLIB_SUCCESS;
    }
    else if (CFE_Status & CFE_SEVERITY_ERROR)
    { /* cFE error-type return code */
        BPLib_Status = BPLIB_ERROR;
    }
    else
    { /* cFE info-type return code */
        switch (CFE_Status)
        {
            case CFE_TBL_INFO_UPDATE_PENDING:
            case CFE_TBL_WARN_SHORT_FILE:
            case CFE_TBL_INFO_UPDATED:
            case CFE_TBL_INFO_VALIDATION_PENDING:
            case CFE_TBL_WARN_PARTIAL_LOAD:
            case CFE_TBL_INFO_DUMP_PENDING:
            case CFE_TBL_INFO_RECOVERED_TBL:
                BPLib_Status = BPLIB_SUCCESS;
                break;
            case CFE_STATUS_NO_COUNTER_INCREMENT:
            case CFE_ES_CDS_ALREADY_EXISTS:
            case CFE_ES_LIB_ALREADY_LOADED:
            case CFE_ES_ERR_SYS_LOG_TRUNCATED:
            case CFE_TBL_WARN_DUPLICATE:
            case CFE_TBL_INFO_NO_UPDATE_PENDING:
            case CFE_TBL_INFO_TABLE_LOCKED:
            case CFE_TBL_INFO_NO_VALIDATION_PENDING:
            case CFE_TBL_WARN_NOT_CRITICAL:
                BPLib_Status = BPLIB_ERROR;
                break;
            default:
                BPLib_Status = BPLIB_UNKNOWN;
                break;
        }
    }

    return BPLib_Status;
}

CFE_Status_t BPNode_BPLib_Status_To_CFE_Status(BPLib_Status_t BPLib_Status)
{
    CFE_Status_t CFE_Status;

    CFE_Status = CFE_SUCCESS;

    return CFE_Status;
}