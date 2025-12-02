##******************************************************************************
##
## Interoperability test with a BPNode Relay Node
##
## See Test Cases DTN-FSW-cFS-INTEROP-0021, DTN-FSW-cFS-INTEROP-0022
#      
##******************************************************************************

import os
from datetime import datetime

# Setting Script Runner line delay
set_line_delay(0.000)

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")

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

## Reset all counters
cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Load Contact 0 table
load_target_table('/cf/con_interop_rly.tbl', target)

##------------------------------------------------------------------------------
## Test Setup
##------------------------------------------------------------------------------

src_dst_node = ask("Choose SRC/DST Node software (HDTN or ION): ")

## Expected number of bundles
if src_dst_node == "HDTN":
    expected_bundle_count = 12235
else:
    expected_bundle_count = 5001

##------------------------------------------------------------------------------
## Test Execution
##------------------------------------------------------------------------------

Start_Time = datetime.now()
print(f"Test Start Time = {Start_Time}")

##------------------------------------------------------------------------------
## Contact Execution
##------------------------------------------------------------------------------


print("Starting Contact")
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait()

print("Stopping Contact")
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

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
