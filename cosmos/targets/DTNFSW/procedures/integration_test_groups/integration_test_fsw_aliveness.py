from openc3.script.suite import Group

# Group class name should indicate what the scripts are testing
class integration_test_fsw_aliveness(Group):
    """
    Test cases for checking FSW "aliveness"
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_fsw_aliveness(self):
        """
        FSW Aliveness Test
        - Send a no-op command to each cFS FSW app in the system
            then verify the command was received (by checking telemetry)
        - Reset the command counter
            then verify the command was received (by checking telemetry)
        """
        # Create a list of app noops to test
        fsw_apps_to_test = []
        fsw_apps_to_test.append(["CFE_ES_CMD_NOOP",   "CFE_ES_CMD_RESET_COUNTERS",     "CFE_ES_HK",                   "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CFE_EVS_CMD_NOOP",  "CFE_EVS_CMD_RESET_COUNTERS",    "CFE_EVS_HK",                  "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CFE_SB_CMD_NOOP",   "CFE_SB_CMD_RESET_COUNTERS",     "CFE_SB_HK",                   "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CFE_TBL_CMD_NOOP",  "CFE_TBL_CMD_RESET_COUNTERS",    "CFE_TBL_HK",                  "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CFE_TIME_CMD_NOOP", "CFE_TIME_CMD_RESET_COUNTERS",   "CFE_TIME_HK",                 "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CI_LAB_CMD_NOOP",   "CI_LAB_CMD_RESET_COUNTERS",     "CI_LAB_HK",                   "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["TO_LAB_CMD_NOOP",   "TO_LAB_CMD_RESET_COUNTERS",     "TO_LAB_HK",                   "COMMAND_COUNTER" ])
        fsw_apps_to_test.append(["CF_CMD_NOOP",       "CF_CMD_RESET",                  "CF_HK",                       "COUNTERS_CMD" ])
        fsw_apps_to_test.append(["BPNODE_CMD_NOOP",   "BPNODE_CMD_RESET_ALL_COUNTERS", "BPNODE_NODE_MIB_COUNTERS_HK", "BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT" ])
        # For each app in the list, verify its alive by sending a noop, then reset the hk counters
        for noop_cmd_name, reset_counters_cmd_name, hk_packet_name, hk_cmd_count_name in fsw_apps_to_test:
            # Group.print(f"Testing <%= target_name %> with: {noop_cmd_name}, {reset_counters_cmd_name}, {hk_packet_name}, {hk_cmd_count_name}.")
            # Verify that we have a recent packet (by waiting for a new one to arrive)
            wait_check_packet(f"<%= target_name %>", f"{hk_packet_name}", 1, 6)
            # Assuming no one else is sending commands, grab the latest command count
            cmd_count = tlm(f"<%= target_name %> {hk_packet_name} {hk_cmd_count_name}")
            # Check accepted NOOP command proving application is up and running
            cmd(f"<%= target_name %> {noop_cmd_name}")
            wait_check(f"<%= target_name %> {hk_packet_name} {hk_cmd_count_name} == {cmd_count+1}", 10)
            # Check accepted Reset Counters command
            cmd(f"<%= target_name %> {reset_counters_cmd_name}")
            wait_check(f"<%= target_name %> {hk_packet_name} {hk_cmd_count_name} == 0", 10)

    def setup(self):
        """
        Test Group Setup
        - Runs when Group Setup button is pressed
        - Runs before all scripts when Group Start is pressed
        """
        pass

    def teardown(self):
        """
        Test Group Setup
        - Runs when Group Teardown button is pressed
        - Runs after all scripts when Group Start is pressed
        """
        pass
