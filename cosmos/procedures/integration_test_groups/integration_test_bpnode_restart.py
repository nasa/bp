from openc3.script.suite import Group

class integration_test_bpnode_restart(Group):
    """
    Test case for restarting BPNode and validating that it restarts with the right state
    """

    def test_bpnode_restart(self):
        """
        Nominal test
        """
        # Verify that contacts are all started
        # They should already be started at this point in the integration tests
        for contact_num in range(<%= $dtnfsw_globals_num_contacts %>):
            wait_check(f'DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STARTED"', 10)

        # Verify that channel 0 is started (should have been loaded automatically)
        wait_check(f"DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_0 == 'STARTED'", 10)

        # Add channel 1 application

        current_valid_cmd_count = tlm(f"DTNFSW-1 BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        cmd(f"DTNFSW-1 BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
        wait_check(f"DTNFSW-1 BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        wait_check(f"DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'ADDED'", 10)

        # Start channel 1 application

        current_valid_cmd_count = tlm(f"DTNFSW-1 BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        cmd(f"DTNFSW-1 BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
        wait_check(f"DTNFSW-1 BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        wait_check(f"DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'", 10)

        # Restart BPNode

        current_valid_cmd_count = tlm(f"DTNFSW-1 CFE_ES_HK COMMAND_COUNTER")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        cmd("DTNFSW-1 CFE_ES_CMD_RESTART_APP with APPLICATION 'BPNODE'")
        wait_check(f"DTNFSW-1 CFE_ES_HK COMMAND_COUNTER == {expected_valid_cmd_count}", 10)

        # Give BPNode 10 seconds to restart
        wait(10)

        # Verify that contact 0 is torndown, channel 0 is started automatically,  
        # channel 1 is removed on a restart, and counters were reset
        wait_check(f'DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == "TORNDOWN"', 10)
        wait_check(f"DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_0 == 'STARTED'", 10)
        wait_check(f"DTNFSW-1 BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 10)
        wait_check(f"DTNFSW-1 BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == 0", 10)

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

        # Restart all contacts
        for contact_num in range(<%= $dtnfsw_globals_num_contacts %>):

            # Set up contact
            current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
            expected_valid_cmd_count = current_valid_cmd_count + 1
            cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_SETUP with CONTACT_ID {contact_num}')
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

            # Check the run state for the contact
            cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
            wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "SETUP"', 10)

            # Start contact
            current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
            expected_valid_cmd_count = current_valid_cmd_count + 1
            cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_START with CONTACT_ID {contact_num}')
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

            # Check the run state for the contact
            cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
            wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STARTED"', 10)


        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == 0", 10)
