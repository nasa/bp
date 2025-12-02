##*******************************************************************************************************************
##
## Subsystem Test Case - Maximum Storage Exceeded
##      
##*******************************************************************************************************************
import os
from datetime import datetime

global rqmnt_status

# Setting Script Runner line delay
set_line_delay(0.000)

##-------------------------------------------------------------------------------------------------------------------
## 1. Test Setup
##-------------------------------------------------------------------------------------------------------------------

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")
load_utility("FLEET/procedures/bundles_send.py")


target_1 = "DTNFSW-1"
target_2 = "DTNFSW-2"

## Start event logging
curtime = str(datetime.now()).replace('-','').replace(':','').replace(' ','_')[:-7]

eventlog_filename_1 = "/eventlogs/eventlog_" + target_1 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_1)
stash_set('eventlog', eventlog_filename_1)
id_1 = script_run(f"{target_1}/procedures/write_event_log.py")
wait(2)

eventlog_filename_2 = "/eventlogs/eventlog_" + target_2 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_2)
stash_set('eventlog', eventlog_filename_2)
id_2 = script_run(f"{target_2}/procedures/write_event_log.py")
wait(2)

bpnode_initialization(target_1)
bpnode_initialization(target_2)

# Initialize requirement status
rqmnt_status = {
    "DTN.5.00210":"U",
    "DTN.6.04220":"U",
    "DTN.6.04300":"U",
    "DTN.6.04312":"U",
    "DTN.6.04430":"U",
    "DTN.6.25020":"U",
}

##-------------------------------------------------------------------------------------------------------------------
## 2.1 Source Node Setup for receiving bundles
##-------------------------------------------------------------------------------------------------------------------

## Reset all counters
cmd(f"{target_1} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)
cmd(f"{target_1} CF_CMD_RESET")
wait(1)

## Verify Source Node Storage Size
SRC_BUNDLE_AGENT_AVAILABLE_STORAGE = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_AGENT_AVAILABLE_STORAGE")
print("Source Node Available Storage:", SRC_BUNDLE_AGENT_AVAILABLE_STORAGE)
SRC_KBYTES_COUNT_STORAGE_AVAILABLE = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK KBYTES_COUNT_STORAGE_AVAILABLE")
print("Source Node kbyte Storage Available:", SRC_KBYTES_COUNT_STORAGE_AVAILABLE)

assert SRC_BUNDLE_AGENT_AVAILABLE_STORAGE == 4000000
assert SRC_KBYTES_COUNT_STORAGE_AVAILABLE == 4000000

## Load MIB PN table modified to set node number to 101
load_target_table('/cf/pn_sub_src.tbl', target_1)

## Load Contact 0 table for sending bundles on port 4556
load_target_table('/cf/con_sub_src_st.tbl', target_1) 

# Set up and start Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

##-------------------------------------------------------------------------------------------------------------------
## 2.2 Send bundles to max out Source Node storage
##-------------------------------------------------------------------------------------------------------------------

prompt("In a terminal window, enter the performance script folder: 'cd /dtn/dtngen/examples/performance-test/'")
prompt("Start sending 250,000 bundles to the Source Node: 'python TX-performance-test.py', prompt answers: 8000, 10.2.8.238, 50")
wait(10)

## Wait for all bundles to have been received
wait_check(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == 250000", 600)
wait(10)

## Print counter values
SRC_BUNDLE_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Source Node Bundles Received:", SRC_BUNDLE_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)
SRC_BUNDLE_COUNT_DELETED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Source Node Bundles Deleted:", SRC_BUNDLE_COUNT_DELETED)
SRC_BUNDLE_COUNT_DELETED_NO_STORAGE = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_NO_STORAGE")
print("Source Node Bundles Deleted No Storage:", SRC_BUNDLE_COUNT_DELETED_NO_STORAGE)

## Verify storage exceeded
assert SRC_BUNDLE_COUNT_RECEIVED == 250000
assert SRC_BUNDLE_COUNT_STORED == 250000
assert SRC_BUNDLE_COUNT_DELETED == 0
assert SRC_BUNDLE_COUNT_DELETED == SRC_BUNDLE_COUNT_DELETED_NO_STORAGE
assert SRC_BUNDLE_COUNT_STORED + SRC_BUNDLE_COUNT_DELETED == 250000

prompt("From the terminal window, start sending another 250,000 bundles to the Source Node: 'python TX-performance-test.py', prompt answers: 8000, 10.2.8.238, 50")
wait(10)

## Confirm that the Source Node has maxed out storage
wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'BPNODE'", 600)
wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 611", 600)
# wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'ERROR'", 6)

## Verify requirements
# rqmnt_status["DTN.6.04300"] = "P"
# rqmnt_status["DTN.6.04430"] = "P"

## Wait for all bundles to have been received
wait_check(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == 500000", 600)
wait(10)

## Print counter values
SRC_BUNDLE_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Source Node Bundles Received:", SRC_BUNDLE_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)
SRC_KBYTES_COUNT_STORAGE_AVAILABLE = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK KBYTES_COUNT_STORAGE_AVAILABLE")
print("Source Node kbyte Storage Available:", SRC_KBYTES_COUNT_STORAGE_AVAILABLE)
SRC_BUNDLE_COUNT_DELETED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Source Node Bundles Deleted:", SRC_BUNDLE_COUNT_DELETED)
SRC_BUNDLE_COUNT_DELETED_NO_STORAGE = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_NO_STORAGE")
print("Source Node Bundles Deleted No Storage:", SRC_BUNDLE_COUNT_DELETED_NO_STORAGE)

## Verify storage exceeded
assert SRC_BUNDLE_COUNT_RECEIVED == 500000
assert SRC_BUNDLE_COUNT_STORED < 500000
assert SRC_KBYTES_COUNT_STORAGE_AVAILABLE < 500
assert SRC_BUNDLE_COUNT_DELETED > 0
assert SRC_BUNDLE_COUNT_DELETED == SRC_BUNDLE_COUNT_DELETED_NO_STORAGE
assert SRC_BUNDLE_COUNT_STORED + SRC_BUNDLE_COUNT_DELETED == 500000

## Verify requirements
rqmnt_status["DTN.5.00210"] = "P"
rqmnt_status["DTN.6.04312"] = "P"

## Tear down Contact 0 on the Source Node
cmd(f"{target_1} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_1} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(20)

##-------------------------------------------------------------------------------------------------------------------
## 2.3 Source Node reconfiguration for the contact
##-------------------------------------------------------------------------------------------------------------------

## Load Contact 0 table for sending bundles on port 4556 for EIDs 103.1-105.1
load_target_table('/cf/con_sub_src.tbl', target_1) 

# Set up Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## 2.4 Relay Node #1 Setup for the contact
##-------------------------------------------------------------------------------------------------------------------

## Reset all counters
cmd(f"{target_2} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Load MIB PN table modified to set node number to 102
load_target_table('/cf/pn_sub_rly1.tbl', target_2)

## Load Contact 0 table for sending bundles on port 4557
load_target_table('/cf/con_sub_rly1_2.tbl', target_2) 

## Set up Contact 0
cmd(f"{target_2} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## 2.5 Execute the contact
##-------------------------------------------------------------------------------------------------------------------

## Start Contact 0
cmd(f"{target_2} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)
cmd(f"{target_1} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(120)

wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'BPNODE'", 120)
wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID == 82", 120)
wait_check(f"{target_1} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE == 'ERROR'", 6)

## Wait for all bundles to have been received by Relay Node #1
wait_check(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {SRC_BUNDLE_COUNT_STORED}", 1200)
wait(20)

## Confirm that the Source Node has maxed out memory pool usage
SRC_BYTES_MEM_HIGH_WATER = tlm(f"{target_1} BPNODE_STORAGE_HK BYTES_MEM_HIGH_WATER")
print("Source Node Bytes Memory High Water:", SRC_BYTES_MEM_HIGH_WATER)
assert SRC_BYTES_MEM_HIGH_WATER > 15990000

## Verify requirements
rqmnt_status["DTN.6.25020"] = "P"

## Print counter values
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)
SRC_KBYTES_COUNT_STORAGE_AVAILABLE = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK KBYTES_COUNT_STORAGE_AVAILABLE")
print("Source Node kbyte Storage Available:", SRC_KBYTES_COUNT_STORAGE_AVAILABLE)
SRC_BUNDLE_COUNT_FORWARDED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Source Node Bundles Forwarded:", SRC_BUNDLE_COUNT_FORWARDED)

RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)

## Verify successful data transfer
assert SRC_BUNDLE_COUNT_STORED == 0
assert SRC_KBYTES_COUNT_STORAGE_AVAILABLE == 4000000
assert RLY1_BUNDLE_COUNT_RECEIVED == SRC_BUNDLE_COUNT_FORWARDED

## Verify requirements
rqmnt_status["DTN.6.04220"] = "P"

## Send another 1000 bundles to the Source Node
bundles_send(1000, 3600, 20, "10.2.8.238", 4556, 50)
wait(30)

## Print counter values
SRC_BUNDLE_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Source Node Bundles Received:", SRC_BUNDLE_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)
SRC_BUNDLE_COUNT_FORWARDED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Source Node Bundles Forwarded:", SRC_BUNDLE_COUNT_FORWARDED)

RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)

## Verify successful data transfer
assert SRC_BUNDLE_COUNT_STORED == 0
assert SRC_BUNDLE_COUNT_RECEIVED == 501000
assert RLY1_BUNDLE_COUNT_RECEIVED == SRC_BUNDLE_COUNT_FORWARDED

## Stop Contact 0 on both nodes
cmd(f"{target_1} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_2} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)

##-------------------------------------------------------------------------------------------------------------------
## 3. Test Cleanup
##-------------------------------------------------------------------------------------------------------------------

## Tear down Contact 0 on both nodes
cmd(f"{target_1} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)
cmd(f"{target_2} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

## Reset all counters
cmd(f"{target_1} BPNODE_CMD_RESET_ALL_COUNTERS")
wait(1)
cmd(f"{target_2} BPNODE_CMD_RESET_ALL_COUNTERS")
wait(1)

# Stop running scripts
running_script_stop(id_1)
running_script_stop(id_2)

# Print Requirement Status
for key, value in rqmnt_status.items():
    print(f"***    {key}: {value}")
