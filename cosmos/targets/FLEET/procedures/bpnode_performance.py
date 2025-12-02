##******************************************************************************
##
## Performance test of BPNode in Relay Mode - ingress -> egress
##
## See Test Case TBD for setup information
##      
##******************************************************************************

import os
from datetime import datetime

# Setting Script Runner line delay
set_line_delay(0.000)

load_utility("FLEET/procedures/bpnode_initialization.py")
load_utility("FLEET/procedures/load_target_table.py")
#
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

##------------------------------------------------------------------------------
## Test Start
##------------------------------------------------------------------------------

prompt("Start a Wireshark capture of UDP packets on port 4558 on the EC2 instance BPNode is egressing to")

Start_Time = datetime.now()
print(f"Test Start Time = {Start_Time}")

## Load contact table modified to set dest EID to 103
load_target_table('/cf/con_perf.tbl', target)

## Setup and start contact
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

## Execute Bundle Ingress
prompt("Execute TX-performance.py script")

##------------------------------------------------------------------------------
## Test Stop
##------------------------------------------------------------------------------

## Stop and tear down contact
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

Stop_Time = datetime.now()
print(f"Test Start Time = {Stop_Time}")

# Stop running scripts
running_script_stop(id_1)