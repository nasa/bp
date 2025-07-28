from openc3.script.suite import Group

class integration_test_bpnode_rebuild_bundle_metadata(Group):
    """
    Test cases for the BPNode "rebuild_bundle_metadata" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_rebuild_bundle_metadata(self):
        """
        Nominal test
        """
        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_REBUILD_BUNDLE_METADATA")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

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
