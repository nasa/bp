from openc3.tools.test_runner.test import Test, SkipTestCase

# Test case for Bundle Egress

load_utility("DTNFSW-1/procedures/build_tests/bundle_egress_nominal.py")


class DTN_FSW_BD_0421_Bundle_Egress(Test):
    def setup(self):
        print("Setup")
        # Use this function for any setup specific to this test case (delete if not used)

    def test_1_nominal(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        
        bundle_egress_nominal(self)

    def teardown(self):
        print("Teardown")
        # Use this function for any teardown specific to this test case (delete if not used)
