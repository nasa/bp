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

#include "bpnode_app.h"
#include "bpnode_eventids.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"


/*
** Function Definitions
*/

/* Validate example table data */
CFE_Status_t BPNode_TblValidationFunc(void *TblData)
{
    CFE_Status_t           ReturnCode = CFE_SUCCESS;
    BPNode_ExampleTable_t *TblDataPtr = (BPNode_ExampleTable_t *)TblData;

    /* Validate data values are within allowed range */
    if (TblDataPtr->Int1 > BPNODE_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range, return an appropriate error code */
        ReturnCode = BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}
