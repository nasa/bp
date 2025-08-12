from openc3.script.suite import Group

class integration_test_bpnode_reset_error_counters(Group):
    """
    Test cases for the BPNode "reset_error_counters" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_reset_error_counters(self):
        """
        Nominal test
        """

        # Store the previous counter val, used to calculated expected/next value
        current_invalid_cmd_count = tlm("<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT")
        expected_invalid_cmd_count = current_invalid_cmd_count + 1

        # Send an invalid command
        cmd("<%= target_name %> BPNODE_CMD_NOOP with CCSDS_FC 255")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT == {expected_invalid_cmd_count}", 10)

        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ERROR_COUNTERS with MIB_ARRAY_INDEX 3")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT == 0", 10)

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
        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == 0", 10)
