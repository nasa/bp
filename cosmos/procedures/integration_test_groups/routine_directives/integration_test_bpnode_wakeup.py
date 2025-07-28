from openc3.script.suite import Group

# Group class name should indicate what the scripts are testing
class integration_test_bpnode_wakeup(Group):
    """
    Test cases for the BPNode "wakeup" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_wakeup(self):
        """
        Nominal test
        """
        Group.print("Skipping unimplemented test case!")

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
