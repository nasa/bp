from openc3.script.suite import Group

class integration_test_dtn_time(Group):
    """
    Test case for DTN time. It jams cFE Time with an STCF and sets the clock state to
    valid in order to allow BPLib's Time Management to calculate a correlation factor for
    a valid DTN timestamp. It then verifies that timestamp's accuracy.
    """

    def test_dtn_time(self):
        """
        Nominal test
        """

        wait_check_packet(f"<%= target_name %>", "CFE_TIME_HK", 1, 6)

        # New STCF = Current Ground Time - Current MET
        # This ignores subseconds, since COSMOS time only has a granularity of seconds
        cfe_time_met = tlm(f"<%= target_name %> CFE_TIME_HK SECONDS_MET")
        pkt_recv_time = tlm(f"<%= target_name %> CFE_TIME_HK RECEIVED_TIMESECONDS")

        new_stcf = int(pkt_recv_time - cfe_time_met)

        # Send new STCF (subseconds are 0 by default)
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STCF with SECONDS {new_stcf}")

        # Set the spacecraft clock state to valid
        cmd(f"<%= target_name %> CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")

        # Verify new cFE time values
        wait_check(f"<%= target_name %> CFE_TIME_HK CLOCK_STATE_API == 'VALID'", 10)
        wait_check(f"<%= target_name %> CFE_TIME_HK SECONDS_STCF == {new_stcf}", 10)
        
        # Verify BPNode's CF has updated as expected
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK TIME_CF != 0", 10)
        
        # Convert DTN time to UNIX epoch (add 946684800000) in seconds in order to compare
        # to packet receive time
        dtn_time = (int(tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK DTN_TIME_RAW")) + \
                    946684800000) / 1000

        pkt_recv_time = int(tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK RECEIVED_TIMESECONDS"))

        time_delta = dtn_time - pkt_recv_time

        # Check that the DTN timestamp and the packet receive timestamp are equal +/- 1 second
        check_expression(f"{time_delta} <= 1 and {time_delta} >= -1")

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
