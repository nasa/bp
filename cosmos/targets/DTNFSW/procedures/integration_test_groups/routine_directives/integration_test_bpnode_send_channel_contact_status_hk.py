from openc3.script.suite import Group

class integration_test_bpnode_send_channel_contact_stat_hk(Group):
    """
    Test cases for the BPNode "send_channel_contact_stat_hk" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_send_channel_contact_stat_hk(self):
        """
        Nominal test
        """

        # Send the command
        cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")

        # Wait for requested packet to be received
        wait_check_packet("<%= target_name %>", "BPNODE_CHAN_CON_STAT_HK", 1, 10)

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
    