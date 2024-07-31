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
 * @file
 *   This file contains the prototypes for the BPNode utility functions
 */

#ifndef BPNODE_UTILS_H
#define BPNODE_UTILS_H

/*
** Include Files
*/

#include "bpnode_app.h"


/*
** Exported Functions
*/

/**
 * \brief Validate Example Table
 *
 *  \par Description
 *       Validates example table data
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] TblData Table data to validate
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *  \retval #BPNODE_TABLE_OUT_OF_RANGE_ERR_CODE Validation failure
 */
CFE_Status_t BPNode_TblValidationFunc(void *TblData);

#endif /* BPNODE_UTILS_H */
