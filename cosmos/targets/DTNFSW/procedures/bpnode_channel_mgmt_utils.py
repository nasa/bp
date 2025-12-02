
from dtntools.dtngen.blocks import (
    PayloadBlock,
    PrevNodeBlock,
    PrimaryBlock,
)
from dtntools.dtngen.bundle import Bundle
from dtntools.dtngen.types import (
    EID,
    BlockType,
    BundlePCFlags,
    CRCFlag,
    CRCType,
    CreationTimestamp
)


##################################################################################
# Add channel X application configurations
##################################################################################
def dtnfsw_bpnode_add_channel(dtnfsw_target_name, bpnode_channel_index):
    # Check that initial application state is REMOVED
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
    wait_check(f"{dtnfsw_target_name} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_{bpnode_channel_index} == 'REMOVED'", 10)

    current_valid_cmd_count = tlm(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    expected_valid_cmd_count = current_valid_cmd_count + 1

    # Add channel 1 application
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_ADD_APPLICATION with CHAN_ID {bpnode_channel_index}")
    wait_check(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

    # Check that application state has changed to ADDED
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
    wait_check(f"{dtnfsw_target_name} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_{bpnode_channel_index} == 'ADDED'", 10)


##################################################################################
# Start channel X application
##################################################################################
def dtnfsw_bpnode_start_channel(dtnfsw_target_name, bpnode_channel_index):
    current_valid_cmd_count = tlm(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    expected_valid_cmd_count = current_valid_cmd_count + 1

    # Start channel 1 application
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_START_APPLICATION with CHAN_ID {bpnode_channel_index}")
    wait_check(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

    # Check that application state has changed to STARTED
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
    wait_check(f"{dtnfsw_target_name} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_{bpnode_channel_index} == 'STARTED'", 10)

##################################################################################
# Stop channel X application
##################################################################################
def dtnfsw_bpnode_stop_channel(dtnfsw_target_name, bpnode_channel_index):
    current_valid_cmd_count = tlm(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    expected_valid_cmd_count = current_valid_cmd_count + 1

    # Stop channel 1 application
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_STOP_APPLICATION with CHAN_ID {bpnode_channel_index}")
    wait_check(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

    # Check that application state has changed to STOPPED
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
    wait_check(f"{dtnfsw_target_name} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_{bpnode_channel_index} == 'STOPPED'", 10)

##################################################################################
# Remove channel X application
##################################################################################
def dtnfsw_bpnode_remove_channel(dtnfsw_target_name, bpnode_channel_index):
    current_valid_cmd_count = tlm(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    expected_valid_cmd_count = current_valid_cmd_count + 1

    # Remove channel 1 application
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID {bpnode_channel_index}")
    wait_check(f"{dtnfsw_target_name} BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

    # Check that application state has changed to REMOVED
    cmd(f"{dtnfsw_target_name} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
    wait_check(f"{dtnfsw_target_name} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_{bpnode_channel_index} == 'REMOVED'", 10)
