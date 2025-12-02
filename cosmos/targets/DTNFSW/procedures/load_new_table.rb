#
# Procedure: Load, validate, and activate a new table
#
# Assumptions:
#   - The new table is already in the FSW /cf/ directory
#

def load_new_table(new_tbl_file = "")
    tbl_buffer = 0 # 0 for inactive buffer, 1 for active buffer

    # Load new table
    if new_tbl_file == ""
        new_tbl_file = ask("Enter the file path of the new file to load (should start with '/cf/'):")
    end

    curr_tbl_cmd_count = tlm("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER")
    exp_tbl_cmd_count = curr_tbl_cmd_count + 1

    cmd("<%= target_name %> CFE_TBL_CMD_LOAD with LOAD_FILENAME '#{new_tbl_file}'")
    wait_check("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER == #{exp_tbl_cmd_count}", 10)

    # Validate new table
    new_tbl_name = tlm("<%= target_name %> CFE_TBL_HK LAST_TABLE_LOADED")

    curr_tbl_cmd_count = tlm("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER")
    exp_tbl_cmd_count = curr_tbl_cmd_count + 1

    curr_success_val_count = tlm('<%= target_name %> CFE_TBL_HK SUCCESS_VAL_COUNTER')
    exp_success_val_count = curr_success_val_count + 1

    cmd("<%= target_name %> CFE_TBL_CMD_VALIDATE with TABLE_NAME '#{new_tbl_name}', ACTIVE_TABLE_FLAG #{tbl_buffer}")
    wait_check("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER == #{exp_tbl_cmd_count}", 10)

    wait_check("<%= target_name %> CFE_TBL_HK SUCCESS_VAL_COUNTER == #{exp_success_val_count}", 20)

    # Activate new table
    curr_tbl_cmd_count = tlm("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER")
    exp_tbl_cmd_count = curr_tbl_cmd_count + 1

    cmd("<%= target_name %> CFE_TBL_CMD_ACTIVATE with TABLE_NAME '#{new_tbl_name}'")
    wait_check("<%= target_name %> CFE_TBL_HK COMMAND_COUNTER == #{exp_tbl_cmd_count}", 10)

    printf("The %s file has been successfully loaded into the %s active buffer", new_tbl_file, new_tbl_name)
end