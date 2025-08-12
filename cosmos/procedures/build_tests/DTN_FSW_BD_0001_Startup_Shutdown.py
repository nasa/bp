from openc3.tools.test_runner.test import Test, SkipTestCase

# Test case for Startup & Shutdown

class DTN_FSW_BD_0001_Startup_Shutdown(Test):
    def setup(self):
        print("Setup")
        # Use this function for any setup specific to this test case (delete if not used)
        load_utility (f"<%= target_name %>/procedures/build_tests/test_utils.py")

        global rqmnt_status, pass_check_1, pass_check_2, pass_check_3, pass_check_4, pass_check_5, pass_check_6

        # Initialize requirement status
        rqmnt_status = { 
            "DTN.5.00862":"U", 
            "DTN.5.00880":"U", 
            "DTN.5.00900":"U", 
            "DTN.5.00920":"U", 
            "DTN.5.00940":"U", 
            "DTN.5.00950":"U", 
            "DTN.5.00960":"U", 
            "DTN.5.00970":"U", 
            "DTN.5.00980":"U", 
            "DTN.5.00990":"U", 
            "DTN.5.01000":"U", 
            "DTN.5.01040":"U", 
            "DTN.5.01042":"U", 
            "DTN.5.01052":"U", 
            "DTN.5.01053":"U", 
            "DTN.5.01060":"U",
            "DTN.ES.04000":"U", 
            "DTN.ES.04050":"U",
            "DTN.ES.04060":"U", 
            "DTN.ES.04070":"U", 
            "DTN.ES.04090":"U", 
            "DTN.ES.04140":"U", 
            "DTN.ES.04160":"U",
            "DTN.ES.04180":"U",
            "DTN.ES.04190":"U",
            "DTN.ES.04200":"U",
            "DTN.6.03161":"U",
            "DTN.6.04640":"U",
            "DTN.6.07180":"U",
            "DTN.6.12060":"U",
            "DTN.6.12910":"U",
            "DTN.6.12990":"U",
            "DTN.6.13110":"U",
            "DTN.6.13120":"U",
            "DTN.6.15158":"U",
            "DTN.6.16000":"U",
            "DTN.6.16010":"U",
            "DTN.6.16022":"U",
            "DTN.6.16032":"U",
            "DTN.6.17000":"U",
            "DTN.6.17020":"U",
            "DTN.6.17030":"U",
            "DTN.6.17040":"U",
            "DTN.6.17050":"U",
            "DTN.6.17111":"U",
            "DTN.6.19060":"U",
            "DTN.6.19080":"U",
            "DTN.6.19100":"U",
            "DTN.6.19110":"U",
            "DTN.6.19120":"U",
            "DTN.6.19130":"U",
            "DTN.6.19160":"U",
            "DTN.6.19170":"U",
            "DTN.6.19180":"U",
            "DTN.6.19190":"U",
            "DTN.6.19200":"U",
            "DTN.6.19260":"U",
            "DTN.6.19320":"U",
            "DTN.6.19340":"U",
            "DTN.6.19370":"U",
            "DTN.6.19380":"U",
            "DTN.6.20100":"U",
            "DTN.6.20130":"U",
            "DTN.6.21030":"U",
            "DTN.6.22120":"U",
            "DTN.6.23087":"U",
            "DTN.6.25040":"U",
            "DTN.6.25060":"U"
        }
        pass_check_1 = False
        pass_check_2 = False
        pass_check_3 = False
        pass_check_4 = False
        pass_check_5 = False
        pass_check_6 = False

    def test_1_bpnode_startup(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        #setup requirements utility tool for startup_shutdown
        #only level , ES and 6.

        # requirements to verify in this section:  DTN.6.10121, DTN.6.16000, DTN.6.19260, DTN.6.19320, DTN.ES.04070, DTN.ES.04140, DTN.ES.04180, DTN.5.00980, DTN.5.01000, DTN.5.01042, DTN.5.01052, DTN.5.01053

        print("Verifying that BPNode Telemetry Packets are received")
        pass_check_1 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_CONFIG_HK", 1, 6)
        pass_check_2 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_COUNTERS_HK", 1, 6)
        pass_check_3 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_REPORTS_HK", 1, 6)
        pass_check_4 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_CONFIG_HK", 1, 6)
        pass_check_5 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_COUNTERS_HK", 1, 6)
        pass_check_6 = wait_packet(f"<%= target_name %>", "BPNODE_STORAGE_HK", 1, 6)
    
        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4 and pass_check_5 and pass_check_6):
            status = "P"
        else:
            status = "F"

        for rqmnt in [
            "DTN.5.00940", "DTN.6.12990", "DTN.6.13110", "DTN.6.19370",
            "DTN.6.19380", "DTN.6.20100", "DTN.6.21030", "DTN.6.25060"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

        
        print("Verifying that BPNode Telemetry Packets are timestamped")
        pass_check_1 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_CONFIG_HK", 1, 6)
        previous_BPNODE_NODE_MIB_CONFIG_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_NODE_MIB_CONFIG_HK", "SECONDS")
        pass_check_1 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_CONFIG_HK", 1, 6)
        pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK SECONDS > {previous_BPNODE_NODE_MIB_CONFIG_HK_seconds}",4)

        if pass_check_1:
            print("BPNODE_NODE_MIB_CONFIG_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_NODE_MIB_CONFIG_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK PACKET_TIMEFORMATTED"))
        
        pass_check_2 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_COUNTERS_HK", 1, 6)
        previous_BPNODE_NODE_MIB_COUNTERS_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_NODE_MIB_COUNTERS_HK", "SECONDS")
        pass_check_2 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_COUNTERS_HK", 1, 6)
        pass_check_2 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK SECONDS > {previous_BPNODE_NODE_MIB_COUNTERS_HK_seconds}",4)

        if pass_check_2:
            print("BPNODE_NODE_MIB_COUNTERS_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_NODE_MIB_COUNTERS_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK PACKET_TIMEFORMATTED"))
        
        pass_check_3 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_REPORTS_HK", 1, 6)
        previous_BPNODE_NODE_MIB_REPORTS_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_NODE_MIB_REPORTS_HK", "SECONDS")
        pass_check_3 = wait_packet(f"<%= target_name %>", "BPNODE_NODE_MIB_REPORTS_HK", 1, 6)
        pass_check_3 = wait(f"<%= target_name %> BPNODE_NODE_MIB_REPORTS_HK SECONDS > {previous_BPNODE_NODE_MIB_REPORTS_HK_seconds}",4)

        if pass_check_3:
            print("BPNODE_NODE_MIB_REPORTS_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_NODE_MIB_REPORTS_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_NODE_MIB_REPORTS_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_NODE_MIB_REPORTS_HK PACKET_TIMEFORMATTED"))
        
        pass_check_4 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_CONFIG_HK", 1, 6)
        previous_BPNODE_SOURCE_MIB_CONFIG_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_SOURCE_MIB_CONFIG_HK", "SECONDS")
        pass_check_4 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_CONFIG_HK", 1, 6)
        pass_check_4 = wait(f"<%= target_name %> BPNODE_SOURCE_MIB_CONFIG_HK SECONDS > {previous_BPNODE_SOURCE_MIB_CONFIG_HK_seconds}",4)

        if pass_check_4:
            print("BPNODE_SOURCE_MIB_CONFIG_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_CONFIG_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_SOURCE_MIB_CONFIG_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_CONFIG_HK PACKET_TIMEFORMATTED"))
        
        pass_check_5 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_COUNTERS_HK", 1, 6)
        previous_BPNODE_SOURCE_MIB_COUNTERS_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_SOURCE_MIB_COUNTERS_HK", "SECONDS")
        pass_check_5 = wait_packet(f"<%= target_name %>", "BPNODE_SOURCE_MIB_COUNTERS_HK", 1, 6)
        pass_check_5 = wait(f"<%= target_name %> BPNODE_SOURCE_MIB_COUNTERS_HK SECONDS > {previous_BPNODE_SOURCE_MIB_COUNTERS_HK_seconds}",4)

        if pass_check_5:
            print("BPNODE_SOURCE_MIB_COUNTERS_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_COUNTERS_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_SOURCE_MIB_COUNTERS_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_COUNTERS_HK PACKET_TIMEFORMATTED"))
        
        pass_check_6 = wait_packet(f"<%= target_name %>", "BPNODE_STORAGE_HK", 1, 6)
        previous_BPNODE_STORAGE_HK_seconds = tlm(f"<%= target_name %>", "BPNODE_STORAGE_HK", "SECONDS")
        pass_check_6 = wait_packet(f"<%= target_name %>", "BPNODE_STORAGE_HK", 1, 6)
        pass_check_6 = wait(f"<%= target_name %> BPNODE_STORAGE_HK SECONDS > {previous_BPNODE_STORAGE_HK_seconds}",4)

        if pass_check_6:
            print("BPNODE_STORAGE_HK DTN_TIME_FORMATTED Incrementing,", tlm(f"<%= target_name %> BPNODE_STORAGE_HK PACKET_TIMEFORMATTED"))
        else:
            print("BPNODE_STORAGE_HK DTN_TIME_FORMATTED not Incrementing, FAILED: ", tlm(f"<%= target_name %> BPNODE_STORAGE_HK PACKET_TIMEFORMATTED"))
        
        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4 and pass_check_5 and pass_check_6):
            status = "P"
        else:
            status = "F"

        for rqmnt in ["DTN.5.00950"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        print("Verifying that BPNode Telemetry Packets are received at the expected frequency")
        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK RECEIVED_COUNT")
        wait (12)
        pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_1:
            print("BPNODE_NODE_MIB_COUNTERS_HK received at 4 second interval")
        else:
            print("BPNODE_NODE_MIB_COUNTERS_HK FAILED the 4 second interval check")

        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK RECEIVED_COUNT")
        wait (12)
        pass_check_2 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_2:
            print("BPNODE_NODE_MIB_CONFIG_HK received at 4 second interval")
        else:
            print("BPNODE_NODE_MIB_CONFIG_HK FAILED the 4 second interval check")
                
        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_NODE_MIB_REPORTS_HK RECEIVED_COUNT")
        wait (12)
        pass_check_3 = wait(f"<%= target_name %> BPNODE_NODE_MIB_REPORTS_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_3:
            print("BPNODE_NODE_MIB_REPORTS_HK received at 4 second interval")
        else:
            print("BPNODE_NODE_MIB_REPORTS_HK FAILED the 4 second interval check")
                
        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_COUNTERS_HK RECEIVED_COUNT")
        wait (12)
        pass_check_4 = wait(f"<%= target_name %> BPNODE_SOURCE_MIB_COUNTERS_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_4:
            print("BPNODE_SOURCE_MIB_COUNTERS_HK received at 4 second interval")
        else:
            print("BPNODE_SOURCE_MIB_COUNTERS_HK FAILED the 4 second interval check")
                 
        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_SOURCE_MIB_CONFIG_HK RECEIVED_COUNT")
        wait (12)
        pass_check_5 = wait(f"<%= target_name %> BPNODE_SOURCE_MIB_CONFIG_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_5:
            print("BPNODE_SOURCE_MIB_CONFIG_HK received at 4 second interval")
        else:
            print("BPNODE_SOURCE_MIB_CONFIG_HK FAILED the 4 second interval check")
        
        bp_packet_count_t1 = tlm(f"<%= target_name %> BPNODE_STORAGE_HK RECEIVED_COUNT")
        wait (12)
        pass_check_6 = wait(f"<%= target_name %> BPNODE_STORAGE_HK RECEIVED_COUNT == {bp_packet_count_t1 + 3}", 2)
        if pass_check_6:
            print("BPNODE_STORAGE_HK received at 4 second interval")
        else:
            print("BPNODE_STORAGE_HK FAILED the 4 second interval check")

        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4 and pass_check_5 and pass_check_6):
            status = "P"
        else:
            status = "F"    
        
        for rqmnt in [
            "DTN.5.00960", "DTN.5.00970", "DTN.5.01040", "DTN.6.03161", 
            "DTN.6.04640", "DTN.6.07180", "DTN.6.13110", "DTN.6.13120", 
            "DTN.6.15158", "DTN.6.16032", "DTN.6.17111",  "DTN.6.20100", 
            "DTN.6.20130", "DTN.6.22120", "DTN.6.23087", "DTN.6.25060", 
            "DTN.ES.04060","DTN.ES.04200"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)
    
    def test_2_bpnode_invalid_time(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        print("Sending a series of Invalid Time and Valid Time Commands and verifying that the correlation factor changes")
        # send command to make clock state INVALID, check CLOCK_STAT_API - Invalid, check Correlation Factor = 0
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'INVALID'")
        pass_check_1 = wait(f"<%= target_name %> CFE_TIME_HK CLOCK_STATE_API == 'INVALID'", 6)
        pass_check_2 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK TIME_CF == 0", 6)
        if (pass_check_1 and pass_check_2):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in [
            "DTN.6.17030", "DTN.6.17040"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)
        # send command to make clock state INVALID, check CLOCK_STAT_API - Valid, check Correlation Factor <> 0
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")
        pass_check_3 = wait(f"<%= target_name %> CFE_TIME_HK CLOCK_STATE_API == 'VALID'", 6)
        pass_check_4 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK TIME_CF > 0", 6)
        
        if (pass_check_3 and pass_check_4):
            status = "P"
        else:
            status = "F"
        for rqmnt in [
            "DTN.6.17020"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4):
            status = "P"
        else:
            status = "F" 
        for rqmnt in [
            "DTN.5.01060", "DTN.ES.04090","DTN.6.17000"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

        #check the number of second in DTN time versus CF_time, if the same DTN.6.17050 is verified.
        pass_check_1 = wait(f"<%= target_name %> CFE_TIME_HK CLOCK_STATE_API == 'VALID'", 6)
        pass_check_2 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK TIME_CF > 0", 6)
        #pass_check_3 = wait(f"<%= target_name %> BPNODE_NODE_MIB_CONFIG_HK SECONDS =  <%= target_name %> BPNODE_NODE_MIB_CONFIG_HK SECONDS",4)
        DTN_Current_time = tlm(f"<%= target_name %>  BPNODE_NODE_MIB_COUNTERS_HK DTN_TIME_FORMATTED")
        CFS_Current_pkt_time = tlm(f"<%= target_name %>  BPNODE_NODE_MIB_COUNTERS_HK PACKET_TIMEFORMATTED")
        #CFS_Current_pkt_time = CFS_Cur_pkt_time_corrected.lstrip()
        CFS_Cur_pkt_time_corrected = (CFS_Current_pkt_time[0:4]+ "-" + CFS_Current_pkt_time[5:7] + "-" + CFS_Current_pkt_time[8:19])
        print("DTN_Current_time: ", DTN_Current_time[0:19], " Type: ", type(DTN_Current_time))
        print("CFS_Curent_pkt_time: ", CFS_Current_pkt_time, " Type: ", type(CFS_Current_pkt_time)) 
        print("CFS_Cur_pkt_time_corrected; ", CFS_Cur_pkt_time_corrected, " Type: ", type(CFS_Cur_pkt_time_corrected)) 
        if DTN_Current_time[0:19] == CFS_Cur_pkt_time_corrected:
            pass_check_3 = True
        else:
            pass_check_3 = False
   
        if (pass_check_1 and pass_check_2 and pass_check_3):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in ["DTN.6.17050"]:
            TestUtils.set_requirement_status(rqmnt, status)

    def test_3_bpnode_startup_noop_cmd(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        
        print("Sending and verifying execution of CFE_EVS_CMD_NOOP directive to activate CFE_EVS_LONG_EVENT_MSG Telemetry")
        cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
        print("cfe_received_event_count = ", cfe_received_event_count)
        cmd(f"<%= target_name %> CFE_EVS_CMD_NOOP")
        wait (2)
    
        cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
        bp_accepted_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        print("Sending and verifying execution of BPNODE No-Op directive")
        cmd(f"<%= target_name %> BPNODE_CMD_NOOP")
        
        pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {bp_accepted_cmd_count + 1}", 6)
        
        print("CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT = ", tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT"), " cfe_received_event_count = ", cfe_received_event_count)
        print("Verifying that the BPNODE No-Op directive execution generates an informational event message") 
        pass_check_2 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT == { cfe_received_event_count + 2}", 6)
        pass_check_3 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'BPNODE'", 6)
        #wait_check(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 3", 6) correct for BP_NOOP, but not second message which is ID 501, once second message is removed will restore the code.
        pass_check_4 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 501", 6)
        pass_check_5 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'INFO'", 6)
        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4 and pass_check_5):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in [
            "DTN.5.00862", "DTN.5.00900", "DTN.5.00980", 
            "DTN.5.01000", "DTN.5.01040", "DTN.5.01053", "DTN.6.16000", 
            "DTN.6.19060", "DTN.6.19180", "DTN.6.19190", "DTN.6.19200", 
            "DTN.6.19320","DTN.6.19340", "DTN.ES.04050", "DTN.ES.04070",
            "DTN.ES.04140", "DTN.ES.04180", "DTN.ES.04190"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

    def test_4_bpnode_disable_enable_event_msg(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        print("Sending a series of Invalid BPNODE Commands and verifying that the BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT increments")    
        print("Sending directive to modify event message threshold to suppress BPNODE INFO events")
        cmd(f"<%= target_name %> CFE_EVS_CMD_DISABLE_APP_EVENT_TYPE with APP_NAME 'BPNODE', BIT_MASK 14")
        
        print("Sending and verifying execution of BPNODE No-Op directive")
        bp_accepted_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
        cmd(f"<%= target_name %> BPNODE_CMD_NOOP")
        pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {bp_accepted_cmd_count + 1}", 6)
        
        print("Verifying that the BPNODE No-Op directive execution DOES NOT generate an informational event message") 
        pass_check_2 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT == { cfe_received_event_count}", 6)
        
        print("Sending directive to modify event message threshold to re-enable BPNODE INFO events")
        cmd(f"<%= target_name %> CFE_EVS_CMD_ENABLE_APP_EVENT_TYPE with APP_NAME 'BPNODE', BIT_MASK 14")
        
        print("Sending and verifying execution of BPNODE No-Op directive")
        bp_accepted_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
        cmd(f"<%= target_name %> BPNODE_CMD_NOOP")
        pass_check_3 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {bp_accepted_cmd_count + 1}", 6)

        print("Verifying that the BPNODE No-Op directive execution DOES generate an informational event message") 
        pass_check_4 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT == { cfe_received_event_count + 2}", 6)

        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4):
            status = "P"
        else:
            status = "F" 
            
        for rqmnt in ["DTN.5.00990", "DTN.ES.04000"]:
            TestUtils.set_requirement_status(rqmnt, status)
                
        
    def test_5_bpnode_invalid_commands(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        print("Sending a series of Invalid BPNODE Commands and verifying that the BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT increments")

        cmd_list = []
        cmd_list.append("INVALID_LONG_BPNODE_CMD_NOOP")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_VERIFY_BUNDLE_STORAGE") 
        cmd_list.append("INVALID_LONG_BPNODE_CMD_INIT_BUNDLE_STORAGE")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_VERIFY_BUNDLE_METADATA")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_CLEAR_VOLATILE")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_RELOAD_SAVED_DATA")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_RESET_ALL_COUNTERS")
        cmd_list.append("INVALID_SHORT_BPNODE_CMD_RESET_COUNTER")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_RESET_COUNTER")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_RESET_BUNDLE_COUNTERS")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_RESET_ERROR_COUNTERS")
        cmd_list.append("INVALID_SHORT_BPNODE_CMD_ADD_APPLICATION")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_ADD_APPLICATION")
        cmd_list.append("INVALID_SHORT_BPNODE_CMD_REMOVE_APPLICATION")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_REMOVE_APPLICATION")
        cmd_list.append("INVALID_SHORT_BPNODE_CMD_START_APPLICATION" )
        cmd_list.append("INVALID_LONG_BPNODE_CMD_START_APPLICATION")
        cmd_list.append("INVALID_SHORT_BPNODE_CMD_STOP_APPLICATION")
        cmd_list.append("INVALID_LONG_BPNODE_CMD_STOP_APPLICATION")

        num_of_cmd_list = len(cmd_list)

        loop_index = 0
        Failed_an_invalid_cmd = False
        Failed_to_generate_err_msg = False

        while loop_index < num_of_cmd_list:
            previous_BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT =  tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
            previous_BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT")
            cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")

            cmd (f"<%= target_name %> {cmd_list[loop_index]}")

            pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT == ({previous_BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT} + 1)", 6)
            pass_check_2 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {previous_BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT}", 6)
            if (pass_check_1 and pass_check_2):
                print("Generate an invalid command response for the command, ", {cmd_list[loop_index]})
            else:
                Failed_an_invalid_cmd = True
                print("Failed to create a invalid comand responce for the command, ",  {cmd_list[loop_index]})

            pass_check_3 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT == { cfe_received_event_count + 1}", 6)
            pass_check_4= wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'ERROR'",4)
            if (pass_check_3 and pass_check_4):
                print("An Event Message was generate for the invalid command, ", {cmd_list[loop_index]})
            else:
                Failed_to_generate_err_msg = True
                print("Failed to generate an invalid comand response message for the command, ", {cmd_list[loop_index]})

            loop_index += 1

        if not Failed_an_invalid_cmd and not Failed_to_generate_err_msg:
            status = "P"
        else:
            status = "F"

        
        for rqmnt in [
            "DTN.5.00880", "DTN.5.00920", "DTN.5.00980", "DTN.ES.04160",
            "DTN.6.12060", "DTN.6.16010", "DTN.6.16022", "DTN.6.19060",
            "DTN.6.19080", "DTN.6.19100", "DTN.6.19110", "DTN.6.19120",
            "DTN.6.19130", "DTN.6.19160", "DTN.6.19170", "DTN.6.19260"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        #for requirement, DTN.5.01052, need to send a bpnode noop to comfirm support of info messages along with not Failed_to_generate_err_msg, steal code from noop command event check above.
        cfe_received_event_count = tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
        bp_accepted_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        print("Sending and verifying execution of BPNODE No-Op directive")
        cmd(f"<%= target_name %> BPNODE_CMD_NOOP")
        
        pass_check_1 = wait(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {bp_accepted_cmd_count + 1}", 6)
        
        print("CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT = ", tlm(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT"), " cfe_received_event_count = ", cfe_received_event_count)
        print("Verifying that the BPNODE No-Op directive execution generates an informational event message") 
        pass_check_2 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT == { cfe_received_event_count + 2}", 6)
        pass_check_3 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'BPNODE'", 6)
        #wait_check(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 3", 6) correct for BP_NOOP, but not second message which is ID 501, once second message is removed will restore the code.
        pass_check_4 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 501", 6)
        pass_check_5 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'INFO'", 6)
        if (pass_check_1 and pass_check_2 and pass_check_3 and pass_check_4 and pass_check_5 and not Failed_to_generate_err_msg):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in ["DTN.5.01052", "DTN.5.01042"]:
            TestUtils.set_requirement_status(rqmnt, status)

    def test_6_bpnode_checking_free_memory(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        print("Checking the amount of free memory in bytes, to the bpnode application")

        pass_check_1 = wait(f"<%= target_name %> BPNODE_STORAGE_HK BYTES_MEM_FREE >= 15900000",6)

        if (pass_check_1):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in ["DTN.6.25040"]:
            TestUtils.set_requirement_status(rqmnt, status)

    def test_7_bpnode_shutdown(self):
        print(
            f"Running {Test.current_test_suite()}:{Test.current_test()}:{Test.current_test_case()}"
        )
        print("Checking the CFS ES command to shutdown the bpnode application")

        cmd(f"<%= target_name %> CFE_ES_CMD_STOP_APP with APPLICATION 'BPNODE'")
        pass_check_1 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'CFE_ES'", 6)
        pass_check_2 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 8", 6)
        pass_check_3 = wait(f"<%= target_name %> CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'INFO'", 6)


        if (pass_check_1 and pass_check_2 and pass_check_3):
            status = "P"
        else:
            status = "F" 
        
        for rqmnt in ["DTN.6.12910"]:
            TestUtils.set_requirement_status(rqmnt, status)


    def teardown(self):
        print("Teardown")
        # Use this function for any teardown specific to this test case (delete if not used)
        print ("******************************")
        print ("***** Requirement Status *****")
        print ("******************************")
        for key, value in rqmnt_status.items():
            print(f"***    {key}: {value}")
        print ("******************************")

    