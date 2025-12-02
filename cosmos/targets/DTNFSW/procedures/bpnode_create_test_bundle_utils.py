
from dtntools.dtngen.blocks import (
    PayloadBlock,
    PrevNodeBlock,
    BundleAgeBlock,
    HopCountBlock,
    PrimaryBlock,
    UnknownBlock
)
from dtntools.dtngen.bundle import Bundle
from dtntools.dtngen.types import (
    EID,
    BlockType,
    BlockPCFlags,
    BundlePCFlags,
    CRCFlag,
    HopCountData,
    CRCType,
    CreationTimestamp
)



# Hopefully you don't have to change anything below here
def bpnode_create_test_bundle_util(dest_node_num, dest_service_num,
                                   src_node_num, src_service_num,
                                   crc_type,
                                   prev_node_eid=None,
                                   age=None,
                                   hop_count=None,
                                   payload_bytes=None,
                                   unknown=False):
    """Create a test bundle."""

    # Until we have security blocks, can't use CRC none for the primary block
    if crc_type == CRCType.NONE:
        print("WARNING: The primary block CRC can't be NONE, automatically setting it to CRC-16")
        pri_crc_type = CRCType.CRC16_X25
    else:
        pri_crc_type = crc_type

    primary_block = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=pri_crc_type,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node_num, "service_num": dest_service_num}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": src_node_num, "service_num": src_service_num}}),
        rpt_eid=EID({"uri": 1, "ssp": 0}), # This is equivalent to dtn:none
        creation_timestamp=CreationTimestamp({"time": 755533838904, "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    canonical_blocks = []

    if (prev_node_eid != None):
        prev_node_block = PrevNodeBlock(
            blk_type=BlockType.AUTO,
            blk_num=6,
            control_flags=0,
            crc_type=crc_type,
            prev_eid=prev_node_eid,
            crc=CRCFlag.CALCULATE,
        )
        canonical_blocks.append(prev_node_block)

    if (age != None):
        age_block = BundleAgeBlock(
            blk_type=BlockType.AUTO,
            blk_num=2,
            control_flags=0,
            crc_type=crc_type,
            bundle_age=age,
            crc=CRCFlag.CALCULATE,
        )
        canonical_blocks.append(age_block)

    if (hop_count != None):
        hop_count_block = HopCountBlock(
            blk_type=BlockType.AUTO,
            blk_num=3,
            control_flags=0,
            crc_type=crc_type,
            hop_data=hop_count,
            crc=CRCFlag.CALCULATE,
        )
        canonical_blocks.append(hop_count_block)
    if (unknown == True):
        unknown_block = UnknownBlock(
            elements=
            [
                193,                                # Block type
                10,                                 # Block number
                0,                                  # Flags
                0,                                  # CRC type
                b'\\x84\\x00\\x00\\x00\\x00\\x00',  # Block-specific data
            ]
        )
        canonical_blocks.append(unknown_block)

    payload_block = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0,
        crc_type=crc_type,
        payload=payload_bytes,
        crc=CRCFlag.CALCULATE,
    )
    canonical_blocks.append(payload_block)

    # Use them to create a bundle object
    bundle = Bundle(
        pri_block=primary_block,
        canon_blocks=canonical_blocks,
    )

    # Encode the bundle
    encoded = bundle.to_bytes()
    return encoded
