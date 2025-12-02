def bundle_creation_nominal(self):
    
    ###################################################################
    ## Bundle creation test - nominal
    ##
    ## K. Narayanan
    ###################################################################
    
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
    load_utility ("DTNFSW-1/procedures/load_new_table.py")
        
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        "DTN.ES.04110":"U", 
        "DTN.ES.04120":"U", 
        "DTN.ES.04220":"U", 
        "DTN.ES.04230":"U", 
        "DTN.6.01000":"U", 
        "DTN.6.01070":"U", 
        "DTN.6.02000":"U", 
        "DTN.6.02100":"U", 
        "DTN.6.02200":"U", 
        "DTN.6.02290":"U", 
        "DTN.6.02402":"U", 
        "DTN.6.02410":"U", 
        "DTN.6.02420":"U", 
        "DTN.6.03151":"U", 
        "DTN.6.04280":"U", 
        "DTN.6.04290":"U", 
        "DTN.6.04316":"U", 
        #"DTN.6.04317":"U",
        "DTN.6.04630":"U", 
        "DTN.6.05200":"U", 
        "DTN.6.05210":"U", 
        "DTN.6.05220":"U", 
        "DTN.6.07030":"U", 
        "DTN.6.08491":"U", 
        #"DTN.6.08492":"U",
        #"DTN.6.12260":"U",
        #"DTN.6.12262":"U",
        "DTN.6.12440":"U", 
        "DTN.6.12450":"U", 
        "DTN.6.12453":"U", 
        "DTN.6.12460":"U", 
        "DTN.6.12642":"U", 
        "DTN.6.12645":"U", 
        "DTN.6.12660":"U", 
        "DTN.6.12670":"U", 
        #"DTN.6.12710":"U",
        "DTN.6.12971":"U", 
        "DTN.6.13051":"U", 
        "DTN.6.13091":"U", 
        "DTN.6.19000":"U", 
        "DTN.6.19010":"U", 
        "DTN.6.19020":"U", 
        "DTN.6.19030":"U", 
        #"DTN.6.19040":"U", 
        "DTN.6.19180":"U", 
        "DTN.6.19190":"U", 
        "DTN.6.19210":"U", 
        "DTN.6.19230":"U", 
        "DTN.6.19360":"U", 
        "DTN.6.19390":"U", 
        "DTN.6.19410":"U", 
        #"DTN.6.20115":"U", 
        "DTN.6.23090":"U",
        "DTN.6.25010":"U",
        
        #Bundle construction
        "DTN.6.02030":"U", 
        "DTN.6.02050":"U", 
        "DTN.6.02340":"U",
        "DTN.6.02350":"U",
        "DTN.6.02360":"U",
        "DTN.6.02370":"U",
        "DTN.6.02080":"U",
        "DTN.6.02110":"U",
        "DTN.6.02210":"U",
        "DTN.6.02180":"U",
        "DTN.6.02190":"U",
        "DTN.6.02130":"U",
        "DTN.6.02140":"U",
        "DTN.6.02310":"U", 
        "DTN.6.02331":"U", 
        "DTN.6.02332":"U", 
        "DTN.6.02333":"U", 
        "DTN.6.02334":"U", 
        "DTN.6.02335":"U",
        "DTN.6.02270":"U", 
        "DTN.6.02020":"U", 
        "DTN.6.02400":"U", 
        "DTN.6.02402":"U", 
        #"DTN.6.02403":"U",
        "DTN.6.02305":"U", 
        "DTN.6.02336":"U",
        "DTN.6.06390":"U",
        "DTN.6.12470":"U", 
        "DTN.6.12480":"U", 
        #"DTN.6.12490":"U", 
        "DTN.6.12540":"U", 
        "DTN.6.12541":"U", 
        "DTN.6.12542":"U", 
        "DTN.6.12510":"U", 
        "DTN.6.12560":"U", 
        "DTN.6.12570":"U", 
        "DTN.6.12580":"U", 
        "DTN.6.12590":"U", 
        "DTN.6.17010":"U",  
        "DTN.6.23061":"U",  
        "DTN.6.23062":"U",  
        "DTN.6.23063":"U",  
        "DTN.6.26000":"U", 
        "DTN.6.26040":"I", 
        "DTN.6.26050":"U", 
        "DTN.6.26070":"U", 
        "DTN.6.27000":"U", 
        #"DTN.6.27010":"U", 
        #"DTN.6.27020":"U",
        "DTN.6.27040":"U", 
        "DTN.6.27050":"U", 
        "DTN.6.27060":"U", 
        "DTN.6.27090":"U", 
        "DTN.6.27100":"U", 
        "DTN.6.27110":"U", 
        "DTN.6.27120":"U", 
        "DTN.6.27130":"U", 
        "DTN.6.27140":"U", 
        "DTN.6.27150":"U", 
        "DTN.6.27160":"U", 
        "DTN.6.27170":"U", 
        "DTN.6.27180":"U", 
        "DTN.6.27190":"U", 
        "DTN.6.27200":"U", 
        "DTN.6.27210":"U", 
        "DTN.6.27230":"U", 
        "DTN.6.27240":"U", 
        "DTN.6.27250":"U", 
        "DTN.6.27260":"U", 
        "DTN.6.27300":"U", 
        "DTN.6.27310":"U", 
        "DTN.6.27320":"U", 
        "DTN.6.27330":"U", 
        "DTN.6.27340":"U", 
        "DTN.6.27350":"U", 
        "DTN.6.27360":"U", 
        "DTN.6.27370":"U", 
        "DTN.6.27380":"U", 
        "DTN.6.27390":"U", 
        "DTN.6.27400":"U", 
        "DTN.6.27420":"U", 
        "DTN.6.27430":"U", 
        "DTN.6.27440":"U", 
        "DTN.6.27470":"U", 
        "DTN.6.27480":"U", 
        "DTN.6.27490":"U", 
        "DTN.6.27500":"U", 
        "DTN.6.27510":"U", 
        "DTN.6.27520":"U", 
        "DTN.6.27530":"U", 

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
    
    mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK"
    mib_reports_pkt = "BPNODE_NODE_MIB_REPORTS_HK"
    chan_stat_pkt = "BPNODE_CHAN_CON_STAT_HK"
    
    ##=========================================================================
    ## send_adu_and_verify_bundle
    ##=========================================================================
    def send_adu_and_verify_bundle():
        """
        - Channel 0 already started
        - Start contact so stored bundles are forwarded (assume these work)
        - Send ADU - CFE_SB_CMD_SEND_SB_STATS command
        - Capture bundle with DTNGen data receiver
        - Verify bundle construction
        """
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
        from dtntools.dtngen.utils import DtnTimeNowMs
        load_utility ("DTNFSW-1/procedures/bpnode_check_eid_equality.py")
        load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
        load_utility ("DTNFSW-1/procedures/load_new_table.py")
        
        # Port / Address Configs
        LOCALHOST_RX = "0.0.0.0"
        PORT_NUM_RX = 4551

        LOCAL_NODE_EID = EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 0}})
        
        # Connect data receiver
        data_receiver = UdpRxSocket(LOCALHOST_RX, PORT_NUM_RX)
        data_receiver.connect()
       
        ##--------------------------------------------------------
        ## Start receive only contact so bundles are stored 
        ## Send ADU - CFE_SB_CMD_SEND_SB_STATS command
        ##--------------------------------------------------------        
        
        # Load receive only contacts table
        load_new_table('/cf/contact_rx_only.tbl')
        
        status = "P"
        for rqmnt in ["DTN.ES.04110", "DTN.ES.04120"]:
            TestUtils.set_requirement_status(rqmnt, status)

        # Setup and start contact
        cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
        wait(2)
        cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")    
        
        # Send ADU command
        dtn_time_now_ms = DtnTimeNowMs()  # for bundle timestamp verification later
        cmd("DTNFSW-1 CFE_SB_CMD_SEND_SB_STATS")
        
        wait_packet(target, mib_counts_pkt, 2, 10)
        
        gen_accept_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_GENERATED_ACCEPTED")
        stored_cnt = tlm(f"{target} {mib_reports_pkt} BUNDLE_COUNT_STORED")
        adu_cnt = tlm(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED")
        
        # Verify BUNDLE_COUNT_GENERATED_ACCEPTED
        if gen_accept_cnt == 1:
            print(f"BUNDLE_COUNT_GENERATED_ACCEPTED: {gen_accept_cnt} as expected")
            status = "P"
        else:
            print(f"ERROR - BUNDLE_COUNT_GENERATED_ACCEPTED {gen_accept_cnt} not as expected")
            status = "F"

        for rqmnt in ["DTN.6.08491", "DTN.ES.04220"]:
            TestUtils.set_requirement_status(rqmnt, status)

        # Verify BUNDLE_COUNT_GENERATED_ACCEPTED[SOURCE] #TBD
        #for rqmnt in ["DTN.6.08492"]:
        #    TestUtils.set_requirement_status(rqmnt, status)

        # Verify BUNDLE_COUNT_STORED
        if stored_cnt == 1:
            print(f"BUNDLE_COUNT_STORED: {stored_cnt} as expected")
            status = "P"
        else:
            print(f"ERROR - BUNDLE_COUNT_STORED {stored_cnt} not as expected")
            status = "F"

        for rqmnt in [
            "DTN.6.02410", "DTN.6.02420", "DTN.6.04280", "DTN.6.04290", 
            "DTN.6.04316", "DTN.6.23090", "DTN.6.25010"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

        # Verify ADU_COUNT_RECEIVED
        if adu_cnt == 1:
            print(f"ADU_COUNT_RECEIVED: {adu_cnt} as expected")
            status = "P"
        else:
            print(f"ERROR - ADU_COUNT_RECEIVED: {adu_cnt} not as expected")
            status = "F"
        
        for rqmnt in [
            "DTN.6.01000", "DTN.6.02000", "DTN.6.02402", "DTN.6.05200", 
            "DTN.6.05210", "DTN.6.05220", "DTN.6.12450", "DTN.6.12642", 
            "DTN.6.12971", "DTN.6.19000", "DTN.6.19010", "DTN.6.19020", 
            "DTN.6.19230"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)
            
        
        # Stop and Teardown contact
        cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
        wait(2)
        cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
        wait(2)

        ##------------------------------------------------------
        ## Start nominal contact so stored bundles are forwarded 
        ##------------------------------------------------------        
        # Load nominal contacts table
        load_new_table('/cf/contact_nominal.tbl')

        # Setup and start contact
        cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
        wait(2)
        cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")    
        
        ##-------------------------------
        ## Receive bundle data and verify
        ##-------------------------------
        print("Waiting for bundle to be returned...")
        received_bundle = Bundle.from_bytes(data_receiver.read())
        print(f"Received bundle: {received_bundle.to_json()}")
        
        status = "P"
        check_expression(f"{received_bundle.pri_block.version} == 7")
        for rqmnt in ["DTN.6.02030", "DTN.6.02370"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.pri_block.control_flags} == 4")     
        for rqmnt in ["DTN.6.02050", "DTN.6.02080", "DTN.6.12510"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.pri_block.crc_type} == {CRCType.CRC16_X25}")     
        for rqmnt in ["DTN.6.02110", "DTN.6.12540"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.pri_block.is_crc_valid()}")
        
        check_expression(f"{received_bundle.pri_block.lifetime} == 3600000")
        for rqmnt in ["DTN.6.02200", "DTN.6.02210", "DTN.6.12542"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.pri_block.creation_timestamp.time} > {dtn_time_now_ms}")    
        check_expression(f"{received_bundle.pri_block.creation_timestamp.time} < {dtn_time_now_ms+20000}")
        for rqmnt in ["DTN.6.17010", "DTN.6.02180", "DTN.6.23061"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.pri_block.creation_timestamp.sequence} == 0")
        rqmnt="DTN.6.02190"
        TestUtils.set_requirement_status(rqmnt, status)
            
        
        # These EIDs are set via channel / bplib configurations
        
        expected_dest_eid = EID({"uri": 2, "ssp": {"node_num": 200, "service_num": 64}})
        expected_src_eid = EID({"uri": 2, "ssp": {"node_num": 100, "service_num": 42}})
        expected_rpt_eid = EID({"uri": 1, "ssp": "dtn:none"})
        
        bpnode_check_eid_equality(received_bundle.pri_block.dest_eid, expected_dest_eid) 
        for rqmnt in ["DTN.6.02130", "DTN.6.12480", "DTN.6.26050"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        bpnode_check_eid_equality(received_bundle.pri_block.src_eid, expected_src_eid) 
        for rqmnt in ["DTN.6.02140", "DTN.6.12470", "DTN.6.26050"]:
            TestUtils.set_requirement_status(rqmnt, status)
        #TBD - DTNGen issue per integration_test_bpnode_auto_adu_ingest
        #bpnode_check_eid_equality(received_bundle.pri_block.rpt_eid, expected_rpt_eid)
        

        exp_new_hop_count = 1
        exp_new_hop_limit = 10

        # Note: See the default channel config table for the source of these configs

        print("Checking canonical blocks ...")
        check_expression(f"{len(received_bundle.canon_blocks)} == 4")

        print("Checking prev node block")
        check_expression(f"{received_bundle.canon_blocks[0].blk_type} == {BlockType.PREVIOUS_NODE}")
        check_expression(f"{received_bundle.canon_blocks[0].blk_num} == {2}")
        for rqmnt in ["DTN.6.02340", "DTN.6.12570"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[0].control_flags} == {0}")
        for rqmnt in ["DTN.6.02310", "DTN.6.12560"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[0].crc_type} == {CRCType.CRC16_X25}")
        for rqmnt in ["DTN.6.02100", "DTN.6.02290", "DTN.6.12541"]:
            TestUtils.set_requirement_status(rqmnt, status)
            
        check_expression(f"{received_bundle.canon_blocks[0].is_crc_valid()}")        
        for rqmnt in ["DTN.6.26070"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        bpnode_check_eid_equality(received_bundle.canon_blocks[0].prev_eid, LOCAL_NODE_EID)
        for rqmnt in ["DTN.6.02335", "DTN.6.07030"]:
            TestUtils.set_requirement_status(rqmnt, status)

        print("Checking age block")
        check_expression(f"{received_bundle.canon_blocks[1].blk_type} == {BlockType.BUNDLE_AGE}")
        check_expression(f"{received_bundle.canon_blocks[1].blk_num} == {3}")
        for rqmnt in ["DTN.6.02350", "DTN.6.12590"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[1].control_flags} == {0}")
        for rqmnt in ["DTN.6.02310", "DTN.6.12560"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[1].crc_type} == {CRCType.CRC16_X25}")
        for rqmnt in ["DTN.6.02290", "DTN.6.12541"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[1].is_crc_valid()}")
        for rqmnt in ["DTN.6.26070"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[1].bundle_age} > {0}")
        check_expression(f"{received_bundle.canon_blocks[1].bundle_age} < {60000}")
        
        for rqmnt in ["DTN.6.02333", "DTN.6.23062"]:
            TestUtils.set_requirement_status(rqmnt, status)

        print("Checking hop count block")
        check_expression(f"{received_bundle.canon_blocks[2].blk_type} == {BlockType.HOP_COUNT}")
        check_expression(f"{received_bundle.canon_blocks[2].blk_num} == {4}")
        for rqmnt in ["DTN.6.02350", "DTN.6.12580"]:
            TestUtils.set_requirement_status(rqmnt, status)

        check_expression(f"{received_bundle.canon_blocks[2].control_flags} == {0}")
        for rqmnt in ["DTN.6.02310", "DTN.6.12560"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[2].crc_type} == {CRCType.CRC16_X25}")
        for rqmnt in ["DTN.6.02290", "DTN.6.12541"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[2].is_crc_valid()}")
        for rqmnt in ["DTN.6.26070"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_limit} == {exp_new_hop_limit}")
        check_expression(f"{received_bundle.canon_blocks[2].hop_data.hop_count} == {exp_new_hop_count}")
        
        for rqmnt in ["DTN.6.02334", "DTN.6.06390"]:
            TestUtils.set_requirement_status(rqmnt, status)

        for rqmnt in ["DTN.6.02331", "DTN.6.02332"]:
            TestUtils.set_requirement_status(rqmnt, status)

        print("Checking payload block")
        check_expression(f"{received_bundle.canon_blocks[3].blk_type} == {BlockType.BUNDLE_PAYLOAD}")
        check_expression(f"{received_bundle.canon_blocks[3].blk_num} == {1}")

        check_expression(f"{received_bundle.canon_blocks[3].control_flags} == {0}")
        for rqmnt in ["DTN.6.12560"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        check_expression(f"{received_bundle.canon_blocks[3].crc_type} == {CRCType.CRC16_X25}")
        for rqmnt in ["DTN.6.12541"]:
            TestUtils.set_requirement_status(rqmnt, status)

        check_expression(f"{received_bundle.canon_blocks[3].is_crc_valid()}")
        for rqmnt in ["DTN.6.26070"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        for rqmnt in ["DTN.6.02270", "DTN.6.02020", "DTN.6.02400", "DTN.6.02402", 
                      "DTN.6.02305", "DTN.6.02336"]:
            TestUtils.set_requirement_status(rqmnt, status)
        
        ## CBOR encoding requirements
        for rqmnt in [
            "DTN.6.26000", 
            "DTN.6.27000", "DTN.6.27040", "DTN.6.27050", "DTN.6.27060", "DTN.6.27090", 
            "DTN.6.27100", "DTN.6.27110", "DTN.6.27120", "DTN.6.27130", "DTN.6.27140", 
            "DTN.6.27150", "DTN.6.27160", "DTN.6.27170", "DTN.6.27180", "DTN.6.27190", 
            "DTN.6.27200", "DTN.6.27210", "DTN.6.27230", "DTN.6.27240", "DTN.6.27250", 
            "DTN.6.27260", 
            "DTN.6.27300", "DTN.6.27310", "DTN.6.27320", "DTN.6.27330", "DTN.6.27340", 
            "DTN.6.27350", "DTN.6.27360", "DTN.6.27370", "DTN.6.27380", "DTN.6.27390", 
            "DTN.6.27400", "DTN.6.27420", "DTN.6.27430", "DTN.6.27440", "DTN.6.27470", 
            "DTN.6.27480", "DTN.6.27490", 
            "DTN.6.27500", "DTN.6.27510", "DTN.6.27520", "DTN.6.27530"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)
                

        print("-------------------------------------------------------------")
        print("Verify Age block creation with creation timestamp zero") #23063
        print("-------------------------------------------------------------")
        '''
        NOTE:
        At this time this works only if bundles don't get stored
        With storage bundle expires right away
        '''

        # Set CFS time INVALID - so creation timestamp is zero
        cmd("DTNFSW-1 CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'INVALID'")
        
        # Load channel table with no extension blocks
        load_new_table('/cf/chan_no_extblk.tbl')    

        # Send ADU command
        cmd("DTNFSW-1 CFE_SB_CMD_SEND_SB_STATS")
        
        # Receive bundle data and verify age block is added
        print("Waiting for bundle to be returned...")
        received_bundle = Bundle.from_bytes(data_receiver.read())
        print(f"Received bundle: {received_bundle.to_json()}")
       
        print("Checking canonical blocks ...")
        check_expression(f"{len(received_bundle.canon_blocks)} == 2")
        for rqmnt in [
            "DTN.6.02340", "DTN.6.02350", "DTN.6.02360", "DTN.6.12570", "DTN.6.12580"
            ]:
            TestUtils.set_requirement_status(rqmnt, status)

        bundle_type = received_bundle.canon_blocks[0].blk_type
        bundle_age = received_bundle.canon_blocks[0].bundle_age
        print(f"Block type: {bundle_type} Bundle Age: {bundle_age}")
        if bundle_type == BlockType.BUNDLE_AGE and bundle_age < 5000:
            status = "P" 
        else:
            print(f"ERROR - Age block not as expected")
            status = "F"
        
        for rqmnt in ["DTN.6.02360", "DTN.6.12590", "DTN.6.23063"]:
            TestUtils.set_requirement_status(rqmnt, status)

        # Set CFS time VALID
        cmd("DTNFSW-1 CFE_TIME_CMD_SET_STATE with CLOCK_STATE 'VALID'")
        
        # Disconnect data receiver
        data_receiver.disconnect()

        # Terminate contact
        cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
        wait(2)
        cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
        wait(6)
        
    ##=========================================================================

    ### main

    # APP states (reported in events)
    # BPA_ADUP_APP_REMOVED = 0
    # BPA_ADUP_APP_STOPPED = 1
    # BPA_ADUP_APP_ADDED   = 2
    # BPA_ADUP_APP_STARTED = 3

    # Event ID
    ADD_APP_EID    = 503
    REMOVE_APP_EID = 525
    START_APP_EID  = 536
    STOP_APP_EID   = 537
    
    ## Copy needed test tables from COSMOS to DTNFSW
    prompt("Copy these tables to FSW build/exe/cpu1/cf folder:\n\n" + 
           " - contact_rx_only.tbl\n" +
           " - contact_nominal.tbl\n" 
           " - chan_no_extblk.tbl" 
          )
    
    cmd("DTNFSW-1 BPNODE_CMD_RESET_ALL_COUNTERS") # assume this works for now
    
    ###################################################################

    # See def integration_test_verify_evs_msg(app_name, evt_id, evt_type):
    #   Check if the last EVS Event Message that was received matches the expected
    
    #TestUtils.verify_event("BPNODE", START_APP_EID, "INFO")
    
    ##=================================================================
    ## Load Configuration
    ##=================================================================
    #1.	Load Storage Configuration table.    
    if tlm(f"{target} BPNODE_NODE_MIB_CONFIG_HK PARAM_SET_MAX_SEQUENCE_NUM") == 100000 and \
       tlm(f"{target} BPNODE_NODE_MIB_CONFIG_HK PARAM_BUNDLE_SIZE_NO_FRAGMENT") == 8192 and \
       tlm(f"{target} BPNODE_NODE_MIB_CONFIG_HK PARAM_SET_MAX_PAYLOAD_LENGTH") == 4096 and \
       tlm(f"{target} BPNODE_NODE_MIB_CONFIG_HK PARAM_SET_MAX_BUNDLE_LENGTH") == 8192:
        status = "P"
    else:
        status = "F"
    
    for rqmnt in ["DTN.6.04630", "DTN.6.13091", "DTN.6.19360", "DTN.6.19390", "DTN.6.19410"]:
        TestUtils.set_requirement_status(rqmnt, status)
            
    
    #************************************************************************
    ## Default ADU Proxy Configuration
    '''
    - Two applications, #1 added and started, #2 removed
    '''
    #************************************************************************
    
    print("=================================================================")
    print("1. Applications/channels initial configuration")
    print("=================================================================")
    
    print("---------------------------------------------------------")
    print("1.1 Verify Application 1 initial state is ACTIVE/STARTED")
    print("---------------------------------------------------------")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_0") == 'ACTIVE'  and \
       tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0")     == 'STARTED':
        status = "P"
    else:
        wait() # for analysis
        
        print("ERROR - CHAN_STAT_REG_STATE_0/CHAN_STAT_STATE_0: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_0") + "/" + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") + \
            " - Expected ACTIVE/STARTED")
        status = "F"
        wait
    
    # Set requirement status
    for rqmnt in ["DTN.6.12440", "DTN.6.12453", "DTN.6.12460"]:
        TestUtils.set_requirement_status(rqmnt, status)
    

    print("---------------------------------------------------------")
    print("1.2 Verify Application 2 initial state is ACTIVE/REMOVED")
    print("---------------------------------------------------------")
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_1") == 'ACTIVE'  and \
       tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1")     == 'REMOVED':
        status = "P"
    else:
        wait() # for analysis
        
        print("ERROR - CHAN_STAT_REG_STATE_1/CHAN_STAT_STATE_1: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_1") + "/" + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") + \
            " - Expected ACTIVE/REMOVED")
        status = "F"

    # Set default channel configuration requirements status
    for rqmnt in ["DTN.6.03151", "DTN.6.13051", "DTN.6.19360", "DTN.6.19390", "DTN.6.19410"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    #************************************************************************
        
    print("=================================================================")
    print("2. Application 1 test - Channel 0 (Added and Started at startup)")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print("2.1 Send ADU and verify bundle generation and construction")
    print("-----------------------------------------------------------")
    send_adu_and_verify_bundle()
    
        
    print("-----------------------------------------------------------")
    print("2.2 Stop Injecting ADUs (Stop Application)")
    print("    Verify State = STOPPED")
    print("-----------------------------------------------------------")
    TestUtils.send_command("BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
    TestUtils.verify_event("BPNODE", STOP_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") == 'STOPPED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_STATE_0: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") + " - Expected STOPPED")
        status = "F"
        
        wait
    
    # Set requirement status
    for rqmnt in ["DTN.6.12645", "DTN.6.19030", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    #************************************************************************
    
    print("=================================================================")
    print("3. Reset Counters Directives")
    print("=================================================================")
    
    print("-----------------------------------------------------------")
    print("3.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    TestUtils.reset_counter("ADU_COUNT_RECEIVED")
    TestUtils.reset_counter("BUNDLE_COUNT_GENERATED_ACCEPTED")
    
    print("-----------------------------------------------------------")
    print("3.2 RESET_ALL_COUNTERS")
    print("-----------------------------------------------------------")
    TestUtils.reset_counters("ALL")
  
    #************************************************************************
    
    print("=================================================================")
    print("4. Application 2 test")
    print("=================================================================")    
    
    print("-----------------------------------------------------------")
    print("4.1 Add Application 2 - channel 1")
    print("    Verify State = ADDED")
    print("-----------------------------------------------------------")
    status = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", ADD_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_1") == 'ACTIVE' and \
       tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1")     == 'ADDED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_REG_STATE_1/CHAN_STAT_STATE_1: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_REG_STATE_1") + "/" + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") + " - Expected ACTIVE/ADDED")
        status = "F"
        
        wait
    
    # Set requirement status
    for rqmnt in ["DTN.6.12440", "DTN.6.12453", "DTN.6.12460", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("Sending Add Application 2 directive again - should be accepted")
    status = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", ADD_APP_EID, "INFO")


    print("-----------------------------------------------------------")
    print("4.2 Start Application 2")
    print("    Verify State = STARTED" )
    print("-----------------------------------------------------------")
    TestUtils.send_command("BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", START_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") != 'STARTED':
        print("ERROR - CHAN_STAT_STATE_1 not STARTED")
        wait()    
    
    # Save initial counts
    adu_recd_cnt = tlm(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED")
    gen_accept_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_GENERATED_ACCEPTED")
    
    print("Sending ADU for Application 2 - CFE_TIME_DIAG_HK (0x806)")
    cmd(f"{target} CFE_TIME_CMD_SEND_DIAGNOSTIC") # chan 1
    
    # Verify ADU_COUNT_RECEIVED
    if wait(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED == {adu_recd_cnt}+1", 6):
        print("ADU_COUNT_RECEIVED incremented as expected")
        status = "P"
    else:
        print("ERROR - ADU_COUNT_RECEIVED did not increment as expected")
        status = "F"
    
    # Set requirement status
    for rqmnt in [
        "DTN.6.02000", "DTN.6.02402", "DTN.6.05210", "DTN.6.05220", "DTN.6.12642", \
        "DTN.6.12971", "DTN.6.19020", "DTN.6.19210", "DTN.6.19230"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    # Verify BUNDLE_COUNT_GENERATED_ACCEPTED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_GENERATED_ACCEPTED == {gen_accept_cnt}+1", 6):
        print(f"BUNDLE_COUNT_GENERATED_ACCEPTED: {gen_accept_cnt} as expected")
        status = "P"
    else:
        print(f"ERROR - BUNDLE_COUNT_GENERATED_ACCEPTED {gen_accept_cnt} not as expected")
        status = "F"

    for rqmnt in ["DTN.6.08491"]:
        TestUtils.set_requirement_status(rqmnt, status)


    print("-----------------------------------------------------------")
    print("4.3 Stop application 2")
    print("    Verify State = STOPPED")
    print("-----------------------------------------------------------")
    TestUtils.send_command("BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", STOP_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'STOPPED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_REG_STATE_1 not STOPPED")
        status = "F"
        
        wait
    
    # Set requirement status *** TBD ***
    for rqmnt in ["DTN.6.12645", "DTN.6.19030", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print("-----------------------------------------------------------")
    print("4.4 Verify no ADU receipt in STOPPED state")
    print("-----------------------------------------------------------")
    adu_recd_cnt = tlm(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED")

    print("Sending ADU for Application 2 - CFE_TIME_DIAG_HK (0x806)")
    cmd(f"{target} CFE_TIME_CMD_SEND_DIAGNOSTIC")
    wait(6)
    
    item_name = "ADU_COUNT_RECEIVED"
    exp_val = adu_recd_cnt
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in ["DTN.ES.04230"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print("-----------------------------------------------------------")
    print("4.5 Start Application 2 from STOPPED state")
    print("    Verify State = STARTED")
    print("-----------------------------------------------------------")
    TestUtils.send_command("BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", START_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") != 'STARTED':
        print("ERROR - CHAN_STAT_STATE_1 not STARTED")
        status = "F"
        
        wait
        
        rqmnt = "DTN.6.19090"
        TestUtils.set_requirement_status(rqmnt, status)
    
    # Stop Application 2
    cmd(f"{target} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STOPPED'", 6)
    
    #************************************************************************
    
    print("=================================================================")
    print("5. All Applications active simultaneously")
    print("=================================================================")
    
    print("-----------------------------------------------------------")
    print("5.1 Start both Applications")
    print("-----------------------------------------------------------")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0 == 'STOPPED'", 6)
    cmd(f"{target} BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
    
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STOPPED'", 6)
    cmd(f"{target} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0 == 'STARTED'", 6)
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STARTED'", 6)
    
    print("-----------------------------------------------------------")
    print("5.2 Send ADUs for both Applications")
    print("-----------------------------------------------------------")    
    # Save initial counts
    adu_recd_cnt = tlm(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED")
    gen_accept_cnt = tlm(f"{target} {mib_counts_pkt} BUNDLE_COUNT_GENERATED_ACCEPTED")
    #stored_cnt = tlm(f"{target} {mib_reports_pkt} BUNDLE_COUNT_STORED")
    
    cmd(f"{target} CFE_SB_CMD_SEND_SB_STATS")
    cmd(f"{target} CFE_TIME_CMD_SEND_DIAGNOSTIC")
    
    # Verify ADU_COUNT_RECEIVED
    if wait(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED == {adu_recd_cnt}+2", 6):
        print("ADU_COUNT_RECEIVED incremented as expected")
        status = "P"
    else:
        print("ERROR - ADU_COUNT_RECEIVED did not increment as expected")
        status = "F"
    
    # Set requirement status
    for rqmnt in ["DTN.6.19230"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
        
    # Verify BUNDLE_COUNT_GENERATED_ACCEPTED
    if wait(f"{target} {mib_counts_pkt} BUNDLE_COUNT_GENERATED_ACCEPTED == {gen_accept_cnt}+2", 6):
        print(f"BUNDLE_COUNT_GENERATED_ACCEPTED: {gen_accept_cnt} as expected")
        status = "P"
    else:
        print(f"ERROR - BUNDLE_COUNT_GENERATED_ACCEPTED {gen_accept_cnt} not as expected")
        status = "F"

    for rqmnt in ["DTN.6.08491"]:
        TestUtils.set_requirement_status(rqmnt, status)

        
    print("-----------------------------------------------------------")
    print("5.3 Stop both Applications")
    print("-----------------------------------------------------------")
    cmd(f"{target} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 0")
    cmd(f"{target} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")

    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0 == 'STOPPED'", 6)
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STOPPED'", 6)
    
    #************************************************************************

    print("=================================================================")
    print("6. Remove all Applications")
    print("=================================================================")
    
    print("-----------------------------------------------------------")
    print("6.1 Remove application 1")
    print("    Verify State = REMOVED and event message")
    print("-----------------------------------------------------------")
        
    print("-------------------------")
    print(" Remove application 1")
    print("-------------------------")
    status = TestUtils.send_command("BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
    TestUtils.verify_event("BPNODE", REMOVE_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") == 'REMOVED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_REG_STATE_0 not REMOVED")
        status = "F"        
    
    # Set requirement status 
    for rqmnt in ["DTN.6.01070", "DTN.6.12660", "DTN.6.12670", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    # Send Remove application 1 directive again - should be accepted
    status = TestUtils.send_command("BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 0")
    TestUtils.verify_event("BPNODE", REMOVE_APP_EID, "INFO")

    print("-----------------------------------------------------------")
    print("6.2 Remove application 2")
    print("    Verify State = REMOVED and event message")
    print("-----------------------------------------------------------")
        
    print("-------------------------")
    print(" Remove application 2")
    print("-------------------------")
    status = TestUtils.send_command("BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", REMOVE_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'REMOVED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_REG_STATE_1 not REMOVED")
        status = "F"        
    
    # Set requirement status 
    for rqmnt in ["DTN.6.01070", "DTN.6.12660", "DTN.6.12670", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print("-----------------------------------------------------------")
    print("6.3 Add application 2 in REMOVED state")
    print("-----------------------------------------------------------")
    status = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", ADD_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'ADDED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_STATE_0: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") + " - Expected ADDED")
        status = "F"
    
    # Set requirement status
    for rqmnt in ["DTN.6.12440", "DTN.6.12453", "DTN.6.12460"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    print("-----------------------------------------------------------")
    print("6.4 Remove application 2 in ADDED state")
    print("-----------------------------------------------------------")
    status = TestUtils.send_command("BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
    TestUtils.verify_event("BPNODE", REMOVE_APP_EID, "INFO")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'REMOVED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_REG_STATE_1 not REMOVED")
        status = "F"
    
    # Set requirement status 
    for rqmnt in ["DTN.6.01070", "DTN.6.12660", "DTN.6.12670", "DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    #************************************************************************
    
    print("=================================================================")
    print("7. RESET_BUNDLE_COUNTERS")
    print("=================================================================")
    
    status = TestUtils.reset_counters("BUNDLE")

    #************************************************************************
    

    '''
    print("=================================================================")
    print(" Add/Start All Applications directives test")
    print("=================================================================")
    ##------------------------------------------------------------------
    ## Add all applications
    ##------------------------------------------------------------------
    status = TestUtils.send_command("BPNODE_CMD_ADD_ALL_APPLICATIONS")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") == 'ADDED' and \
       tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'ADDED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_STATE_0/CHAN_STAT_STATE_1: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") + "/" + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") + " - Expected ADDED/ADDED")
        status = "F"
    
    # Set requirement status 
    for rqmnt in ["DTN.6.12431", "DTN.6.12440", "DTN.6.12453", "DTN.6.12460", "DTN.6.19000"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("------------------------------------------------------------------")
    print(" Start all Applications")
    print("------------------------------------------------------------------")
    status = TestUtils.send_command("BPNODE_CMD_START_ALL_APPLICATIONS")
    
    if tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") == 'STARTED' and \
       tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") == 'STARTED':
        status = "P"
    else:
        print("ERROR - CHAN_STAT_STATE_0/CHAN_STAT_STATE_1: " + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0") + "/" + \
            tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1") + " - Expected STARTED/STARTED")
        status = "F"
    
    print("-------------------------------")
    print(" Send ADUs for all Applications")
    print("-------------------------------")
    adu_recd = tlm (f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED")
    
    cmd(f"{target} CFE_SB_CMD_SEND_SB_STATS")
    cmd(f"{target} CFE_TIME_CMD_SEND_DIAGNOSTIC")
    
    wait(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED == {adu_recd}+2", 6)
    if tlm(f"{target} {mib_counts_pkt} ADU_COUNT_RECEIVED") == adu_recd+2:
        status = "P"
    else:
        print("ERROR - ADU_COUNT_RECEIVED did not increment as expected")
        status = "F"
    
    # Set requirement status 
    for rqmnt in ["DTN.6.12230", "DTN.6.12971", "DTN.6.19040"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    '''
    #************************************************************************

    ## Print MIB Reports packet
    TestUtils.print_mib_reports_pkt()
    
    ## Print Storage packet
    TestUtils.print_storage_pkt()
 
    #******************************************************************

    ###################################################################
    
    ##=================================================================
    ## Print requirement status
    ##=================================================================
    
    print("******************************")
    print("***** Requirement Status *****")
    print("******************************")
    for key, value in rqmnt_status.items():
        print(f"***    {key}: {value}")
    print("******************************")
    
    ###################################################################
    
#bundle_creation_nominal("xxx")