#
# This test demonstrates using the DTN Test Tools to
# send a payload to the BPNode (using the UDP CLA)
# This payload includes a cfe ES command no-op
# This test checks that the ES command no-op is delivered to ES
#
# Assumptions:
# - FSW is running and telemetry is enabled
# - FSW is set up to route a payload of 8 bytes to BPNode channel 0
#   - FSW is set up to check for this size,
#     and jam the Dest EID to "ipn:100.42" which will route to BPNode channel 0
# - BPNode channel 0 egress has ADU Wrapping disabled

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
DEST_SERVICE_NUM = 42

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
# Prepare to transmit
##################################################################################

cmd(f"<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
wait_check(f"<%= target_name %> BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_0 == 'STARTED'", 10)

data_sender = UdpTxSocket(BPNODE_IP, BPNODE_PORT)

try:
    curr_cfe_es_cmd_cnt = tlm(f"<%= target_name %> CFE_ES_HK COMMAND_COUNTER")
    exp_cfe_es_cmd_cnt = curr_cfe_es_cmd_cnt + 1

    curr_bundle_rcv_cnt = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
    exp_bundle_rcv_cnt = curr_bundle_rcv_cnt + 1

    curr_adu_delvr_cnt = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED")
    exp_adu_delvr_cnt = curr_adu_delvr_cnt + 1

    # Connect the data sender and receiver tools
    data_sender.connect()

    encoded_bundle = create_test_bundle(ccsds_cmd_inject_payload)
    data_sender.write(encoded_bundle)
    print(f"Sending bundle of {len(encoded_bundle)} bytes")

    # Wait for ADU packet to reach its destination and increment right counters
    wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {exp_bundle_rcv_cnt}", 10)
    wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED == {exp_adu_delvr_cnt}", 10)
    wait_check(f"<%= target_name %> CFE_ES_HK COMMAND_COUNTER == {exp_cfe_es_cmd_cnt}", 10)

except KeyboardInterrupt:
    pass

except Exception:
    print(traceback.format_exc())

finally:
    data_sender.disconnect()
