from openc3.tools.test_runner.test import Test, SkipTestCase

# Test case for Lifetime Expiration

class DTN_FSW_BD_0601_Lifetime_Expiration(Test):
    def setup(self):
        print("Setup")
        # Use this function for any setup specific to this test case (delete if not used)

    def test_1_TBD(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        raise SkipTestCase

    def teardown(self):
        print("Teardown")
        # Use this function for any teardown specific to this test case (delete if not used)
