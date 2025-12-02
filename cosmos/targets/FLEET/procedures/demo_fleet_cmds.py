from openc3.script import *

# Create a list of fsw targets
fsw_targets_to_test = []
fsw_targets_to_test.append("DTNFSW-1")
fsw_targets_to_test.append("DTNFSW-2")
fsw_targets_to_test.append("DTNFSW-3")
fsw_targets_to_test.append("DTNFSW-4")
fsw_targets_to_test.append("DTNFSW-5")

# Enable Telemetry for all FSW nodes
cosmos_ip_addr = ask("Enter the IP address that FSW nodes should send telemetry to (default: 127.0.0.1):")
for fsw_target in fsw_targets_to_test:
    cmd(f"{fsw_target} TO_LAB_CMD_ENABLE_OUTPUT with DEST_IP '{cosmos_ip_addr}'")

"""
FSW Aliveness Test
- Send a no-op command to each cFS FSW app in the system
    then verify the command was received (by checking telemetry)
- Reset the command counter
    then verify the command was received (by checking telemetry)
"""
# Create a list of app noops to test
fsw_apps_to_test = []
fsw_apps_to_test.append(["CFE_ES_CMD_NOOP",   "CFE_ES_CMD_RESET_COUNTERS",     "CFE_ES_HK",                   "COMMAND_COUNTER" ])
fsw_apps_to_test.append(["CFE_EVS_CMD_NOOP",  "CFE_EVS_CMD_RESET_COUNTERS",    "CFE_EVS_HK",                  "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["CFE_SB_CMD_NOOP",   "CFE_SB_CMD_RESET_COUNTERS",     "CFE_SB_HK",                   "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["CFE_TBL_CMD_NOOP",  "CFE_TBL_CMD_RESET_COUNTERS",    "CFE_TBL_HK",                  "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["CFE_TIME_CMD_NOOP", "CFE_TIME_CMD_RESET_COUNTERS",   "CFE_TIME_HK",                 "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["CI_LAB_CMD_NOOP",   "CI_LAB_CMD_RESET_COUNTERS",     "CI_LAB_HK",                   "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["TO_LAB_CMD_NOOP",   "TO_LAB_CMD_RESET_COUNTERS",     "TO_LAB_HK",                   "COMMAND_COUNTER" ])
# fsw_apps_to_test.append(["CF_CMD_NOOP",       "CF_CMD_RESET",                  "CF_HK",                       "COUNTERS_CMD" ])
# fsw_apps_to_test.append(["BPNODE_CMD_NOOP",   "BPNODE_CMD_RESET_ALL_COUNTERS", "BPNODE_NODE_MIB_COUNTERS_HK", "BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT" ])

for fsw_target in fsw_targets_to_test:
    # For each app in the list, verify its alive by sending a noop, then reset the hk counters
    for noop_cmd_name, reset_counters_cmd_name, hk_packet_name, hk_cmd_count_name in fsw_apps_to_test:
        # Verify that we have a recent packet (by waiting for a new one to arrive)
        wait_check_packet(f"{fsw_target}", f"{hk_packet_name}", 1, 6)
        # Assuming no one else is sending commands, grab the latest command count
        cmd_count = tlm(f"{fsw_target} {hk_packet_name} {hk_cmd_count_name}")
        # Check accepted NOOP command proving application is up and running
        cmd(f"{fsw_target} {noop_cmd_name}")
        wait_check(f"{fsw_target} {hk_packet_name} {hk_cmd_count_name} == {cmd_count+1}", 10)
        # Check accepted Reset Counters command
        cmd(f"{fsw_target} {reset_counters_cmd_name}")
        wait_check(f"{fsw_target} {hk_packet_name} {hk_cmd_count_name} == 0", 10)
