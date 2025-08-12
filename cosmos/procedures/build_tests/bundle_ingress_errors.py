def bundle_ingress_errors(self):

    print("###################################################################")
    print("### Bundle Ingress test - Errors")
    print("###################################################################")
    '''
    https://aetd-git.gsfc.nasa.gov/gsfc-dtn/forks/bp/bplib/-/blob/main/inc/bplib_api_types.h?ref_type=heads
    
    CBOR Decode Errors
    BPLIB_CBOR_DEC_BUNDLE_TOO_SHORT_ERR            -120 CBOR decode error: bundle too short
    BPLIB_CBOR_DEC_BUNDLE_ENTER_ARRAY_ERR          -121 CBOR decode error: entry array
    BPLIB_CBOR_DEC_BUNDLE_MAX_BLOCKS_ERR           -122 CBOR decode error: max blocks
    BPLIB_CBOR_DEC_BUNDLE_EXIT_ARRAY_ERR           -123 CBOR decode error: entry array

    BPLIB_CBOR_DEC_PRIM_ENTER_ARRAY_ERR            -124 CBOR primary block decode error: entry array
    BPLIB_CBOR_DEC_PRIM_EXIT_ARRAY_ERR             -125 CBOR primary block decode error: exit array
    BPLIB_CBOR_DEC_PRIM_VERSION_DEC_ERR            -126 CBOR primary block decode error: decode version field
    BPLIB_CBOR_DEC_PRIM_WRONG_VERSION_ERR          -127 CBOR primary block decode error: wrong version field
    BPLIB_CBOR_DEC_PRIM_FLAG_DEC_ERR               -128 CBOR primary block decode error: decode flag field
    BPLIB_CBOR_DEC_PRIM_WRONG_FLAG_ERR             -129 CBOR primary block decode error: wrong flag field
    BPLIB_CBOR_DEC_PRIM_CRC_TYPE_DEC_ERR           -130 CBOR primary block decode error: decode crc type field
    BPLIB_CBOR_DEC_PRIM_DEST_EID_DEC_ERR           -131 CBOR primary block decode error: decode Dest EID field
    BPLIB_CBOR_DEC_PRIM_SRC_EID_DEC_ERR            -132 CBOR primary block decode error: decode Src EID field
    BPLIB_CBOR_DEC_PRIM_REPORT_EID_DEC_ERR         -133 CBOR primary block decode error: decode Report-To EID field
    BPLIB_CBOR_DEC_PRIM_CREATE_TIME_DEC_ERR        -134 CBOR primary block decode error: decode create time field
    BPLIB_CBOR_DEC_PRIM_LIFETIME_DEC_ERR           -135 CBOR primary block decode error: decode lifetime field
    BPLIB_CBOR_DEC_PRIM_CRC_VAL_DEC_ERR            -136 CBOR primary block decode error: decode crc value field

    BPLIB_CBOR_DEC_CANON_BLOCK_INDEX_ERR           -137 CBOR canon block decode error: invalid block index
    BPLIB_CBOR_DEC_CANON_ENTER_ARRAY_ERR           -138 CBOR canon block decode error: entry array
    BPLIB_CBOR_DEC_CANON_EXIT_ARRAY_ERR            -139 CBOR canon block decode error: exit array
    BPLIB_CBOR_DEC_CANON_BLOCK_TYPE_DEC_ERR        -140 CBOR canon block decode error: decode block type
    BPLIB_CBOR_DEC_CANON_BLOCK_NUM_DEC_ERR         -141 CBOR canon block decode error: decode block num
    BPLIB_CBOR_DEC_CANON_BLOCK_FLAG_DEC_ERR        -142 CBOR canon block decode error: decode block proc flags
    BPLIB_CBOR_DEC_CANON_CRC_TYPE_DEC_ERR          -143 CBOR canon block decode error: decode crc type
    BPLIB_CBOR_DEC_CANON_ENTER_BYTE_STR_ERR        -144 CBOR canon block decode error: enter data byte string
    BPLIB_CBOR_DEC_CANON_EXIT_BYTE_STR_ERR         -145 CBOR canon block decode error: exit data byte string
    BPLIB_CBOR_DEC_CANON_CRC_VAL_DEC_ERR           -146 CBOR canon block decode error: decode crc value field

    BPLIB_CBOR_DEC_PREV_NODE_EID_DEC_ERR           -147 CBOR Prev Node block decode error: decode eid field

    BPLIB_CBOR_DEC_AGE_BLOCK_DEC_ERR               -148 CBOR Age block decode error: decode eid field

    BPLIB_CBOR_DEC_HOP_BLOCK_ENTER_ARRAY_ERR       -149 CBOR Hop Count block decode error: enter array
    BPLIB_CBOR_DEC_HOP_BLOCK_EXIT_ARRAY_ERR        -150 CBOR Hop Count block decode error: exit array
    BPLIB_CBOR_DEC_HOP_BLOCK_HOP_LIMIT_DEC_ERR     -151 CBOR Hop Count block decode error: hop limit decode
    BPLIB_CBOR_DEC_HOP_BLOCK_HOP_COUNT_DEC_ERR     -152 CBOR Hop Count block decode error: hop count decode
    BPLIB_CBOR_DEC_HOP_BLOCK_INVALID_DEC_ERR       -153 CBOR Hop Count block decode error: invalid block data values
    BPLIB_CBOR_DEC_HOP_BLOCK_EXCEEDED_ERR          -154 CBOR Hop Count block decode error: hop limit exceeded
    
    BPLIB_CBOR_DEC_UNKNOWN_BLOCK_DEC_ERR           -155 CBOR canon block decode error: can't process block
    BPLIB_CBOR_DEC_BUNDLE_TOO_LONG_DEC_ERR         -156 CBOR decode error: bundle is too long
    BPLIB_CBOR_DEC_EXTRA_DATA_DEC_ERR              -157 CBOR decode error: extra data after payload
    BPLIB_CBOR_DEC_NO_PAYLOAD_ERR                  -158 CBOR decode error: no payload detected in bundle

    BPLIB_CBOR_DEC_TYPES_ENTER_DEF_ARRAY_QCBOR_ERR -160 CBOR decode types error: enter def array
    BPLIB_CBOR_DEC_TYPES_ENTER_DEF_ARRAY_COUNT_ERR -161 CBOR decode types error: def array size
    BPLIB_CBOR_DEC_TYPES_EXIT_DEF_ARRAY_QCBOR_ERR  -162 CBOR decode types error: exit def array
    BPLIB_CBOR_DEC_TYPES_GET_UINT64_QCBOR_ERR      -163 CBOR decode types error: get uint64
    BPLIB_CBOR_DEC_TYPES_EID_ENTER_OUTER_ARRAY_ERR -164 CBOR decode types error: EID enter outer array
    BPLIB_CBOR_DEC_TYPES_EID_SCHEME_NOT_IMPL_ERR   -165 CBOR decode types error: EID scheme not implemented
    BPLIB_CBOR_DEC_TYPES_EID_ENTER_SSP_ARRAY_ERR   -166 CBOR decode types error: EID enter SSP array
    BPLIB_CBOR_DEC_TYPES_EID_IPN_NODE_DEC_ERR      -167 CBOR decode types error: EID IPN node num decode
    BPLIB_CBOR_DEC_TYPES_EID_IPN_SERV_DEC_ERR      -168 CBOR decode types error: EID IPN serv num decode
    BPLIB_CBOR_DEC_TYPES_EID_EXIT_SSP_ARRAY_ERR    -169 CBOR decode types error: EID exit SSP array
    BPLIB_CBOR_DEC_TYPES_EID_EXIT_OUTER_ARRAY_ERR  -170 CBOR decode types error: EID exit outer array

    BPLIB_CBOR_DEC_TYPES_TIMESTAMP_ENTER_ARRAY_ERR -171 CBOR decode types error: timestamp enter array
    BPLIB_CBOR_DEC_TYPES_TIMESTAMP_EXIT_ARRAY_ERR  -172 CBOR decode types error: timestamp exit array
    BPLIB_CBOR_DEC_TYPES_TIMESTAMP_CREATE_DEC_ERR  -173 CBOR decode types error: timestamp create time decode
    BPLIB_CBOR_DEC_TYPES_TIMESTAMP_SEQ_NUM_DEC_ERR -174 CBOR decode types error: timestamp seq num decode

    BPLIB_CBOR_DEC_TYPES_CRC_ENTER_BYTE_STR_ERR    -175 CBOR decode types error: CRC Val enter byte-string
    BPLIB_CBOR_DEC_TYPES_CRC_16_LEN_ERR            -176 CBOR decode types error: CRC Val length not 16
    BPLIB_CBOR_DEC_TYPES_CRC_32_LEN_ERR            -177 CBOR decode types error: CRC Val length not 32
    BPLIB_CBOR_DEC_TYPES_CRC_UNSUPPORTED_TYPE_ERR  -178 CBOR decode types error: CRC Val type

    Encode Errors
    BPLIB_CBOR_ENC_PRIM_COPY_SIZE_GT_OUTPUT_ERR    -182 BPLib_CBOR_CopyOrEncodePrimary: Copy Size Error
    BPLIB_CBOR_ENC_PAYL_COPY_SIZE_GT_OUTPUT_ERR    -183 BPLib_CBOR_CopyOrEncodePayload: Copy Size Error
    BPLIB_CBOR_ENC_PRIM_QCBOR_FINISH_ERR           -184 BPLib_CBOR_EncodePrimary: QCBOREncode_Finish Error
    BPLIB_CBOR_ENC_EXT_INPUT_BLOCK_INDEX_ERR       -185 BPLib_CBOR_EncodeExtensionBlock: Ext Block Index Error
    BPLIB_CBOR_ENC_EXT_QCBOR_FINISH_ERR            -186 BPLib_CBOR_EncodeExtensionBlock: QCBOREncode_Finish Error
    BPLIB_CBOR_ENC_PAYL_QCBOR_FINISH_HEAD_ERR      -187 BPLib_CBOR_EncodeExtensionBlock: QCBOREncode_Finish Error
    BPLIB_CBOR_ENC_PAYL_ADD_BYTE_STR_HEAD_ERR      -188 BPLib_CBOR_EncodeExtensionBlock: QCBOREncode_Finish Error
    BPLIB_CBOR_ENC_PAYL_QCBOR_FINISH_TAIL_ERR      -189 BPLib_CBOR_EncodeExtensionBlock: QCBOREncode_Finish Error

    BPLIB_CBOR_ENC_BUNDLE_OUTPUT_BUF_LEN_1_ERR     -190 BPLib_CBOR_EncodeBundle: Output buf too small (check 1)
    BPLIB_CBOR_ENC_BUNDLE_OUTPUT_BUF_LEN_2_ERR     -191 BPLib_CBOR_EncodeBundle: Output buf too small (check 2)
    BPLIB_CBOR_ENC_BUNDLE_OUTPUT_BUF_LEN_3_ERR     -192 BPLib_CBOR_EncodeBundle: Output buf too small (check 3)
    BPLIB_CBOR_ENC_BUNDLE_OUTPUT_BUF_LEN_4_ERR     -193 BPLib_CBOR_EncodeBundle: Output buf too small (check 4)
    
    Bundle Interface Errors
    BPLIB_BI_INVALID_BUNDLE_ERR                    -260
    
    
    Testcase:
    #Invalid bundle
    #4. too large bundle
    #5. invalid Primary Block data and a Valid EID
    #6.	invalid Primary Block data and Invalid EID
    #7. invalid Primary Block CRC
    #8. Primary Block flags that are not checked
    #9. Unsupported Extension Block (delete)
    #10. Unsupported Block (remove)
    #11. “Unintelligible” Canonical Block without a Payload Block
    #12. “Unintelligible” Canonical Block with a Payload Block
    #13. Canonical Block containing a bad CRC without a Payload Block
    #14. Canonical Block containing a bad CRC with a Payload Block
    #15. Valid DTN time, lifetime exceeded
    #16. Invalid DTN time
    #17. Creation Time = 0
    #18. After the first build: Creation Time = 0, with Age block
    #19. After the first build: Creation Time = 0 and no Age Block
    #20. source that is not authorized
    
    #Storage errors
    #21. no or too little space allocated for the bundle (or both?)
    #22. no or too little space left available for the bundle (or both?)
    #23. DST EID = Local EID (this is the destination) but an output queue to deliver the message does not exist (Maybe delete a message queue to an app? This will cause the system to be unstable). The system is configured to not allow storage in this case.
    #24. DST EID = Local EID (this is the destination) but an output queue to deliver the message does not exist (Maybe delete a message queue to an app? This will cause the system to be unstable). The system is configured to allow storage in this case.
    #25. Fragmented bundle
    #26. At this point, all bundles that were sent should have been stored or discarded 
    '''
    ##*************************************************************************
    
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
    load_utility ("DTNFSW-1/procedures/load_new_table.py")
    import codecs
    import subprocess
    import time
    import traceback
    import warnings
    import copy
    from datetime import datetime

    from dtntools.dtncla.udp import UdpTxSocket, UdpRxSocket
    from dtntools.dtngen.bundle import Bundle
    from dtntools.dtngen.types import InvalidCBOR
    from dtntools.dtngen.utils import DtnTimeNowMs
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
        RawData,
    )

    warnings.simplefilter("always")

    ###
    
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        "DTN.6.04030":"U",
        #"DTN.6.04070":"U",
        #"DTN.6.04080":"U", #paramSetMaxLifetime not set - not possible
        #"DTN.6.04090":"U",
        #"DTN.6.04100":"U",
        #"DTN.6.04110":"U",
        "DTN.6.04180":"U",
        #"DTN.6.04195":"U", #creation timestamp cannot be compared to the current time - how?
        "DTN.6.04314":"U", 
        "DTN.6.04318":"U",
        "DTN.6.04321":"U", 
        "DTN.6.06000":"U",
        "DTN.6.06010":"U",
        "DTN.6.06012":"U",
        "DTN.6.06014":"U",
        "DTN.6.06016":"U",
        "DTN.6.06040":"U",
        "DTN.6.06050":"U",
        "DTN.6.06060":"U",
        #"DTN.6.06070":"U",
        "DTN.6.06080":"U",
        "DTN.6.06092":"U",
        "DTN.6.06102":"U",
        "DTN.6.06105":"U",
        "DTN.6.06110":"U",
        "DTN.6.06120":"U",
        "DTN.6.06130":"U",
        #"DTN.6.06140":"U",
        "DTN.6.06150":"U",
        "DTN.6.06151":"U",
        "DTN.6.06160":"U",
        "DTN.6.06170":"U",
        "DTN.6.06172":"U",
        #"DTN.6.06173":"U",
        "DTN.6.06174":"U",
        #"DTN.6.06175":"U",
        #"DTN.6.06176":"U",
        #"DTN.6.06177":"U",
        "DTN.6.06180":"U",
        "DTN.6.06190":"U",
        "DTN.6.06200":"U",
        "DTN.6.06210":"U",
        "DTN.6.06230":"U",
        "DTN.6.06240":"U",
        "DTN.6.06250":"U",
        "DTN.6.06260":"U",
        "DTN.6.06270":"U",
        "DTN.6.06280":"U",
        "DTN.6.06282":"U",
        #"DTN.6.06283":"U",
        #"DTN.6.06290":"U",
        #"DTN.6.06300":"U",
        #"DTN.6.06310":"U",
        #"DTN.6.06320":"U",
        "DTN.6.06330":"U",
        #"DTN.6.06340":"U",
        "DTN.6.06350":"U",
        "DTN.6.06352":"U",
        #"DTN.6.06353":"U",
        #"DTN.6.06358":"U",
        "DTN.6.06360":"U",
        #"DTN.6.06370":"U",
        "DTN.6.06380":"U",
        "DTN.6.06400":"U",
        "DTN.6.06410":"U",
        #"DTN.6.06420":"U",
        "DTN.6.06440":"U",
        #"DTN.6.06450":"U",
        #"DTN.6.06480":"U",
        #"DTN.6.06490":"U",
        #"DTN.6.06492":"U",
        #"DTN.6.06500":"U",
        #"DTN.6.06501":"U",
        #"DTN.6.06510":"U",
        #"DTN.6.06520":"U",
        #"DTN.6.06530":"U",
        #"DTN.6.06540":"U",
        #"DTN.6.06542":"U",
        #"DTN.6.06544":"U",
        #"DTN.6.06546":"U",
        "DTN.6.06552":"U",
        #"DTN.6.06553":"U",
        #"DTN.6.06554":"U",
        #"DTN.6.06555":"U",
        #"DTN.6.06560":"U",
        #"DTN.6.06570":"U",
        #"DTN.6.06580":"U",
        #"DTN.6.06590":"U",
        #"DTN.6.06600":"U",
        #"DTN.6.06602":"U",
        #"DTN.6.06604":"U",
        #"DTN.6.06606":"U",
        "DTN.6.06610":"I",
        "DTN.6.06620":"I",
        "DTN.6.06630":"I",
        "DTN.6.06640":"U",
        "DTN.6.06650":"U",
        "DTN.6.06660":"A", #event message contents need analysis
        "DTN.6.06670":"U",
        "DTN.6.12165":"U",
        "DTN.6.13071":"U",
        "DTN.6.13081":"U",
        "DTN.6.15031":"U",
        "DTN.6.16015":"U",
        "DTN.6.19160":"U",
        "DTN.6.19180":"U",
        "DTN.6.19190":"U",
        "DTN.6.19360":"U", 
        "DTN.6.19390":"U", 
        "DTN.6.19410":"U", 
        "DTN.6.26010":"U",
        "DTN.6.26020":"U",
        "DTN.6.26030":"U",
        "DTN.6.26190":"U",
        "DTN.6.26200":"U",
        "DTN.6.26210":"U",
        "DTN.6.26220":"U",
        "DTN.6.26240":"U",
        "DTN.6.26250":"U",
        "DTN.6.26260":"U",

        #reset requirements
        "DTN.6.12118":"U", 
        "DTN.6.12120":"U", 
        "DTN.6.12150":"U",
        "DTN.6.12940":"U", 
        "DTN.6.12950":"U", 
        "DTN.6.19090":"U", 
        "DTN.6.20000":"U",
        "DTN.6.20010":"U",
        "DTN.6.20030":"U",
        "DTN.6.20080":"U",
        "DTN.6.20090":"U",
    }
    
    ## Destination EID configuration
    dest_node    = 200
    dest_service = 64
    '''
    ## Destination EID configuration (to Channel 1 configured for ADU delivery)
    dest_node    = 100
    dest_service = 53
    '''
    ## Address/port configuration
    dest_ip   = "10.2.4.20"
    dest_port = 4501

    local_ip = "0.0.0.0"
    local_port = 4551

    ## Configure and connect data sender and receiver
    data_sender = UdpTxSocket(dest_ip, dest_port)
    data_sender.connect()
    
    data_receiver = UdpRxSocket(local_ip, local_port)
    data_receiver.connect()
    

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
        payload=b'\xAA'*8,
        crc=CRCFlag.CALCULATE,
    )
    
    global received_cnt, discarded_cnt, deleted_cnt, event_pkt_cnt, \
           del_hop_cnt, del_unint_cnt
    mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK" 
    mib_config_pkt = "BPNODE_NODE_MIB_CONFIG_HK" 
    
    ##=========================================================================
    ## get_initial_counts
    ##=========================================================================
    def get_initial_counts():

        global received_cnt, discarded_cnt, deleted_cnt, event_pkt_cnt, \
               del_hop_cnt, del_unint_cnt
        
        received_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED")
        discarded_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DISCARDED")
        deleted_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED")
        event_pkt_cnt = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")        
        del_hop_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_HOP_EXCEEDED")
        del_unint_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")


    ##=========================================================================
    ## verify_telem_and_event
    ##=========================================================================
    def verify_telem_and_event():

        global received_cnt, discarded_cnt, deleted_cnt, event_pkt_cnt
        
        ret_status = "P"
        
        ## Verify BUNDLE_COUNT_DISCARDED and BUNDLE_COUNT_DELETED increment, but not BUNDLE_COUNT_RECEIVED
        
        wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DISCARDED == {discarded_cnt+1}", 10)
        received_cnt_now = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED")
        discarded_cnt_now = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DISCARDED")
        deleted_cnt_now = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED")
        
        print(f"BUNDLE_COUNT_RECEIVED: {received_cnt_now}")
        print(f"BUNDLE_COUNT_DISCARDED: {discarded_cnt_now}")
        print(f"BUNDLE_COUNT_DELETED: {deleted_cnt_now}")
        
        if discarded_cnt_now == discarded_cnt+1:
            print("!!! BUNDLE_COUNT_DISCARDED incremented as expected")
            status = "P"
        else:
            print("!!! ERROR - BUNDLE_COUNT_DISCARDED did not increment as expected")
            status = "F"

        for rqmnt in ["DTN.6.04030", "DTN.6.04314"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        if status == "F":
            ret_status = "F"             

        if deleted_cnt_now == deleted_cnt+1:
            print("!!! BUNDLE_COUNT_DELETED incremented as expected")
            status = "P"
        else:
            print("!!! ERROR - BUNDLE_COUNT_DELETED did not increment as expected")
            status = "F"
    
        for rqmnt in ["DTN.6.04180", "DTN.6.04318"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        if status == "F":
            ret_status = "F"             
        

        rqmnt = "DTN.6.06552"
        if received_cnt_now == received_cnt:
            print("!!! BUNDLE_COUNT_RECEIVED remained same as expected")
            status = "P"
        else:
            print("!!! ERROR - BUNDLE_COUNT_RECEIVED did not remain same as expected")
            status = "F"
    
        TestUtils.set_requirement_status(rqmnt, status)
        
        if status == "F":
            ret_status = "F"             
        

        ## Verify expected event

        wait_check(f"{target} CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT > {event_pkt_cnt}", 10)
        
        app   = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME")
        eid   = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_ID")
        etype = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG PACKET_ID_EVENT_TYPE")
        emsg  = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG MESSAGE")
        
        # Print received event message
        print ("!!! Event: " + app + " " + str(eid) + " " + etype + " " + emsg) 
            
                   
        #if app == 'BPNODE' and (eid == 66 or eid == 650) and etype == 'ERROR': #TBD
        if app == 'BPNODE' and (eid == 66 or eid == 650) and etype == 'INFO':   
            print ("!!! Expected event message received")
            status = "P"
        else:
            print ("!!! ERROR - Expected event message not received")
            status = "F"
        
        for rqmnt in ["DTN.6.06650", "DTN.6.16015"]:
            TestUtils.set_requirement_status(rqmnt, status)
        '''
        if status == "F":
            ret_status = "F"             
        '''
        return ret_status
        

    ##=========================================================================
    ## send_invalid_cbor_bundle
    ##=========================================================================
    def send_invalid_cbor_bundle(major_type, add_info, payload_data):
        
        '''
        Creates and sends a bundle with an invalid CBOR item
        
            major_type      [0-7]
            add_info        [28-31]
            payload_data    any data consistent with major_type
        
        Primary block is unchanged, only payload in payload block
        '''
        
        # Create bundle with payload_data codded with invalid CBOR
        payload_block_inv = copy.deepcopy(payload_block)
        payload_block_inv.payload = InvalidCBOR(payload_data,additional_info=add_info)

        bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
        data_sender.write(bundle_inv.to_bytes())

    ##=========================================================================
    ## verify_primary_error_counts
    ##=========================================================================
    def verify_primary_error_counts():

        global del_unint_cnt
        
        # Verify BUNDLE_COUNT_DELETED_UNINTELLIGIBLE
        print("BUNDLE_COUNT_DELETED_UNINTELLIGIBLE: ", 
            tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_UNINTELLIGIBLE"))

        if tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_UNINTELLIGIBLE") == del_unint_cnt+1:
            status = "P"
        else:
            status = "F"
            
        for rqmnt in ["DTN.6.06170", "DTN.6.06440"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        return status
        
    ##=========================================================================
    ## verify_canon_error_counts
    ##=========================================================================
    def verify_canon_error_counts():
        
        global del_unint_cnt

        print("BUNDLE_COUNT_DELETED_UNINTELLIGIBLE: ", 
            tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_UNINTELLIGIBLE"))

        # Verify BUNDLE_COUNT_DELETED_UNINTELLIGIBLE
        if tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_UNINTELLIGIBLE") == del_unint_cnt+1:
            status = "P"
        else:
            status = "F"
            
        for rqmnt in ["DTN.6.06440"]:
            TestUtils.set_requirement_status(rqmnt, status)
            
    ###################################################################

    ##=========================================================================
    ## main
    ##=========================================================================
    prompt("Copy following tables to CFS cf folder\n\n" +
           " - contact_nominal.tbl\n" +
           " - contact_rx_only.tbl\n" +
           " - cont_dup_dest.tbl\n" +
           " - cont_inv_dest.tbl\n" +
           " - mib_pn_blen_0.tbl\n" +
           " - mib_pn_blen_200.tbl\n" +
           " - mib_pn_invplen.tbl"
          )
    
    ## Load nominal contact table and start contact

    load_new_table('/cf/contact_nominal.tbl')

    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")

    cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")
    
    
    print("-----------------------------------------------------------------")
    print("1. Invalid CBOR items")
    print("   Expected -  BPLIB_CBOR_DEC_CANON_ENTER_BYTE_STR_ERR: -144")
    print("-----------------------------------------------------------------")

    sub_test=0
    for major_type, additional_info, payload_data in [
            [0,29,100],
            [0,31,100],
            [1,29,-1],
            [1,31,-1],
            [2,30,b'\xAA'*100],
            [3,30,"ABCDEFGHIJKLMNOPQRSTUVWXYZ"],
            [4,28,[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]],
            [5,29,{"abc":1, "xyz":2}],
            [6,29,RawData(b'\xc2\x49\x01\x00\x00\x00\x00\x00\x00\x00\x00')],
            [6,31,RawData(b'\xc2\x49\x01\x00\x00\x00\x00\x00\x00\x00\x00')],
            [7,30,12346.789]
        ]:
        sub_test+=1
        print("...............................................................")
        print(f'1.{sub_test} - Encoded item {major_type}_{additional_info}')
        print("...............................................................")
        
        get_initial_counts()
   
        send_invalid_cbor_bundle(major_type, additional_info, payload_data)

        status = verify_telem_and_event()

        #wait()

    for rqmnt in [
        "DTN.6.06000", "DTN.6.06010", "DTN.6.06012", "DTN.6.26190", 
        "DTN.6.26200"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
    

    print("-----------------------------------------------------------------")
    print("2. Too many blocks")
    print("   Expected - BPLIB_CBOR_DEC_BUNDLE_MAX_BLOCKS_ERR: -122")
    print("-----------------------------------------------------------------")
    #BPLIB_MAX_NUM_EXTENSION_BLOCKS=4, add 1 for primary block
    
    canon_blocks=[]
    for _ in range(10):
        canon_blocks.append(payload_block)
    
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=canon_blocks)
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06000", "DTN.6.06014", "DTN.6.06016", "DTN.6.26210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    

    print("-----------------------------------------------------------------")
    print("3. Too large/short bundle ")
    print("-----------------------------------------------------------------")
    
    print(".................................................................")
    print("3.1 Too large bundle") #06350, 06352
    print("    Expected - BPLIB_CBOR_DEC_BUNDLE_TOO_LONG_DEC_ERR: -156")
    print(".................................................................")
    
    ## Load MIB PN table with PARAM_SET_MAX_BUNDLE_LENGTH 200
    #load_new_table('/cf/mib_pn_blen_200.tbl')
    cmd(f"{target} BPNODE_CMD_SET_MIB_ITEM with EID_MAXNODE 100, EID_MINNODE 100, MIB_ITEM 'PARAM_SET_MAX_PAYLOAD_LENGTH', VALUE 100")
    cmd(f"{target} BPNODE_CMD_SET_MIB_ITEM with EID_MAXNODE 100, EID_MINNODE 100, MIB_ITEM 'PARAM_SET_MAX_BUNDLE_LENGTH', VALUE 200")
    if wait(f"{target} {mib_config_pkt} PARAM_SET_MAX_BUNDLE_LENGTH == 200", 6) and \
       wait(f"{target} {mib_config_pkt} PARAM_SET_MAX_PAYLOAD_LENGTH == 100", 6):
        status = "P"
    else:
        status = "F"
        
    for rqmnt in ["DTN.6.12165"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    ## Define a payload of length 200
    payload_block_inv = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*200,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    del_too_long_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_TOO_LONG")
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06350"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    ## Verify BUNDLE_COUNT_DELETED_TOO_LONG
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_TOO_LONG == {del_too_long_cnt+1}", 6):
        status = "P"
    else:
        status = "F"

    for rqmnt in ["DTN.6.06352"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    
    ## Restore nominal MIB PN table
    #load_new_table('/cf/bpnode_mib_pn.tbl')
    cmd(f"{target} BPNODE_CMD_SET_MIB_ITEM with EID_MAXNODE 100, EID_MINNODE 100, MIB_ITEM 'PARAM_SET_MAX_BUNDLE_LENGTH', VALUE 8192")
    cmd(f"{target} BPNODE_CMD_SET_MIB_ITEM with EID_MAXNODE 100, EID_MINNODE 100, MIB_ITEM 'PARAM_SET_MAX_PAYLOAD_LENGTH', VALUE 4096")
    wait(f"{target} {mib_config_pkt} PARAM_SET_MAX_BUNDLE_LENGTH == 8192", 6)
    wait(f"{target} {mib_config_pkt} PARAM_SET_MAX_PAYLOAD_LENGTH == 4096", 6)
    


    print(".................................................................")
    print("3.2 Too short bundle - no requirement")
    print("    Expected - BPLIB_CBOR_DEC_BUNDLE_TOO_SHORT_ERR: -120")
    print(".................................................................")
    
    get_initial_counts()
    
    bundle_inv = Bundle() # empty
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    print("-----------------------------------------------------------------")
    print("5. Primary block errors")
    print("-----------------------------------------------------------------")
    
    print(".................................................................")
    print("5.1 Missing Primary block")
    print("    Expected - BPLIB_CBOR_DEC_PRIM_WRONG_VERSION_ERR: -127")
    print(".................................................................")

    get_initial_counts()    
    
    bundle_inv = Bundle(canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())    

    status = verify_telem_and_event()
    
    print(".................................................................")
    print("5.2 Primary block, invalid version") #06040
    print("    Expected - BPLIB_CBOR_DEC_PRIM_VERSION_DEC_ERR: -126")
    print(".................................................................")
    primary_block_inv = PrimaryBlock(
        version=0.7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.NONE,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06040", "DTN.6.26010"]:   
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify primary block error counts
    verify_primary_error_counts()
    
    print(".................................................................")
    print("5.3 PCF not unsigned integer") #06050
    print("    Expected - BPLIB_CBOR_DEC_PRIM_FLAG_DEC_ERR: -128")
    print(".................................................................")

    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=-1,#BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06050"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    

    '''
    print(".................................................................")
    print("5.x PCF Fragment not unsigned integer") #06150
    print("    Expected - BPLIB_CBOR_DEC_PRIM_FLAG_DEC_ERR: -128")
    print(".................................................................")

    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.IS_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
        0,
        -1
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06150"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    
    '''
    
    print(".................................................................")    
    print("5.4 PCF unrecognized") #06060 - ignore PCD
    print(".................................................................")
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=40,#BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    forwarded_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED")
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    #Verify bundle is forwarded
    rqmnt = "DTN.6.06060"
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED == {forwarded_cnt}+1", 10):
        status = "P"
    else:
        status = "F"
    TestUtils.set_requirement_status(rqmnt, status)
    

    print(".................................................................")
    print("5.5 No crc_type")
    print("    Expected - BPLIB_CBOR_DEC_PRIM_CRC_TYPE_DEC_ERR: -130")
    print(".................................................................")    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        #   crc_type=CRCType.NONE,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
        
    # Verify primary block error counts
    verify_primary_error_counts()

    print(".................................................................")
    print("5.6 CRC type not 0, 1, or 2") #06080
    print("    Expected - BPLIB_CBOR_DEC_TYPES_CRC_UNSUPPORTED_TYPE_ERR: -178")
    print(".................................................................")    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=3, #CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06080"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    


    print(".................................................................")
    print("5.7 CRC type 0 and primary block not part of BIB") #06092
    print("    Expected - : -")
    print(".................................................................")    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=0, #CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06092"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    

    
    print(".................................................................")   
    print("5.8 Invalid dest EID") #06105
    print("    Expected - BPLIB_CBOR_DEC_PRIM_DEST_EID_DEC_ERR: -131")
    print(".................................................................")   
    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": -200, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    # Verify primary block error counts
    status = verify_primary_error_counts()    
    
    for rqmnt in ["DTN.6.06105"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    print(".................................................................")        
    print("5.9 Invalid source EID") #06105
    print("    Expected - BPLIB_CBOR_DEC_PRIM_SRC_EID_DEC_ERR: -132")
    print(".................................................................")
    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 20, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06000", "DTN.6.06102", "DTN.6.06670"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    # Verify primary block error counts
    status = verify_primary_error_counts()    
    
    for rqmnt in ["DTN.6.06105"]:
        TestUtils.set_requirement_status(rqmnt, status)

    print(".................................................................")    
    print("5.10 Invalid report EID") #06105
    print("     Expected - BPLIB_CBOR_DEC_PRIM_REPORT_EID_DEC_ERR: -133")
    print(".................................................................")
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": -10}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    # Verify primary block error counts
    status = verify_primary_error_counts()    
    
    for rqmnt in ["DTN.6.06105"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    
    print(".................................................................")
    print("5.11 Creation time = 0 and no Bundle Age block") #06120
    print("      Expected - BPLIB_BI_INVALID_BUNDLE_ERR: -260")
    print(".................................................................")
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": 0, "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06120"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify canonical block error counts
    verify_canon_error_counts()


    print("-----------------------------------------------------------------")
    print("5.12 Invalid creation time")
    print("     Expected - BPLIB_CBOR_DEC_PRIM_CREATE_TIME_DEC_ERR: -134")
    print("-----------------------------------------------------------------")   
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": -100, "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    # Verify canonical block error counts
    verify_canon_error_counts()
        
    print("-----------------------------------------------------------------")
    print("5.13 Invalid creation time - not a 2-element array") #06110
    print("     Expected - BPLIB_CBOR_DEC_PRIM_CREATE_TIME_DEC_ERR: -134")
    print("-----------------------------------------------------------------")   
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=DtnTimeNowMs(),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06110"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("5.14 No lifetime")
    print("     Expected - BPLIB_CBOR_DEC_PRIM_LIFETIME_DEC_ERR: -135")
    print(".................................................................")
    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
            #lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())

    status = verify_telem_and_event()
    
    # Verify primary block error counts
    verify_primary_error_counts()    
        
    print(".................................................................")   
    print("5.15 Lifetime not unsigned integer") #06130
    print("     Expected - BPLIB_CBOR_DEC_PRIM_LIFETIME_DEC_ERR: -135")
    print(".................................................................")
    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=-1,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06130"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    

    print(".................................................................")   
    print("5.16 CRC Failure")
    print("     Expected - BPLIB_INVALID_CRC_ERROR: -11)")
    print(".................................................................")   
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=b'\x00\x00',
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
   
    for rqmnt in [
        "DTN.6.06000", "DTN.6.06160", "DTN.6.06670", "DTN.6.26240", "DTN.6.26250"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    # Verify primary block error counts
    verify_primary_error_counts()
    
    print(".................................................................")   
    print("5.17 Primary block not an array of 8-11 elements") #26030
    print("     Or CRC missing") #16150
    print("     Expected - BPLIB_CBOR_DEC_PRIM_CRC_VAL_DEC_ERR: -135")
    print(".................................................................")   
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        #crc=b'\x00\x00',
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
   
    for rqmnt in ["DTN.6.06150", "DTN.6.26030"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    # Verify primary block error counts
    verify_primary_error_counts()
    
    
    print(".................................................................")
    print("5.18 CRC type 2 and CRC missing ") #06151
    print("     BPLIB_CBOR_DEC_PRIM_CRC_VAL_DEC_ERR: -135")
    print(".................................................................")    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=2, #CRCType.CRC32,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        #crc=1234,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06151"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    
    

    print(".................................................................")
    print("5.19 CRC type 2 and CRC not 4 element array ") #06151
    print("     BPLIB_CBOR_DEC_PRIM_CRC_VAL_DEC_ERR: -135")
    print(".................................................................")    
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=2, #CRCType.CRC32,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=3600000,
        crc=b'\x00\x00',
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    for rqmnt in ["DTN.6.06151"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify primary block error counts
    verify_primary_error_counts()    
    
    print("-----------------------------------------------------------------")
    print("6. Payload block errors")
    print("-----------------------------------------------------------------")
    
    print(".................................................................")
    print("6.1 Payload block - Invalid block number")
    print("    Expected - BPLIB_CBOR_DEC_CANON_BLOCK_NUM_DEC_ERR: -141")
    print(".................................................................")
    payload_block_inv = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=2, #1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*8,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06000", "DTN.6.06270", "DTN.6.06330", "DTN.6.26010"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print(".................................................................")
    print("6.2 Payload Block CRC Failure")
    print("    Expected - BPLIB_INVALID_CRC_ERROR: -11")    
    print(".................................................................")

    payload_block_inv = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0, 
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*8,
        crc=b'\x00\x00',
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in [
        "DTN.6.06000", "DTN.6.06230", "DTN.6.26240", "DTN.6.26250"        
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()


    print("-----------------------------------------------------------------")
    print("7. Invalid HOP Count block")
    print("-----------------------------------------------------------------")
    
    print("7.1 HOP Count block invalid data") #26260, 06380
    print("    Expected - BPLIB_CBOR_DEC_HOP_BLOCK_INVALID_DEC_ERR: -153")
    #   
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 0}),
        #hop_data=HopCountData({"hop_limit": -15, "hop_count": 3}),
        #hop_data=HopCountData({"hop_limit": 0, "hop_count": 0}),
        #hop_data=HopCountData({"hop_limit": 255, "hop_count": 3}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.26010", "DTN.6.26260", "DTN.6.06380"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()
        
    print(".................................................................")
    print("7.2 HOP Count block hop limit exceeded") #06400,06410
    print("     Expected - BPLIB_CBOR_DEC_HOP_BLOCK_EXCEEDED_ERR: -154")
    print(".................................................................")
    
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 13}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    # Verify BUNDLE_COUNT_DELETED_HOP_EXCEEDED
    if tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_HOP_EXCEEDED") == del_hop_cnt+1:
        status = "P"
    else:
        status = "F"
        
    for rqmnt in ["DTN.6.06400", "DTN.6.06410"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    print("-----------------------------------------------------------------")
    print("8. Blocks processed in order") #26020
    print("    Expected - BPLIB_CBOR_DEC_HOP_BLOCK_EXCEEDED_ERR: -154")
    print("    Expected - BPLIB_CBOR_DEC_EXTRA_DATA_DEC_ERR: -157")    
    print("-----------------------------------------------------------------")
    
    # Hop block with hop count error
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 13}),
        crc=CRCFlag.CALCULATE,
    )

    # Payload block with CRC error
    payload_block_inv = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0, 
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*8,
        crc=b'\x00\x00',
    )

    get_initial_counts()
    
    status = "P"
    
    # Array has hop block followed by payload block
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    print("Verify hop block error is reported")
    verify_telem_and_event()
    
    HOP_BLOCK_EXCEEDED_ERR = "RC = -154"
    if HOP_BLOCK_EXCEEDED_ERR not in tlm(f"DTNFSW-1 CFE_EVS_LONG_EVENT_MSG MESSAGE"):
        print(f"!!! ERROR - Error not {HOP_BLOCK_EXCEEDED_ERR} as expected")
        status = "F"
            
    # Array has payload block is reported
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv, hop_count_block])
    data_sender.write(bundle_inv.to_bytes())
    
    print("Verify payload block error is reported")
    verify_telem_and_event()
    
    EXTRA_DATA_ERROR = "RC = -157"
    if EXTRA_DATA_ERROR not in tlm(f"DTNFSW-1 CFE_EVS_LONG_EVENT_MSG MESSAGE"):
        print(f"!!! ERROR - Error not {EXTRA_DATA_ERROR} as expected")
        status = "F"

    for rqmnt in ["DTN.6.26020"]:
        TestUtils.set_requirement_status(rqmnt, status)
        

    print("-----------------------------------------------------------------")
    print("9. Unknown block")
    print("-----------------------------------------------------------------")
    #bundleCountDeletedUnsupportedBlock
    
    print(".................................................................")
    print("9.1 Invalid block type")
    print("    Expected - BPLIB_CBOR_DEC_UNKNOWN_BLOCK_DEC_ERR: -155")
    print(".................................................................")   
    payload_block_inv = PayloadBlock(
        blk_type=99,
        blk_num=1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*8,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    for rqmnt in ["DTN.6.06000", "DTN.6.06270", "DTN.6.06330"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print(".................................................................")    
    print("9.2 PCF bit 2 set (Delete bundle if block can't be processed)")
    print("     Expected - BPLIB_CBOR_DEC_UNKNOWN_BLOCK_DEC_ERR: -155")
    print(".................................................................")        
    unknown_block = UnknownBlock(
        elements=
        [
            73, #invalid block type
            5,
            4, #bit 2 set
            CRCType.CRC16_X25,
            b'\x55',
            CRCFlag.CALCULATE,
        ]
    )
    
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[unknown_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    
    #DTN.6.04180, DTN.6.06000, DTN.6.06270, DTN.6.06330, DTN.6.06340, DTN.6.06650, DTN.6.06660, DTN.6.16015, DTN.6.04030
    #discard the bundle for reason "Block unsupported."

    
    print(".................................................................")    
    print("9.3 - PCF bit 4 set (Discard block if block can't be processed)") #06280, 06282
    print(".................................................................")
    unknown_block = UnknownBlock(
        elements=
        [
            73,
            5,
            16, #bit 4 set
            CRCType.NONE,
            b'\x55',
        ]
    )
    
    get_initial_counts()
    unproc_blks_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_UNPROCESSED_BLOCKS")
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[unknown_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    # Verify bundle accepted with block removed
    item_name = "BUNDLE_COUNT_RECEIVED"
    exp_val = received_cnt+1    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)

    # Receive bundle data and verify block is removed
    received_bundle = Bundle.from_bytes(data_receiver.read())
    print(f"Received bundle: {received_bundle.to_json()}")   
    if len(received_bundle.canon_blocks) != 1:
        status = "F"
        
    for rqmnt in ["DTN.6.06280"]:
        TestUtils.set_requirement_status(rqmnt, status)    
    
    # Verify BUNDLE_COUNT_UNPROCESSED_BLOCKS
    item_name = "BUNDLE_COUNT_UNPROCESSED_BLOCKS"
    exp_val = unproc_blks_cnt+1    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
        
    for rqmnt in ["DTN.6.06282"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
        
    print("-----------------------------------------------------------------")
    print("11. No Payload Block")
    print("-----------------------------------------------------------------")

    print(".................................................................")
    print("11.1 Missing Canonical block - incorrect payload block number")
    print("     Expected - BPLIB_CBOR_DEC_CANON_BLOCK_NUM_DEC_ERR: -141 ")
    print(".................................................................")    
    payload_block_inv = PayloadBlock(
        blk_type=BlockType.AUTO,
        blk_num=5,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        payload=b'\xAA'*8,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[payload_block_inv])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
        

    for rqmnt in ["DTN.6.06000", "DTN.6.06360", "DTN.6.06640", "DTN.6.04030"]:
        TestUtils.set_requirement_status(rqmnt, status)

    print(".................................................................")
    print("11.2 Missing Canonical block - no payload block")
    print("     Expected - BPLIB_CBOR_DEC_NO_PAYLOAD_ERR: -158")
    print(".................................................................")
    #Incorrect - BPNODE 650: [CLA In #0]: Error ingressing bundle, RC = -11 bundle not deleted TBD DTNN-1314

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block)
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    
    print(".................................................................")
    print("11.3 Missing Canonical block - no payload block")
    print("      Expected - BPLIB_CBOR_DEC_NO_PAYLOAD_ERR: -158")
    print(".................................................................")

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
   

    print("-----------------------------------------------------------------")
    print("12.  Canonical block BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
    print("-----------------------------------------------------------------")
    
    #TBD - using hop_count_block, to be repeated for other canonical blocks
    
    print(".................................................................")
    print("12.1 Multiple blocks with same Block number") #06250    
    #BPNODE 650: [CLA In #0]: Error ingressing bundle, RC = -260
    print(".................................................................")
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=1,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06250"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.2 More than one block of same type") #06260
    print("     Expected -  BPLIB_BI_INVALID_BUNDLE_ERR: -260")
    print(".................................................................")    
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06260"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.3 Block number 0") #06240
    print("     Expected: - BPLIB_CBOR_DEC_CANON_BLOCK_NUM_DEC_ERR: -141")
    print(".................................................................")    
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=0,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06240"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.4 Valid CRC Type and crc signed integer") #06210
    print("     Expected: - BPLIB_CBOR_DEC_CANON_CRC_VAL_DEC_ERR: -146")
    print(".................................................................")
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=-1,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06210"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.5 Valid CRC Type and crc missing") #06210
    print("     Expected: - BPLIB_CBOR_DEC_CANON_CRC_VAL_DEC_ERR: -146")
    print(".................................................................")   
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        #crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06210", "DTN.6.26220"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.6 Invalid CRC type") #06200
    print("     Expected - BPLIB_CBOR_DEC_CANON_CRC_TYPE_DEC_ERR: -143")
    print(".................................................................")   
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=3,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06200"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.7 PCF not unsigned integer") #06190
    print(".................................................................")
    #BPNODE 60: [CLA In #0]: Failed to ingress bundle. Error = -122 TBD
    
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=-5,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06190"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify canonical block error counts
    verify_canon_error_counts()

    print(".................................................................")
    print("12.8 Less than 5 or 6 elements") #06180
    print(".................................................................")
    #-122 or -123 TBD
    
    hop_count_block = HopCountBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=0,
        crc_type=CRCType.CRC16_X25,
        hop_data=HopCountData({"hop_limit": 10, "hop_count": 1}),
        #crc=CRCFlag.CALCULATE,
    )
    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[hop_count_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
            
    for rqmnt in ["DTN.6.06180"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify canonical block error counts
    verify_canon_error_counts()
        

    #Note: This has to be done at the end since contact table is changed
    print("-----------------------------------------------------------------")
    print("15. Lifetime tests")
    print("-----------------------------------------------------------------")
    '''
    ###TBD REMOVE AFTER INITIAL TESTS
    cfe_time_met = tlm(f"{target} CFE_TIME_HK SECONDS_MET")
    pkt_recv_time = tlm(f"{target} CFE_TIME_HK RECEIVED_TIMESECONDS")
    new_stcf = int(pkt_recv_time - cfe_time_met)
    cmd(f"{target} CFE_TIME_CMD_SET_STCF with SECONDS {new_stcf}")
    cmd(f"{target} CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")
    ###########
    '''
    ## Set up receive only contact so bundle gets stored and not forwarded
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    
    load_new_table('/cf/contact_rx_only.tbl')
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    
    print(".................................................................")
    print("15.1 Lifetime Expired with no Age Block") #04090, 04110, 06172, 06174
    print("     Expected - BPLIB_BI_EXPIRED_BUNDLE_ERR: -241")
    print(".................................................................")
    
    ## Set bundle lifetime to 60s and creation time to 70s before now so bundle
    ## expires on arrival
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs()-70000, "sequence": 0}),
        lifetime=60000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    del_exp_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED")
    print(f"BUNDLE_COUNT_DELETED_EXPIRED: {del_exp_cnt}")

    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    # Verify BUNDLE_COUNT_DELETED_EXPIRED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED == {del_exp_cnt+1}", 6):
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.06172", "DTN.6.06174"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    
    '''
    TBD - Not in 7.0
    
    print(".................................................................")
    print("15.2 Node's maximum lifetime exceeded") #04070, 04314, 06172, 06176
    print(".................................................................")
    #TBD - DTNN-1346, DTNN-1349
        
    ## Load MIB PN Config table with short lifetime (60 sec)
    load_new_table('/cf/mib_short_life.tbl')
    
    ## Set bundle creation time 80 sec before now, so it expires on arrival
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs()-80000, "sequence": 0}),
        lifetime=3600000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    del_exp_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED")
    del_traf_pared_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_TRAFFIC_PARED")
    
    print(f"BUNDLE_COUNT_DELETED_EXPIRED: {del_exp_cnt}")

    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()
    
    # Verify BUNDLE_COUNT_DELETED_EXPIRED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED == {del_exp_cnt+1}", 6):
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.04070", "DTN.6.04314", "DTN.6.06172"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify BUNDLE_COUNT_DELETED_TRAFFIC_PARED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_TRAFFIC_PARED == {del_traf_pared_cnt+1}", 6):
        status = "P"
    else:
        status = "F"
        
    for rqmnt in ["DTN.6.06176"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    ## Restore MIB PN Config table 
    load_new_table('/cf/bpnode_mib_pn.tbl')
    

    print(".................................................................")
    print("15.3 Bundle Age >= Lifetime") #04100
    print("     Expected - BPLIB_BI_EXPIRED_BUNDLE_ERR: -241")
    print(".................................................................")
    primary_block_1 = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs(), "sequence": 0}),
        lifetime=600000,
        crc=CRCFlag.CALCULATE,
    )

    bundle_age_block = BundleAgeBlock(
        blk_type=BlockType.AUTO,
        blk_num=3,
        control_flags=BlockPCFlags.FRAG_REPLICATE | BlockPCFlags.DEL_UNPROC,
        crc_type=CRCType.CRC16_X25,
        bundle_age=600000, # same as lifetime
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    
    bundle_inv = Bundle(pri_block=primary_block, canon_blocks=[bundle_age_block, payload_block])
    data_sender.write(bundle_inv.to_bytes())
    
    status = verify_telem_and_event()

    # Verify canonical block error counts
    verify_canon_error_counts()

    for rqmnt in ["DTN.6.04100"]:
        TestUtils.set_requirement_status(rqmnt, status)
    '''
    '''
    TBD - Future
    print(".................................................................")
    print("15.4 Invalid Creation timestamp in the future") #04195
    print(".................................................................")
    #DTNN-1319
    primary_block_inv = PrimaryBlock(
        version=7,
        control_flags=BundlePCFlags.MUST_NOT_FRAGMENT,
        crc_type=CRCType.CRC16_X25,
        dest_eid=EID({"uri": 2, "ssp": {"node_num": dest_node, "service_num": dest_service}}),
        src_eid=EID({"uri": 2, "ssp": {"node_num": 101, "service_num": 1}}),
        rpt_eid=EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 1}}),
        creation_timestamp=CreationTimestamp({"time": DtnTimeNowMs()+3600000, "sequence": 0}),
        lifetime=10000,
        crc=CRCFlag.CALCULATE,
    )

    get_initial_counts()
    del_exp_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED")
    print(f"BUNDLE_COUNT_DELETED_EXPIRED: {del_exp_cnt}")

    bundle_inv = Bundle(pri_block=primary_block_inv, canon_blocks=[payload_block])
    data_sender.write(bundle_inv.to_bytes())
       
    # Verify BUNDLE_COUNT_DELETED_EXPIRED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED_EXPIRED == {del_exp_cnt+1}", 30):
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.04195"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    
    # Verify BUNDLE_COUNT_DELETED and BUNDLE_COUNT_DISCARDED
    if tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DELETED") == deleted_cnt+1:
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.04090", "DTN.6.04180", "DTN.6.04318", "DTN.6.06172"]:
        TestUtils.set_requirement_status(rqmnt, status)

    if tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_DISCARDED") == discarded_cnt+1:
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.04314"]:
        TestUtils.set_requirement_status(rqmnt, status)
    '''
    
    print("-----------------------------------------------------------------")
    print("20. Storage errors")
    print("-----------------------------------------------------------------")

    #21. no or too little space allocated for the bundle (or both?)
    #22. no or too little space left available for the bundle (or both?)
    #23. DST EID = Local EID (this is the destination) but an output queue to deliver the message does not exist (Maybe delete a message queue to an app? This will cause the system to be unstable). The system is configured to not allow storage in this case.
    #24. DST EID = Local EID (this is the destination) but an output queue to deliver the message does not exist (Maybe delete a message queue to an app? This will cause the system to be unstable). The system is configured to allow storage in this case.
    #25. Fragmented bundle
    

    print("-----------------------------------------------------------------")
    print("21. Contact Directive errors")
    print("-----------------------------------------------------------------")
    
    # Maximum number of contacts BPNode is configured for
    # $dtnfsw_globals_num_contacts = 1
    print(".................................................................")
    print(" 21.1 CONTACT_ID = 1")    
    print(".................................................................")
    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 1", TestUtils.INVALID_CMD_TYPE)   
    TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 1", TestUtils.INVALID_CMD_TYPE)
    
    '''
    print(".................................................................")
    print(" 21.2 Maximum number of simultaneous contacts exceeded ") #12282
    print(".................................................................")
    # Set paramSetMaxSimultaneousContacts to 1, try to set up contacts 0 and 1
    # TBD Build 7.1
    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    
    rqmnt = "DTN.6.12282"    
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.set_requirement_status(rqmnt, status)
    '''
    
    print("-----------------------------------------------------------------")
    print("22. Contact Directives sequencing errors")
    print("-----------------------------------------------------------------")

    ## Teardown contact
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    wait (1)
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    
    
    print(".................................................................")
    print("22.1 Invalid directives when contact not setup")
    print("     Verify START and STOP directives are rejected")
    print(".................................................................")

    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'TORNDOWN'", 6)
    
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    rqmnt = "DTN.6.15031"
    TestUtils.set_requirement_status(rqmnt, status)
    
    TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    
    # Another TEARDOWN command should do no harm
    TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0") 
    # BPLIB_CLA_CONTACT_NO_STATE_CHG_DBG_EID 661
        
   
    print(".................................................................")
    print("22.2 Invalid directives when contact is setup")
    print("     Verify STOP directive is rejected")
    print(".................................................................")

    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")         
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'SETUP'", 6)

    TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    
    # Another SETUP command should do no harm
    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    # BPLIB_CLA_CONTACT_NO_STATE_CHG_DBG_EID 661 
    
    
    print(".................................................................")
    print("22.3 Invalid directives when contact has started")
    print("     Verify SETUP and TEARDOWN directives are rejected")
    print(".................................................................")

    TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0")     
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'STARTED'", 6)

    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    
    # Another START command should do no harm
    TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0") 
    # BPLIB_CLA_CONTACT_NO_STATE_CHG_DBG_EID 661
    
    
    print(".................................................................")
    print("22.4 Invalid directives when contact has stopped")
    print("     Verify SETUP directive is rejected")
    print(".................................................................")
    
    TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")     
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'STOPPED'", 6)

    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0", TestUtils.INVALID_CMD_TYPE)
    
    # Another STOP command should do no harm
    TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    # BPLIB_CLA_CONTACT_NO_STATE_CHG_DBG_EID 661
    

    #Teardown contact
    TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
            

    print(".................................................................")
    print("22.5 Duplicate dest EID - valid") #04321
    print(".................................................................")
    load_new_table("/cf/cont_dup_dest.tbl") 
    
    rqmnt = "DTN.6.04321"
    status = "P"
    #DTN.6.04321	Upon receiving a contact-setup request from AA Node Configuration, if the given set of destination EIDs for an "output queue" contains duplicate destination EIDs, BPA Storage shall issue a warning event.
    if TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0") == "F" or \
       TestUtils.verify_event("BPNODE", 512, "INFO") == "F": #EID TBD DTNN-1348
        status = "F"
        
    TestUtils.set_requirement_status(rqmnt, status)
    
    ##TBD
    #DTN.6.04340 If a query selects a destination already selected by another query, BPA Storage shall reply with an error indication and fail to create the output queue.
    #Setting invalid IPNSSPFORMAT for destination EID correctly fails validation for channel table, but not for contact table DTNN-1304
    
    print("-----------------------------------------------------------------")
    print("23. Contacts table errors")
    print("-----------------------------------------------------------------")
    rqmnt = "DTN.6.13071"

    print(".................................................................")
    print("23.1 Invalid dest EID (max<min)")
    print(".................................................................")
    status = TestUtils.validate_invalid_table("/cf/cont_inv_dest.tbl") 
    TestUtils.set_requirement_status(rqmnt, status)
    
    # Verify contact continues to perform normally since invalid table was not loaded
    TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0") 

   
    print("-----------------------------------------------------------------")
    print("24. MIB PN table errors")
    print("-----------------------------------------------------------------")
    rqmnt = "DTN.6.13081"
    
    print(".................................................................")
    print("24.1 Bundle length 0")
    print(".................................................................")
    status = TestUtils.validate_invalid_table('/cf/mib_pn_blen_0.tbl') 
    TestUtils.set_requirement_status(rqmnt, status)

    print(".................................................................")
    print("24.2 Payload length > Bundle length")
    print(".................................................................")
    status = TestUtils.validate_invalid_table('/cf/mib_pn_invplen.tbl') 
    TestUtils.set_requirement_status(rqmnt, status)
    

    print("=================================================================")
    print("30. Reset Counter Directives")
    print("=================================================================")
    
    print("-----------------------------------------------------------")
    print(" 30.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_COUNT_DISCARDED")
    TestUtils.reset_counter("BUNDLE_COUNT_DELETED")
    TestUtils.reset_counter("BUNDLE_COUNT_DELETED_UNINTELLIGIBLE")
    TestUtils.reset_counter("BUNDLE_COUNT_DELETED_HOP_EXCEEDED")

    print("-----------------------------------------------------------")
    print(" 30.2 Reset Counters - ERROR/BUNDLE")
    print("-----------------------------------------------------------")

    TestUtils.reset_counters("ERROR")
    TestUtils.reset_counters("BUNDLE")

    ###################################################################

    ##=================================================================
    ## Print requirement status
    ##=================================================================
    
    print ("******************************")
    print ("***** Requirement Status *****")
    print ("******************************")
    for key, value in rqmnt_status.items():
        print(f"***    {key}: {value}")
    print ("******************************")
    
    ###################################################################
    
# main
#bundle_ingress_errors("xxx")