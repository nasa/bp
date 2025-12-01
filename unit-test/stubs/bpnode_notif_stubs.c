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
 * National Aeronautics and Space Administration. All Rights Reserved.
 *
 */

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in bpnode_notif header
 */

#include "bpnode_notif.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifDestroy()
 * ----------------------------------------------------
 */
void BPNode_NotifDestroy(BPNode_Notif_t *Event)
{
    UT_GenStub_AddParam(BPNode_NotifDestroy, BPNode_Notif_t *, Event);

    UT_GenStub_Execute(BPNode_NotifDestroy, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifGetCount()
 * ----------------------------------------------------
 */
uint32 BPNode_NotifGetCount(BPNode_Notif_t *Notif)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NotifGetCount, uint32);

    UT_GenStub_AddParam(BPNode_NotifGetCount, BPNode_Notif_t *, Notif);

    UT_GenStub_Execute(BPNode_NotifGetCount, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NotifGetCount, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifInit()
 * ----------------------------------------------------
 */
int32 BPNode_NotifInit(BPNode_Notif_t *Event, const char *EventName)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NotifInit, int32);

    UT_GenStub_AddParam(BPNode_NotifInit, BPNode_Notif_t *, Event);
    UT_GenStub_AddParam(BPNode_NotifInit, const char *, EventName);

    UT_GenStub_Execute(BPNode_NotifInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NotifInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifSet()
 * ----------------------------------------------------
 */
void BPNode_NotifSet(BPNode_Notif_t *Event)
{
    UT_GenStub_AddParam(BPNode_NotifSet, BPNode_Notif_t *, Event);

    UT_GenStub_Execute(BPNode_NotifSet, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifWait()
 * ----------------------------------------------------
 */
int32 BPNode_NotifWait(BPNode_Notif_t *Notif, uint32 OldCount, int32 TimeoutMs)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NotifWait, int32);

    UT_GenStub_AddParam(BPNode_NotifWait, BPNode_Notif_t *, Notif);
    UT_GenStub_AddParam(BPNode_NotifWait, uint32, OldCount);
    UT_GenStub_AddParam(BPNode_NotifWait, int32, TimeoutMs);

    UT_GenStub_Execute(BPNode_NotifWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NotifWait, int32);
}
