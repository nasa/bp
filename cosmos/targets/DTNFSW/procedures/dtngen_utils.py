class DTNGenUtils:
    
    """
    Extracted from: examples/dtncla/DTN_Test_Tool_Demo_E2E_HDTN_Relay_Bundle_Set.py
    """
    
    @classmethod
    def generate_bundles(cls, dest_node=100, dest_service=1, num_bundles=10, payload=None):

        # Generate bundles and store as /bundles/<dest_node>/generated_bundle_*.json
        
        from dtntools.dtngen.bundle import Bundle
        
        import time
        import traceback

        from dtntools.dtngen.blocks import (
            CanonicalBlock,
            PrevNodeBlock,
            BundleAgeBlock,
            HopCountBlock,
            CustodyTransferBlock,
            CompressedReportingBlock,
            PayloadBlock,
            PayloadBlockSettings,
            PrimaryBlock,
            PrimaryBlockSettings,
            UnknownBlock,
        )

        from dtntools.dtngen.types import (
            TypeWarning,
            BlockPCFlags,
            BlockType,
            BundlePCFlags,
            StatusRRFlags,
            CRCFlag,
            CRCType,
            EID,
            CreationTimestamp,
            HopCountData,
            CTEBData,
            CREBData,
        )

        from dtntools.dtngen.utils import DtnTimeNowMs
        import warnings

        warnings.simplefilter("always")

        #print("Define new primary and payload blocks")
        primary_block = PrimaryBlock(
            version=7,
            control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
            crc_type=CRCType.CRC16_X25,
            dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
            src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
            rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
            creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
            lifetime=3600000,
            crc=CRCFlag.CALCULATE,
        )

        payload_block = PayloadBlock(
            blk_type=BlockType.AUTO,
            blk_num=1,
            control_flags=0,
            crc_type=CRCType.CRC16_X25,
            payload=payload,
            crc=CRCFlag.CALCULATE,
        )

        #print("Creating the new set of bundles")
        generated_bundles = []
        for x in range(num_bundles):
            generated_bundles.append(Bundle(
                pri_block=primary_block,
                canon_blocks=[payload_block]
                )
        )

        #print("Writing the generated bundles to json files")
        for idx, x in enumerate(generated_bundles):
            x.to_json_file(f'/bundles/{dest_node}/generated_bundle_{idx+1}.json')


    @classmethod
    def send_bundles(cls, num_bundles, dest_node, data_sender):

        # Send specified number of bundles stored as /bundles/<dest_node>/generated_bundle_*.json
        
        from dtntools.dtngen.bundle import Bundle

        bundle_data = []        
        for n in range(num_bundles):
            bundle_data.append(Bundle.from_json_file(f'/bundles/{dest_node}/generated_bundle_{n+1}.json').to_bytes())
        
        try:
            #print("Sending the bundles to the DTN Node")
            for x in (bundle_data):
                data_sender.write(x)

            #print(f'Packets sent = {data_sender.get_packets_sent()}')

        except KeyboardInterrupt:
            pass

        except Exception:
            print(traceback.format_exc())
            

    @classmethod
    def receive_bundles(cls, num_bundles, data_receiver):

        # Receive specified number of bundles
        
        try:
            #print("Receiving the bundles returned from the DTN Node")
            received_bundle = []
            for x in range(num_bundles):
                received_bundle.append(data_receiver.read(timeout = 5))

            print(f'Packets received = {data_receiver.get_packets_received()}')

        except KeyboardInterrupt:
            pass

        except Exception:
            print(traceback.format_exc())
