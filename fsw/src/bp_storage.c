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

/************************************************
 * Includes
 ************************************************/

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "cfe.h"
#include "bp_eventids.h"
#include "bp_platform_cfg.h"
#include "bp_storage.h"
#include "bp_global.h"
#include "cfe_resourceid_basevalue.h"

/************************************************
 * Typedefs
 ************************************************/

/************************************************
 * File Data
 ************************************************/

/************************************************
 * Local Functions
 ************************************************/

/*
 * Base for unique-ish BP Io ID handles. This is just a random
 * offset from OS_OBJECT_ID_USER.
 *
 * This is chosen to not overlap with default CFE resource IDs, but
 * even if there is overlap, or if another CFS app chooses the same
 * random value, it is not going to break anything because type
 * checking still applies.  So an overlap/collision just slightly weakens
 * the protection offered here, but it doesn't cause a problem.
 */
enum
{
    BP_STORAGE_HANDLE_BASE = CFE_RESOURCEID_MAKE_BASE(OS_OBJECT_TYPE_USER + 21)
};

static int32 BP_StorageHandle_ToIndex(BP_StorageHandle_t fh, uint32 *Idx)
{
    return CFE_ResourceId_ToIndex(CFE_RESOURCEID_UNWRAP(fh), BP_STORAGE_HANDLE_BASE, BP_NUM_STORAGE_SERVICES, Idx);
}

static BP_StorageService_t *BP_LocateStorageEntryByHandle(BP_StorageHandle_t fh)
{
    BP_StorageService_t *StoragePtr;
    uint32               Idx;

    if (BP_StorageHandle_ToIndex(fh, &Idx) == CFE_SUCCESS)
    {
        StoragePtr = &BP_GlobalData.StorageServices[Idx];
    }
    else
    {
        StoragePtr = NULL;
    }

    return StoragePtr;
}

static inline bool BP_StorageEntryIsMatch(const BP_StorageService_t *StoragePtr, BP_StorageHandle_t fh)
{
    return (StoragePtr != NULL && CFE_RESOURCEID_TEST_EQUAL(StoragePtr->Handle, fh));
}

/*----------------------------------------------------------------
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool BP_CheckStorageSlotUsed(CFE_ResourceId_t CheckId)
{
    BP_StorageService_t *StoragePtr;
    /*
     * Note - The pointer here should never be NULL because the ID should always be
     * within the expected range, but if it ever is NULL, this should return true
     * such that the caller will _not_ attempt to use the record.
     */
    StoragePtr = BP_LocateStorageEntryByHandle(BP_STORAGEHANDLE_C(CheckId));
    return (StoragePtr == NULL || CFE_RESOURCEID_TEST_DEFINED(StoragePtr->Handle));
}

void BP_StorageService_Init(void)
{
    BP_GlobalData.LastStorageHandle = CFE_ResourceId_FromInteger(BP_STORAGE_HANDLE_BASE);
}

void BP_StorageService_Register(const char *Name, BP_StoreInitFunc_t InitFunc, const bp_store_t *Cfg)
{
    CFE_ResourceId_t     PendingStorageHandle;
    BP_StorageService_t *StoragePtr;

    PendingStorageHandle =
        CFE_ResourceId_FindNext(BP_GlobalData.LastStorageHandle, BP_NUM_STORAGE_SERVICES, BP_CheckStorageSlotUsed);
    StoragePtr = BP_LocateStorageEntryByHandle(BP_STORAGEHANDLE_C(PendingStorageHandle));

    if (StoragePtr != NULL)
    {
        StoragePtr->Handle        = BP_STORAGEHANDLE_C(PendingStorageHandle);
        StoragePtr->IsInitialized = false;
        strncpy(StoragePtr->Name, Name, sizeof(StoragePtr->Name) - 1);
        StoragePtr->Name[sizeof(StoragePtr->Name) - 1] = 0;
        StoragePtr->InitFunc                           = InitFunc;
        StoragePtr->Store                              = *Cfg;
        BP_GlobalData.LastStorageHandle                = PendingStorageHandle;
    }
}

BP_StorageHandle_t BP_StorageService_FindByName(const char *Name)
{
    BP_StorageService_t *StoragePtr;
    uint32               i;

    for (i = 0; i < BP_NUM_STORAGE_SERVICES; ++i)
    {
        StoragePtr = &BP_GlobalData.StorageServices[i];

        if (CFE_RESOURCEID_TEST_DEFINED(StoragePtr->Handle) && strcmp(StoragePtr->Name, Name) == 0)
        {
            return StoragePtr->Handle;
        }
    }

    return BP_INVALID_STORAGE_HANDLE;
}

const bp_store_t *BP_StorageService_Get(BP_StorageHandle_t sh)
{
    BP_StorageService_t *StoragePtr;

    StoragePtr = BP_LocateStorageEntryByHandle(sh);

    /* Check storage */
    if (!BP_StorageEntryIsMatch(StoragePtr, sh))
    {
        return NULL;
    }

    /* Initialize storage service the first time it is used */
    if (!StoragePtr->IsInitialized)
    {
        StoragePtr->InitFunc();
    }

    return &StoragePtr->Store;
}
