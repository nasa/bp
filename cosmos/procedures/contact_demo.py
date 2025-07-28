##******************************************************************************
## CONTACT DEMO OUTLINE
##
## 1. Contact 0 sequence 1 - receives and stores bundles (no forwarding)
##      Load contact table 1 - bundle DEST EID x NOT included
##      Setup contact
##      Start contact
##      Send a set of bundles from DTN Tools with DEST EID = x
##      Observe the set of bundles are stored on the node
##      Stop contact
##      Teardown contact
##
## 2. Contact 0 sequence 2 - forwards bundles
##      Load contact table 2 - bundle DEST EID x included
##      Setup contact
##      Start contact
##      Observe the first set of bundles being received by DTN Tools
##      Send a second set of bundles from DTN Tools with DEST EID = x
##      Observe the second set of bundles being received by DTN Tools
##      Stop contact
##      Teardown contact
##******************************************************************************
from dtntools.dtncla.udp import UdpTxSocket, UdpRxSocket
load_utility ("DTNFSW-1/procedures/dtngen_utils.py")
load_utility ("DTNFSW-1/procedures/load_new_table.py")
   
target = "DTNFSW-1"
mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK"

## Address/port configuration
dest_ip   = "10.2.11.75"
dest_port = 4501
local_ip = "0.0.0.0"
local_port = 4551

## Setup data sender/receiver connections
data_sender = UdpTxSocket(dest_ip, dest_port)
data_receiver = UdpRxSocket(local_ip, local_port)

data_sender.connect()
data_receiver.connect()

## Destination EID configuration
dest_node    = 200
dest_service = 64

## Generate test bundles
num_bundles_gen = 10
payload = b'\xAA'*1000
DTNGenUtils.generate_bundles(dest_node, dest_service, num_bundles_gen, payload)

cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")    

##******************************************************************************

##------------------------------------------------------------------------------
## Contact 0 sequence 1 - receives and stores bundles (no forwarding)
##------------------------------------------------------------------------------

## Load receive-only contact table
load_new_table('/cf/contact_rx_only.tbl')

# Connect data sender
#data_sender.connect()

## Setup and start contact
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait (2)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(5)

## Send a set of bundles to node for storage
num_loops = 20
with disable_instrumentation():
    for _ in range(num_loops):
        DTNGenUtils.send_bundles(num_bundles_gen, dest_node, data_sender)
num_bundles_1 = num_loops*num_bundles_gen

## Observe bundles are received and stored on the node
wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED == {num_bundles_1}", 10)
#wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_STORED == {num_bundles_1}", 1)
wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED == 0", 1)

wait()

## Stop and teardown contact
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait (2)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")

# Disconnect data sender
#data_sender.disconnect()

##------------------------------------------------------------------------------
## Contact 0 sequence 2 - forwards bundles
##------------------------------------------------------------------------------

## Load nominal (receive and transmit) contacts table
load_new_table('/cf/contact_nominal.tbl')

# Make data sender/receiver connections
#data_sender.connect()
#data_receiver.connect()

## Setup and start contact
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait (2)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")

## Observe stored bundles are forwarded, and received by data receiver
wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED == {num_bundles_1}", 10)

DTNGenUtils.receive_bundles(num_bundles_1, data_receiver)

wait()

## Send second set of bundles to node
num_loops = 10
with disable_instrumentation():
    for _ in range(num_loops):
        DTNGenUtils.send_bundles(num_bundles_gen, dest_node, data_sender)
num_bundles_2 = num_loops*num_bundles_gen

## Observe second set of bundles are forwarded, and received by data receiver
wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED == {num_bundles_1+num_bundles_2}", 10)
wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED == {num_bundles_1+num_bundles_2}", 10)

DTNGenUtils.receive_bundles(num_bundles_2, data_receiver)

wait()

## Stop and teardown contact
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait (2)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")

##******************************************************************************

## Disconnect data sender/receiver
data_receiver.disconnect()
data_sender.disconnect()


