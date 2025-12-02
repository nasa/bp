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
    HopCountData,
    CreationTimestamp
)


# Networking Configuration: Please ensure you have port forwarding rules
BPNODE_IP = "172.17.0.1"
BPNODE_PORT = <%= dtnfsw_get_cla_in_port(target_name, 0) %>
LOCAL_IP = "0.0.0.0"
LOCAL_PORT = <%= dtnfsw_get_cla_out_port(target_name, 0) %>

# EID Configuration
DEST_NODE_NUM = <%= $dtnfsw_globals_contact_0_dest_eid_node %>
DEST_SERVICE_NUM = <%= $dtnfsw_globals_contact_0_dest_eid_service %>
SRC_NODE_NUM = 300
SRC_SERVICE_NUM = 1

LOCAL_NODE_EID = EID(
    {
        "uri": 2,
        "ssp": {"node_num": <%= $dtnfsw_globals_channel_dest_eid_node %>,
                "service_num": <%= $dtnfsw_globals_local_service_num %>}
    }
)

class integration_test_bpnode_contact_ingress_06(Group):
    """
    Test Group
    """

    def test_bpnode_bundle_ingress_06(self):
        """
        Nominal test
        """

        data_receiver = UdpRxSocket(LOCAL_IP, LOCAL_PORT)
        data_sender = UdpTxSocket(BPNODE_IP, BPNODE_PORT)

        # Prompt the user for a payload length, to add a little spice to the test
        # payload_length = ask("Enter a payload length, in bytes (min: 0, max: 3018, exception: 8):")
        # payload_length = int(payload_length)
        payload_length = 40
        payload_data = bytes([0xAA]*payload_length)

        try:
            # Connect the data sender and receiver tools
            data_receiver.connect()
            data_sender.connect()

            encoded_input_bundle = bpnode_create_test_bundle_util(dest_node_num=DEST_NODE_NUM,
                                                            dest_service_num=DEST_SERVICE_NUM,
                                                            src_node_num=SRC_NODE_NUM,
                                                            src_service_num=SRC_SERVICE_NUM,
                                                            crc_type=CRCType.CRC32_C,
                                                            prev_node_eid=EID({"uri": 2, "ssp": {"node_num": 300, "service_num": 2}}),
                                                            age=108000,
                                                            hop_count=HopCountData({"hop_limit": 15, "hop_count": 3}),
                                                            payload_bytes=payload_data,
                                                            unknown=True)
            deserialized_input_bundle = Bundle.from_bytes(encoded_input_bundle)

            exp_new_hop_count = deserialized_input_bundle.canon_blocks[2].hop_data.hop_count + 1

            print(f"Sending Bundle of {len(encoded_input_bundle)} bytes: {Bundle.from_bytes(encoded_input_bundle).to_json()}")
            data_sender.write(encoded_input_bundle)
            print("Waiting for bundle to be returned...")
            looped_back_bundle = data_receiver.read()
            print(f"Received Bundle of {len(looped_back_bundle)} bytes: {Bundle.from_bytes(looped_back_bundle).to_json()}")

            # Use DTNGen to decode/translate our raw bytes to the Bundle class
            received_bundle = Bundle.from_bytes(looped_back_bundle)

            # Check primary block fields
            check_expression(f"{received_bundle.pri_block.version} == 7")
            check_expression(f"{received_bundle.pri_block.control_flags} == 4")
            check_expression(f"{received_bundle.pri_block.crc_type} == {CRCType.CRC32_C}")
            check_expression(f"{received_bundle.pri_block.crc} == {deserialized_input_bundle.pri_block.crc}")

            # the received EIDs should match what we sent in the original bundle.
            bpnode_check_eid_equality(received_bundle.pri_block.dest_eid, deserialized_input_bundle.pri_block.dest_eid)
            bpnode_check_eid_equality(received_bundle.pri_block.src_eid, deserialized_input_bundle.pri_block.src_eid)
            bpnode_check_eid_equality(received_bundle.pri_block.rpt_eid, deserialized_input_bundle.pri_block.rpt_eid)

            # Check the canonical blocks
            check_expression(f"{len(received_bundle.canon_blocks)} == 5")

            # Check the prev node block
            check_expression(f"{received_bundle.canon_blocks[0].blk_type} == {BlockType.PREVIOUS_NODE}")
            check_expression(f"{received_bundle.canon_blocks[0].blk_num} == {6}")
            check_expression(f"{received_bundle.canon_blocks[0].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[0].crc_type} == {CRCType.CRC32_C}")
            check_expression(f"{received_bundle.canon_blocks[0].is_crc_valid()}")
            bpnode_check_eid_equality(received_bundle.canon_blocks[0].prev_eid, LOCAL_NODE_EID)

            # Check the age block
            check_expression(f"{received_bundle.canon_blocks[1].blk_type} == {BlockType.BUNDLE_AGE}")
            check_expression(f"{received_bundle.canon_blocks[1].blk_num} == {2}")
            check_expression(f"{received_bundle.canon_blocks[1].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[1].crc_type} == {CRCType.CRC32_C}")
            check_expression(f"{received_bundle.canon_blocks[1].is_crc_valid()}")
            check_expression(f"{received_bundle.canon_blocks[1].bundle_age} >= {deserialized_input_bundle.canon_blocks[1].bundle_age}")

            # Check the hop count block
            check_expression(f"{received_bundle.canon_blocks[2].blk_type} == {BlockType.HOP_COUNT}")
            check_expression(f"{received_bundle.canon_blocks[2].blk_num} == {3}")
            check_expression(f"{received_bundle.canon_blocks[2].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[2].crc_type} == {CRCType.CRC32_C}")
            check_expression(f"{received_bundle.canon_blocks[2].is_crc_valid()}")
            check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_limit} == {deserialized_input_bundle.canon_blocks[2].hop_data.hop_limit}")
            check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_count} == {exp_new_hop_count}")

            # Check the unknown block
            check_expression(f"{received_bundle.canon_blocks[3].elements[0]} == {193}")
            check_expression(f"{received_bundle.canon_blocks[3].elements[1]} == {10}")
            check_expression(f"{received_bundle.canon_blocks[3].elements[2]} == {0}")
            check_expression(f"{received_bundle.canon_blocks[3].elements[3]} == {CRCType.NONE}")
            check_expression(f"{len(received_bundle.canon_blocks[3].elements)} == {5}")

            # Check the payload block
            check_expression(f"{received_bundle.canon_blocks[4].blk_type} == {BlockType.BUNDLE_PAYLOAD}")
            check_expression(f"{received_bundle.canon_blocks[4].blk_num} == {1}")
            check_expression(f"{received_bundle.canon_blocks[4].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[4].crc_type} == {CRCType.CRC32_C}")
            check_expression(f"{received_bundle.canon_blocks[4].crc} == {deserialized_input_bundle.canon_blocks[4].crc}")

        except KeyboardInterrupt:
            pass

        except Exception:
            print(traceback.format_exc())

        finally:
            data_receiver.disconnect()
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
