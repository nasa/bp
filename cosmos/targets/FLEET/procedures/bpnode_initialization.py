#
# Procedure: Initialize BPNode telemetry and set cFS time
# 
# Example usage: bpnode_initialization("DTNFSW-2")
#
def bpnode_initialization(target = None):

    if target is None:
        target = ask("Enter target name:")
        
    ## Get host IP address
    host_ip_raw = os.environ['HOST_IP_ADDRESS']
    host_ip_address = host_ip_raw.replace ("ip-", "").replace ("-", ".")
    print("Host IP:", host_ip_address)

    ## Establish telemetry downlink from cFS
    cmd(f"{target} TO_LAB_CMD_ENABLE_OUTPUT with DEST_IP {host_ip_address}")
    wait(10)

    wait_check_packet(f"{target}", "CFE_TIME_HK", 1, 6)

    ## Calculate new STCF
    cfe_time_met = tlm(f"{target} CFE_TIME_HK SECONDS_MET")
    pkt_recv_time = tlm(f"{target} CFE_TIME_HK RECEIVED_TIMESECONDS")
    new_stcf = int(pkt_recv_time - cfe_time_met)

    ## Send commands to set new STCF and set clock state to valid
    cmd(f"{target} CFE_TIME_CMD_SET_STCF with SECONDS {new_stcf}")
    cmd(f"{target} CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")

    wait_check(f"{target} CFE_TIME_HK CLOCK_STATE_API == 'VALID'", 6)
    wait_check(f"{target} CFE_TIME_HK SECONDS_STCF == {new_stcf}", 6)
