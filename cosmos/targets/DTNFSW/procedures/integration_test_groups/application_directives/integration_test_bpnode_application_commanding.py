from openc3.script.suite import Group

class integration_test_bpnode_application_commanding(Group):
    """
    Test case for nominal application commanding. This tests the add-application, 
    start-application, and stop-application directives
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_start_application_commanding(self):
        """
        Nominal test
        """

        ##################################################################################
        # Add channel 1 application configurations
        ##################################################################################

        # Check that initial application state is REMOVED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 10)
        
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Add channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to ADDED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'ADDED'", 10)

        ##################################################################################
        # Start channel 1 application and verify ADU ingest/loopback
        ##################################################################################

        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Start channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to STARTED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'", 10)

        current_valid_adu_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
        expected_valid_adu_count = current_valid_adu_count + 1
        current_adu_delvr_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED")
        expected_adu_delvr_count = current_adu_delvr_count + 1

        current_adu_pkt_count = tlm(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT")
        if current_adu_pkt_count is None:
            current_adu_pkt_count = 0        
        exp_adu_pkt_count = current_adu_pkt_count + 1

        # Send the command to generate an ADU for channel 1 to ingest
        cmd(f"<%= target_name %> CFE_TIME_CMD_SEND_DIAGNOSTIC")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED == {expected_valid_adu_count}", 10)
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED == {expected_adu_delvr_count}", 10)


        # Wait for one ADU out packet to be received by COSMOS
        wait_check(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT == {exp_adu_pkt_count}", 10)

        recvd_adu_len = tlm(f"<%= target_name %> DTN_ADU_OUT CCSDS_LENGTH")
        exp_adu_len = tlm(f"<%= target_name %> CFE_TIME_DIAG CCSDS_LENGTH") + 16

        # Verify that the length stated in the header matches the relevant packet plus
        # 16 bytes for the telemetry header wrapping
        check_expression(f"'{recvd_adu_len}' == '{exp_adu_len}'")

        ##################################################################################
        # Stop channel 1 application and verify no ADUs are being ingested
        ##################################################################################

        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Stop channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to STOPPED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STOPPED'", 10)

        # Send the command to generate an ADU that channel 1 should ignore
        cmd(f"<%= target_name %> CFE_TIME_CMD_SEND_DIAGNOSTIC")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED == {expected_valid_adu_count}", 10)

        ##################################################################################
        # Restart channel 1 application to verify you can restart a stopped application
        ##################################################################################

        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Start channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to STARTED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'", 10)

        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Stop channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to STOPPED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STOPPED'", 10)

        ##################################################################################
        # Remove channel 1 application
        ##################################################################################

        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Remove channel 1 application
        cmd(f"<%= target_name %> BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Check that application state has changed to REMOVED
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 10)

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
