from openc3.script.suite import Group

class integration_test_bpnode_send_source_mib_counters_hk(Group):
    """
    Test cases for the BPNode "send_source_mib_counters_hk" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_send_source_mib_counters_hk(self):
        """
        Nominal test
        """
        
        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_SOURCE_MIB_COUNTERS_HK")

        # Wait for requested packet to be received
        wait_check_packet("<%= target_name %>", "BPNODE_SOURCE_MIB_COUNTERS_HK", 1, 10)

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
