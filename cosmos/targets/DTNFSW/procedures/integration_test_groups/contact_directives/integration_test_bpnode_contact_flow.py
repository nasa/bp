from openc3.script.suite import Group
from dtntools.dtncla.udp import UdpTxSocket, UdpRxSocket
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
import traceback

<% require 'dtnfsw_get_port_nums.rb' %>
<% require 'dtnfsw_globals.rb' %>


class integration_test_bpnode_contact_flow(Group):
    """
    Test cases for the BPNode "contact_flow" command
    - Methods beginning with script_ or test_ are added to Script dropdown
    """

    def test_bpnode_contact_loopback(self):
        """
        Nominal test
        """

        # Port / Address Configs
        DOCKER_LOCALHOST_TX = "172.17.0.1"
        PORT_NUM_TX = <%= dtnfsw_get_cla_in_port(target_name, 0) %>
        LOCALHOST_RX = "0.0.0.0"
        PORT_NUM_RX = <%= dtnfsw_get_cla_out_port(target_name, 0) %>

        # EID Configuration
        DEST_NODE_NUM = <%= $dtnfsw_globals_contact_0_dest_eid_node %>
        DEST_SERVICE_NUM = <%= $dtnfsw_globals_contact_0_dest_eid_service %>
        SRC_NODE_NUM = 300
        SRC_SERVICE_NUM = 1

        # Make sure port numbers were set properly
        check_expression(f"{PORT_NUM_RX} != 0")
        check_expression(f"{PORT_NUM_TX} != 0")

        # Store the previous counter val, used to calculated expected/next value
        current_bundle_recv_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
        expected_bundle_recv_count = current_bundle_recv_count + 1
        current_bundle_delvr_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED")
        expected_bundle_delvr_count = current_bundle_delvr_count + 1

        # Create out test bundle
        primary_block = PrimaryBlock(
            version=7,
            control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
            crc_type=CRCType.CRC16_X25,
            dest_eid=EID({"uri": 2, "ssp": {"node_num": DEST_NODE_NUM, "service_num": DEST_SERVICE_NUM}}),
            src_eid=EID({"uri": 2, "ssp": {"node_num": SRC_NODE_NUM, "service_num": SRC_SERVICE_NUM}}),
            rpt_eid=EID({"uri": 1, "ssp": 0}), # This is equivalent to dtn:none
            creation_timestamp=CreationTimestamp({"time": 755533838904, "sequence": 0}),
            lifetime=3600000,
            crc=CRCFlag.CALCULATE,
        )

        payload_block = PayloadBlock(
            blk_type=BlockType.AUTO,
            blk_num=1,
            control_flags=0,
            crc_type=CRCType.CRC16_X25,
            payload=b"\x00\x00\x00\x00\x00\x00\x00\x0chello world\n",
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
        # tx_bundle = bytes([0x41] * int(1024))
        encoded_bundle = bundle.to_bytes()

        # Connect to CLA #0 In/Out sockets
        data_sender = UdpTxSocket(DOCKER_LOCALHOST_TX, PORT_NUM_TX)
        data_receiver = UdpRxSocket(LOCALHOST_RX, PORT_NUM_RX)

        try:
            # Connect the data sender and receiver tools
            data_receiver.connect()
            data_sender.connect()

            # Write data to the CLA In task
            data_sender.write(encoded_bundle)
            Group.print(f"Sending bundle of {len(encoded_bundle)} bytes")

            # Wait for data to be sent back by CLA Out task
            # rx_bundle = rx_sock.read(timeout=6)
            Group.print("Waiting for bundle to be returned...")
            looped_back_bundle = data_receiver.read()
            Group.print(f"Received bundle of {len(looped_back_bundle)} bytes")

            # Check that the right data was returned by the CLA Out task
            check_expression(f"{looped_back_bundle} != None")

            print(f"Sent Bundle: {Bundle.from_bytes(encoded_bundle).to_json()}")
            print(f"Received Bundle: {Bundle.from_bytes(looped_back_bundle).to_json()}")

            check_expression(f"'{len(looped_back_bundle)}' == '{len(encoded_bundle)}'")
            check_expression(f"'{looped_back_bundle == encoded_bundle}' == 'True'")
            check_expression(f"'{Bundle.from_bytes(looped_back_bundle).to_json() == Bundle.from_bytes(encoded_bundle).to_json()}' == 'True'")

            # Check that the BPNode telemetry incremented as expected
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {expected_bundle_recv_count}", 10)
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_FORWARDED == {expected_bundle_delvr_count}", 10)

        except KeyboardInterrupt:
            pass
        except Exception:
            print(traceback.format_exc())

        finally:
            # Clean up connections
            data_receiver.disconnect()
            data_sender.disconnect()



    def test_bpnode_contact_to_adu_delivery_wrapped(self):
        """
        This test demonstrates using the DTN Test Tools to
        send a payload to the BPNode (using the UDP CLA)
        This payload includes a cfe ES command no-op
        This test checks that the payload is wrapped in a DTN_ADU_OUT packet and sent to COSMOS

        Assumptions:
        - FSW is running and telemetry is enabled
        - FSW is set up to route a payload of 8 bytes to BPNode channel 0
          - FSW is set up to check for this size,
            and jam the Dest EID to "ipn:100.53" which will route to BPNode channel 1
          - see FSW defs:
            - BPLIB_TEMPORARY_EID_NODE_NUM_FOR_CHANNEL_ROUTES
            - BPLIB_TEMPORARY_EID_SERVICE_NUM_FOR_CHANNEL_0_ROUTES
            - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_1
            - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_2
        - BPNode channel 1 egress has ADU Wrapping enabled
        """

        # add and start channel 1
        # channel 1 should already be configured for ADU wrapping during delivery
        dtnfsw_bpnode_add_channel(f"<%= target_name %>", 1)
        dtnfsw_bpnode_start_channel(f"<%= target_name %>", 1)


        # Port / Address Configs
        DOCKER_LOCALHOST_TX = "172.17.0.1"
        PORT_NUM_TX = <%= dtnfsw_get_cla_in_port(target_name, 0) %>

        # Dest EID Configuration
        DEST_NODE_NUM = 100
        DEST_SERVICE_NUM = 53

        # Source EID Configuration (not really used, at the moment)
        SRC_NODE_NUM = 99
        SRC_SERVICE_NUM = 1

        # Make sure port numbers were set properly
        check_expression(f"{PORT_NUM_TX} != 0")

        # Store the previous counter val, used to calculated expected/next value
        current_bundle_recv_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
        expected_bundle_recv_count = current_bundle_recv_count + 1
        current_adu_delvr_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED")
        expected_adu_delvr_count = current_adu_delvr_count + 1
        current_adu_pkt_count = tlm(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT")
        if current_adu_pkt_count is None:
            current_adu_pkt_count = 0
        expected_adu_pkt_count = current_adu_pkt_count + 1

        # Create out test bundle
        primary_block = PrimaryBlock(
            version=7,
            control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
            crc_type=CRCType.CRC16_X25,
            dest_eid=EID({"uri": 2, "ssp": {"node_num": DEST_NODE_NUM, "service_num": DEST_SERVICE_NUM}}),
            src_eid=EID({"uri": 2, "ssp": {"node_num": SRC_NODE_NUM, "service_num": SRC_SERVICE_NUM}}),
            rpt_eid=EID({"uri": 1, "ssp": 0}), # This is equivalent to dtn:none
            creation_timestamp=CreationTimestamp({"time": 755533838904, "sequence": 0}),
            lifetime=3600000,
            crc=CRCFlag.CALCULATE,
        )

        # This payload is a Raw ES No-Op Command
        # FSW Ref: CFE_ES_CMD_MID
        ccsds_cmd_inject_payload=bytes([0x18, 0x06, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x00])

        payload_block = PayloadBlock(
            blk_type=BlockType.AUTO,
            blk_num=1,
            control_flags=0,
            crc_type=CRCType.CRC16_X25,
            payload=ccsds_cmd_inject_payload,
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
        encoded_bundle = bundle.to_bytes()

        # Connect to CLA #0 In/Out sockets
        data_sender = UdpTxSocket(DOCKER_LOCALHOST_TX, PORT_NUM_TX)

        try:
            # Connect the data sender and receiver tools
            data_sender.connect()

            # Write data to the CLA In task
            Group.print(f"Sending bundle of {len(encoded_bundle)} bytes")
            data_sender.write(encoded_bundle)

            # Check that the DTN_ADU_OUT packet was generated
            wait_check(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT == {expected_adu_pkt_count}", 10)

            # Check that the BPNode telemetry incremented as expected
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {expected_bundle_recv_count}", 10)
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED == {expected_adu_delvr_count}", 10)

        except KeyboardInterrupt:
            pass
        except Exception:
            print(traceback.format_exc())

        finally:
            # Clean up connections
            data_sender.disconnect()

        # stop and remove channel 1
        dtnfsw_bpnode_stop_channel(f"<%= target_name %>", 1)
        dtnfsw_bpnode_remove_channel(f"<%= target_name %>", 1)



    def test_bpnode_contact_to_adu_delivery_es_cmd(self):
        """
        This test demonstrates using the DTN Test Tools to
        send a payload to the BPNode (using the UDP CLA)
        This payload includes a cfe ES command no-op

        Assumptions:
        - FSW is running and telemetry is enabled
        - FSW is set up to route a payload of 8 bytes to BPNode channel 0
          - FSW is set up to check for this size,
            and jam the Dest EID to "ipn:100.42" which will route to BPNode channel 0
          - see FSW defs:
            - BPLIB_TEMPORARY_EID_NODE_NUM_FOR_CHANNEL_ROUTES
            - BPLIB_TEMPORARY_EID_SERVICE_NUM_FOR_CHANNEL_0_ROUTES
            - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_1
            - BPLIB_TEMPORARY_BUNDLE_SIZE_FOR_CHAN_DELIVERY_HACK_2
        - BPNode channel 1 egress has ADU Wrapping enabled
        """

        # Port / Address Configs
        DOCKER_LOCALHOST_TX = "172.17.0.1"
        PORT_NUM_TX = <%= dtnfsw_get_cla_in_port(target_name, 0) %>

        # Dest EID Configuration
        DEST_NODE_NUM = 100
        DEST_SERVICE_NUM = 42

        # Source EID Configuration (not really used, at the moment)
        SRC_NODE_NUM = 99
        SRC_SERVICE_NUM = 1

        # Make sure port numbers were set properly
        check_expression(f"{PORT_NUM_TX} != 0")

        # Store the previous counter val, used to calculated expected/next value
        current_bundle_recv_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED")
        expected_bundle_recv_count = current_bundle_recv_count + 1
        current_adu_delvr_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED")
        expected_adu_delvr_count = current_adu_delvr_count + 1

        # Grab current ES command count
        # Verify that we have a recent packet (by waiting for a new one to arrive)
        # Assuming no one else is sending commands, grab the latest command count
        es_hk_packet_name = "CFE_ES_HK"
        es_hk_cmd_count_name = "COMMAND_COUNTER"
        wait_check_packet(f"<%= target_name %>", f"{es_hk_packet_name}", 1, 6)
        cmd_count = tlm(f"<%= target_name %> {es_hk_packet_name} {es_hk_cmd_count_name}")

        current_adu_pkt_count = tlm(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT")
        if current_adu_pkt_count is None:
            current_adu_pkt_count = 0
        expected_adu_pkt_count = current_adu_pkt_count + 1

        # Create out test bundle
        primary_block = PrimaryBlock(
            version=7,
            control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
            crc_type=CRCType.CRC16_X25,
            dest_eid=EID({"uri": 2, "ssp": {"node_num": DEST_NODE_NUM, "service_num": DEST_SERVICE_NUM}}),
            src_eid=EID({"uri": 2, "ssp": {"node_num": SRC_NODE_NUM, "service_num": SRC_SERVICE_NUM}}),
            rpt_eid=EID({"uri": 1, "ssp": 0}), # This is equivalent to dtn:none
            creation_timestamp=CreationTimestamp({"time": 755533838904, "sequence": 0}),
            lifetime=3600000,
            crc=CRCFlag.CALCULATE,
        )

        # This payload is a Raw ES No-Op Command
        # FSW Ref: CFE_ES_CMD_MID
        ccsds_cmd_inject_payload=bytes([0x18, 0x06, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x00])

        payload_block = PayloadBlock(
            blk_type=BlockType.AUTO,
            blk_num=1,
            control_flags=0,
            crc_type=CRCType.CRC16_X25,
            payload=ccsds_cmd_inject_payload,
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
        encoded_bundle = bundle.to_bytes()

        # Connect to CLA #0 In/Out sockets
        data_sender = UdpTxSocket(DOCKER_LOCALHOST_TX, PORT_NUM_TX)

        try:
            # Connect the data sender and receiver tools
            data_sender.connect()

            # Write data to the CLA In task
            Group.print(f"Sending bundle of {len(encoded_bundle)} bytes")
            data_sender.write(encoded_bundle)

            # Check that the DTN_ADU_OUT packet was generated
            # wait_check(f"<%= target_name %> DTN_ADU_OUT RECEIVED_COUNT == {expected_adu_pkt_count}", 10)

            # Check accepted NOOP command proving application is up and running
            wait_check(f"<%= target_name %> {es_hk_packet_name} {es_hk_cmd_count_name} == {cmd_count+1}", 10)

            # Check that the BPNode telemetry incremented as expected
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == {expected_bundle_recv_count}", 10)
            wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK ADU_COUNT_DELIVERED == {expected_adu_delvr_count}", 10)

        except KeyboardInterrupt:
            pass
        except Exception:
            print(traceback.format_exc())

        finally:
            # Clean up connections
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
        # Reset counters
        cmd(f"<%= target_name %> BPNODE_CMD_RESET_ALL_COUNTERS")

        # Wait for the expected results
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_COUNT_RECEIVED == 0", 10)
