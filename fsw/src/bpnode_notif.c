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
#include "bpnode_notif.h"
#include "osapi.h"

int32 BPNode_NotifInit(BPNode_Notif_t* Notif, const char* NotifName)
{
    Notif->IsSet = false;
    Notif->Count = 0;
    return OS_CondVarCreate(&Notif->CondVar, NotifName, 0);
}

void BPNode_NotifDestroy(BPNode_Notif_t* Notif)
{
    OS_CondVarDelete(Notif->CondVar);
}

bool BPNode_NotifIsSet(BPNode_Notif_t* Notif)
{
    bool IsSet;

    if (Notif == NULL)
    {
        return false;
    }

    OS_CondVarLock(Notif->CondVar);
    IsSet = Notif->IsSet;
    OS_CondVarUnlock(Notif->CondVar);
    return IsSet;
}

uint32 BPNode_NotifGetCount(BPNode_Notif_t* Notif)
{
    uint32 Count;

    if (Notif == NULL)
    {
        return 0;
    }

    OS_CondVarLock(Notif->CondVar);
    Count = Notif->Count;
    OS_CondVarUnlock(Notif->CondVar);
    return Count;
}


void BPNode_NotifSet(BPNode_Notif_t* Notif)
{
    if (Notif == NULL)
    {
        return;
    }

    OS_CondVarLock(Notif->CondVar);
    Notif->IsSet = true;
    Notif->Count++;
    OS_CondVarBroadcast(Notif->CondVar);
    OS_CondVarUnlock(Notif->CondVar);
}

void BPNode_NotifClear(BPNode_Notif_t* Notif)
{
    if (Notif == NULL)
    {
        return;
    }

    OS_CondVarLock(Notif->CondVar);
    Notif->IsSet = false;
    OS_CondVarUnlock(Notif->CondVar);
}

/* Currently unused code that appears to work but is untested.
** If this code is re-enabled, a new unit test is required for this feature
*/
int32 BPNode_NotifWait(BPNode_Notif_t* Notif, uint32 OldCount, int32 TimeoutMs)
{
    OS_time_t AbsWaitTime;
    int32 OsStatus = OS_SUCCESS;

    /* Perform wait on the Notif availability */
    AbsWaitTime = OS_TimeFromRelativeMilliseconds(TimeoutMs);
    OS_CondVarLock(Notif->CondVar);
    while (Notif->Count == OldCount)
    {
        OsStatus = OS_CondVarTimedWait(Notif->CondVar, &AbsWaitTime);
        if (OsStatus == OS_SUCCESS)
        {
            /* Note: No break here incase there's a spurious wakeup */
        }
        else if (OsStatus == OS_ERROR_TIMEOUT)
        {
            break;
        }
        else
        {
            /* Case is separate incase event message needed */
            break;
        }
    }
    OS_CondVarUnlock(Notif->CondVar);

    return OsStatus;
}
