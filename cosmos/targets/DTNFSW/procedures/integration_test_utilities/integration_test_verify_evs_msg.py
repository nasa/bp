# Temporary verification, sending CI_LAB_CMD_NOOP and verifying it was sent.
# This is not intended to be used by the actual test suite
def integration_test_verify_evs_msg(app_name, evt_id, evt_type):
    """
    Check if the last EVS Event Message that was received matches the expected
    - App Name (app_name)
    - Event ID (evt_id)
    - Event Type (evt_type)
    Warning:
    this way of checking events is error prone, because we have to check the fields separately
    really what we want is for all fields in the same packet to match these expectations.
    """
    wait_check(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == \'{app_name}\'", 10)
    wait_check(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == {evt_id}", 10)
    wait_check(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == \'{evt_type}\'", 10)
