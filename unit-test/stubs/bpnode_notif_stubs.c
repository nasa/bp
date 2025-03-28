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
 *
 * Auto-Generated stub implementations for functions defined in bpnode_notif header
 */

#include "bpnode_notif.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for BPNode_NotifClear()
 * ----------------------------------------------------
 */
void BPNode_NotifClear(BPNode_Notif_t *Event)
{
    UT_GenStub_AddParam(BPNode_NotifClear, BPNode_Notif_t *, Event);

    UT_GenStub_Execute(BPNode_NotifClear, Basic, NULL);
}

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
 * Generated stub function for BPNode_NotifIsSet()
 * ----------------------------------------------------
 */
bool BPNode_NotifIsSet(BPNode_Notif_t *Event)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NotifIsSet, bool);

    UT_GenStub_AddParam(BPNode_NotifIsSet, BPNode_Notif_t *, Event);

    UT_GenStub_Execute(BPNode_NotifIsSet, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NotifIsSet, bool);
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
int32 BPNode_NotifWait(BPNode_Notif_t *Event, int32 TimeoutMs)
{
    UT_GenStub_SetupReturnBuffer(BPNode_NotifWait, int32);

    UT_GenStub_AddParam(BPNode_NotifWait, BPNode_Notif_t *, Event);
    UT_GenStub_AddParam(BPNode_NotifWait, int32, TimeoutMs);

    UT_GenStub_Execute(BPNode_NotifWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(BPNode_NotifWait, int32);
}
