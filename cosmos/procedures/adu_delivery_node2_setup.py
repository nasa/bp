'''
ADU Delivery Demo - Node-2

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

load_utility('DTNFSW-2/procedures/load_new_table.py')

## Modify MIB PN table to set node EID as destination EID
load_new_table('/cf/mib_pn_200_64.tbl')

## Modify Contact 0 table for receive only on port 4502
load_new_table('/cf/cont_n2_rx_only.tbl')

## Modify CF Config table to receive 0x83E PDUs (default 0x183F)
cmd("DTNFSW-2 CF_CMD_DISABLE_ENGINE")
load_new_table('/cf/cf_rx_083E.tbl')
cmd("DTNFSW-2 CF_CMD_ENABLE_ENGINE")

## Modify Channel 0 table to set local service number to 64 (destination)
load_new_table('/cf/chan_serv_64.tbl')

## Restart Channel 0 after table updates
cmd("DTNFSW-2 BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-2 BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-2 BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
wait(1)
cmd("DTNFSW-2 BPNODE_CMD_START_APPLICATION with CHAN_ID 0")

## Setup and start Contact 0
cmd("DTNFSW-2 BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait (1)
cmd("DTNFSW-2 BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait (1)
cmd("DTNFSW-2 BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait (1)
cmd("DTNFSW-2 BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(1)


cmd("DTNFSW-2 BPNODE_CMD_RESET_ALL_COUNTERS")
