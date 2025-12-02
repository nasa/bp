#
# This test demonstrates using the DTN Test Tools to
# send a bundle to the BPNode (using the UDP CLA)
# This bundle includes a cfe ES command no-op
#
# This bundle is expected to be routed to channel 1,
# then wrapped in an DTN_ADU_OUT tlm header, then posted to the SB
# then we expect to receive this tlm packet in cosmos
#
# Assumptions:
# - FSW is running and telemetry is enabled
# - FSW is set up to route a bundle of 61 bytes to BPNode channel 1
#   - FSW is set up to check for this size,
#     and jam the Dest EID to "ipn:100.53" which will route to BPNode channel 1
#   - see FSW defs:
#     - BPLIB_TEMPORARY_EID_NODE_NUM_FOR_CHANNEL_ROUTES
#     - BPLIB_TEMPORARY_EID_SERVICE_NUM_FOR_CHANNEL_0_ROUTES
#     - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_1
#     - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_2
# - BPNode channel 1 egress has ADU Wrapping enabled
# - BPNode Channel 1 is not added by default (so we need to add/start it)
#   then we also stop/remove it at the end


import os
import time
import traceback

from dtntools.dtncla.udp import UdpRxSocket, UdpTxSocket
from dtntools.dtngen.blocks import (
    PayloadBlock,
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

# Networking Configuration: Please ensure you have port forwarding rules
BPNODE_IP = "172.17.0.1"
BPNODE_PORT = <%= dtnfsw_get_cla_in_port(target_name, 0) %>
LOCAL_IP = "0.0.0.0"
LOCAL_PORT = <%= dtnfsw_get_cla_out_port(target_name, 0) %>


# Dest EID Configuration
DEST_NODE_NUM = 100
DEST_SERVICE_NUM = 0x42

# Source EID Configuration (not really used, at the moment)
SRC_NODE_NUM = 99
SRC_SERVICE_NUM = 1

# This test sends a Raw ES No-Op Command in the bundle payload
ccsds_cmd_inject_payload=bytes([0x18, 0x06, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x00])



# Hopefully you don't have to change anything below here
def create_test_bundle(payload_bytes):
    """Create a test bundle."""
    primary_block = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": DEST_NODE_NUM, "service_num": DEST_SERVICE_NUM}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": SRC_NODE_NUM, "service_num": SRC_SERVICE_NUM}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": SRC_NODE_NUM, "service_num": SRC_SERVICE_NUM}}),
        creation_timestamp=CreationTimestamp({"time": 755533838904, "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    payload_block = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=payload_bytes,
        crc=CRCFlag.CALCULATE,
    )

    # Use them to create a bundle object
    bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            payload_block,
        ],
    )

    # Encode the bundle
    encoded = bundle.to_bytes()
    return encoded




##################################################################################
# Add channel 1 application configurations
##################################################################################

# Check that initial application state is REMOVED
cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 10)

current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
expected_valid_cmd_count = current_valid_cmd_count + 1

# Add channel 1 application
cmd(f"<%= target_name %> BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

# Check that application state has changed to ADDED
cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'ADDED'", 10)




##################################################################################
# Start channel 1 application
##################################################################################

current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
expected_valid_cmd_count = current_valid_cmd_count + 1

# Start channel 1 application
cmd(f"<%= target_name %> BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

# Check that application state has changed to STARTED
cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'", 10)

current_valid_adu_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
expected_valid_adu_count = current_valid_adu_count + 1

# if we decide to use ADU wrapping
current_adu_pkt_count = tlm(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT")
if current_adu_pkt_count is None:
    current_adu_pkt_count = 0        
exp_adu_pkt_count = current_adu_pkt_count + 1





##################################################################################
# Prepare to transmit
##################################################################################

data_sender = UdpTxSocket(BPNODE_IP, BPNODE_PORT)

try:
    # Connect the data sender and receiver tools
    data_sender.connect()

    encoded_bundle = create_test_bundle(ccsds_cmd_inject_payload)
    data_sender.write(encoded_bundle)
    print(f"Sending bundle of {len(encoded_bundle)} bytes")

    # Wait for one ADU out packet to be received by COSMOS
    wait_check(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT == {exp_adu_pkt_count}", 10)

    # Verify that the length stated in the header matches the relevant payload plus
    # 16 bytes for the telemetry header wrapping
    # CCSDS_LENGTH field is n-1 and doesn't include the primary header (so full length - 7)
    recvd_adu_len = tlm(f"<%= target_name %> DTN_ADU_OUT CCSDS_LENGTH")
    exp_adu_len = 16 + len(encoded_bundle) - 7
    check_expression(f"'{recvd_adu_len}' == '{exp_adu_len}'")


except KeyboardInterrupt:
    pass

except Exception:
    print(traceback.format_exc())

finally:
    data_sender.disconnect()


##################################################################################
# Stop channel 1 application
##################################################################################

current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
expected_valid_cmd_count = current_valid_cmd_count + 1

# Stop channel 1 application
cmd(f"<%= target_name %> BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

# Check that application state has changed to STOPPED
cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STOPPED'", 10)

##################################################################################
# Remove channel 1 application
##################################################################################

current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
expected_valid_cmd_count = current_valid_cmd_count + 1

# Remove channel 1 application
cmd(f"<%= target_name %> BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)

# Check that application state has changed to REMOVED
cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 10)
