from openc3.tools.test_runner.test import Test, TestSuite, SkipTestCase

load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0001_Startup_Shutdown.py")
load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0401_Bundle_Creation.py")
load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0421_Bundle_Egress.py")
load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0441_Bundle_Ingress.py")
load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0461_ADU_Delivery.py")
load_utility(f"<%= target_name %>/procedures/build_tests/DTN_FSW_BD_0601_Lifetime_Expiration.py")

class BuildTestSuite(TestSuite):
    def __init__(self):
        super().__init__()
        self.add_test(DTN_FSW_BD_0001_Startup_Shutdown)
        self.add_test(DTN_FSW_BD_0401_Bundle_Creation) 
        self.add_test(DTN_FSW_BD_0421_Bundle_Egress)
        self.add_test(DTN_FSW_BD_0441_Bundle_Ingress)
        self.add_test(DTN_FSW_BD_0461_ADU_Delivery)
        self.add_test(DTN_FSW_BD_0601_Lifetime_Expiration)
        
        self.running_scripts = []

    def setup(self):
        import os
        from datetime import datetime

        print("Getting host IP adress")
        host_ip_raw = os.environ['HOST_IP_ADDRESS']
        host_ip_address = host_ip_raw.replace ("ip-", "").replace ("-", ".")
        print("Host IP:", host_ip_address)

        print("Sending command for cFS to start downlinking telemetry")
        cmd(f"<%= target_name %> TO_LAB_CMD_ENABLE_OUTPUT with DEST_IP {host_ip_address}")
        wait(10)
        
        print("Verifying that telemetry is received")
        wait_check_packet(f"<%= target_name %>", "CFE_TIME_HK", 1, 6)

        print("Calculating new STCF")
        # New STCF = Current Ground Time - Current MET
        # This ignores subseconds, since COSMOS time only has a granularity of seconds
        cfe_time_met = tlm(f"<%= target_name %> CFE_TIME_HK SECONDS_MET")
        pkt_recv_time = tlm(f"<%= target_name %> CFE_TIME_HK RECEIVED_TIMESECONDS")

        new_stcf = int(pkt_recv_time - cfe_time_met)

        print("Sending command to set new STCF")
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STCF with SECONDS {new_stcf}")

        print("Sending command to set the spacecraft clock state to valid")
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")

        print("Verifying that time has been set")
        wait_check(f"<%= target_name %> CFE_TIME_HK CLOCK_STATE_API == 'VALID'", 6)
        wait_check(f"<%= target_name %> CFE_TIME_HK SECONDS_STCF == {new_stcf}", 6)
        
        print("Starting event logger")
        curtime = str(datetime.now()).replace('-','').replace(':','').replace(' ','_')[:-7]
        eventlog_filename = "/eventlogs/eventlog_" + curtime + ".txt"
        print ("Eventlog filename: ", eventlog_filename)
        stash_set('eventlog', eventlog_filename)
        id = script_run(f"<%= target_name %>/procedures/write_event_log.py")
        self.running_scripts.append(id)

    def teardown(self):
        print("Teardown")
        
        # Stop running scripts
        for id in running_scripts:
            self.running_script_stop(id)

