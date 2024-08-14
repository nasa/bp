/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
** @file
**   FWP's Table Proxy (TABLEP) header definitions
*/

#ifndef FWP_TABLEP_H
#define FWP_TABLEP_H

#include "cfe.h"
#include "bpnode_eventids.h"
#include "bpnode_tbl.h"

#define NUM_OF_TABLES 12
CFE_Status_t BPA_TableP_TableInit(void);
CFE_Status_t BPA_TableP_TableUpdate(void);
CFE_Status_t BPA_TableP_SingleTableUpdate(CFE_TBL_Handle_t TblHandle);


#endif /* FWP_TABLEP_H */

