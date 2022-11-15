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

#ifndef BP_APP_H
#define BP_APP_H

#include "cfe_error.h"
#include "bp_msg.h"

/*-----------------------------------------------
 * Command Handlers
 *-----------------------------------------------*/
CFE_Status_t BP_NoopCmd(const BP_NoopCmd_t *cmd);
CFE_Status_t BP_ResetAppCmd(const BP_ResetAppCmd_t *cmd);
CFE_Status_t BP_ReloadFlowTableCmd(const BP_ReloadFlowTableCmd_t *cmd);
CFE_Status_t BP_EnableFlowCmd(const BP_EnableFlowCmd_t *cmd);
CFE_Status_t BP_DisableFlowCmd(const BP_DisableFlowCmd_t *cmd);
CFE_Status_t BP_FlushFlowCmd(const BP_FlushFlowCmd_t *cmd);
CFE_Status_t BP_PauseFlowCmd(const BP_PauseFlowCmd_t *cmd);
CFE_Status_t BP_ResumeFlowCmd(const BP_ResumeFlowCmd_t *cmd);
CFE_Status_t BP_OverrideTimeoutCmd(const BP_OverrideTimeoutCmd_t *cmd);
CFE_Status_t BP_DisableOverrideTimeoutCmd(const BP_DisableOverrideTimeoutCmd_t *cmd);
CFE_Status_t BP_OverridePriorityCmd(const BP_OverridePriorityCmd_t *cmd);
CFE_Status_t BP_DisableOverridePriorityCmd(const BP_DisableOverridePriorityCmd_t *cmd);

CFE_Status_t BP_SendAppTlmCmd(const BP_SendAppTlmCmd_t *cmd);
CFE_Status_t BP_SendEnabledFlowTlmCmd(const BP_SendEnabledFlowTlmCmd_t *cmd);
CFE_Status_t BP_SendDisabledFlowTlmCmd(const BP_SendDisabledFlowTlmCmd_t *cmd);

CFE_Status_t BP_ProcessWakeupCmd(const CFE_MSG_CommandHeader_t *cmd);

CFE_Status_t BP_ProcessPkt(const CFE_SB_Buffer_t *MessagePtr);

/************************************************
 * Exported Functions
 ************************************************/

void BP_AppMain(void);

#endif /* BP_APP_H */
