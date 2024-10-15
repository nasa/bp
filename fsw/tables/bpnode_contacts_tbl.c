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

/*
** \file
**   This file contains the source code for the Contacts Table
*/

#include "cfe_tbl_filedef.h" /* Required to obtain the CFE_TBL_FILEDEF macro definition */
#include "bpnode_tbl.h"
#include "bplib.h"


BPLib_ContactsTable_t ContactsTable = 
{
    .ContactSet = {
        {
            .ContactID              = 0, /*Contact ID, uint32*/ 
            .DestEIDs               = "100.1,101.2", /*Comma-separated list of Dest EIDs*/
            .CLAType                = 1, /*CLA Type, uint32 */
            .CLAddr                 = "127.0.0.1", /*CL ip address*/
            .PortNum                = 1001, /*Port Number, int32*/
            .DestLTPEngineID        = 1, /*Destination LTP engine ID*/
            .SendBytePerCycle       = 101, /*Maximum bytes to send per wakeup, uint32*/
            .ReceiveBytePerCycle    = 200, /*Maximum bytes to receive per wakeup, uint32*/
            .RetransmitTimeout      = 102, /*bundle reforwarding timeout in seconds, uint32*/
            .CSTimeTrigger          = 103, /*Custody Signal time trigger in seconds, uint32*/
            .CSSizeTrigger          = 10 /*Custody signal size trigger in bytes, size_t*/
        },            
    }
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Example Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(ContactsTable, BPNODE.ContactsTable, Contacts Setup Table, bpnode_contacts.tbl) 