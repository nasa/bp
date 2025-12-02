# Send bad bundles to DTN Node
#
# Example usage: bad_bundles_send(104, 2, 600, 5000, "10.2.7.206", 4556, 45)
#

def bad_bundles_send(dest_number= None, dest_service= None, lifetime_in_sec = None, number_of_bundles = None, send_to_ip = None, send_to_port = None, rate_limit = None):

    import codecs
    import copy
    import time
    import traceback
    import warnings

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
    from dtntools.dtncla.errors.inject import inject_errors

    if dest_number is None:
        dest_number = 103
    
    if dest_service is None:
        dest_service = 1
    
    if lifetime_in_sec is None:
        lifetime_in_sec = 3600
    
    if number_of_bundles is None:
        number_of_bundles = 10
    
    if send_to_ip is None:
        send_to_ip = '127.0.0.1'

    if send_to_port is None:
        send_to_port = 4556
        
    if rate_limit is None:
        rate_limit = 1

    print("Configuring the Data Sender")
    data_sender = UdpTxSocket(send_to_ip, send_to_port, bps_limit=(1000000 * rate_limit))

    print("Connecting the Data Sender")
    data_sender.connect()

    loop = 0

    print("Sending bundles")
    with disable_instrumentation():
        while loop < number_of_bundles:

            loop = loop + 1

            # Defining valid bundle blocks
            primary_block = PrimaryBlock(
                version=7,
                control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
                crc_type=CRCType.CRC16_X25,
                dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_number, "service_num": dest_service}}),
                src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
                rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 0}}),
                creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": loop}),
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
            payload_block = PayloadBlock(
                blk_type=BlockType.AUTO,
                blk_num=1,
                control_flags=0,
                crc_type=CRCType.NONE,
                payload=b"\x00\x00\x00\x00\x00\x00\x00\x0chello world\n",
                crc=CRCFlag.CALCULATE,
            )

            # Creating a nominal bundle containing all defined nominal blocks
            nominal_bundle = Bundle(
                pri_block=primary_block,
                canon_blocks=[
                    prev_node_block,
                    bundle_age_block,
                    hop_count_block,
                    payload_block,
                ],
            )

            # Creating a bundle with random errors
            valid_bundle = nominal_bundle.to_bytes()
            modified_bundle = inject_errors(data_bytes = valid_bundle, error_rate = 256)

            # Sending the modified bundle
            data_sender.write(modified_bundle)

    time.sleep(5)

    print(f"Packets sent = {data_sender.get_packets_sent()}")

    print("Disconnecting the Data Sender")
    data_sender.disconnect()
