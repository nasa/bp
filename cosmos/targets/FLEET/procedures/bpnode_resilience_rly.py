##******************************************************************************
##
## Resilience test of BPNode in Relay Mode
##
## See Test Case DTN-FSW-cFS-RES-0001 for setup information
##      
##******************************************************************************

import os
from datetime import datetime

## Setting Script Runner line delay
set_line_delay(0.000)

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")
load_utility("FLEET/procedures/bad_bundles_send.py")

target = "DTNFSW-2"

## Start event logging
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

## Load contact table modified to set dest EID to 103
load_target_table('/cf/con_long_rly.tbl', target)

##------------------------------------------------------------------------------
## Test Setup
##------------------------------------------------------------------------------

dest_number = 103
print("Bundle destination node number ", dest_number)

dest_service = 1
print("Bundle destination service number: ", dest_service)

lifetime_in_sec = 3600
print("Bundle lifetime (s): ", lifetime_in_sec)

print("Selecting how many bundles to send")
number_of_bundles = ask("Choose number of bundles to send: ")

print("Selecting BPNode IP adress")
send_to_ip = ask("Enter BPNode IP address (X.X.X.X): ")

send_to_port = 4556
print("BPNode ingress port: ", send_to_port)

rate_limit = 1
print("Rate Limit (Mbps): ", rate_limit)

##------------------------------------------------------------------------------
## Test Start
##------------------------------------------------------------------------------

Start_Time = datetime.now()
print(f"Test Start Time = {Start_Time}")

## Setup and start Contact 0
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(6)

## Execute Bundle Ingress
bad_bundles_send(dest_number, dest_service, lifetime_in_sec, number_of_bundles, send_to_ip, send_to_port, rate_limit)
wait(1)

## Stop and tear down Contact 0
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(6)

## Print counter values
RLY_BUNDLE_COUNT_RECEIVED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node Bundles Received:", RLY_BUNDLE_COUNT_RECEIVED)
RLY_BUNDLE_COUNT_DISCARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DISCARDED")
print("Relay Node Bundles Discarded:", RLY_BUNDLE_COUNT_DISCARDED)
RLY_BUNDLE_COUNT_DELETED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node Bundles Deleted:", RLY_BUNDLE_COUNT_DELETED)
RLY_BUNDLE_COUNT_DELETED_HOP_EXCEEDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_HOP_EXCEEDED")
print("Relay Node Bundles Deleted Hop Exceeded:", RLY_BUNDLE_COUNT_DELETED_HOP_EXCEEDED)
RLY_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
print("Relay Node Bundles Deleted Unintelligible:", RLY_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE)
RLY_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK")
print("Relay Node Bundles Deleted Unsupported Block:", RLY_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK)
RLY_BUNDLE_COUNT_STORED = tlm(f"{target} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node Bundles Stored:", RLY_BUNDLE_COUNT_STORED)
RLY_BUNDLE_COUNT_FORWARDED = tlm(f"{target} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Relay Node Bundles Forwarded:", RLY_BUNDLE_COUNT_FORWARDED)

## Verify successful bundle validation
assert RLY_BUNDLE_COUNT_RECEIVED + RLY_BUNDLE_COUNT_DISCARDED == number_of_bundles
assert RLY_BUNDLE_COUNT_DELETED == RLY_BUNDLE_COUNT_DISCARDED
assert RLY_BUNDLE_COUNT_DELETED_HOP_EXCEEDED + RLY_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE + RLY_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK == RLY_BUNDLE_COUNT_DELETED
assert RLY_BUNDLE_COUNT_STORED == 0
assert RLY_BUNDLE_COUNT_FORWARDED == RLY_BUNDLE_COUNT_RECEIVED

# Stop running scripts
running_script_stop(id_1)
