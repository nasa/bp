#
# Procedure: Confirm that an invalid table fails validation and activation
# 
# Example usage: reject_target_table("/cf/test_channel.tbl", "DTNFSW-2")
#
def reject_target_table(new_tbl_file = None, target = None):
    tbl_buffer = 0 # 0 for inactive buffer, 1 for active buffer

    # Load new table
    if new_tbl_file is None:
        new_tbl_file = ask("Enter the file path of the new file to load (should start with '/cf/'):")
    
    if target is None:
        target = 'DTNFSW-1'
        
    curr_tbl_cmd_count = tlm(f"{target} CFE_TBL_HK COMMAND_COUNTER")    
    exp_tbl_cmd_count = (curr_tbl_cmd_count + 1)%256

    cmd(f"{target} CFE_TBL_CMD_LOAD with LOAD_FILENAME '{new_tbl_file}'")
    wait_check(f"{target} CFE_TBL_HK COMMAND_COUNTER == {exp_tbl_cmd_count}", 10)

    # Validate new table
    new_tbl_name = tlm(f"{target} CFE_TBL_HK LAST_TABLE_LOADED")

    curr_tbl_cmd_count = tlm(f"{target} CFE_TBL_HK COMMAND_COUNTER")    
    exp_tbl_cmd_count = (curr_tbl_cmd_count + 1)%256

    curr_success_val_count = tlm(f'{target} CFE_TBL_HK SUCCESS_VAL_COUNTER')
    exp_success_val_count = (curr_success_val_count)%256

    cmd(f"{target} CFE_TBL_CMD_VALIDATE with TABLE_NAME '{new_tbl_name}', ACTIVE_TABLE_FLAG {tbl_buffer}")
    wait_check(f"{target} CFE_TBL_HK COMMAND_COUNTER == {exp_tbl_cmd_count}", 10)

    wait_check(f"{target} CFE_TBL_HK SUCCESS_VAL_COUNTER == {exp_success_val_count}", 20)

    # Activate new table
    curr_tbl_cmd_count = tlm(f"{target} CFE_TBL_HK COMMAND_COUNTER")    
    exp_tbl_cmd_count = (curr_tbl_cmd_count)%256

    cmd(f"{target} CFE_TBL_CMD_ACTIVATE with TABLE_NAME '{new_tbl_name}'")
    wait_check(f"{target} CFE_TBL_HK COMMAND_COUNTER == {exp_tbl_cmd_count}", 10)

    print(f"The {new_tbl_file} file failed validation and has NOT been loaded into the {new_tbl_name} active buffer")
