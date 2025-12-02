#
# This test demonstrates using the DTN Test Tools to
# - send a bundle to the BPNode (using the UDP CLA)
# - receive a bundle from the BPNode (using the UDP CLA)
#
# Assumptions:
# - FSW is running
# - FSW is set up to send whatever it received (loopback config)
# 

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

# EID Configuration
DEST_NODE_NUM = 200
DEST_SERVICE_NUM = 1
SRC_NODE_NUM = 100
SRC_SERVICE_NUM = 1

# Hopefully you don't have to change anything below here
def create_test_bundle(payload_len):
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
        payload=bytes([0xAA]*payload_len),
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


data_receiver = UdpRxSocket(LOCAL_IP, LOCAL_PORT)
data_sender = UdpTxSocket(BPNODE_IP, BPNODE_PORT)

# Prompt the user for a payload length, to add a little spice to the test
payload_length = ask("Enter a payload length, in bytes (min: 0, max: 3018, exception: 8):")
payload_length = int(payload_length)

try:
    # Connect the data sender and receiver tools
    data_receiver.connect()
    data_sender.connect()

    encoded_bundle = create_test_bundle(payload_length)
    data_sender.write(encoded_bundle)
    print(f"Sending bundle of {len(encoded_bundle)} bytes")

    print("Waiting for bundle to be returned...")
    looped_back_bundle = data_receiver.read()
    print(f"Received bundle of {len(looped_back_bundle)} bytes")


    print(f"Sent Bundle: {Bundle.from_bytes(encoded_bundle).to_json()}")
    print(f"Received Bundle: {Bundle.from_bytes(looped_back_bundle).to_json()}")

    check_expression(f"'{len(looped_back_bundle)}' == '{len(encoded_bundle)}'")
    check_expression(f"'{looped_back_bundle == encoded_bundle}' == 'True'")
    check_expression(f"'{Bundle.from_bytes(looped_back_bundle).to_json() == Bundle.from_bytes(encoded_bundle).to_json()}' == 'True'")


except KeyboardInterrupt:
    pass

except Exception:
    print(traceback.format_exc())

finally:
    data_receiver.disconnect()
    data_sender.disconnect()
