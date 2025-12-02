##*******************************************************************************************************************
##
## Subsystem Test Case - End to End (ETE) Transfer
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
load_utility("FLEET/procedures/reject_target_table.py")
load_utility("FLEET/procedures/invalid_bundles_send.py")
load_utility("FLEET/procedures/bad_bundles_send.py")
load_utility("FLEET/procedures/bundles_send.py")
load_utility("FLEET/procedures/unknown_blocks_send.py")


target_1 = "DTNFSW-1"
target_2 = "DTNFSW-2"
target_3 = "DTNFSW-3"
target_4 = "DTNFSW-4"
target_5 = "DTNFSW-5"

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

eventlog_filename_3 = "/eventlogs/eventlog_" + target_3 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_3)
stash_set('eventlog', eventlog_filename_3)
id_3 = script_run(f"{target_3}/procedures/write_event_log.py")
wait(1)

eventlog_filename_4 = "/eventlogs/eventlog_" + target_4 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_4)
stash_set('eventlog', eventlog_filename_4)
id_4 = script_run(f"{target_4}/procedures/write_event_log.py")
wait(1)

eventlog_filename_5 = "/eventlogs/eventlog_" + target_5 + "_" + curtime + ".txt"
print ("Eventlog filename: ", eventlog_filename_5)
stash_set('eventlog', eventlog_filename_5)
id_5 = script_run(f"{target_5}/procedures/write_event_log.py")
wait(1)

bpnode_initialization(target_1)
bpnode_initialization(target_2)
bpnode_initialization(target_3)
bpnode_initialization(target_4)
bpnode_initialization(target_5)

prompt("Start a Wireshark capture of bundles on Relay Node #2 (capture filter: 'udp port 4557', interface: 'ens5'")

# Initialize requirement status
rqmnt_status = {
    "DTN.5.00006":"U",
    "DTN.5.00007":"U",
    "DTN.5.00008":"U",
    "DTN.5.00009":"U",
    "DTN.5.00010":"U",
    "DTN.5.00012":"U",
    "DTN.5.00040":"U",
    "DTN.5.00060":"U",
    "DTN.5.00070":"U",
    "DTN.5.00090":"U",
    "DTN.5.00100":"U",
    "DTN.5.00110":"U",
    "DTN.5.00120":"U",
    "DTN.5.00150":"U",
    "DTN.5.00170":"U",
    "DTN.5.00174":"U",
    "DTN.5.00180":"U",
    "DTN.5.00190":"U",
    "DTN.5.00192":"U",
    "DTN.5.00197":"U",
    "DTN.5.00200":"U",
    "DTN.5.00220":"U",
    "DTN.5.00250":"P",
    "DTN.5.00255":"U",
    "DTN.5.00260":"U",
    "DTN.5.00270":"U",
    "DTN.5.00275":"U",
    "DTN.5.00280":"U",
    "DTN.5.00290":"U",
    "DTN.5.00300":"U",
    "DTN.5.00320":"U",
    "DTN.5.00330":"U",
    "DTN.5.00370":"U",
    "DTN.5.00660":"U",
    "DTN.5.00670":"U",
    "DTN.5.00680":"U",
    "DTN.5.00692":"U",
    "DTN.5.00694":"U",
    "DTN.5.00700":"U",
    "DTN.5.00723":"U",
    "DTN.5.00724":"U",
    "DTN.5.00726":"U",
    "DTN.5.00728":"U",
    "DTN.5.00730":"U",
    "DTN.5.00760":"U",
    "DTN.5.00770":"U",
    "DTN.5.00788":"U",
    "DTN.5.00800":"U",
    "DTN.5.00820":"U",
    "DTN.5.00930":"U",
    "DTN.5.01025":"U",
    "DTN.5.01050":"U",
    "DTN.5.01054":"U",
    "DTN.5.01065":"U",
    "DTN.6.10111":"U",
}

##-------------------------------------------------------------------------------------------------------------------
## 2.1 Source Node setup and start of bundle generation
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
load_target_table('/cf/pn_sub_src.tbl', target_1)

## Load ADU Proxy table modified to receive CF PDUs (0x83E) on Channel 1
load_target_table('/cf/adu_sub_src.tbl', target_1) 

## Verify requirements
rqmnt_status["DTN.5.00008"] = "P"
rqmnt_status["DTN.5.00009"] = "P"

## Load Channel 1 table with bad configuration
reject_target_table('/cf/cha_sub_bad.tbl', target_1) 

## Load Channel 1 table modified to create bundles with DST EID 105.1
load_target_table('/cf/cha_sub_src.tbl', target_1)

## Load Contact 0 table for sending bundles on port 4556
load_target_table('/cf/con_sub_src.tbl', target_1) 

## Load modified CF configuration table to send 20 PDUs per second
cmd(f"{target_1} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_sub_src.tbl', target_1)
cmd(f"{target_1} CF_CMD_ENABLE_ENGINE")

## Verify requirements
rqmnt_status["DTN.5.00006"] = "P"
rqmnt_status["DTN.5.00007"] = "P"
rqmnt_status["DTN.5.00930"] = "P"
rqmnt_status["DTN.5.01050"] = "P"

## Restart Channel 0
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Start Channel 1
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(10)

## Print Channel 1 status
SRC_CHANNEL_1_STATE = tlm(f"{target_1} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1")
print("Source Node Channel 1 State:", SRC_CHANNEL_1_STATE)

## Verify that channel 1 is started on the Source Node
assert SRC_CHANNEL_1_STATE == "STARTED"

## Set up Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

## Start creating bundles (CF transmitting a file)
CFDP_PDU_count_1 = 1836
cmd(f"{target_1} CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/Jojo1.jpg', DST_FILENAME '/cf/Jojo1.jpg'")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## 2.2 Relay Node #1 Setup for the first contact
##-------------------------------------------------------------------------------------------------------------------

## Reset all counters
cmd(f"{target_2} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Load MIB PN table modified to set node number to 102
load_target_table('/cf/pn_sub_rly1.tbl', target_2)

## Load Contact 0 table for sending bundles on port 4557
load_target_table('/cf/con_sub_rly1_st.tbl', target_2) 

## Set up Contact 0
cmd(f"{target_2} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## 2.3 Execution of the first contact (Source Node -> Relay Node #1)
##-------------------------------------------------------------------------------------------------------------------

## Wait for all bundles to have been created
wait_check(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_GENERATED_ACCEPTED == {CFDP_PDU_count_1}", 600)
wait(10)

## Print counter values
SRC_CFDP_PDU_COUNT_SENT = tlm(f"{target_1} CF_HK CHANNEL_HK_0_COUNTERS_SENT_PDU")
print("Source Node CFDP PDUs Sent:", SRC_CFDP_PDU_COUNT_SENT)
SRC_ADU_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
print("Source Node ADUs Received:", SRC_ADU_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_GENERATED_ACCEPTED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_GENERATED_ACCEPTED")
print("Source Node Bundles Generated:", SRC_BUNDLE_COUNT_GENERATED_ACCEPTED)
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)

## Verify successful data creation
assert SRC_CFDP_PDU_COUNT_SENT == CFDP_PDU_count_1
assert SRC_ADU_COUNT_RECEIVED == CFDP_PDU_count_1
assert SRC_BUNDLE_COUNT_GENERATED_ACCEPTED == CFDP_PDU_count_1
assert SRC_BUNDLE_COUNT_STORED == CFDP_PDU_count_1

## Verify requirements
rqmnt_status["DTN.5.00010"] = "P"
rqmnt_status["DTN.5.00012"] = "P"
rqmnt_status["DTN.5.00040"] = "P"
rqmnt_status["DTN.5.00110"] = "P"
rqmnt_status["DTN.5.00120"] = "P"
rqmnt_status["DTN.5.00150"] = "P"
rqmnt_status["DTN.5.00200"] = "P"
rqmnt_status["DTN.5.00330"] = "P"

## Start Contact 0
cmd(f"{target_2} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)
cmd(f"{target_1} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

## Print Channel 1 status
SRC_CONTACT_0_STATE = tlm(f"{target_1} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0")
print("Source Node Contact 0 State:", SRC_CONTACT_0_STATE)

## Verify that Contact 0 is started on the Source Node
assert SRC_CONTACT_0_STATE == "STARTED"

## Verify requirements
rqmnt_status["DTN.5.01054"] = "P"

## Wait for all bundles to have been received by Relay Node #1
wait_check(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {CFDP_PDU_count_1}", 300)
wait(20)

## Print counter values
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)
SRC_BUNDLE_COUNT_FORWARDED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Source Node Bundles Forwarded:", SRC_BUNDLE_COUNT_FORWARDED)

RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)

## Verify successful data transfer
assert SRC_BUNDLE_COUNT_STORED == 0
assert SRC_BUNDLE_COUNT_FORWARDED == CFDP_PDU_count_1
assert RLY1_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1
assert RLY1_BUNDLE_COUNT_STORED == CFDP_PDU_count_1

## Verify requirements
rqmnt_status["DTN.5.00170"] = "P"
rqmnt_status["DTN.5.00174"] = "P"
rqmnt_status["DTN.5.00692"] = "P"
rqmnt_status["DTN.5.00694"] = "P"
rqmnt_status["DTN.5.00700"] = "P"

## Inject three bundles containing an unknown extension block
## Destination EID = 103.1
unknown_blocks_send(104, 1, 900, "10.2.7.206", 4556)
wait(20)

## Print counter values
RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)
RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK")
print("Relay Node #1 Bundles Deleted Unsupported Block:", RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK)
RLY1_BUNDLE_COUNT_DISCARDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DISCARDED")
print("Relay Node #1 Bundles Discarded:", RLY1_BUNDLE_COUNT_DISCARDED)
RLY1_BUNDLE_COUNT_DELETED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node #1 Bundles Deleted:", RLY1_BUNDLE_COUNT_DELETED)

## Verify successful data transfer
assert RLY1_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1 + 2
assert RLY1_BUNDLE_COUNT_STORED == CFDP_PDU_count_1 + 2
assert RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK == 1
assert RLY1_BUNDLE_COUNT_DISCARDED == 1
assert RLY1_BUNDLE_COUNT_DELETED == 1

## Verify requirements
rqmnt_status["DTN.5.00260"] = "P"

## Inject a set of 50 bundles including nine invalid bundles
## Destination EID = 103.1
invalid_bundles_send(103, 1, 900, "10.2.7.206", 4556)
wait(20)

## Print counter values
RLY1_BUNDLE_COUNT_DISCARDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DISCARDED")
print("Relay Node #1 Bundles Discarded:", RLY1_BUNDLE_COUNT_DISCARDED)
RLY1_BUNDLE_COUNT_DELETED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node #1 Bundles Deleted:", RLY1_BUNDLE_COUNT_DELETED)
RLY1_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #1 Bundles Deleted Expired:", RLY1_BUNDLE_COUNT_DELETED_EXPIRED)
RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_HOP_EXCEEDED")
print("Relay Node #1 Bundles Deleted Hop Exceeded:", RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED)
RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
print("Relay Node #1 Bundles Deleted Unintelligible:", RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE)
RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK")
print("Relay Node #1 Bundles Deleted Unsupported Block:", RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK)
RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)

## Verify successful data transfer
assert RLY1_BUNDLE_COUNT_DISCARDED == 10
assert RLY1_BUNDLE_COUNT_DELETED == 10
assert RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE == 9
assert RLY1_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1 + 43
assert RLY1_BUNDLE_COUNT_STORED == CFDP_PDU_count_1 + 43

## Verify requirements
rqmnt_status["DTN.5.00180"] = "P"
rqmnt_status["DTN.5.00190"] = "P"
rqmnt_status["DTN.5.00192"] = "P"
rqmnt_status["DTN.5.00220"] = "P"
rqmnt_status["DTN.5.00255"] = "P"
rqmnt_status["DTN.5.00300"] = "P"
rqmnt_status["DTN.5.00320"] = "P"
rqmnt_status["DTN.5.00370"] = "P"

## Inject randomly corrupted bundles (destination EID, lifetime in seconds, number_of_bundles, send to ip/port, rate limit in Mbps)
## Destination EID = 104.2
bad_bundles_send(104, 2, 900, 5000, "10.2.7.206", 4556, 45)
wait(20)

## Print counter values
RLY1_BUNDLE_COUNT_DISCARDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DISCARDED")
print("Relay Node #1 Bundles Discarded:", RLY1_BUNDLE_COUNT_DISCARDED)
RLY1_BUNDLE_COUNT_DELETED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node #1 Bundles Deleted:", RLY1_BUNDLE_COUNT_DELETED)
RLY1_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #1 Bundles Deleted Expired:", RLY1_BUNDLE_COUNT_DELETED_EXPIRED)
RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_HOP_EXCEEDED")
print("Relay Node #1 Bundles Deleted Hop Exceeded:", RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED)
RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
print("Relay Node #1 Bundles Deleted Unintelligible:", RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE)
RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK")
print("Relay Node #1 Bundles Deleted Unsupported Block:", RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK)
RLY1_BUNDLE_COUNT_RECEIVED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #1 Bundles Received:", RLY1_BUNDLE_COUNT_RECEIVED)
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)

## Verify successful data transfer
assert RLY1_BUNDLE_COUNT_DELETED == RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED + RLY1_BUNDLE_COUNT_DELETED_UNINTELLIGIBLE + RLY1_BUNDLE_COUNT_DELETED_UNSUPPORTED_BLOCK
assert RLY1_BUNDLE_COUNT_DELETED == RLY1_BUNDLE_COUNT_DISCARDED
assert RLY1_BUNDLE_COUNT_DELETED_EXPIRED == 0
assert RLY1_BUNDLE_COUNT_DELETED_HOP_EXCEEDED > 0
assert RLY1_BUNDLE_COUNT_DELETED + RLY1_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1 + 3 + 50 + 5000

## Verify requirements
rqmnt_status["DTN.5.00260"] = "P"
rqmnt_status["DTN.5.00290"] = "P"

## Stop Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_2} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_2} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(20)

##-------------------------------------------------------------------------------------------------------------------
## 2.4 Setup for the second contact (Relay Node #1, Relay Node #2, Relay Node #3, Destination Node)
##-------------------------------------------------------------------------------------------------------------------

## Relay Node #1: Load Contact 0 table for sending bundles to port 4557 with EID Range 103.1-105.1
load_target_table('/cf/con_sub_rly1.tbl', target_2) 

## Relay Node #1: Set up Contact 0
cmd(f"{target_2} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

## Relay Node #2: Reset all counters
cmd(f"{target_3} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Relay Node #2: Load MIB PN table modified to set node number to 103
load_target_table('/cf/pn_sub_rly2.tbl', target_3)

## Relay Node #2: Load Contact 0 table for sending bundles to port 4558 with EID 103.1
load_target_table('/cf/con_sub_rly2.tbl', target_3) 

## Relay Node #2: Set up Contact 0
cmd(f"{target_3} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

## Relay Node #3: Reset all counters
cmd(f"{target_4} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)

## Relay Node #3: Load MIB PN table modified to set node number to 104
load_target_table('/cf/pn_sub_rly3.tbl', target_4)

## Relay Node #3: Load Contact 0 table for sending bundles to port 4559 with EID 103.1
load_target_table('/cf/con_sub_rly3.tbl', target_4) 

## Relay Node #3: Set up Contact 0
cmd(f"{target_4} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

## Destination Node: Reset all counters
cmd(f"{target_5} BPNODE_CMD_RESET_ALL_COUNTERS")    
wait(1)
cmd(f"{target_5} CF_CMD_RESET")
wait(1)

## Destination Node: Load MIB PN table modified to set node number to 105
load_target_table('/cf/pn_sub_dst.tbl', target_5)

## Destination Node: Load Contact 0 table for receiving bundles on port 4559
load_target_table('/cf/con_sub_dst.tbl', target_5) 

## Destination Node: Set up Contact 0
cmd(f"{target_5} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)

##-------------------------------------------------------------------------------------------------------------------
## 2.5 Execution of the second contact (Relay Node #1 -> Relay Node #2 -> Relay Node #3 -> Destination Node)
##-------------------------------------------------------------------------------------------------------------------

## Start Contact 0
cmd(f"{target_5} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)
cmd(f"{target_4} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)
cmd(f"{target_3} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)
cmd(f"{target_2} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

## Wait for all bundles to have been received by the Destination Node
wait_check(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {CFDP_PDU_count_1}", 300)
wait(10)

## Print counter values
RLY1_BUNDLE_COUNT_FORWARDED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Relay Node #1 Bundles Forwarded:", RLY1_BUNDLE_COUNT_FORWARDED)

RLY2_BUNDLE_COUNT_RECEIVED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #2 Bundles Received:", RLY2_BUNDLE_COUNT_RECEIVED)
RLY2_BUNDLE_COUNT_DELETED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node #2 Bundles Deleted:", RLY2_BUNDLE_COUNT_DELETED)
RLY2_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #2 Bundles Deleted Expired:", RLY2_BUNDLE_COUNT_DELETED_EXPIRED)
RLY2_BUNDLE_COUNT_STORED = tlm(f"{target_3} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #2 Bundles Stored:", RLY2_BUNDLE_COUNT_STORED)
RLY2_BUNDLE_COUNT_FORWARDED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Relay Node #2 Bundles Forwarded:", RLY2_BUNDLE_COUNT_FORWARDED)

RLY3_BUNDLE_COUNT_RECEIVED = tlm(f"{target_4} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Relay Node #3 Bundles Received:", RLY3_BUNDLE_COUNT_RECEIVED)
RLY3_BUNDLE_COUNT_DELETED = tlm(f"{target_4} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Relay Node #3 Bundles Deleted:", RLY3_BUNDLE_COUNT_DELETED)
RLY3_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_4} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #3 Bundles Deleted Expired:", RLY3_BUNDLE_COUNT_DELETED_EXPIRED)
RLY3_BUNDLE_COUNT_STORED = tlm(f"{target_4} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #3 Bundles Stored:", RLY3_BUNDLE_COUNT_STORED)
RLY3_BUNDLE_COUNT_FORWARDED = tlm(f"{target_4} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
print("Relay Node #3 Bundles Forwarded:", RLY3_BUNDLE_COUNT_FORWARDED)

DST_BUNDLE_COUNT_RECEIVED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Destination Node Bundles Received:", DST_BUNDLE_COUNT_RECEIVED)
DST_BUNDLE_COUNT_DELETED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Destination Node Bundles Deleted:", DST_BUNDLE_COUNT_DELETED)
DST_BUNDLE_COUNT_STORED = tlm(f"{target_5} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Destination Node Bundles Stored:", DST_BUNDLE_COUNT_STORED)
DST_BUNDLE_COUNT_DELIVERED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED")
print("Destination Node Bundles Delivered:", DST_BUNDLE_COUNT_DELIVERED)

## Verify successful data transfer
assert RLY1_BUNDLE_COUNT_FORWARDED == CFDP_PDU_count_1 + 43
assert RLY2_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1 + 43
assert RLY2_BUNDLE_COUNT_DELETED_EXPIRED == 0
assert RLY2_BUNDLE_COUNT_STORED == 43
assert RLY2_BUNDLE_COUNT_FORWARDED == CFDP_PDU_count_1
assert RLY3_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1
assert RLY3_BUNDLE_COUNT_DELETED_EXPIRED == 0
assert RLY3_BUNDLE_COUNT_STORED == 0
assert RLY3_BUNDLE_COUNT_FORWARDED == CFDP_PDU_count_1
assert DST_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_1
assert DST_BUNDLE_COUNT_DELETED == 0
assert DST_BUNDLE_COUNT_STORED == CFDP_PDU_count_1
assert DST_BUNDLE_COUNT_DELIVERED == 0

## Verify requirements
rqmnt_status["DTN.5.00660"] = "P"
rqmnt_status["DTN.6.04380"] = "P"

##-------------------------------------------------------------------------------------------------------------------
## 2.6 Destination Node Service Setup and ADU Delivery
##-------------------------------------------------------------------------------------------------------------------

## Stop and remove Channel 0
cmd(f"{target_5} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)

## Load ADU Proxy table modified to deliver CF PDUs (0x83E) on Channel 1
load_target_table('/cf/adu_sub_dst.tbl', target_5) 

## Verify requirements
rqmnt_status["DTN.5.00726"] = "P"
rqmnt_status["DTN.5.00728"] = "P"

## Load Channel 1 table modified to deliver bundles with DST EID 105.1
load_target_table('/cf/cha_sub_dst.tbl', target_5)

## Load modified CF configuration table to receive 20 PDUs per second
cmd(f"{target_5} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_sub_dst.tbl', target_5)
cmd(f"{target_5} CF_CMD_ENABLE_ENGINE")

## Verify requirements
rqmnt_status["DTN.5.00723"] = "P"
rqmnt_status["DTN.5.00724"] = "P"

## Restart Channel 0
cmd(f"{target_5} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Start Channel 1
cmd(f"{target_5} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_5} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(10)

## Print Channel 1 status
DST_CHANNEL_1_STATE = tlm(f"{target_5} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1")
print("Destination Node Channel 1 State:", DST_CHANNEL_1_STATE)

## Verify that channel 1 is started on the Source Node
assert DST_CHANNEL_1_STATE == "STARTED"

## Wait for all bundles to have been delivered by the Destination Node
wait_check(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED == {CFDP_PDU_count_1}", 300)
wait(10)

## Print counter values
DST_BUNDLE_COUNT_DELETED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Destination Node Bundles Deleted:", DST_BUNDLE_COUNT_DELETED)
DST_BUNDLE_COUNT_STORED = tlm(f"{target_5} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Destination Node Bundles Stored:", DST_BUNDLE_COUNT_STORED)
DST_BUNDLE_COUNT_DELIVERED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED")
print("Destination Node Bundles Delivered:", DST_BUNDLE_COUNT_DELIVERED)
DST_CFDP_PDU_COUNT_RECEIVED = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_PDU")
print("Destination Node CFDP PDUs received:", DST_CFDP_PDU_COUNT_RECEIVED)
DST_CFDP_PDU_COUNT_DROPPED = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_DROPPED")
print("Destination Node CFDP PDUs received and dropped:", DST_CFDP_PDU_COUNT_DROPPED)
DST_CFDP_PDU_COUNT_ERROR = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_ERROR")
print("Destination Node CFDP received PDU errors:", DST_CFDP_PDU_COUNT_ERROR)

## Verify successful data transfer
assert DST_BUNDLE_COUNT_DELETED == CFDP_PDU_count_1
assert DST_BUNDLE_COUNT_STORED == 0
assert DST_BUNDLE_COUNT_DELIVERED == CFDP_PDU_count_1
assert DST_CFDP_PDU_COUNT_RECEIVED == CFDP_PDU_count_1
assert DST_CFDP_PDU_COUNT_DROPPED == 0
assert DST_CFDP_PDU_COUNT_ERROR == 0

##-------------------------------------------------------------------------------------------------------------------
## 2.6 Source Node Restart
##-------------------------------------------------------------------------------------------------------------------

## Print Node Startup Counter prior to node restart
SRC_NODE_STARTUP_COUNTER_PRE = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Source Node Startup Counter:", SRC_NODE_STARTUP_COUNTER_PRE)

## Send restart command
cmd(f"{target_1} CFE_ES_CMD_RESTART_APP with APPLICATION 'BPNODE'")
wait(20)

## Print Node Startup Counter post node restart
SRC_NODE_STARTUP_COUNTER_POST = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Source Node Startup Counter:", SRC_NODE_STARTUP_COUNTER_POST)

## Verify successful node restart
assert SRC_NODE_STARTUP_COUNTER_POST == SRC_NODE_STARTUP_COUNTER_PRE + 1

## Stop and remove Channel 0
cmd(f"{target_1} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)

## Load MIB PN table modified to set node number to 101
load_target_table('/cf/pn_sub_src.tbl', target_1)

## Load ADU Proxy table modified to receive CF PDUs (0x83E) on Channel 1
load_target_table('/cf/adu_sub_src.tbl', target_1) 

## Load Channel 1 table modified to create bundles with DST EID 105.1
load_target_table('/cf/cha_sub_src.tbl', target_1)

## Load Contact 0 table for sending bundles on port 4556
load_target_table('/cf/con_sub_src.tbl', target_1) 

## Load modified CF configuration table to send 20 PDUs per second
cmd(f"{target_1} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_sub_src.tbl', target_1)
cmd(f"{target_1} CF_CMD_ENABLE_ENGINE")

## Restart Channel 0
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Restart Channel 1
cmd(f"{target_1} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_1} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(10)

## Set up and start Contact 0
cmd(f"{target_1} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target_1} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

##-------------------------------------------------------------------------------------------------------------------
## 2.7 Relay Node #3 Reload
##-------------------------------------------------------------------------------------------------------------------

## Print Node Startup Counter prior to node reload
RLY3_NODE_STARTUP_COUNTER_PRE = tlm(f"{target_4} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Relay Node #3 Startup Counter:", RLY3_NODE_STARTUP_COUNTER_PRE)

## Send reload command
cmd(f"{target_4} CFE_ES_CMD_RELOAD_APP with APPLICATION 'BPNODE', APP_FILE_NAME '/cf/bpnode.so'")
wait(20)

## Print Node Startup Counter post node reload
RLY3_NODE_STARTUP_COUNTER_POST = tlm(f"{target_4} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Relay Node #3 Startup Counter:", RLY3_NODE_STARTUP_COUNTER_POST)

## Verify successful node reload
assert RLY3_NODE_STARTUP_COUNTER_POST == RLY3_NODE_STARTUP_COUNTER_PRE + 1

## Load MIB PN table modified to set node number to 104
load_target_table('/cf/pn_sub_rly3.tbl', target_4)

## Load Contact 0 table for sending bundles to port 4559 with EID 103.1
load_target_table('/cf/con_sub_rly3.tbl', target_4) 

## Set up and start Contact 0
cmd(f"{target_4} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target_4} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

##-------------------------------------------------------------------------------------------------------------------
## 2.8 Destination Node Stop/Start
##-------------------------------------------------------------------------------------------------------------------

## Print Node Startup Counter prior to node stop/start
DST_NODE_STARTUP_COUNTER_PRE = tlm(f"{target_5} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Destination Node Startup Counter:", DST_NODE_STARTUP_COUNTER_PRE)

## Send stop command
cmd(f"{target_5} CFE_ES_CMD_STOP_APP with APPLICATION 'BPNODE'")
wait(40)

## Send start command
cmd(f"{target_5} CFE_ES_CMD_START_APP with APPLICATION 'BPNODE', APP_ENTRY_POINT 'BPNode_AppMain', APP_FILE_NAME '/cf/bpnode.so'")
wait(20)

## Print Node Startup Counter post node stop/start
DST_NODE_STARTUP_COUNTER_POST = tlm(f"{target_5} BPNODE_NODE_MIB_REPORTS_HK NODE_STARTUP_COUNTER")
print("Destination Node Startup Counter:", DST_NODE_STARTUP_COUNTER_POST)

## Verify successful node stop/start
assert DST_NODE_STARTUP_COUNTER_POST == DST_NODE_STARTUP_COUNTER_PRE + 1

## Verify requirements
rqmnt_status["DTN.5.01025"] = "P"
rqmnt_status["DTN.5.01065"] = "P"
rqmnt_status["DTN.6.10111"] = "P"

## Stop and remove Channel 0
cmd(f"{target_5} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)

## Load MIB PN table modified to set node number to 105
load_target_table('/cf/pn_sub_dst.tbl', target_5)

## Load ADU Proxy table modified to deliver CF PDUs (0x83E) on Channel 1
load_target_table('/cf/adu_sub_dst.tbl', target_5) 

## Load Channel 1 table modified to deliver bundles with DST EID 105.1
load_target_table('/cf/cha_sub_dst.tbl', target_5)

## Load Contact 0 table for receiving bundles on port 4559
load_target_table('/cf/con_sub_dst.tbl', target_5) 

## Load modified CF configuration table to receive 20 PDUs per second
cmd(f"{target_5} CF_CMD_DISABLE_ENGINE")
load_target_table('/cf/cf_sub_dst.tbl', target_5)
cmd(f"{target_5} CF_CMD_ENABLE_ENGINE")

## Restart Channel 0
cmd(f"{target_5} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
wait(1)

## Start Channel 1
cmd(f"{target_5} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_5} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait(10)

## Set up and start Contact 0
cmd(f"{target_5} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

##-------------------------------------------------------------------------------------------------------------------
## 2.9 ADU Delivery when Channel 1 is active
##-------------------------------------------------------------------------------------------------------------------

## Start creating bundles (CF transmitting a second file)
CFDP_PDU_count_2 = 764
cmd(f"{target_1} CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/Jojo2.jpg', DST_FILENAME '/cf/Jojo2.jpg'")

## Wait for all bundles to have been delivered by the Destination Node
wait_check(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED == {CFDP_PDU_count_2}", 300)
wait(10)

## Print counter values
SRC_CFDP_PDU_COUNT_SENT = tlm(f"{target_1} CF_HK CHANNEL_HK_0_COUNTERS_SENT_PDU")
print("Source Node CFDP PDUs Sent:", SRC_CFDP_PDU_COUNT_SENT)
SRC_ADU_COUNT_RECEIVED = tlm(f"{target_1} BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
print("Source Node ADUs Received:", SRC_ADU_COUNT_RECEIVED)
SRC_BUNDLE_COUNT_STORED = tlm(f"{target_1} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Source Node Bundles Stored:", SRC_BUNDLE_COUNT_STORED)

RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)
RLY2_BUNDLE_COUNT_STORED = tlm(f"{target_3} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #2 Bundles Stored:", RLY2_BUNDLE_COUNT_STORED)
RLY3_BUNDLE_COUNT_STORED = tlm(f"{target_4} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #3 Bundles Stored:", RLY3_BUNDLE_COUNT_STORED)

DST_BUNDLE_COUNT_RECEIVED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
print("Destination Node Bundles Received:", DST_BUNDLE_COUNT_RECEIVED)
DST_BUNDLE_COUNT_DELETED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
print("Destination Node Bundles Deleted:", DST_BUNDLE_COUNT_DELETED)
DST_BUNDLE_COUNT_STORED = tlm(f"{target_5} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Destination Node Bundles Stored:", DST_BUNDLE_COUNT_STORED)
DST_BUNDLE_COUNT_DELIVERED = tlm(f"{target_5} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELIVERED")
print("Destination Node Bundles Delivered:", DST_BUNDLE_COUNT_DELIVERED)
DST_CFDP_PDU_COUNT_RECEIVED = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_PDU")
print("Destination Node CFDP PDUs received:", DST_CFDP_PDU_COUNT_RECEIVED)
DST_CFDP_PDU_COUNT_DROPPED = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_DROPPED")
print("Destination Node CFDP PDUs received and dropped:", DST_CFDP_PDU_COUNT_DROPPED)
DST_CFDP_PDU_COUNT_ERROR = tlm(f"{target_5} CF_HK CHANNEL_HK_0_COUNTERS_RECV_ERROR")
print("Destination Node CFDP received PDU errors:", DST_CFDP_PDU_COUNT_ERROR)

## Verify successful end to end data transmission
assert SRC_CFDP_PDU_COUNT_SENT == CFDP_PDU_count_1 + CFDP_PDU_count_2
assert SRC_ADU_COUNT_RECEIVED == CFDP_PDU_count_2
assert SRC_BUNDLE_COUNT_STORED == 0
assert RLY1_BUNDLE_COUNT_STORED >= 43
assert RLY2_BUNDLE_COUNT_STORED == 43
assert RLY3_BUNDLE_COUNT_STORED == 0
assert DST_BUNDLE_COUNT_RECEIVED == CFDP_PDU_count_2
assert DST_BUNDLE_COUNT_DELETED == 0
assert DST_BUNDLE_COUNT_STORED == 0
assert DST_BUNDLE_COUNT_DELIVERED == CFDP_PDU_count_2
assert DST_CFDP_PDU_COUNT_RECEIVED == CFDP_PDU_count_1 + CFDP_PDU_count_2
assert DST_CFDP_PDU_COUNT_DROPPED == 0
assert DST_CFDP_PDU_COUNT_ERROR == 0

## Verify requirements
rqmnt_status["DTN.5.00730"] = "P"
rqmnt_status["DTN.5.00760"] = "P"
rqmnt_status["DTN.5.00770"] = "P"
rqmnt_status["DTN.5.00788"] = "P"

##-------------------------------------------------------------------------------------------------------------------
## 2.10 Handling of expired bundles
##-------------------------------------------------------------------------------------------------------------------

## Inject 100 expired bundles to Relay Node #3
bundles_send(1000, 5, 2, "10.2.14.233", 4558, 1)
wait(10)

## Print counter values
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)
RLY1_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #1 Bundles Deleted Expired:", RLY1_BUNDLE_COUNT_DELETED_EXPIRED)

RLY2_BUNDLE_COUNT_STORED = tlm(f"{target_3} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #2 Bundles Stored:", RLY2_BUNDLE_COUNT_STORED)
RLY2_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #2 Bundles Deleted Expired:", RLY2_BUNDLE_COUNT_DELETED_EXPIRED)

RLY3_BUNDLE_COUNT_STORED = tlm(f"{target_4} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #3 Bundles Stored:", RLY3_BUNDLE_COUNT_STORED)
RLY3_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_4} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #3 Bundles Deleted Expired:", RLY3_BUNDLE_COUNT_DELETED_EXPIRED)

## Verify successful bundle expiration on ingress
assert RLY1_BUNDLE_COUNT_STORED >= 43
assert RLY1_BUNDLE_COUNT_DELETED_EXPIRED == 0
assert RLY2_BUNDLE_COUNT_STORED == 43
assert RLY2_BUNDLE_COUNT_DELETED_EXPIRED == 0
assert RLY3_BUNDLE_COUNT_STORED == 0
assert RLY3_BUNDLE_COUNT_DELETED_EXPIRED == 100

## Verify requirements
rqmnt_status["DTN.5.00197"] = "P"

## Stop Contact 0 on all nodes
cmd(f"{target_1} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_2} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_3} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_4} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)
cmd(f"{target_5} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(10)

## Wait for all stored bundles to have been expired
wait_check(f"{target_3} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED == 0", 300)
wait(60)

## Print counter values
RLY1_BUNDLE_COUNT_STORED = tlm(f"{target_2} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #1 Bundles Stored:", RLY1_BUNDLE_COUNT_STORED)
RLY1_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_2} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #1 Bundles Deleted Expired:", RLY1_BUNDLE_COUNT_DELETED_EXPIRED)

RLY2_BUNDLE_COUNT_STORED = tlm(f"{target_3} BPNODE_NODE_MIB_REPORTS_HK BUNDLE_COUNT_STORED")
print("Relay Node #2 Bundles Stored:", RLY2_BUNDLE_COUNT_STORED)
RLY2_BUNDLE_COUNT_DELETED_EXPIRED = tlm(f"{target_3} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_EXPIRED")
print("Relay Node #2 Bundles Deleted Expired:", RLY2_BUNDLE_COUNT_DELETED_EXPIRED)

## Verify successful bundle expiration from storage
assert RLY1_BUNDLE_COUNT_STORED == 0
assert RLY1_BUNDLE_COUNT_DELETED_EXPIRED >= 43
assert RLY2_BUNDLE_COUNT_STORED == 0
assert RLY2_BUNDLE_COUNT_DELETED_EXPIRED == 43

## Verify requirements
rqmnt_status["DTN.5.00800"] = "P"
rqmnt_status["DTN.5.00820"] = "P"

prompt("Stop the Wireshark capture of bundles on Relay Node #2")
prompt("Change the packet decoding from UDP to BP (using right click/decode as.../Current)")
prompt("Reorder the packets by Time")

prompt("Verify that the FIRST bundle with Destination EID 104.1 and verify that it contains a Type 73 unknown canonical block")
prompt("Verify that the SECOND bundle with Destination EID 104.1 DOES NOT contain an unknown canonical block")

prompt("Scroll down to the FIRST bundle with Destination EID 105.1")
prompt("Verify that this bundle contains valid primary, previous node, bundle age, hop count, and payload blocks")
prompt("Verify that the primary block of this bundle contains a valid creation timestamp")
prompt("Verify that the Previous Node ID = 102.0 in the Previous Node block")
prompt("Verify that the Bundle Age > 340,000 ms in the Bundle Age block")
prompt("Verify that the Hop Count = 2 in the Hop Count block")

## Verify requirements
rqmnt_status["DTN.5.00060"] = "P"
rqmnt_status["DTN.5.00070"] = "P"
rqmnt_status["DTN.5.00090"] = "P"
rqmnt_status["DTN.5.00100"] = "P"
rqmnt_status["DTN.5.00250"] = "P"
rqmnt_status["DTN.5.00270"] = "P"
rqmnt_status["DTN.5.00275"] = "P"
rqmnt_status["DTN.5.00280"] = "P"
rqmnt_status["DTN.5.00670"] = "P"
rqmnt_status["DTN.5.00680"] = "P"

prompt("Save the Wireshark capture as an artifact")

##-------------------------------------------------------------------------------------------------------------------
## 3. Test Cleanup
##-------------------------------------------------------------------------------------------------------------------

## Tear down Contact 0 on all nodes
cmd(f"{target_1} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)
cmd(f"{target_2} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)
cmd(f"{target_3} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)
cmd(f"{target_4} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)
cmd(f"{target_5} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

## Stop and remove Channel 1 on Source and Destination Nodes
cmd(f"{target_1} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_1} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_5} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
wait(1)
cmd(f"{target_5} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
wait(1)

## Reset all error counters
cmd(f"{target_1} BPNODE_CMD_RESET_ERROR_COUNTERS")
wait(1)
cmd(f"{target_2} BPNODE_CMD_RESET_ERROR_COUNTERS")
wait(1)
cmd(f"{target_3} BPNODE_CMD_RESET_ERROR_COUNTERS")
wait(1)
cmd(f"{target_4} BPNODE_CMD_RESET_ERROR_COUNTERS")
wait(1)
cmd(f"{target_5} BPNODE_CMD_RESET_ERROR_COUNTERS")
wait(10)

## Reset all bundle counters
cmd(f"{target_1} BPNODE_CMD_RESET_BUNDLE_COUNTERS")
wait(1)
cmd(f"{target_2} BPNODE_CMD_RESET_BUNDLE_COUNTERS")
wait(1)
cmd(f"{target_3} BPNODE_CMD_RESET_BUNDLE_COUNTERS")
wait(1)
cmd(f"{target_4} BPNODE_CMD_RESET_BUNDLE_COUNTERS")
wait(1)
cmd(f"{target_5} BPNODE_CMD_RESET_BUNDLE_COUNTERS")
wait(10)

## Reset all CFDP counters
cmd(f"{target_1} CF_CMD_RESET")
wait(1)
cmd(f"{target_5} CF_CMD_RESET")
wait(1)

# Stop running scripts
running_script_stop(id_1)
running_script_stop(id_2)
running_script_stop(id_3)
running_script_stop(id_4)
running_script_stop(id_5)

# Print Requirement Status
for key, value in rqmnt_status.items():
    print(f"***    {key}: {value}")
