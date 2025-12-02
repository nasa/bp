'''
ADU Delivery Demo outline

Note: Set up and start Node-2 first

Node 1
- Setup
    - Load  ADU Proxy table modified to receive CF PDUs (0x83E) on Channel 0
    - Load Contact 0 table modified to set Node-2 destination address/port 4502
    - Restart Channel 0 after table updates
- Start contact
- Send CF command to transmit a file
    - Receive ADUs, generate bundles and forward to Node-2

Node 2
- Setup
    - Load MIB PN table modified to set node EID as destination EID
    - Load Contact 0 table for receive only (no destination EID) on port 4502
    - Load CF Config table modified to receive PDUs (0x83E, default 0x183F)
    - Load Channel 0 table modified to set local service number to 64 (destination)
    - Restart Channel 0 after table updates
- Start Contact
    - Wait for bundles from Node-1
- Receive bundles
    - Deliver ADUs, assembled file
'''

'''
NOTE: 
    - For large CF files load cf_config_slow.tbl to work around:
        CFE_SB 17: Msg Limit Err,MsgId 0x83e,pipe BPNODE_ADU_PIPE_0,sender CF
'''

prompt("Set up and start Node-2 first ...")

load_utility('DTNFSW-1/procedures/load_new_table.py')

## Modify ADU Proxy table to receive CF PDUs (0x83E) on Channel 0
load_new_table('/cf/adupcftx.tbl') 

## Modify Contact 0 table to set Node-2 destination address/port 4502
load_new_table('/cf/contact_node2.tbl')

'''
## Modify CF configuration table to slow down PDU transmission
cmd("DTNFSW-1 CF_CMD_DISABLE_ENGINE")
load_new_table('/cf/cf_config_slow.tbl')
cmd("DTNFSW-1 CF_CMD_ENABLE_ENGINE")
'''

## Restart Channel 0 after table updates
cmd("DTNFSW-1 BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-1 BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-1 BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-1 BPNODE_CMD_START_APPLICATION with CHAN_ID 0")

## Setup and start contact
cmd("DTNFSW-1 BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait (1)
cmd("DTNFSW-1 BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(1)

cmd("DTNFSW-1 BPNODE_CMD_RESET_ALL_COUNTERS")

wait()

## Send CF command to transmit a file
cmd("DTNFSW-1 CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/file_16.dat', DST_FILENAME '/cf/file16.temp'")
#cmd("DTNFSW-1 CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/file_1024.dat', DST_FILENAME '/cf/file1024.temp'")
#cmd("DTNFSW-1 CF_CMD_TX_FILE with CFDP_CLASS 'CLASS_1', KEEP 1, CHAN_NUM 0, DEST_ID 1, SRC_FILENAME '/cf/ci_lab.so', DST_FILENAME '/cf/test.temp'")

wait()

## Stop and teardown contact
cmd("DTNFSW-1 BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait (1)
cmd("DTNFSW-1 BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait()
