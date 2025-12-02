wait_check_packet(f"<%= target_name %>", "BPACC_FUN_HK", 1, 6)
cmd_count = tlm(f"<%= target_name %> BPACC_FUN_HK COMMAND_COUNTER")

cmd(f"<%= target_name %> BPACC_FUN_CMD_NOOP")
wait_check(f"<%= target_name %> BPACC_FUN_HK COMMAND_COUNTER == {cmd_count+1}", 10)
# Check accepted Reset Counters command
cmd(f"<%= target_name %> BPACC_FUN_CMD_RESET_COUNTERS")
wait_check(f"<%= target_name %> BPACC_FUN_HK COMMAND_COUNTER == 0", 10)