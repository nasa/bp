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
 *   This file contains the implementation for a thread-safe signaling event
 */

#ifndef BPNODE_NOTIF_H
#define BPNODE_NOTIF_H

#include "cfe.h"

typedef struct BPNode_Notif
{
    uint32    Count;
    osal_id_t CondVar;
} BPNode_Notif_t;

int32 BPNode_NotifInit(BPNode_Notif_t* Event, const char* EventName);

void BPNode_NotifDestroy(BPNode_Notif_t* Event);

void BPNode_NotifSet(BPNode_Notif_t* Event);

uint32 BPNode_NotifGetCount(BPNode_Notif_t* Notif);

int32 BPNode_NotifWait(BPNode_Notif_t* Notif, uint32 OldCount, int32 TimeoutMs);

#endif /* BPNODE_NOTIF_H */
