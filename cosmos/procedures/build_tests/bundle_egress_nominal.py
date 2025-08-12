def bundle_egress_nominal(self):

    print("###################################################################")
    print("### Bundle Egress test - Nominal")
    print("###################################################################")

    from dtntools.dtncla.udp import UdpTxSocket, UdpRxSocket
    from dtntools.dtngen.bundle import Bundle
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
    load_utility ("DTNFSW-1/procedures/dtngen_utils.py")
    load_utility ("DTNFSW-1/procedures/load_new_table.py")
       
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        "DTN.6.04160":"U", 
        "DTN.6.04180":"U", 
        #"DTN.6.04190":"U", 
        "DTN.6.04200":"U", 
        "DTN.6.04272":"U", 
        "DTN.6.04320":"I",
        "DTN.6.04370":"I",
        "DTN.6.04380":"U",
        "DTN.6.04390":"U",
        "DTN.6.04610":"U", 
        "DTN.6.04620":"U", 
        "DTN.6.07000":"U", 
        "DTN.6.07040":"U", 
        "DTN.6.07042":"U", 
        "DTN.6.07050":"U", 
        #"DTN.6.07060":"U",
        "DTN.6.08712":"U", 
        "DTN.6.12062":"U", 
        "DTN.6.12290":"U", 
        "DTN.6.12295":"U", 
        "DTN.6.12310":"I", 
        "DTN.6.12360":"U", 
        "DTN.6.12362":"U", 
        "DTN.6.12364":"U",
        "DTN.6.12370":"U", 
        "DTN.6.12372":"U", 
        "DTN.6.12374":"U",
        "DTN.6.12390":"U",
        "DTN.6.12395":"U",
        "DTN.6.12400":"U",
        "DTN.6.13071":"U", 
        "DTN.6.15001":"U", 
        "DTN.6.15011":"U", 
        "DTN.6.15012":"U", 
        "DTN.6.15061":"U", 
        "DTN.6.15071":"U", 
        #"DTN.6.15081":"U", 
        #"DTN.6.15121":"U", 
        #"DTN.6.15131":"U",
        "DTN.6.15141":"U", 
        "DTN.6.15144":"U", 
        #"DTN.6.15157":"U", 
        #"DTN.6.15161":"U", 
        #"DTN.6.15181":"U", 
        #"DTN.6.15191":"U", 
        #"DTN.6.15201":"U", 
        #"DTN.6.15221":"U", 
        #"DTN.6.15231":"U", 
        #"DTN.6.15251":"U", 
        #"DTN.6.15261":"U",
        "DTN.6.15281":"U",
        "DTN.6.19180":"U", 
        "DTN.6.19190":"U", 
        "DTN.6.19360":"U", 
        "DTN.6.19390":"U", 
        "DTN.6.19410":"U", 
        "DTN.6.23060":"U", 

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

    ## Copy needed test tables from COSMOS to DTNFSW
    prompt("Copy these tables to FSW build/exe/cpu1/cf folder:\n\n" + 
           " - contact_rx_only.tbl\n" +
           " - contact_nominal.tbl\n" +
           " - cont_erate_lim.tbl"          )

    ## Destination EID configuration (to Channel 0 configured for Contact 0)
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

    mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK"
    mib_reports_pkt = "BPNODE_NODE_MIB_REPORTS_HK"
    
    cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")    
    #cmd(f"{target} CFE_EVS_CMD_ENABLE_EVENT_TYPE with BIT_MASK 15")
    #causes TO_LAB_TLM_PIPE flooding and processor crash
    
    ## Print MIB Reports packet
    TestUtils.print_mib_reports_pkt()
    
    ## Print Storage packet
    TestUtils.print_storage_pkt()

    print("===========================================================")
    print("1. Egress Bundles - Nominal")
    print("===========================================================")
    
    print("-----------------------------------------------------------")
    print("1.1 Store a known number of bundles")
    print("-----------------------------------------------------------")

    # Set up a contact so bundles are stored but not forwarded
    # Destination node in contact table is not dest_node (bundle destination)
    load_new_table('/cf/contact_rx_only.tbl')

    # Generate bundles
    num_bundles = 20
    payload = b'\xAA'*1000
    
    print("Generating bundles ...")
    DTNGenUtils.generate_bundles(dest_node, dest_service, num_bundles, payload)

    data_sender = UdpTxSocket(dest_ip, dest_port, bps_limit=1E6)
    data_receiver = UdpRxSocket(local_ip, local_port)
    data_sender.connect()
    data_receiver.connect()
    
    # Set up and start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    
    # Send bundles to storage
    print(f"Sending {num_bundles} bundles to DTN Node ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)

    item_name = "BUNDLE_COUNT_STORED"
    exp_val = num_bundles
    
    status = TestUtils.verify_item(mib_reports_pkt, item_name, exp_val)

    # Stop and teardown contact
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    
    print("-----------------------------------------------------------")
    print("1.2 Set up nominal contact")
    print("-----------------------------------------------------------")
        
    # Load nominal contacts table
    load_new_table('/cf/contact_nominal.tbl')

    status = TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")

    for rqmnt in [
        "DTN.6.12062", "DTN.6.12290", "DTN.6.12295", "DTN.6.13071", "DTN.6.15001",
        "DTN.6.15281", "DTN.6.19360", "DTN.6.19390", "DTN.6.19410"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)

    print("-----------------------------------------------------------")
    print("1.3 Start contact")
    print("-----------------------------------------------------------")

    status = TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0")

    for rqmnt in ["DTN.6.12362", "DTN.6.12364"]:
        TestUtils.set_requirement_status(rqmnt, status)

    
    print(".........................................")
    print("1.3.1 Verify stored bundles are forwarded")
    print(".........................................")
    
    item_name = "BUNDLE_COUNT_FORWARDED"
    exp_val = num_bundles
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    '''
    sent_bundle = Bundle.from_json_file(f'/bundles/{dest_node}/generated_bundle_1.json')
    received_bundle = data_receiver.read()
    print(f"Received Bundle: {Bundle.from_bytes(received_bundle).to_json()}")    
    
    check_expression(f"'{received_bundle == sent_bundle}' == 'True'")
    '''

    for rqmnt in [
        "DTN.6.04380", 
        "DTN.6.04390", "DTN.6.07000", "DTN.6.07040", "DTN.6.07042", "DTN.6.07050", 
        "DTN.6.12360", "DTN.6.15011", "DTN.6.15061", "DTN.6.15281", "DTN.6.23060"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("........................................................")
    print("1.3.2 Verify forwarded bundles are deleted from storage ")
    print("........................................................")

    item_name = "BUNDLE_COUNT_STORED"
    exp_val = 0
    
    status = TestUtils.verify_item(mib_reports_pkt, item_name, exp_val)

    for rqmnt in ["DTN.6.04200", "DTN.6.04272", "DTN.6.04390", "DTN.6.08712"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    '''
    TBD
    for rqmnt in ["DTN.6.04610","DTN.6.04620"]:
        TestUtils.set_requirement_status(rqmnt, status)
    '''
    item_name = "BUNDLE_COUNT_DELETED"
    exp_val = num_bundles
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    for rqmnt in ["DTN.6.04180"]:
        TestUtils.set_requirement_status(rqmnt, status)

    item_name = "BUNDLE_COUNT_DISCARDED"
    exp_val = num_bundles
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    for rqmnt in ["DTN.6.04160"]:
        TestUtils.set_requirement_status(rqmnt, status)

    print("-----------------------------------------------------------")
    print("1.4 Stop contact")
    print("-----------------------------------------------------------")

    status = TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")

    for rqmnt in ["DTN.6.12372", "DTN.6.12374"]:
        TestUtils.set_requirement_status(rqmnt, status)

    ## Tear down contact
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'TORNDOWN'", 6)
    

    print("............................................................")
    print("1.4.1 Verify bundle forwarding stops when contact is stopped")
    print("............................................................")
    '''
    - set up temporary contact that only receives bundles and does not send    
    - store a large number of bundles
    - set up egress-rate-limited contact
    - start contact - forwarding will start 
    - stop contact after a short delay
    - verify forwarded bundles stops incrementing
    '''    
    
    ## Set up temporary contact that only receives bundles and does not send
    ## Send bundles to storage
    load_new_table('/cf/contact_rx_only.tbl')
    
    ## Set up and start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'STARTED'", 6)
    
    num_loops = 20
    print(f"Sending {num_bundles*num_loops} bundles to DTN node ...")
    with disable_instrumentation():
        for _ in range(num_loops):
            DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)
    
    wait_packet(target, mib_counts_pkt, 1, 6)
    wait_packet(target, mib_reports_pkt, 1, 6)
    received_cnt = tlm(f'DTNFSW-1 {mib_counts_pkt} BUNDLE_COUNT_RECEIVED')
    stored_cnt = tlm(f'DTNFSW-1 {mib_reports_pkt} BUNDLE_COUNT_STORED')
    print(f"BUNDLE_COUNT_RECEIVED: {received_cnt}  BUNDLE_COUNT_STORED: {stored_cnt}")

    ## Stop and teardown contact
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    
    ## Load egress rate limited contacts table - forwarding will start 
    load_new_table('/cf/cont_erate_lim.tbl')
    
    ## Set up and start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    #wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'STARTED'", 6)

    ## Stop contact after a short delay
    wait(5)
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    
    wait_packet(target, mib_counts_pkt, 1, 6)
    forwarded_cnt = tlm(f'DTNFSW-1 {mib_counts_pkt} BUNDLE_COUNT_FORWARDED')
    print(f"BUNDLE_COUNT_FORWARDED: {forwarded_cnt}")
    
    ## Verify forwarded bundles stops incrementing
    wait_packet(target, mib_counts_pkt, 1, 6)
            
    if tlm(f'{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED') == forwarded_cnt and \
            forwarded_cnt < received_cnt:
        print("Bundle forwarding stopped as expected")
        status = "P"
    else:
        print("ERROR - Bundle forwarding did not stop as expected")
        status = "F"    
    
    for rqmnt in ["DTN.6.12370", "DTN.6.15012", "DTN.6.15071", "DTN.6.15141", "DTN.6.15144"]:
        TestUtils.set_requirement_status(rqmnt, status)    

    print("-----------------------------------------------------------")
    print("1.5 Bundle flow continuity within and between contacts")
    print("-----------------------------------------------------------")

    print("............................................................")
    print("1.5.1 Start contact, verify bundle forwarding resumes")
    print("............................................................")
    forwarded_cnt = tlm(f'DTNFSW-1 {mib_counts_pkt} BUNDLE_COUNT_FORWARDED')
    
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    wait(10)
    if tlm(f'{target} {mib_counts_pkt} BUNDLE_COUNT_FORWARDED') > forwarded_cnt:
        print("Bundle forwarding resumed as expected")
    else:
        print("ERROR - Bundle forwarding did not resume as expected")
    
    print("...........................................................")
    print("1.5.2 Stop and teardown contact")
    print("...........................................................")
    # Bundles in queue flushed and pulled back to storage, queue deleted
    # CLA resources released
    
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'TORNDOWN'", 6)
    
    print("...........................................................")
    print("1.5.3 Start next contact, verify successful start")
    print("...........................................................")
    # CLA resources made available    
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    if wait(f"{target} BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_0 == 'STARTED'", 6):
        status = "P"
    else:
        status = "F"

    for rqmnt in ["DTN.6.12390", "DTN.6.12395"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    print("...........................................................")
    print("1.5.4 Verify all remaining bundles in storage get out")
    print("...........................................................")
    # No bundles should be lost in queue management    
    if wait(f"{target} {mib_reports_pkt} BUNDLE_COUNT_STORED == 0", 60):
        status = "P" 
    else:
        status = "F" 
    forwarded_cnt = tlm(f'DTNFSW-1 {mib_counts_pkt} BUNDLE_COUNT_FORWARDED')
    print(f"BUNDLE_COUNT_FORWARDED: {forwarded_cnt}")
        
    for rqmnt in ["DTN.6.12400"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    
    ## Teardown contact
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")

    #******************************************************************
    
    print("===========================================================")
    print(" 2. Reset Counters Directives")
    print("===========================================================")
    
    print("-----------------------------------------------------------")
    print(" 2.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_COUNT_RECEIVED")
    TestUtils.reset_counter("BUNDLE_COUNT_FORWARDED")
    
    print("-----------------------------------------------------------")
    print("2.2. RESET_BUNDLE_COUNTERS")
    print("-----------------------------------------------------------")
    
    TestUtils.reset_counters("BUNDLE")

    print("-----------------------------------------------------------")
    print("2.3. RESET_ALL_COUNTERS")
    print("-----------------------------------------------------------")
    
    TestUtils.reset_counters("ALL")

    ##*****************************************************************
    ## Disconnect data sender/receiver
    data_receiver.disconnect()
    data_sender.disconnect()

    ## Print MIB Reports packet
    TestUtils.print_mib_reports_pkt()
    
    ## Print Storage packet
    TestUtils.print_storage_pkt()
    
    #******************************************************************

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
    
    
## main
#bundle_egress_nominal("xxx")
#set_line_delay(0)

