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
 *   This file contains the source code for the FWP ADU Proxy.
 */

/*
** Include Files
*/

#include "cfe.h"
#include "fwp_adup.h"


/*
** Function Definitions
*/

/* Ingest an ADU */
BPLib_Status_t BPA_ADUP_In(void *AduPtr)
{
    return BPLIB_SUCCESS;
}

/* Send out an ADU */
BPLib_Status_t BPA_ADUP_Out(void *AduPtr)
{
    return BPLIB_SUCCESS;
}

/* Add a new application's configurations */
void BPA_ADUP_AddApplication(BPA_ADUP_Configs_t *AppConfigs)
{
    return;
}

/* Start an application */
void BPA_ADUP_StartApplication(uint8_t AppId)
{
    return;
}

/* Stop an application */
void BPA_ADUP_StopApplication(uint8_t AppId)
{
    return;
}