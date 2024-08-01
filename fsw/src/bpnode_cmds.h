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
 *   This file contains the prototypes for the BPNode command-handling functions
 */

#ifndef BPNODE_CMDS_H
#define BPNODE_CMDS_H


/*
** Include Files
*/

#include "cfe_error.h"
#include "bpnode_msg.h"


/*
** Exported Functions
*/

/**
 * \brief Noop command
 *
 *  \par Description
 *       This function processes a noop command.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_NoopCmd(const BPNode_NoopCmd_t *Msg);

/**
 * \brief Add all applications command
 *
 *  \par Description
 *       This function adds all defined applications.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddAllApplicationsCmd(const BPNode_AddAllApplicationsCmd_t *Msg);

/**
 * \brief Start all applications command
 *
 *  \par Description
 *       This function starts all applications
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_StartAllApplicationsCmd(const BPNode_StartAllApplicationsCmd_t *Msg);

/**
 * \brief Verify bundle storage command
 *
 *  \par Description
 *       This function verifies bundle storage.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_VerifyBundleStorageCmd(const BPNode_VerifyBundleStorageCmd_t *Msg);

/**
 * \brief Initialize bundle storage command
 *
 *  \par Description
 *       This function initializes bundle storage.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_InitBundleStorageCmd(const BPNode_InitBundleStorageCmd_t *Msg);

/**
 * \brief Verify bundle metadata (7.2) command
 *
 *  \par Description
 *       This function verifies stored bundle metadata.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_VerifyBundleMetadataCmd(const BPNode_VerifyBundleMetadataCmd_t *Msg);

/**
 * \brief Rebuild bundle metadata command
 *
 *  \par Description
 *       This function rebuilds the cache from stored bundle metadata.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RebuildBundleMetadataCmd(const BPNode_RebuildBundleMetadataCmd_t *Msg);

/**
 * \brief Clear volatile command
 *
 *  \par Description
 *       This function clears the cache.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ClearVolatileCmd(const BPNode_ClearVolatileCmd_t *Msg);

/**
 * \brief Reload saved data (7.2) command
 *
 *  \par Description
 *       This function reloads data saved by modules.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ReloadSavedDataCmd(const BPNode_ReloadSavedDataCmd_t *Msg);

/**
 * \brief Reset all counters command
 *
 *  \par Description
 *       This function resets all node counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ResetAllCountersCmd(const BPNode_ResetAllCountersCmd_t *Msg);

/**
 * \brief Reset counter command
 *
 *  \par Description
 *       This function resets an individual counter.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg);

/**
 * \brief Reset source counters command
 *
 *  \par Description
 *       This function resets all counters indexed by source eid.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg);

/**
 * \brief Reset bundle counters command
 *
 *  \par Description
 *       This function resets all bundle counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ResetBundleCountersCmd(const BPNode_ResetBundleCountersCmd_t *Msg);

/**
 * \brief Reset error counters command
 *
 *  \par Description
 *       This function resets all error counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ResetErrorCountersCmd(const BPNode_ResetErrorCountersCmd_t *Msg);

/**
 * \brief Add application command
 *
 *  \par Description
 *       This function adds an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg);

/**
 * \brief Remove application command
 *
 *  \par Description
 *       This function removes an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg);

/**
 * \brief Set registration state command
 *
 *  \par Description
 *       This function sets an application's registration state.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg);

/**
 * \brief Start application command
 *
 *  \par Description
 *       This function starts an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg);

/**
 * \brief Stop application command
 *
 *  \par Description
 *       This function stops an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg);

/**
 * \brief Add authorized sources command
 *
 *  \par Description
 *       This function adds one or more authorized source eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg);

/**
 * \brief Remove authorized sources command
 *
 *  \par Description
 *       This function removes one or more authorized source eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg);

/**
 * \brief Add authorized custody sources command
 *
 *  \par Description
 *       This function adds one or more authorized custody source eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg);

/**
 * \brief Remove authorized custody sources command
 *
 *  \par Description
 *       This function removes one or more authorized custody source eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg);

/**
 * \brief Add authorized custodians command
 *
 *  \par Description
 *       This function adds one or more authorized custodian eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg);

/**
 * \brief Remove authorized custodians command
 *
 *  \par Description
 *       This function removes one or more auhorized custodian eids.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg);

/**
 * \brief Add authorized report-to eid command
 *
 *  \par Description
 *       This function adds an authorized report-to-eid.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg);

/**
 * \brief Remove authorized report-to eid command
 *
 *  \par Description
 *       This function removes an authorized report-to eid.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg);

/**
 * \brief Add latency command
 *
 *  \par Description
 *       This function  adds a latency (low, medium, high) to a bundle queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg);

/**
 * \brief Remove latency command
 *
 *  \par Description
 *       This function removes a latency from a bundle queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg);

/**
 * \brief Contact setup command
 *
 *  \par Description
 *       This function sets up a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg);

/**
 * \brief Contact start command
 *
 *  \par Description
 *       This function starts a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg);

/**
 * \brief Contact stop command
 *
 *  \par Description
 *       This function stops a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg);

/**
 * \brief Contact teardown command
 *
 *  \par Description
 *       This function tears down a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg);

/**
 * \brief Add mib array key command
 *
 *  \par Description
 *       This function adds a mib array key.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg);

/**
 * \brief Remove mib array key command
 *
 *  \par Description
 *       This function removes a mib array key.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg);

/**
 * \brief Set mib item command
 *
 *  \par Description
 *       This function sets the value of a mib item.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg);

/**
 * \brief Add storage allocation command
 *
 *  \par Description
 *       This function adds a given storage allocation assigned to a given source eid.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg);

/**
 * \brief Remove storage allocation command
 *
 *  \par Description
 *       This function removes a storage allocation by source eid.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg);

/**
 * \brief Perform self test (7.2) command
 *
 *  \par Description
 *       This function performs a bpnode self test.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_PerformSelfTestCmd(const BPNode_PerformSelfTestCmd_t *Msg);

/**
 * \brief Send node mib config hk command
 *
 *  \par Description
 *       This function sends node mib configuration hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendNodeMibConfigHkCmd(const BPNode_SendNodeMibConfigHkCmd_t *Msg);

/**
 * \brief Send per-source mib config hk command
 *
 *  \par Description
 *       This function sends per-source eid mib configuration hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendSourceMibConfigHkCmd(const BPNode_SendSourceMibConfigHkCmd_t *Msg);

/**
 * \brief Send node mib counter hk command
 *
 *  \par Description
 *       This function sends mib counter hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendNodeMibCountersHkCmd(const BPNode_SendNodeMibCountersHkCmd_t *Msg);

/**
 * \brief Send per-source mib counter hk command
 *
 *  \par Description
 *       This function sends per-source eid mib counter hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendSourceMibCountersHkCmd(const BPNode_SendSourceMibCountersHkCmd_t *Msg);

/**
 * \brief Send storage hk command
 *
 *  \par Description
 *       This function sends storage hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendStorageHk(const BPNode_SendStorageHk_t *Msg);

/**
 * \brief Send channel/contact status hk command
 *
 *  \par Description
 *       This function sends channel/contact status hk.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t BPNode_SendChannelContactStatHkCmd(const BPNode_SendChannelContactStatHkCmd_t *Msg);


#endif /* BPNODE_CMDS_H */
