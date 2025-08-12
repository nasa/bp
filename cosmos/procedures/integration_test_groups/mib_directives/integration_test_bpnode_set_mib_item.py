from openc3.script.suite import Group

class integration_test_bpnode_set_mib_item(Group):
    """
    Test cases for the BPNode "set_mib_item" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_set_mib_item_node(self):
        """
        Nominal test for setting a Node MIB config
        """
        instance_node_num = <%= $dtnfsw_globals_channel_dest_eid_node %>
        new_lifetime = 1234

        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_SET_MIB_ITEM with EID_MaxNode {instance_node_num}, MIB_ITEM 'PARAM_SET_MAX_LIFETIME', VALUE {new_lifetime}")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK PARAM_SET_MAX_LIFETIME == {new_lifetime}", 10)

    def test_bpnode_set_mib_item_source(self):
        """
        Nominal test for setting a Source MIB config. TODO 7.1
        """
        pass

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
