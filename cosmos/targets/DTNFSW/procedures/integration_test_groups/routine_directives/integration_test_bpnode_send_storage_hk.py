from openc3.script.suite import Group
<% require 'dtnfsw_globals.rb' %>
class integration_test_bpnode_send_storage_hk(Group):
    """
    Test cases for the BPNode "send_storage_hk" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_send_storage_hk(self):
        """
        Nominal test
        """

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_STORAGE_HK")

        # Wait for requested packet to be received
        wait_check_packet("<%= target_name %>", "BPNODE_STORAGE_HK", 1, 10)

        # No memory should be in use when contacts aren't started
        wait_check(f'<%= target_name %> BPNODE_STORAGE_HK BYTES_MEM_IN_USE == 0', 10)

        # Most of the maximum allowed memory should be free when contacts aren't started
        wait_check(f'<%= target_name %> BPNODE_STORAGE_HK BYTES_MEM_FREE > {(.9 * <%= $dtnfsw_globals_mem_pool_len %>)}', 10)

        # Storage should be completely free when the contacts haven't been started
        wait_check(f'<%= target_name %> BPNODE_STORAGE_HK KB_BUNDLES_IN_STOR == 0', 10)

    def setup(self):
        """
        Test Group Setup
        - Runs when Group Setup button is pressed
        - Runs before all scripts when Group Start is pressed
        """

        contact_num = 0

        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Stop contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_STOP with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STOPPED"', 10)

        # Tear down contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "TORNDOWN"', 10)

    def teardown(self):
        """
        Test Group Setup
        - Runs when Group Teardown button is pressed
        - Runs after all scripts when Group Start is pressed
        """

        contact_num = 0

        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_SETUP with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "SETUP"', 10)

        # Start contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_START with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == 0", 10)
