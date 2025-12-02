##******************************************************************************
##
## Long duration test of BPNode in Relay Mode
##
## See Test Case DTN-FSW-cFS-LONG-0001 for setup information
##      
##******************************************************************************

import os
from datetime import datetime

# Setting Script Runner line delay
set_line_delay(0.000)

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")
load_utility("FLEET/procedures/bundles_send.py")

target = "DTNFSW-2"

# Start event logging
curtime = str(datetime.now()).replace('-','').replace(':','').replace(' ','_')[:-7]
eventlog_filename = "/eventlogs/eventlog_" + target + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename)
stash_set('eventlog', eventlog_filename)
id_1 = script_run(f"{target}/procedures/write_event_log.py")
wait(2)

##------------------------------------------------------------------------------
## BPNode Initialization
##------------------------------------------------------------------------------

bpnode_initialization(target)

print("Resetting all counters")
cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Load MIB PN table modified to set node number to 102
load_target_table('/cf/pn_long_rly.tbl', target)

##------------------------------------------------------------------------------
## Test Setup
##------------------------------------------------------------------------------

print("Selecting how many contact loops to execute")
number_of_contact_loops = ask("Choose number of contact loops: ")

payload_size = 1000
print("Bundle payload size (bytes): ", payload_size)

lifetime_in_sec = 3600
print("Bundle lifetime (s): ", lifetime_in_sec)

total_send_loops = 10000
print("Total send loops: ", total_send_loops)

print("Selecting BPNode IP adress")
send_to_ip = ask("Enter BPNode IP address (X.X.X.X): ")

send_to_port = 4556
print("BPNode ingress port: ", send_to_port)

rate_limit = 50
print("Rate Limit (Mbps): ", rate_limit)

expected_bundle_count = 100 * total_send_loops * number_of_contact_loops

##------------------------------------------------------------------------------
## Test Start
##------------------------------------------------------------------------------

loop = 0

Start_Time = datetime.now()
print(f"Test Start Time = {Start_Time}")

while loop < number_of_contact_loops:

    loop = loop + 1

    ##------------------------------------------------------------------------------
    ## Contact 1 (Ingress Only) Setup, Execution, and Teardown
    ##------------------------------------------------------------------------------

    print("Starting Contact Loop", loop)

    ## Load contact table modified to set dest EID to 104
    load_target_table('/cf/con_long_rly_st.tbl', target)

    ## Setup and start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    wait(10)

    ## Execute Bundle Ingress
    bundles_send(payload_size, lifetime_in_sec, total_send_loops, send_to_ip, send_to_port, rate_limit)
    wait(10)

    ## Stop and tear down Contact 1
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    wait(10)

    ## Print counter values
    RLY_BUNDLE_COUNT_RECEIVED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
    print("Relay Node Bundles Received:", RLY_BUNDLE_COUNT_RECEIVED)
    RLY_BUNDLE_COUNT_STORED = tlm(f"{target} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
    print("Relay Node Bundles Stored:", RLY_BUNDLE_COUNT_STORED)
    RLY_BUNDLE_COUNT_FORWARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
    print("Relay Node Bundles Forwarded:", RLY_BUNDLE_COUNT_FORWARDED)

    ##------------------------------------------------------------------------------
    ## Contact 2 (Egress Only) Setup, Execution, and Stop
    ##------------------------------------------------------------------------------

    ## Load contact table modified to set dest EID to 103
    load_target_table('/cf/con_long_rly.tbl', target)

    ## Setup and start Contact 2
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    
    wait(120)

    ## Stop Contact 2
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    wait(10)

    ## Print counter values
    RLY_BUNDLE_COUNT_RECEIVED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
    print("Relay Node Bundles Received:", RLY_BUNDLE_COUNT_RECEIVED)
    RLY_BUNDLE_COUNT_STORED = tlm(f"{target} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
    print("Relay Node Bundles Stored:", RLY_BUNDLE_COUNT_STORED)
    RLY_BUNDLE_COUNT_FORWARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
    print("Relay Node Bundles Forwarded:", RLY_BUNDLE_COUNT_FORWARDED)

    ##------------------------------------------------------------------------------
    ## Contact 3 (Ingress and Egress) Start, Execution, and Teardown
    ##------------------------------------------------------------------------------

    ## Start Contact 3
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    wait(10)

    ## Execute Bundle Ingress and Bundle Egress
    bundles_send(payload_size, lifetime_in_sec, total_send_loops, send_to_ip, send_to_port, rate_limit)
    wait(10)

    ## Stop and tear down Contact 3
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    wait(10)

    ## Print counter values
    RLY_BUNDLE_COUNT_RECEIVED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
    print("Relay Node Bundles Received:", RLY_BUNDLE_COUNT_RECEIVED)
    RLY_BUNDLE_COUNT_STORED = tlm(f"{target} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
    print("Relay Node Bundles Stored:", RLY_BUNDLE_COUNT_STORED)
    RLY_BUNDLE_COUNT_FORWARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
    print("Relay Node Bundles Forwarded:", RLY_BUNDLE_COUNT_FORWARDED)

    print("Stopping Contact Loop", loop)

End_Time = datetime.now()
print(f"Test End Time = {End_Time}")

##-------------------------------------------------------------------------------------------------------------------
## End of Test Verifications
##-------------------------------------------------------------------------------------------------------------------

## Print counter values
RLY_BUNDLE_COUNT_RECEIVED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node Bundles Received:", RLY_BUNDLE_COUNT_RECEIVED)
RLY_BUNDLE_COUNT_STORED = tlm(f"{target} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node Bundles Stored:", RLY_BUNDLE_COUNT_STORED)
RLY_BUNDLE_COUNT_FORWARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Relay Node Bundles Forwarded:", RLY_BUNDLE_COUNT_FORWARDED)

## Verify successful data transfer
assert RLY_BUNDLE_COUNT_RECEIVED == expected_bundle_count
assert RLY_BUNDLE_COUNT_STORED == 0
assert RLY_BUNDLE_COUNT_FORWARDED == expected_bundle_count

# Stop running scripts
running_script_stop(id_1)
