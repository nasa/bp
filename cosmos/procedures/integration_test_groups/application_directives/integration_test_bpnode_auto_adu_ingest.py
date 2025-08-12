from openc3.script.suite import Group

from dtntools.dtncla.udp import UdpRxSocket
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

LOCAL_NODE_EID = EID(
    {
        "uri": 2,
        "ssp": {"node_num": <%= $dtnfsw_globals_channel_dest_eid_node %>,
                "service_num": <%= $dtnfsw_globals_local_service_num %>}
    }
)

import traceback

# Group class name should indicate what the scripts are testing
class integration_test_bpnode_auto_adu_ingest(Group):
    """
    Test cases for checking ADU ingest for an application loaded automatically
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_auto_adu_ingest(self):
        """
        FSW ADU Ingest
        - Send an SB statistics HK request (which the ADU In task should be subscribed
        to) and verify that the ADU Received Count increments
        """

        # Port / Address Configs
        LOCALHOST_RX = "0.0.0.0"
        PORT_NUM_RX = <%= dtnfsw_get_cla_out_port(target_name, 0) %>

        # Make sure port numbers were set properly
        check_expression(f"{PORT_NUM_RX} != 0")

        # Connect to CLA #0 Out sockets
        data_receiver = UdpRxSocket(LOCALHOST_RX, PORT_NUM_RX)

        # Store the previous counter val, used to calculated expected/next value
        current_valid_adu_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED")
        expected_valid_adu_count = current_valid_adu_count + 1
        current_bundle_delvr_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
        expected_bundle_delvr_count = current_bundle_delvr_count + 1

        try:
            # Connect the data receiver tool
            data_receiver.connect()

            # Send the command
            cmd(f"<%= target_name %> CFE_SB_CMD_SEND_SB_STATS")

            # Wait for the expected results
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED == {expected_valid_adu_count}", 10)
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED == {expected_bundle_delvr_count}", 10)

            # Wait for data to be sent back by CLA Out task
            Group.print("Waiting for bundle to be returned...")
            received_data = data_receiver.read()
            Group.print(f"Received bundle of {len(received_data)} bytes")

            # Check that the right data was returned by the CLA Out task
            check_expression(f"{received_data} != None")
            print(f"Received bundle of size: {len(received_data)}.")
            received_bundle = Bundle.from_bytes(received_data)
            print(f"Received bundle: {received_bundle.to_json()}")
            check_expression(f"{received_bundle.pri_block.version} == 7")
            check_expression(f"{received_bundle.pri_block.control_flags} == 4")
            check_expression(f"{received_bundle.pri_block.crc_type} == {CRCType.CRC16_X25}")
            check_expression(f"{received_bundle.pri_block.is_crc_valid()}")

            # these EIDs are set via channel / bplib configurations
            expected_dest_eid = EID({"uri": 2, "ssp": {"node_num": 200, "service_num": 64}})
            expected_src_eid = EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 42}})
            expected_rpt_eid = EID({"uri": 1, "ssp": 0}) # dtn:none
            bpnode_check_eid_equality(received_bundle.pri_block.dest_eid, expected_dest_eid)
            bpnode_check_eid_equality(received_bundle.pri_block.src_eid, expected_src_eid)
            # TODO add me back once EID test tool logic is fixed
            #bpnode_check_eid_equality(received_bundle.pri_block.rpt_eid, expected_rpt_eid)

            exp_new_hop_count = 1
            exp_new_hop_limit = 10

            # Note: See the default channel config table for the source of these configs

            # Check the canonical blocks
            check_expression(f"{len(received_bundle.canon_blocks)} == 4")

            # Check the prev node block
            check_expression(f"{received_bundle.canon_blocks[0].blk_type} == {BlockType.PREVIOUS_NODE}")
            check_expression(f"{received_bundle.canon_blocks[0].blk_num} == {2}")
            check_expression(f"{received_bundle.canon_blocks[0].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[0].crc_type} == {CRCType.CRC16_X25}")
            check_expression(f"{received_bundle.canon_blocks[0].is_crc_valid()}")
            bpnode_check_eid_equality(received_bundle.canon_blocks[0].prev_eid, LOCAL_NODE_EID)

            # Check the age block
            check_expression(f"{received_bundle.canon_blocks[1].blk_type} == {BlockType.BUNDLE_AGE}")
            check_expression(f"{received_bundle.canon_blocks[1].blk_num} == {3}")
            check_expression(f"{received_bundle.canon_blocks[1].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[1].crc_type} == {CRCType.CRC16_X25}")
            check_expression(f"{received_bundle.canon_blocks[1].is_crc_valid()}")

            # Check the hop count block
            check_expression(f"{received_bundle.canon_blocks[2].blk_type} == {BlockType.HOP_COUNT}")
            check_expression(f"{received_bundle.canon_blocks[2].blk_num} == {4}")
            check_expression(f"{received_bundle.canon_blocks[2].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[2].crc_type} == {CRCType.CRC16_X25}")
            check_expression(f"{received_bundle.canon_blocks[2].is_crc_valid()}")
            check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_limit} == {exp_new_hop_limit}")
            check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_count} == {exp_new_hop_count}")

            # Check the payload block
            check_expression(f"{received_bundle.canon_blocks[3].blk_type} == {BlockType.BUNDLE_PAYLOAD}")
            check_expression(f"{received_bundle.canon_blocks[3].blk_num} == {1}")
            check_expression(f"{received_bundle.canon_blocks[3].control_flags} == {0}")
            check_expression(f"{received_bundle.canon_blocks[3].crc_type} == {CRCType.CRC16_X25}")
            check_expression(f"{received_bundle.canon_blocks[3].is_crc_valid()}")


        except KeyboardInterrupt:
            pass
        except Exception:
            print(traceback.format_exc())

        finally:
            # Clean up connections
            data_receiver.disconnect()



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
        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_RECEIVED == 0", 10)
