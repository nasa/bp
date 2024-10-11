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

#ifndef FWP_DP_H
#define FWP_DP_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe_error.h"
#include "bpnode_msg.h"
#include "bpnode_msgids.h"
#include "bpnode_version.h"
#include "bpnode_tbl.h"
#include "bpnode_utils.h"
#include "bpnode_msg.h"

#include "bplib.h"
#include "bplib_eventids.h"


/* =================== */
/* Function Prototypes */
/* =================== */

/**
 * \brief Noop command
 *
 *  \par Description
 *       This function processes a noop command.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_NoopCmd(void);

/**
 * \brief Add All Applications command
 *
 *  \par Description
 *       This function adds all defined applications.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddAllApplicationsCmd(void);

/**
 * \brief Start All Applications command
 *
 *  \par Description
 *       This function starts all applications
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_StartAllApplicationsCmd(void);

/**
 * \brief Verify Bundle Storage command
 *
 *  \par Description
 *       This function verifies bundle storage.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_VerifyBundleStorageCmd(void);

/**
 * \brief Initialize Bundle Storage command
 *
 *  \par Description
 *       This function initializes bundle storage.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_InitBundleStorageCmd(void);

/**
 * \brief Verify Bundle Metadata (7.2) command
 *
 *  \par Description
 *       This function verifies stored bundle metadata.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_VerifyBundleMetadataCmd(void);

/**
 * \brief Rebuild Bundle Metadata command
 *
 *  \par Description
 *       This function rebuilds the cache from stored bundle metadata.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RebuildBundleMetadataCmd(void);

/**
 * \brief Clear Volatile command
 *
 *  \par Description
 *       This function clears the cache.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ClearVolatileCmd(void);

/**
 * \brief Reload Saved Data (7.2) command
 *
 *  \par Description
 *       This function reloads data saved by modules.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ReloadSavedDataCmd(void);

/**
 * \brief Reset All Counters command
 *
 *  \par Description
 *       This function resets all node counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ResetAllCountersCmd(void);

/**
 * \brief Reset Counter command
 *
 *  \par Description
 *       This function resets an individual counter.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg);

/**
 * \brief Reset Source Counters command
 *
 *  \par Description
 *       This function resets all counters indexed by Source EID.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg);

/**
 * \brief Reset Bundle Counters command
 *
 *  \par Description
 *       This function resets all bundle counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ResetBundleCountersCmd(void);

/**
 * \brief Reset Error Counters command
 *
 *  \par Description
 *       This function resets all error counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ResetErrorCountersCmd(void);

/**
 * \brief Add Application command
 *
 *  \par Description
 *       This function adds an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg);

/**
 * \brief Remove Application command
 *
 *  \par Description
 *       This function removes an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg);

/**
 * \brief Set Registration State command
 *
 *  \par Description
 *       This function sets an application's registration state.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg);

/**
 * \brief Start Application command
 *
 *  \par Description
 *       This function starts an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg);

/**
 * \brief Stop Application command
 *
 *  \par Description
 *       This function stops an application.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg);

/**
 * \brief Add Authorized Sources command
 *
 *  \par Description
 *       This function adds one or more authorized Source EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg);

/**
 * \brief Remove Authorized Sources command
 *
 *  \par Description
 *       This function removes one or more authorized Source EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg);

/**
 * \brief Add Authorized Custody Sources command
 *
 *  \par Description
 *       This function adds one or more authorized custody Source EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg);

/**
 * \brief Remove Authorized Custody Sources command
 *
 *  \par Description
 *       This function removes one or more authorized custody Source EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg);

/**
 * \brief Add Authorized Custodians command
 *
 *  \par Description
 *       This function adds one or more authorized custodian EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg);

/**
 * \brief Remove Authorized Custodians command
 *
 *  \par Description
 *       This function removes one or more auhorized custodian EIDs.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg);

/**
 * \brief Add Authorized Report-to EID command
 *
 *  \par Description
 *       This function adds an authorized Report-to-EID.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg);

/**
 * \brief Remove Authorized Report-to EID command
 *
 *  \par Description
 *       This function removes an authorized Report-to EID.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg);

/**
 * \brief Add Latency command
 *
 *  \par Description
 *       This function  adds a latency (low, medium, high) to a bundle queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg);

/**
 * \brief Remove Latency command
 *
 *  \par Description
 *       This function removes a latency from a bundle queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg);

/**
 * \brief Contact Setup command
 *
 *  \par Description
 *       This function sets up a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg);

/**
 * \brief Contact Start command
 *
 *  \par Description
 *       This function starts a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg);

/**
 * \brief Contact Stop command
 *
 *  \par Description
 *       This function stops a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg);

/**
 * \brief Contact Teardown command
 *
 *  \par Description
 *       This function tears down a contact.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg);

/**
 * \brief Add MIB Array Key command
 *
 *  \par Description
 *       This function adds a MIB Array Key.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg);

/**
 * \brief Remove MIB Array Key command
 *
 *  \par Description
 *       This function removes a MIB Array Key.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg);

/**
 * \brief Set MIB Item command
 *
 *  \par Description
 *       This function sets the value of a MIB item.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg);

/**
 * \brief Add Storage Allocation command
 *
 *  \par Description
 *       This function adds a given storage allocation assigned to a given Source EID.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg);

/**
 * \brief Remove Storage Allocation command
 *
 *  \par Description
 *       This function removes a storage allocation by Source EID.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param[in] Msg Pointer to Software Bus buffer
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg);

/**
 * \brief Perform Self Test (7.2) command
 *
 *  \par Description
 *       This function performs a BPNode self test.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_PerformSelfTestCmd(void);

/**
 * \brief Send Node MIB Config HK command
 *
 *  \par Description
 *       This function sends Node MIB configuration HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendNodeMibConfigHkCmd(void);

/**
 * \brief Send Per-Source MIB Config HK command
 *
 *  \par Description
 *       This function sends per-source EID MIB configuration HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendSourceMibConfigHkCmd(void);

/**
 * \brief Send Node MIB Counter HK command
 *
 *  \par Description
 *       This function sends MIB counter HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendNodeMibCountersHkCmd(void);

/**
 * \brief Send Per-Source MIB Counter HK command
 *
 *  \par Description
 *       This function sends per-source EID MIB counter HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendSourceMibCountersHkCmd(void);

/**
 * \brief Send Storage HK command
 *
 *  \par Description
 *       This function sends storage HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendStorageHkCmd(void);

/**
 * \brief Send Channel/Contact Status HK command
 *
 *  \par Description
 *       This function sends channel/contact status HK.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Execution status
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
void BPA_DP_SendChannelContactStatHkCmd(void);

#endif /* FWP_DP_H */
