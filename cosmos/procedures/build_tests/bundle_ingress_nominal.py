def bundle_ingress_nominal(self):

    print("###################################################################")
    print("### Bundle Ingress test - Nominal")
    print("###################################################################")

    from dtntools.dtncla.udp import UdpTxSocket
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
    load_utility ("DTNFSW-1/procedures/dtngen_utils.py")
    load_utility ("DTNFSW-1/procedures/load_new_table.py")
       
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        #"DTN.6.04280":"U", 
        #"DTN.6.04290":"U", 
        "DTN.6.04316":"U", 
        #"DTN.6.04317":"U", 
        #"DTN.6.04420":"U", 
        #"DTN.6.04422":"U", 
        "DTN.6.06430":"I", 
        "DTN.6.06552":"U", 
        #"DTN.6.06553":"U", 
        "DTN.6.06610":"I", 
        "DTN.6.06620":"I", 
        "DTN.6.06630":"I", 
        "DTN.6.12062":"U", 
        "DTN.6.12290":"U", 
        "DTN.6.12295":"U", 
        "DTN.6.12360":"U", 
        "DTN.6.12362":"U", 
        "DTN.6.12364":"U",
        "DTN.6.12370":"U", 
        "DTN.6.12372":"U", 
        "DTN.6.12374":"U",
        "DTN.6.13071":"U", 
        "DTN.6.15001":"U", 
        "DTN.6.15013":"U", 
        "DTN.6.15014":"U", 
        "DTN.6.15041":"U", 
        "DTN.6.15051":"U", 
        "DTN.6.15073":"U", 
        #"DTN.6.15121":"U", 
        #"DTN.6.15131":"U",
        "DTN.6.15141":"U", 
        "DTN.6.15143":"U", 
        #"DTN.6.15155":"U", 
        #"DTN.6.15161":"U", 
        #"DTN.6.15195":"U", 
        #"DTN.6.15235":"U",
        #"DTN.6.15241":"U",
        "DTN.6.15281":"U",
        "DTN.6.19180":"U", 
        "DTN.6.19190":"U", 
        "DTN.6.19210":"U", 
        "DTN.6.19260":"U", 
        "DTN.6.19360":"U", 
        "DTN.6.19390":"U", 
        "DTN.6.19410":"U", 
        "DTN.6.25010":"I",

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
           " - cont_irate_lim.tbl"
          )

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

    ## Configure and connect Data Sender
    data_sender = UdpTxSocket(dest_ip, dest_port) #, bps_limit=10E6, inter_msg_delay=1)
    data_sender.connect()
    
    mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK"
    mib_reports_pkt = "BPNODE_NODE_MIB_REPORTS_HK"
    chan_stat_pkt = "BPNODE_CHAN_CON_STAT_HK"
    
    cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")
    #cmd(f"{target} CFE_EVS_CMD_ENABLE_EVENT_TYPE with BIT_MASK 15")

    ## Print MIB Reports packet
    TestUtils.print_mib_reports_pkt()
    
    ## Print Storage packet
    TestUtils.print_storage_pkt()

    print("===========================================================")
    print("1. CONTACT 0")
    print("===========================================================")

    print("-----------------------------------------------------------")
    print("1.1 Set up contact")
    print("-----------------------------------------------------------")
    
    # Set up a receive only contact so bundles are stored but not forwarded
    # - Destination node in contact table is not bundle destination
    load_new_table('/cf/contact_rx_only.tbl')
    
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")    

    for rqmnt in [
        "DTN.6.12290", "DTN.6.12295", "DTN.6.13071", "DTN.6.15001", 
        "DTN.6.15281", "DTN.6.19360", "DTN.6.19390", "DTN.6.19410"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
   
    if tlm(f"{target} {chan_stat_pkt} CON_STAT_RUN_STATE_0") == 'SETUP':
        status = "P"
    else:
        print("ERROR - CON_STAT_RUN_STATE_0 not SETUP")
        status = "F"        
    
    for rqmnt in ["DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    print("-----------------------------------------------------------")
    print("1.2 Start contact")
    print("-----------------------------------------------------------")
    
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0")
    for rqmnt in ["DTN.6.12362", "DTN.6.12364"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    if tlm(f"{target} {chan_stat_pkt} CON_STAT_RUN_STATE_0") == 'STARTED':
        status = "P"
    else:
        print("ERROR - CON_STAT_RUN_STATE_0 not STARTED")
        status = "F"        
    
    for rqmnt in ["DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("-----------------------------------------------------------")
    print("1.3 Send known number of bundles with Data Sender")
    print("-----------------------------------------------------------")

    print("Generating bundles ...")
    num_bundles = 10
    payload = b'\xAA'*1000
    DTNGenUtils.generate_bundles(dest_node, dest_service, num_bundles, payload)

    print("Sending bundles to DTN Node ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)
    print(f" ... sent {num_bundles} bundles to DTN Node")


    print("............................................")
    print("1.3.1 Verify bundles are received and stored")
    print("............................................")

    status = "P"
    item_name = "BUNDLE_COUNT_RECEIVED"
    exp_val = num_bundles
    if TestUtils.verify_item(mib_counts_pkt, item_name, exp_val) == "F":
        status = "F"

    item_name = "BUNDLE_COUNT_STORED"
    exp_val = num_bundles
    if TestUtils.verify_item(mib_reports_pkt, item_name, exp_val) == "F":
        status = "F"
    
    for rqmnt in [
            "DTN.6.12360", "DTN.6.15041", "DTN.6.15051", 
            "DTN.6.15281", "DTN.6.04316", 
            "DTN.6.06552"
            ]:
        TestUtils.set_requirement_status(rqmnt, status)


    print("-----------------------------------------------------------")
    print("1.4 Stop contact")
    print("-----------------------------------------------------------")

    status = TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0")
    
    for rqmnt in ["DTN.6.19210", "DTN.6.12372", "DTN.6.12374"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    if tlm(f"{target} {chan_stat_pkt} CON_STAT_RUN_STATE_0") == 'STOPPED':
        status = "P"
    else:
        print("ERROR - CON_STAT_RUN_STATE_0 not STOPPED")
        status = "F"        
    
    for rqmnt in ["DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("-----------------------------------------------------------")
    print("1.5 Send bundles when contact is stopped")
    print("    Verify bundle reception stopped")
    print("-----------------------------------------------------------")

    received_cnt = tlm (f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED")

    print("Sending bundles to DTN Node ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)
    print(f" ... sent {num_bundles} bundles to DTN Node")
    
    wait_packet (target, mib_counts_pkt, 2, 10)
    if tlm (f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED") == received_cnt:
        print("!!! No Bundle reception stopped as expected")
        status = "P"
    else:
        print("!!! ERROR - Bundles reception did not stop as expected")
        status = "F"
    
    for rqmnt in ["DTN.6.12370", "DTN.6.15141", "DTN.6.15143"]:
        TestUtils.set_requirement_status(rqmnt, status)

    
    print("-----------------------------------------------------------")
    print("1.6 Teardown contact")
    print("-----------------------------------------------------------")
 
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0")
      
    if tlm(f"{target} {chan_stat_pkt} CON_STAT_RUN_STATE_0") == 'TORNDOWN':
        status = "P"
    else:
        print("ERROR - CON_STAT_RUN_STATE_0 not TORNDOWN")
        status = "F"        
    
    for rqmnt in ["DTN.6.19210"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    #******************************************************************
    '''
    print("===========================================================")
    print("2. CONTACT 1")
    print("===========================================================")
    
    # NOTE: Contact 1 is designed for SB CLA and can't be reconfigured at this time
    
    ## Verify contact 1 directives are accepted

    print("-----------------------------------------------------------")
    print("2.1 Set up contact")
    print("-----------------------------------------------------------")
        
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 1")
    for rqmnt in [
        "DTN.6.12290", "DTN.6.12295", "DTN.6.13071", "DTN.6.15001", 
        "DTN.6.15281", "DTN.6.19360", "DTN.6.19390", "DTN.6.19410"
        ]:
        TestUtils.set_requirement_status(rqmnt, status)
   
    print("-----------------------------------------------------------")
    print("2.2 Start contact")
    print("-----------------------------------------------------------")
    
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 1")
    for rqmnt in ["DTN.6.12362", "DTN.6.12364"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    print("-----------------------------------------------------------")
    print("2.3 Stop contact")
    print("-----------------------------------------------------------")

    status = TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 1")    
    for rqmnt in ["DTN.6.19210", "DTN.6.12372", "DTN.6.12374"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("-----------------------------------------------------------")
    print("2.4 Teardown contact")
    print("-----------------------------------------------------------")
 
    status = TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 1")
    
    #******************************************************************
      
    print("===========================================================")
    print("3. CONTACT 0 and 1")
    print("===========================================================")
    
    if TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0") == "F" or \
       TestUtils.send_command("BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 1") == "F":
        print("ERROR - could not set up simultaneous contacts")
        
    if TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 0") == "F" or \
       TestUtils.send_command("BPNODE_CMD_CONTACT_START with CONTACT_ID 1") == "F":
        print("ERROR - could not start simultaneous contacts")

    if TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 0") == "F" or \
       TestUtils.send_command("BPNODE_CMD_CONTACT_STOP with CONTACT_ID 1") == "F":
        print("ERROR - could not stop simultaneous contacts")

    if TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 0") == "F" or \
       TestUtils.send_command("BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID 1") == "F":
        print("ERROR - could not tear down simultaneous contacts")

    #TBD "DTN.6.15155" - receive bundles from multiple incoming simultaneous contacts
    
    #******************************************************************
    '''
    print("===========================================================")
    print("4. Ingress Rate")
    print("===========================================================")
    
    ## Load contact table with ingress rate 1000 bits per cycle (10Hz)
    load_new_table('/cf/cont_irate_lim.tbl')

    ## Start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_START")
    wait(1)
    
    ## Send bundles
    num_bundles = 1
    payload = b'\xAA'*1000
    DTNGenUtils.generate_bundles(dest_node, dest_service, num_bundles, payload)

    received_cnt = tlm (f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED")
    num_loops=200
    print(f"Sending {num_loops*num_bundles} bundles to DTN Node ...")
    with disable_instrumentation():
        for _ in range(num_loops):
            DTNGenUtils.send_bundles(1, dest_node, data_sender)
    
    ## Verify bundles are received at low rate - expected ~20 sec
    elapsed = wait_check(f"{target} {mib_counts_pkt} BUNDLE_COUNT_RECEIVED == {received_cnt+num_bundles*num_loops}", 30)
    if elapsed > 15 and elapsed < 30:
        print("!!! Bundle ingress rate low as expected")
        status = "P"
    else:
        print("!!! ERROR - Bundle ingress rate not low as expected")
        status = "F"
    
    for rqmnt in ["DTN.6.15013", "DTN.6.15073"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    #TBD - 15014 exceed the rate limit, the CLA shall send a warning event DTNN-1309
    #TBD - 19260 translate all Event Management event messages event types to host event types 

    ## Stop and teardown contact
    cmd(f"{target} BPNODE_CMD_CONTACT_STOP")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_TEARDOWN")
    wait(1)
    
    #******************************************************************
 
    print("===========================================================")
    print("5. Reset Counters Directives")
    print("===========================================================")
    
    print("-----------------------------------------------------------")
    print("5.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_COUNT_RECEIVED")
    status = "P"
    for rqmnt in ["DTN.6.12062"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    print("-----------------------------------------------------------")
    print("5.2 RESET_ALL_COUNTERS")
    print("-----------------------------------------------------------") 
    
    TestUtils.reset_counters("ALL")

    #******************************************************************
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
    
# main
#bundle_ingress_nominal("xxx")
#set_line_delay(0)

