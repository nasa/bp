# Send invalid bundles to DTN Node during subsystem test
#
# Example usage: invalid_bundles_send(103, 1, 600, "10.2.7.206", 4556, 1)
#

def invalid_bundles_send(dest_number= None, dest_service= None, lifetime_in_sec = None, send_to_ip = None, send_to_port = None, rate_limit = None):

    import codecs
    import copy
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
        InvalidCBOR,
        RawData,
        StatusRRFlags,
        TypeWarning,
    )
    from dtntools.dtngen.utils import DtnTimeNowMs
    from dtntools.dtncla.udp import UdpRxSocket, UdpTxSocket
    from dtntools.dtncla.errors.inject import inject_errors

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
        
    if rate_limit is None:
        rate_limit = 1

    # SECTION 1: Creating test bundles

    print("Defining valid bundle blocks")
    primary_block = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_number, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 0}}),
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

    bundle_age_block = BundleAgeBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=BlockPCFlags.FRAG_REPLICATE | BlockPCFlags.DEL_UNPROC,
        crc_type=CRCType.NONE,
        bundle_age=108000,
    )

    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=4,
        control_flags=BlockPCFlags.FRAG_REPLICATE,
        crc_type=CRCType.NONE,
        hop_data=HopCountData({"hop_limit": 15, "hop_count": 3}),
    )

    unknown_block = UnknownBlock(
        elements=
        [
            73,
            5,
            0,
            1,
            b'\x82\x02\x82\x18\x64\x0a',
            b'\x0c\x71'
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

    print("Defining invalid bundle blocks")
    # Primary block with invalid BP version
    primary_block_bad = PrimaryBlock(
        version=7.0,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": 103, "service_num": 1}}),
        src_eid=EID({"uri": 23, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 0}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    # Primary block that will be used to introduce bad CBOR
    primary_block_bad_CBOR = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": 103, "service_num": 1}}),
        src_eid=EID({"uri": 23, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 0}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    # Canonical block with incorrect number of elements
    block_with_extra_element = UnknownBlock(
        elements=
        [
            73,
            5,
            0,
            1,
            2,
            b'\x82\x02\x82\x18\x64\x0a',
            b'\x0c\x71'
        ]
    )

    # Previous Node block with invalid control flags
    prev_node_block_bad = PrevNodeBlock(
        blk_type=BlockType.AUTO,
        blk_num=2,
        control_flags=-20,
        crc_type=CRCType.NONE,
        prev_eid=EID({"uri": 2, "ssp": {"node_num": 200, "service_num": 1}}),
    )

    # Payload block with invalid block number
    payload_block_bad = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=22,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b"\x00\x00\x00\x00\x00\x00\x00\x0chello world\n",
        crc=CRCFlag.CALCULATE,
    )

    print("Creating a nominal bundle containing all defined nominal blocks")
    nominal_bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            hop_count_block,
            unknown_block,
            payload_block,
        ],
    )

    test_bundle_1 = nominal_bundle.to_bytes()

    print("Creating a bundle with random data and writing it to file")
    #test_bundle_2 = Bundle.generate_random(size=1095, filename='bundles/test_bundle_2')
    test_bundle_2 = Bundle.generate_random(size=1095)

    print("Creating a bundle with invalid CBOR")
    # Creating a bad CBOR encoded bundle block by modifying an existing block
    orig_lifetime = primary_block_bad_CBOR.lifetime
    primary_block_bad_CBOR.lifetime = InvalidCBOR(value=orig_lifetime, additional_info=31)

    # Creating a bundle object
    bad_cbor_bundle = Bundle(
        pri_block=copy.deepcopy(primary_block_bad_CBOR),
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            hop_count_block,
            unknown_block,
            payload_block,
        ],
    )

    # Encoding it to bytes, which contains the invalid CBOR encoding
    test_bundle_3 = bad_cbor_bundle.to_bytes()
        
    print("Creating a bundle with less than two CBOR blocks")
    primary_only = Bundle(pri_block=primary_block)
    test_bundle_4 = primary_only.to_bytes()

    print("Creating a bundle with an extra CBOR element")
    block_with_extra_element_bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            hop_count_block,
            block_with_extra_element,
            payload_block,
        ],
    )

    test_bundle_5 = block_with_extra_element_bundle.to_bytes()

    print("Creating a bundle with the payload block in the wrong position")
    out_of_order_bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            payload_block,
            hop_count_block,
            unknown_block,
            payload_block,
        ],
    )
    test_bundle_6 = out_of_order_bundle.to_bytes()

    print("Creating a bundle with invalid primary block")
    nominal_bundle = Bundle(
        pri_block=primary_block_bad,
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            hop_count_block,
            unknown_block,
            payload_block,
        ],
    )

    test_bundle_7 = nominal_bundle.to_bytes()

    print("Creating a bundle with invalid previous node block")
    nominal_bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            prev_node_block_bad,
            bundle_age_block,
            hop_count_block,
            unknown_block,
            payload_block,
        ],
    )

    test_bundle_8 = nominal_bundle.to_bytes()

    print("Creating a bundle with invalid payload block")
    nominal_bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=[
            prev_node_block,
            bundle_age_block,
            hop_count_block,
            unknown_block,
            payload_block_bad,
        ],
    )

    test_bundle_9 = nominal_bundle.to_bytes()

    print("Creating a bundle with random errors")
    valid_bundle = nominal_bundle.to_bytes()
    test_bundle_10 = inject_errors(data_bytes = valid_bundle, error_rate = 256)

    
    # SECTION 2: Sending bundles to DTN Node

    print("Configuring the Data Sender")
    data_sender = UdpTxSocket(send_to_ip, send_to_port, bps_limit=(1000000 * rate_limit))

    print("Connecting the Data Sender")
    data_sender.connect()

    print("Sending valid and invalid bundles to the DTN Node")
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_2)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_3)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_4)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_5)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_6)
    data_sender.write(test_bundle_7)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_8)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_9)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_10)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)
    data_sender.write(test_bundle_1)

    print(f"Packets sent = {data_sender.get_packets_sent()}")
    
    print("Disconnecting the Data Sender")
    data_sender.disconnect()
