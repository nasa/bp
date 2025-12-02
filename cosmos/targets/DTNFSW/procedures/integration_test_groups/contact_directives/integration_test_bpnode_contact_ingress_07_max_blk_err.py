from openc3.script.suite import Group

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



# Primary Block: 
#          CRC Type: 1
#          Flags: 4
#          Dest EID (scheme.node.service): 2.200.1
#          Source EID (scheme.node.service): 2.100.1
#          Report-To EID (scheme.node.service): 2.100.1
#          Timestamp (created, seq): 755533838904, 0
#          Lifetime: 3600000
#          CRC Value: 0xB19
# Canonical Block [0]: 
#          Block Type: 6
#          Block Number: 6
#          Flags: 0
#          CRC Type: 1
#          Offset Into Encoded Bundle: 42
# Canonical Block [1]: 
#          Block Type: 7
#          Block Number: 2
#          Flags: 5
#          CRC Type: 1
#          Offset Into Encoded Bundle: 58
# Canonical Block [2]: 
#          Block Type: 10
#          Block Number: 3
#          Flags: 1
#          CRC Type: 1
#          Offset Into Encoded Bundle: 72
# Canonical Block [3]: 
#          Block Type: 15
#          Block Number: 4
#          Flags: 2
#          CRC Type: 1
#          Offset Into Encoded Bundle: 84
# Canonical Block [4]: 
#          Block Type: 16
#          Block Number: 5
#          Flags: 0
#          CRC Type: 1
#          Offset Into Encoded Bundle: 103

bundle_with_too_many_blocks = bytes([
    0x9f, 0x89, 0x07, 0x04, 0x01, 0x82, 0x02, 0x82, 0x18, 0xc8, 0x01, 0x82,
    0x02, 0x82, 0x18, 0x64, 0x01, 0x82, 0x02, 0x82, 0x18, 0x64, 0x01, 0x82,
    0x1b, 0x00, 0x00, 0x00, 0xaf, 0xe9, 0x53, 0x7a, 0x38, 0x00, 0x1a, 0x00,
    0x36, 0xee, 0x80, 0x42, 0x0b, 0x19, 0x86, 0x06, 0x06, 0x00, 0x01, 0x47,
    0x82, 0x02, 0x82, 0x19, 0x01, 0x2c, 0x02, 0x42, 0x25, 0xd4, 0x86, 0x07,
    0x02, 0x05, 0x01, 0x45, 0x1a, 0x00, 0x01, 0xa5, 0xe0, 0x42, 0x3a, 0xed,
    0x86, 0x0a, 0x03, 0x01, 0x01, 0x43, 0x82, 0x0f, 0x03, 0x42, 0xf8, 0x13,
    0x86, 0x0f, 0x04, 0x02, 0x01, 0x4a, 0x83, 0x0a, 0x02, 0x82, 0x02, 0x82,
    0x19, 0x01, 0x2f, 0x01, 0x42, 0x25, 0xc7, 0x86, 0x10, 0x05, 0x00, 0x01,
    0x52, 0x85, 0x01, 0x04, 0x00, 0x82, 0x02, 0x82, 0x19, 0x01, 0x2f, 0x01,
    0x82, 0x02, 0x82, 0x19, 0x01, 0x31, 0x02, 0x42, 0x66, 0xce, 0x86, 0x01,
    0x01, 0x00, 0x01, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
    0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x0a,
    0x42, 0x7a, 0x2f, 0xff
])

class integration_test_bpnode_contact_ingress_07(Group):
    """
    Test Group
    """

    def test_bpnode_bundle_ingress_07(self):
        """
        Nominal test
        """

        # Store the previous counter val, used to calculated expected/next value
        current_bundle_count_deleted = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED")
        expected_bundle_count_deleted = current_bundle_count_deleted + 1
        current_bundle_count_deleted_unintel = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
        expected_bundle_count_deleted_unintel = current_bundle_count_deleted_unintel + 1

        # data_receiver = UdpRxSocket(LOCAL_IP, LOCAL_PORT)
        data_sender = UdpTxSocket(BPNODE_IP, BPNODE_PORT)

        try:
            # Connect the data sender and receiver tools
            data_sender.connect()

            encoded_bundle = bundle_with_too_many_blocks
            data_sender.write(encoded_bundle)
            print(f"Sending bundle of {len(encoded_bundle)} bytes")
            print(f"Sent Bundle: {Bundle.from_bytes(encoded_bundle).to_json()}")

            # Check that the BPNode telemetry incremented as expected
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED == {expected_bundle_count_deleted}", 10)
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_DELETED_UNINTELLIGIBLE == {expected_bundle_count_deleted_unintel}", 10)

        except KeyboardInterrupt:
            pass

        except Exception:
            print(traceback.format_exc())

        finally:
            data_sender.disconnect()



    def setup(self):
        """
        Test Group Setup
        - Runs when Group Setup button is pressed
        - Runs before all scripts when Group Start is pressed
        """
        pass

    def teardown(self):
        """
        Test Group Setup
        - Runs when Group Teardown button is pressed
        - Runs after all scripts when Group Start is pressed
        """
        pass
