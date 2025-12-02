# Send bundles containing unknown blocks to DTN Node
#
# Example usage: unknown_blocks_send(104, 2, 600, "10.2.7.206", 4556, 45)
#

def unknown_blocks_send(dest_number= None, dest_service= None, lifetime_in_sec = None, send_to_ip = None, send_to_port = None):

    import codecs
    import os
    import time
    import traceback

    from dtntools.dtngen.blocks import (
        BundleAgeBlock,
        CanonicalBlock,
        CompressedReportingBlock,
        CustodyTransferBlock,
        HopCountBlock,
        PayloadBlock,
        PayloadBlockSettings,
        PrevNodeBlock,
        PrimaryBlock,
        PrimaryBlockSettings,
        UnknownBlock,
    )
    from dtntools.dtngen.bundle import Bundle
    from dtntools.dtngen.types import (
        EID,
        BlockPCFlags,
        BlockType,
        BundlePCFlags,
        CRCFlag,
        CRCType,
        CreationTimestamp,
        CREBData,
        CTEBData,
        HopCountData,
        StatusRRFlags,
        TypeWarning,
    )
    from dtntools.dtngen.utils import DtnTimeNowMs
    from dtntools.dtncla.udp import UdpRxSocket, UdpTxSocket

    if dest_number is None:
        dest_number = 103
    
    if dest_service is None:
        dest_service = 1
    
    if lifetime_in_sec is None:
        lifetime_in_sec = 3600
    
    if send_to_ip is None:
        send_to_ip = '127.0.0.1'

    if send_to_port is None:
        send_to_port = 4556


# SECTION 1: Creating test bundles

    print("Define new primary, hop count, unknown, and payload blocks")
    primary_block = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_number, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=(lifetime_in_sec * 1000),
        crc=CRCFlag.CALCULATE,
    )

    prev_node_block = PrevNodeBlock(
        blk_type=BlockType.AUTO,
        blk_num=2,
        control_flags=0,
        crc_type=CRCType.NONE,
        prev_eid=EID({"uri": 2, "ssp": {"node_num": 200, "service_num": 1}}),
    )

    unknown_block_passthrough = UnknownBlock(
        elements=
        [
            73,
            3,
            0,
            1,
            b'\x82\x02\x82\x18\x64\x0a',
            b'\x11\x9f'
        ]
    )

    unknown_block_delete_bundle = UnknownBlock(
        elements=
        [
            73,
            3,
            4,
            1,
            b'\x82\x02\x82\x18\x64\x0a',
            b'\x14\xe9'
        ]
    )

    unknown_block_remove = UnknownBlock(
        elements=
        [
            73,
            3,
            16,
            1,
            b'\x82\x02\x82\x18\x64\x0a',
            b'\x04\x47'
        ]
    )

    payload_block = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b"\x00\x00\x00\x00\x00\x00\x00\x0chello world\n",
        crc=CRCFlag.CALCULATE,
    )

    print("Creating unknown block bundle for passthrough")
    bundle_1 = Bundle(
        pri_block=primary_block,
        canon_blocks=[prev_node_block, unknown_block_passthrough, payload_block],
    )

    print("Encoding the bundle")
    original_bundle_passthrough = bundle_1.to_bytes()

    print("Creating unknown block bundle for bundle deletion")
    bundle_2 = Bundle(
        pri_block=primary_block,
        canon_blocks=[prev_node_block, unknown_block_delete_bundle, payload_block],
    )

    print("Encoding the bundle")
    original_bundle_delete = bundle_2.to_bytes()

    print("Creating unknown block bundle for block removal")
    bundle_3 = Bundle(
        pri_block=primary_block,
        canon_blocks=[prev_node_block, unknown_block_remove, payload_block],
    )

    print("Encoding the bundle")
    original_bundle_remove_block = bundle_3.to_bytes()


    # SECTION 2: Sending bundles to DTN Node

    print("Configuring the Data Sender")
    data_sender = UdpTxSocket(send_to_ip, send_to_port)

    print("Connecting the Data Sender")
    data_sender.connect()

    print("Sending the bundles to the DTN Node")
    data_sender.write(original_bundle_passthrough)
    data_sender.write(original_bundle_delete)
    data_sender.write(original_bundle_remove_block)

    print(f"Packets sent = {data_sender.get_packets_sent()}")
    
    print("Disconnecting the Data Sender")
    data_sender.disconnect()
