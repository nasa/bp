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
 * \file
 *  Unit tests for fwp_evp.c
 */

/* ============= */
/* Include Files */
/* ============= */

#include "fwp_evp.h"
#include "bpnode_test_utils.h"

/* ================ */
/* Global Variables */
/* ================ */

UT_CheckEvent_t EventTest;

/* ==================== */
/* Function Definitions */
/* ==================== */

// Test a nominal call to BPA_EVP_Init()
void Test_BPA_EVP_Init_Nominal(void)
{
    BPLib_Status_t Status;

    /* Call the function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);
}

// Test that BPA_EVP_Init() returns a bad value when expected
void Test_BPA_EVP_Init_BadReturn(void)
{
    BPLib_Status_t Status;
    uint16_t stubCount;

    stubCount = 1;

    /* ===== CFE_EVS_APP_ILLEGAL_APP_ID / BPLIB_EM_ILLEGAL_APP_ID returned ===== */
    /* Set return code for CFE_EVS_Register to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_EVS_APP_ILLEGAL_APP_ID);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_ILLEGAL_APP_ID);


    /* ===== CFE_EVS_UNKNOWN_FILTER / BPLIB_EM_UNKNOWN_FILTER returned ===== */
    /* Set return code for CFE_EVS_Register to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_EVS_UNKNOWN_FILTER);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_UNKNOWN_FILTER);


    /* ===== CFE_ES_BAD_ARGUMENT / BPLIB_EM_BAD_ARGUMENT returned ===== */
    /* Set return code for CFE_EVS_Register to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_ES_BAD_ARGUMENT);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_BAD_ARGUMENT);


    /* ===== -1 / BPLIB_UNKNOWN returned ===== */
    /* Set return code for CFE_EVS_Register to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), -1);

    /* Call function under test */
    Status = BPLIB_SUCCESS;
    Status = BPA_EVP_Init();

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_Register, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_UNKNOWN);
}

// Test that BPA_EVP_SendEvent() returns BPLIB_SUCCESS when run nominally
void Test_BPA_EVP_SendEvent_Nominal(void)
{
    BPLib_Status_t Status;
    uint16_t stubCount;

    stubCount = 1;

    /* === INFO event message test === */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_INFORMATION, "INFO event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);


    /* === ERROR event message test === */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(01, CFE_EVS_EventType_ERROR, "ERROR event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);


    /* === DEBUG event message test === */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(0x42, CFE_EVS_EventType_DEBUG, "DEBUG event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);


    /* === CRITICAL event message test === */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent((unsigned long) 123, CFE_EVS_EventType_CRITICAL, "CRITICAL event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_SUCCESS);
}

// Test that BPA_EVP_SendEvent returns a bad value when expected
void Test_BPA_EVP_SendEvent_BadReturn(void)
{
    BPLib_Status_t Status;
    uint16_t stubCount;

    stubCount = 1;

    /* ===== CFE_EVS_INVALID_PARAMETER / BPLIB_EM_INVALID_PARAMETER returned ===== */
    /* Set return code for CFE_EVS_SendEvent to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), CFE_EVS_INVALID_PARAMETER);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_INFORMATION,
                                "Bad return INFO event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_INVALID_PARAMETER);


    /* ===== CFE_EVS_APP_ILLEGAL_APP_ID / BPLIB_EM_ILLEGAL_APP_ID returned ===== */
    /* Set return code for CFE_EVS_SendEvent to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), CFE_EVS_APP_ILLEGAL_APP_ID);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_ERROR,
                                "Bad return ERROR event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_ILLEGAL_APP_ID);


    /* ===== CFE_EVS_APP_NOT_REGISTERED / BPLIB_EM_APP_NOT_REGISTERED returned ===== */
    /* Set return code for CFE_EVS_SendEvent to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), CFE_EVS_APP_NOT_REGISTERED);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_DEBUG,
                        "Bad return DEBUG event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_APP_NOT_REGISTERED);


    /* ===== CFE_EVS_APP_SQUELCHED / BPLIB_EM_APP_SQUELCHED returned ===== */
    /* Set return code for CFE_EVS_SendEvent to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), CFE_EVS_APP_SQUELCHED);

    /* Call function under test */
    Status = BPLIB_UNKNOWN;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_INFORMATION,
                                "Bad return INFO event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_APP_SQUELCHED);


    /* ===== -1 / BPLIB_UNKNOWN returned ===== */
    /* Set return code for CFE_EVS_SendEvent to be non-success */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_SendEvent), -1);

    /* Call function under test */
    Status = BPLIB_SUCCESS;
    Status = BPA_EVP_SendEvent(42, CFE_EVS_EventType_ERROR,
                                "Bad return ERROR event message test");

    /* Verify that the EVS function that is being proxied, was called */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, stubCount++);
    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_UNKNOWN);
}

void Test_BPA_EVP_SendEvent_TruncatedEvent(void)
{
    char LongString[BPLIB_EM_MAX_MESSAGE_LENGTH + 5];
    BPLib_Status_t Status;
    int stringLoopControl;
    char ExpectedEvent[BPLIB_EM_MAX_MESSAGE_LENGTH];

    for(stringLoopControl = 0; stringLoopControl < BPLIB_EM_MAX_MESSAGE_LENGTH + 4; stringLoopControl++)
    {
        LongString[stringLoopControl] = 'A';
    }

    LongString[stringLoopControl] = 'H';

    strncpy(ExpectedEvent, LongString, BPLIB_EM_MAX_MESSAGE_LENGTH - 2);
    ExpectedEvent[BPLIB_EM_MAX_MESSAGE_LENGTH - 1] = '$';
    ExpectedEvent[BPLIB_EM_MAX_MESSAGE_LENGTH] = '\0';

    UT_CHECKEVENT_SETUP(&EventTest, 142, ExpectedEvent);

    Status = BPLIB_UNKNOWN;
    Status = BPLib_EM_SendEvent(142, BPLib_EM_EventType_INFO, LongString);

    UtAssert_EQ(BPLib_Status_t, Status, BPLIB_EM_STRING_TRUNCATED);
    UtAssert_INT32_EQ(EventTest.MatchCount, 1);
}

/* Register the test cases to execute with the unit test tool */
void UtTest_Setup(void)
{
    ADD_TEST(Test_BPA_EVP_Init_Nominal);
    ADD_TEST(Test_BPA_EVP_Init_BadReturn);
    ADD_TEST(Test_BPA_EVP_SendEvent_Nominal);
    ADD_TEST(Test_BPA_EVP_SendEvent_BadReturn);
    ADD_TEST(Test_BPA_EVP_SendEvent_TruncatedEvent);
}
