/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
** File: bpnode_eds_dispatch_tests.c
**
** Purpose:
** Coverage Unit Test cases for the BPNode Application
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the BPNode application.
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */

#include "bpnode_test_utils.h"
#include "bpnode_app.h"
#include "bpnode_dispatch.h"
#include "bpnode_cmds.h"

#include "cfe_msg_dispatcher.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

/*
** Test Case For:
** void BPNode_TaskPipe
*/
void Test_BPNode_TaskPipe(void)
{
    UT_CheckEvent_t EventTest;
    CFE_SB_Buffer_t   Buf;
    CFE_SB_MsgId_t    MsgId = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_Size_t    MsgSize = 0;
    CFE_MSG_FcnCode_t FcnCode = 0;

    /* Nominal case should return success */
    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_SUCCESS);
    memset(&Buf, 0, sizeof(Buf));
    UtAssert_VOIDCALL(BPNode_TaskPipe(&Buf));

    /* Invalid message ID */
    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_UNKNOWN_MSG_ID);
    memset(&Buf, 0, sizeof(Buf));
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_MID_ERR_EID, NULL);

    /* Invalid message length */
    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_WRONG_MSG_LENGTH);
    memset(&Buf, 0, sizeof(Buf));
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CMD_LEN_ERR_EID, NULL);

    /* Invalid command code */
    UT_SetDeferredRetcode(UT_KEY(CFE_MSG_EdsDispatch), 1, CFE_STATUS_BAD_COMMAND_CODE);
    memset(&Buf, 0, sizeof(Buf));
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(MsgId), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &MsgSize, sizeof(MsgSize), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(FcnCode), false);
    
    BPNode_TaskPipe(&Buf);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UT_CHECKEVENT_SETUP(&EventTest, BPNODE_CC_ERR_EID, NULL);


}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(BPNode_TaskPipe);
}
