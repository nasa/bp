from openc3.script.suite import Group

class integration_test_bpnode_configuration_update(Group):
    """
    Test cases for a BPNode wake up cycle and how it modifies the configurations
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_configuration_update(self):
        """
        Nominal test
        """

        load_new_table("/cf/bpnode_adup.tbl")
        load_new_table("/cf/bpnode_channel.tbl")
        load_new_table("/cf/bpnode_contacts.tbl")
        load_new_table("/cf/bpnode_crs.tbl")
        load_new_table("/cf/bpnode_custodia.tbl")
        load_new_table("/cf/bpnode_custody.tbl")
        load_new_table("/cf/bpnode_latency.tbl")
        load_new_table("/cf/bpnode_mib_pn.tbl")
        load_new_table("/cf/bpnode_mib_ps.tbl")
        load_new_table("/cf/bpnode_reportto.tbl")
        load_new_table("/cf/bpnode_src_auth.tbl")
        load_new_table("/cf/bpnode_storage.tbl")

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
    