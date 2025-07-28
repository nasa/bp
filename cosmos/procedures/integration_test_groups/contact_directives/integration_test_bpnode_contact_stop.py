from openc3.script.suite import Group

class integration_test_bpnode_contact_stop(Group):
    """
    Test cases for the BPNode "contact_stop" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_contact_stop(self):
        """
        Nominal test
        """
        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_CONTACT_STOP")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == "STOPPED"', 10)

    def setup(self):
        """
        Test Group Setup
        - Runs when Group Setup button is pressed
        - Runs before all scripts when Group Start is pressed
        """
        
        # Enable debug events
        cmd("<%= target_name %> CFE_EVS_CMD_ENABLE_EVENT_TYPE with BIT_MASK 0xF")

    def teardown(self):
        """
        Test Group Setup
        - Runs when Group Teardown button is pressed
        - Runs after all scripts when Group Start is pressed
        """
        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Start contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_START with CONTACT_ID 0')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == "STARTED"', 10)

        # Disable debug events
        cmd("<%= target_name %> CFE_EVS_CMD_DISABLE_EVENT_TYPE with BIT_MASK 0x1")

        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == 0", 10)
