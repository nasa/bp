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

#ifndef BP_STORAGE_H
#define BP_STORAGE_H

#include "cfe_mission_cfg.h"
#include "bp_interface_cfg.h"

#include "cfe_resourceid_api_typedefs.h"

#include "bp_storage.h"
#include "bplib.h"

/* Storage Services */

#define BP_STORAGEHANDLE_C(x)     ((BP_StorageHandle_t)CFE_RESOURCEID_WRAP(x))
#define BP_INVALID_STORAGE_HANDLE BP_STORAGEHANDLE_C(CFE_RESOURCEID_UNDEFINED)

/************************************************
 * Typedefs
 ************************************************/

typedef CFE_RESOURCEID_BASE_TYPE BP_StorageHandle_t;

typedef void (*BP_StoreInitFunc_t)(void);

typedef struct
{
    BP_StorageHandle_t Handle;
    bool               IsInitialized;
    char               Name[BP_STORE_NAME_SIZE];
    BP_StoreInitFunc_t InitFunc;
    bp_store_t         Store;
} BP_StorageService_t;

void               BP_StorageService_Init(void);
void               BP_StorageService_Register(const char *Name, BP_StoreInitFunc_t InitFunc, const bp_store_t *Cfg);
BP_StorageHandle_t BP_StorageService_FindByName(const char *Name);
const bp_store_t  *BP_StorageService_Get(BP_StorageHandle_t sh);

#endif /* BP_STORAGE_H */
