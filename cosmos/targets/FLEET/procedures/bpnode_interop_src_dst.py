##*******************************************************************************************************************
##
## Interoperability test with BPNode Source and Destination Nodes
##
## See Test Cases DTN-FSW-cFS-INTEROP-0001, DTN-FSW-cFS-INTEROP-0002
##      
##*******************************************************************************************************************
import os
from datetime import datetime

# Setting Script Runner line delay
set_line_delay(0.000)

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")

target_1 = "DTNFSW-1"
target_3 = "DTNFSW-3"

## Start event logging
curtime = str(datetime.now()).replace('-','').replace(':','').replace(' ','_')[:-7]

eventlog_filename_1 = "/eventlogs/eventlog_" + target_1 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_1)
stash_set('eventlog', eventlog_filename_1)
id_1 = script_run(f"{target_1}/procedures/write_event_log.py")
wait(2)

eventlog_filename_3 = "/eventlogs/eventlog_" + target_3 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_3)
stash_set('eventlog', eventlog_filename_3)
id_3 = script_run(f"{target_3}/procedures/write_event_log.py")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## Source, Relay, and Destination Node Initialization
##-------------------------------------------------------------------------------------------------------------------

bpnode_initialization(target_1)
bpnode_initialization(target_3)

##-------------------------------------------------------------------------------------------------------------------
## Source Node Setup
##-------------------------------------------------------------------------------------------------------------------

## Reset all counters
cmd(f"{target_1} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)
cmd(f"{target_1} CF_CMD_RESET")
wait(1)

## Stop and remove Channel 0
cmd(f"{target_1} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)

## Load MIB PN table modified to set node number to 101
load_target_table('/cf/pn_long_src.tbl', target_1)

## Load ADU Proxy table modified to receive CF PDUs (0x83E) on Channel 1
load_target_table('/cf/adu_long_src.tbl', target_1) 

## Load Channel 1 table modified to create bundles with DST EID 103.1
load_target_table('/cf/cha_interop_src.tbl', target_1) 

## Load Contact 0 table for sending bundles on port 4556
load_target_table('/cf/con_interop_src.tbl', target_1) 

## Load modified CF configuration table to send 10 PDUs per second
cmd(f"{target_1} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_long_src.tbl', target_1)
cmd(f"{target_1} CF_CMD_ENABLE_ENGINE")

## Restart Channel 0
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Start Channel 1
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(1)

## Set up Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## Destination Node Setup
##-------------------------------------------------------------------------------------------------------------------

## Reset all counters
cmd(f"{target_3} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)
cmd(f"{target_3} CF_CMD_RESET")
wait(1)

## Stop and remove Channel 0
cmd(f"{target_3} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_3} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)

## Load MIB PN table modified to set node number to 103
load_target_table('/cf/pn_long_dst.tbl', target_3)

## Load ADU Proxy table modified to not receive CF PDUs (0x83E) on Channel 0
load_target_table('/cf/adu_long_dst.tbl', target_3) 

## Load Channel 1 table modified to set local service number to 1
load_target_table('/cf/cha_long_dst.tbl', target_3)

## Load Contact 0 table for receiving bundles on port 4558
load_target_table('/cf/con_long_dst.tbl', target_3)

## Load CF Config table modified to receive 0x83E PDUs (default 0x183F)
cmd(f"{target_3} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_long_dst.tbl', target_3)
cmd(f"{target_3} CF_CMD_ENABLE_ENGINE")

## Restart Channel 0
cmd(f"{target_3} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_3} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Start Channel 1
cmd(f"{target_3} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_3} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(1)

## Set up Contact 0
cmd(f"{target_3} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## Test Setup
##-------------------------------------------------------------------------------------------------------------------

## Expected number of bundles
CFDP_PDU_count = 2600

##-------------------------------------------------------------------------------------------------------------------
## Test Execution
##-------------------------------------------------------------------------------------------------------------------

Start_Time = datetime.now()
print(f"Test Start Time = {Start_Time}")

##---------------------------------------------------------------------------------------------------------------
## Contact Execution
##---------------------------------------------------------------------------------------------------------------
    
print("Starting Contact")
cmd(f"{target_3} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(1)

## Transmit the first file
cmd(f"{target_1} CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/Jojo1.jpg', DST_FILENAME '/cf/Jojo1.jpg'")
wait(120)

## Transmit the second file
cmd(f"{target_1} CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/Jojo2.jpg', DST_FILENAME '/cf/Jojo2.jpg'")
wait(60)

print("Stopping Contact")
cmd(f"{target_1} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target_3} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)

End_Time = datetime.now()
print(f"Test End Time = {End_Time}")

##-------------------------------------------------------------------------------------------------------------------
## End of Test Verifications
##-------------------------------------------------------------------------------------------------------------------

## Print counter values
SRC_CFDP_PDUS_SENT = tlm(f"{target_1} CF_HK CHANNEL_HK_0_COUNTERS_SENT_PDU")
print("Source Node CFDP PDUs Sent:", SRC_CFDP_PDUS_SENT)
SRC_ADU_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
print("Source Node ADUs Received:", SRC_ADU_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_GENERATED_ACCEPTED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_GENERATED_ACCEPTED")
print("Source Node Bundles Generated:", SRC_BUNDLE_COUNT_GENERATED_ACCEPTED)
SRC_BUNDLE_COUNT_FORWARDED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Source Node Bundles Forwarded:", SRC_BUNDLE_COUNT_FORWARDED)
DST_BUNDLE_COUNT_RECEIVED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Destination Node Bundles Received:", DST_BUNDLE_COUNT_RECEIVED)
DST_BUNDLE_COUNT_DELIVERED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED")
print("Destination Node Bundles Delivered:", DST_BUNDLE_COUNT_DELIVERED)
DST_ADU_COUNT_DELIVERED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED")
print("Destination Node ADUs Delivered:", DST_ADU_COUNT_DELIVERED)
DST_CFDP_PDUS_RECEIVED = tlm(f"{target_3} CF_HK CHANNEL_HK_0_COUNTERS_RECV_PDU")
print("Destination Node CFDP PDUs Received:", DST_CFDP_PDUS_RECEIVED)

## Verify successful data transfer
assert SRC_CFDP_PDUS_SENT == CFDP_PDU_count
assert SRC_ADU_COUNT_RECEIVED == CFDP_PDU_count
assert SRC_BUNDLE_COUNT_GENERATED_ACCEPTED == CFDP_PDU_count
assert SRC_BUNDLE_COUNT_FORWARDED == CFDP_PDU_count
assert DST_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count
assert DST_BUNDLE_COUNT_DELIVERED == CFDP_PDU_count
assert DST_ADU_COUNT_DELIVERED == CFDP_PDU_count
assert DST_CFDP_PDUS_RECEIVED == CFDP_PDU_count

##-------------------------------------------------------------------------------------------------------------------
## Source Node Teardown
##-------------------------------------------------------------------------------------------------------------------

## Tear down Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

## Stop and Remove Channel 1
cmd(f"{target_1} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_1} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## Destination Node Teardown
##-------------------------------------------------------------------------------------------------------------------

## Tear down Contact 0
cmd(f"{target_3} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

## Stop and Remove Channel 1
cmd(f"{target_3} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_3} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
wait(1)

# Stop running scripts
running_script_stop(id_1)
running_script_stop(id_3)
