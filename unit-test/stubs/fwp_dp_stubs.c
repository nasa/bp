/*
 * NASA Docket No. GSC-19,559-1, and identified as "Delay/Disruption Tolerant Networking 
 * (DTN) Bundle Protocol (BP) v7 Core Flight System (cFS) Application Build 7.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software distributed under 
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF 
 * ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. The copyright notice to be 
 * included in the software is as follows: 
 *
 * Copyright 2025 United States Government as represented by the Administrator of the 
 * National Aeronautics and Space Administration. All Rights Reserved. */

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in fwp_dp header
 */

#include "fwp_dp.h"
#include "utgenstub.h"

/* Stub for BPA_DP_VerifyCmdLength() */
bool BPA_DP_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    UT_GenStub_SetupReturnBuffer(BPA_DP_VerifyCmdLength, bool);

    UT_GenStub_AddParam(BPA_DP_VerifyCmdLength, const CFE_MSG_Message_t*, MsgPtr);
    UT_GenStub_AddParam(BPA_DP_VerifyCmdLength, size_t, ExpectedLength);

    UT_GenStub_Execute(BPA_DP_VerifyCmdLength, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPA_DP_VerifyCmdLength, bool);
}

/* Stub for BPA_DP_ProcessGroundCommand() */
void BPA_DP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr)
{
    UT_GenStub_AddParam(BPA_DP_ProcessGroundCommand, const CFE_SB_Buffer_t*, SBBufPtr);

    UT_GenStub_Execute(BPA_DP_ProcessGroundCommand, Basic, NULL);
}

void BPA_DP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    UT_GenStub_AddParam(BPA_DP_TaskPipe, const CFE_SB_Buffer_t*, SBBufPtr);

    UT_GenStub_Execute(BPA_DP_TaskPipe, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddAllApplicationsCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddAllApplicationsCmd(void)
{
    UT_GenStub_Execute(BPA_DP_AddAllApplicationsCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddApplicationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddApplicationCmd(const BPNode_AddApplicationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddApplicationCmd, const BPNode_AddApplicationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddApplicationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddAuthCustodiansCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddAuthCustodiansCmd(const BPNode_AddAuthCustodiansCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddAuthCustodiansCmd, const BPNode_AddAuthCustodiansCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddAuthCustodiansCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddAuthCustodySourcesCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddAuthCustodySourcesCmd(const BPNode_AddAuthCustodySourcesCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddAuthCustodySourcesCmd, const BPNode_AddAuthCustodySourcesCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddAuthCustodySourcesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddAuthReportToEidCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddAuthReportToEidCmd(const BPNode_AddAuthReportToEidCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddAuthReportToEidCmd, const BPNode_AddAuthReportToEidCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddAuthReportToEidCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddAuthSourcesCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddAuthSourcesCmd(const BPNode_AddAuthSourcesCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddAuthSourcesCmd, const BPNode_AddAuthSourcesCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddAuthSourcesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddLatencyCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddLatencyCmd(const BPNode_AddLatencyCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddLatencyCmd, const BPNode_AddLatencyCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddLatencyCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddMibArrayKeyCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddMibArrayKeyCmd(const BPNode_AddMibArrayKeyCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddMibArrayKeyCmd, const BPNode_AddMibArrayKeyCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddMibArrayKeyCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_AddStorageAllocationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_AddStorageAllocationCmd(const BPNode_AddStorageAllocationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_AddStorageAllocationCmd, const BPNode_AddStorageAllocationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_AddStorageAllocationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ClearVolatileCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ClearVolatileCmd(void)
{

    UT_GenStub_Execute(BPA_DP_ClearVolatileCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ContactSetupCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ContactSetupCmd(const BPNode_ContactSetupCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ContactSetupCmd, const BPNode_ContactSetupCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ContactSetupCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ContactStartCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ContactStartCmd(const BPNode_ContactStartCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ContactStartCmd, const BPNode_ContactStartCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ContactStartCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ContactStopCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ContactStopCmd(const BPNode_ContactStopCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ContactStopCmd, const BPNode_ContactStopCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ContactStopCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ContactTeardownCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ContactTeardownCmd(const BPNode_ContactTeardownCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ContactTeardownCmd, const BPNode_ContactTeardownCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ContactTeardownCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_InitBundleStorageCmd()
 * ----------------------------------------------------
 */
void BPA_DP_InitBundleStorageCmd(void)
{

    UT_GenStub_Execute(BPA_DP_InitBundleStorageCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_NoopCmd()
 * ----------------------------------------------------
 */
void BPA_DP_NoopCmd(void)
{

    UT_GenStub_Execute(BPA_DP_NoopCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_PerformSelfTestCmd()
 * ----------------------------------------------------
 */
void BPA_DP_PerformSelfTestCmd(void)
{

    UT_GenStub_Execute(BPA_DP_PerformSelfTestCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RebuildBundleMetadataCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RebuildBundleMetadataCmd(void)
{

    UT_GenStub_Execute(BPA_DP_RebuildBundleMetadataCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ReloadSavedDataCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ReloadSavedDataCmd(void)
{

    UT_GenStub_Execute(BPA_DP_ReloadSavedDataCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveApplicationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveApplicationCmd(const BPNode_RemoveApplicationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveApplicationCmd, const BPNode_RemoveApplicationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveApplicationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveAuthCustodiansCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveAuthCustodiansCmd(const BPNode_RemoveAuthCustodiansCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveAuthCustodiansCmd, const BPNode_RemoveAuthCustodiansCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveAuthCustodiansCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveAuthCustodySourcesCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveAuthCustodySourcesCmd(const BPNode_RemoveAuthCustodySourcesCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveAuthCustodySourcesCmd, const BPNode_RemoveAuthCustodySourcesCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveAuthCustodySourcesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveAuthReportToEidCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveAuthReportToEidCmd(const BPNode_RemoveAuthReportToEidCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveAuthReportToEidCmd, const BPNode_RemoveAuthReportToEidCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveAuthReportToEidCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveAuthSourcesCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveAuthSourcesCmd(const BPNode_RemoveAuthSourcesCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveAuthSourcesCmd, const BPNode_RemoveAuthSourcesCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveAuthSourcesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveLatencyCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveLatencyCmd(const BPNode_RemoveLatencyCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveLatencyCmd, const BPNode_RemoveLatencyCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveLatencyCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveMibArrayKeyCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveMibArrayKeyCmd(const BPNode_RemoveMibArrayKeyCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveMibArrayKeyCmd, const BPNode_RemoveMibArrayKeyCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveMibArrayKeyCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_RemoveStorageAllocationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_RemoveStorageAllocationCmd(const BPNode_RemoveStorageAllocationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_RemoveStorageAllocationCmd, const BPNode_RemoveStorageAllocationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_RemoveStorageAllocationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ResetAllCountersCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ResetAllCountersCmd(void)
{

    UT_GenStub_Execute(BPA_DP_ResetAllCountersCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ResetBundleCountersCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ResetBundleCountersCmd(void)
{

    UT_GenStub_Execute(BPA_DP_ResetBundleCountersCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ResetCounterCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ResetCounterCmd(const BPNode_ResetCounterCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ResetCounterCmd, const BPNode_ResetCounterCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ResetCounterCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ResetErrorCountersCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ResetErrorCountersCmd(void)
{

    UT_GenStub_Execute(BPA_DP_ResetErrorCountersCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_ResetSourceCountersCmd()
 * ----------------------------------------------------
 */
void BPA_DP_ResetSourceCountersCmd(const BPNode_ResetSourceCountersCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_ResetSourceCountersCmd, const BPNode_ResetSourceCountersCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_ResetSourceCountersCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendChannelContactStatHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendChannelContactStatHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendChannelContactStatHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendNodeMibConfigHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendNodeMibConfigHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendNodeMibConfigHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendNodeMibCountersHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendNodeMibCountersHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendNodeMibCountersHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendSourceMibConfigHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendSourceMibConfigHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendSourceMibConfigHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendSourceMibCountersHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendSourceMibCountersHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendSourceMibCountersHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SendStorageHkCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SendStorageHkCmd(void)
{

    UT_GenStub_Execute(BPA_DP_SendStorageHkCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SetMibItemCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SetMibItemCmd(const BPNode_SetMibItemCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_SetMibItemCmd, const BPNode_SetMibItemCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_SetMibItemCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_SetRegistrationStateCmd()
 * ----------------------------------------------------
 */
void BPA_DP_SetRegistrationStateCmd(const BPNode_SetRegistrationStateCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_SetRegistrationStateCmd, const BPNode_SetRegistrationStateCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_SetRegistrationStateCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_StartAllApplicationsCmd()
 * ----------------------------------------------------
 */
void BPA_DP_StartAllApplicationsCmd(void)
{

    UT_GenStub_Execute(BPA_DP_StartAllApplicationsCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_StartApplicationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_StartApplicationCmd(const BPNode_StartApplicationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_StartApplicationCmd, const BPNode_StartApplicationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_StartApplicationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_StopApplicationCmd()
 * ----------------------------------------------------
 */
void BPA_DP_StopApplicationCmd(const BPNode_StopApplicationCmd_t *Msg)
{
    UT_GenStub_AddParam(BPA_DP_StopApplicationCmd, const BPNode_StopApplicationCmd_t *, Msg);

    UT_GenStub_Execute(BPA_DP_StopApplicationCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_VerifyBundleMetadataCmd()
 * ----------------------------------------------------
 */
void BPA_DP_VerifyBundleMetadataCmd(void)
{

    UT_GenStub_Execute(BPA_DP_VerifyBundleMetadataCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPA_DP_VerifyBundleStorageCmd()
 * ----------------------------------------------------
 */
void BPA_DP_VerifyBundleStorageCmd(void)
{

    UT_GenStub_Execute(BPA_DP_VerifyBundleStorageCmd, Basic, NULL);
}
