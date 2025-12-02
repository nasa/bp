##******************************************************************************
##
## Performance test of BPNode in Relay Mode - ingress -> storage -> egress
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

##------------------------------------------------------------------------------
## Contact 1 (Ingress Only) Setup, Execution, and Teardown
##------------------------------------------------------------------------------

## Load contact table modified to set dest EID to 104
load_target_table('/cf/con_perf_st.tbl', target)

## Setup and start contact
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait(10)

## Execute Bundle Ingress
prompt("Execute TX-performance.py script")

## Stop and tear down Contact 1
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(10)

##------------------------------------------------------------------------------
## Contact 2 (Egress Only) Setup, Execution, and Stop
##------------------------------------------------------------------------------

prompt("Start a Wireshark capture of UDP packets on port 4558 on the EC2 instance BPNode is egressing to")

## Load contact table modified to set dest EID to 103
load_target_table('/cf/con_perf.tbl', target)

## Setup and start Contact 2
cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
wait()

## Stop Contact 2
cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
wait(1)
cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
wait(1)

End_Time = datetime.now()
print(f"Test End Time = {End_Time}")

# Stop running scripts
running_script_stop(id_1)
