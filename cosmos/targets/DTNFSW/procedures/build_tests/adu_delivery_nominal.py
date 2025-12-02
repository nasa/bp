def adu_delivery_nominal(self):

    print("###################################################################")
    print("### ADU Delivery test - Nominal")
    print("###################################################################")

    from dtntools.dtncla.udp import UdpTxSocket
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
    load_utility ("DTNFSW-1/procedures/dtngen_utils.py")
       
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        "DTN.6.01000":"U",
        "DTN.6.01010":"U",
        "DTN.6.01030":"U",
        "DTN.6.01040":"U",
        "DTN.6.01070":"U",
        "DTN.6.03000":"U", 
        "DTN.6.03010":"U",
        "DTN.6.03060":"U",
        "DTN.6.04180":"U",
        #"DTN.6.04190":"U",
        "DTN.6.04412":"U",
        "DTN.6.05140":"U",
        #"DTN.6.05150":"U",
        "DTN.6.12440":"U",
        "DTN.6.12450":"U",
        "DTN.6.12620":"I",
        "DTN.6.12630":"I",
        "DTN.6.12642":"U",
        "DTN.6.12644":"U",
        "DTN.6.12645":"U",
        "DTN.6.12630":"I",
        "DTN.6.12660":"U",
        "DTN.6.12670":"U",
        "DTN.6.12680":"I",
        "DTN.6.12971":"U",
        "DTN.6.19000":"U",
        "DTN.6.19010":"U",
        "DTN.6.19020":"U",
        "DTN.6.19030":"U",
        #"DTN.6.19040":"U",
        "DTN.6.19180":"U", 
        "DTN.6.19190":"U",
        "DTN.6.19220":"U",
        "DTN.6.19250":"U",
        "DTN.6.26010":"U",

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

    ## Set up and start contact
    cmd(f"{target} BPNODE_CMD_CONTACT_SETUP with CONTACT_ID 0")
    wait(1)
    cmd(f"{target} BPNODE_CMD_CONTACT_START with CONTACT_ID 0")    

    ## Destination EID configuration (to Channel 1 configured for ADU delivery)
    dest_node    = 100
    dest_service = 53
    
    ## Address/port configuration
    dest_ip   = DTN_NODE_IP_ADDR
    dest_port = 4501

    cmd(f"{target} BPNODE_CMD_RESET_ALL_COUNTERS")
    
    mib_counts_pkt = "BPNODE_NODE_MIB_COUNTERS_HK" 
    mib_reports_pkt = "BPNODE_NODE_MIB_REPORTS_HK" 

    ## Print MIB Reports packet
    TestUtils.print_mib_reports_pkt()


    print("===========================================================")
    print("1. Pre-stored Bundles")
    print("===========================================================")

    ## if Channel 1 is not REMOVED, remove it
    
    if tlm(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1") != 'REMOVED':
        
        print("Stopping/Removing channel 1 ...")
        
        # Stop channel 1
        cmd(f"{target} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
        wait(1)

        # Remove channel 1
        cmd(f"{target} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
        wait(1)

        #cmd(f"{target} BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 6)


    
    print("----------------------------------------------------------")
    print ("1.1 Send known number of bundles with Data Sender and ")
    print ("    store them in Storage")
    print("----------------------------------------------------------")

    print("Generating bundles ...")
    num_gen_bundles = 20
    payload = b'\xAA'*50
    DTNGenUtils.generate_bundles(dest_node, dest_service, num_gen_bundles, payload)

    print("Configuring/connecting Data Sender ...")
    data_sender = UdpTxSocket(dest_ip, dest_port)
    data_sender.connect()

    num_bundles = 15 # N1
    print(f"Sending {num_bundles} bundles ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)

    print(".........................")
    print("Verify bundles are stored")
    print(".........................")

    item_name   = "BUNDLE_COUNT_STORED"
    exp_val     = num_bundles
    
    status = TestUtils.verify_item(mib_reports_pkt, item_name, exp_val)

    # Set requirement status
    for rqmnt in ["DTN.6.04412"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    # Save bundles stored count for later check
    bundles_stored = tlm(f"{target} {mib_reports_pkt} BUNDLE_COUNT_STORED")
    

    print("----------------------------------------------------------")
    print ("1.2 Add Channel 1 Application")
    print("----------------------------------------------------------")

    cmd_count  = tlm(f"{target} {mib_counts_pkt} BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    cmd(f"{target} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    wait_check(f"{target} {mib_counts_pkt} BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {cmd_count+1}", 6)


    print("----------------------------------------------------------")
    print("1.3 Start Channel 1 Application")
    print("    Verify all stored bundles are delivered")
    print("----------------------------------------------------------")

    '''
    Verify:
    - bundles delivered count = bundles stored count from previous step (N1)
    - ADUs delivered count =  N1
    - bundles deleted count = N1
    - bundles stored count decrements from N1 to 0
    '''
    
    print(".................")
    print("Start application")
    print(".................")
    
    cmd_count = tlm(f"{target} {mib_counts_pkt} BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
    cmd(f"{target} BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    wait_check(f"{target} {mib_counts_pkt} BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {cmd_count+1}", 6)

    print(".............................................")
    print("Verify bundles delivered count incremented to")
    print("saved bundles stored count")
    print(".............................................")
    
    item_name = "BUNDLE_COUNT_DELIVERED"
    exp_val = bundles_stored
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in ["DTN.6.03000", "DTN.6.03010", "DTN.6.03060", 
                  "DTN.6.12642", "DTN.6.12644", "DTN.6.12971", "DTN.6.19020"
                 ]:
        TestUtils.set_requirement_status(rqmnt, status)
        
        
    print("........................................................")
    print("Verify ADUs delivered count incremented to saved bundles")
    print("stored count")
    print("........................................................")
    
    item_name = "ADU_COUNT_DELIVERED"
    exp_val = bundles_stored
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in [
        "DTN.6.01000", "DTN.6.01010", "DTN.6.01030", "DTN.6.01040", 
        "DTN.6.05140", "DTN.6.12440", "DTN.6.12450", "DTN.6.19000", 
        "DTN.6.19010", "DTN.6.19250",         
        ]:
        TestUtils.set_requirement_status(rqmnt, status)

    ## Print ADU data for CBOR decode verification
    ADU_data = tlm("DTNFSW-1 DTN_ADU_OUT DATA")
    ADU_size = len(ADU_data)
    print("ADU length: ", ADU_size)
    print("ADU Data: ", ADU_data)
    
    status = "P" if (ADU_size == 50 and ADU_data[0] == 0xAA) else "F" # matches payload
    for rqmnt in ["DTN.6.26010"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    
    print(".........................................................")
    print("Verify bundles deleted count incremented to saved bundles")
    print("stored count")
    print(".........................................................")
    
    status = "P"
    
    item_name = "BUNDLE_COUNT_DELETED"
    exp_val = bundles_stored
    
    if TestUtils.verify_item(mib_counts_pkt, item_name, exp_val) == "F":
        status = "F"
        
    
    print(".......................................")
    print("Verify bundles are deleted from storage")
    print(".......................................")
    
    item_name = "BUNDLE_COUNT_STORED"
    exp_val = 0
    
    if TestUtils.verify_item(mib_reports_pkt, item_name, exp_val) == "F":
        status = "F"

    # Set requirement status
    for rqmnt in ['DTN.6.04180']:
        TestUtils.set_requirement_status(rqmnt, status)

    wait()

    #******************************************************************

    print("===========================================================")
    print("2. Bundles sent after Application start")
    print("===========================================================")

    print("----------------------------------------------------------")
    print("2.1 Send known number of bundles (N2) with Data Sender")
    print("    Verify all bundles are delivered")
    print("----------------------------------------------------------")
    
    '''
    bundles don't get stored/deleted since channel contact is on
    
    Verify:
    - bundles delivered count increments by N2
    - ADUs delivered count increments by N2
    '''

    num_bundles = 20  # N2
    print (f"!!! Application already started, sending {num_bundles} bundles ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)
    
    exp_bundle_cnt = bundles_stored + num_bundles
    
    print(".............................................")
    print("Verify bundles delivered count incremented to")
    print("saved bundles stored count")
    print(".............................................")

    item_name = "BUNDLE_COUNT_DELIVERED"
    exp_val = exp_bundle_cnt
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in \
        ["DTN.6.12642", "DTN.6.12971", "DTN.6.19020"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
        
    print("................................................")
    print("Verify ADUs delivered count incremented to saved")
    print("bundles stored count")
    print("................................................")
    
    item_name = "ADU_COUNT_DELIVERED"
    exp_val = exp_bundle_cnt
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in ["DTN.6.05140", "DTN.6.19220", "DTN.6.19250"]:
        TestUtils.set_requirement_status(rqmnt, status)

    #******************************************************************
        
    print("===========================================================")
    print("3. Stop channel 1 Application")
    print("===========================================================")

    cmd(f"{target} BPNODE_CMD_STOP_APPLICATION with CHAN_ID 1")
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STOPPED'", 6)
        
    print("----------------------------------------------------------")
    print("Send bundles and verify they don't get delivered")
    print("----------------------------------------------------------")
    print (f"!!! Application stopped, sending {num_bundles} bundles ...")
    DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)

    item_name = "BUNDLE_COUNT_DELIVERED"
    exp_val = exp_bundle_cnt
    
    status = TestUtils.verify_item(mib_counts_pkt, item_name, exp_val)
    
    # Set requirement status
    for rqmnt in ["DTN.6.12645", "DTN.6.19030"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    #******************************************************************
    
    print("===========================================================")
    print("4. Remove channel 1 Application")
    print("===========================================================")
    cmd(f"{target} BPNODE_CMD_REMOVE_APPLICATION with CHAN_ID 1")
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'REMOVED'", 6)

    print(".......................................................")
    print("Verify channel is available so application can be added")
    print(".......................................................")
    cmd(f"{target} BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    if wait(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'ADDED'", 6):
        status = "P"
    else:
        status = "F"
        
    for rqmnt in ["DTN.6.01070", "DTN.6.12660", "DTN.6.12670"]:
        TestUtils.set_requirement_status(rqmnt, status)
    
    #******************************************************************

    print("===========================================================")
    print(" 5. Reset Counters Directives")
    print("===========================================================")
    
    print("-----------------------------------------------------------")
    print(" 5.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_COUNT_DELIVERED")
    TestUtils.reset_counter("ADU_COUNT_DELIVERED")
    '''
    print("-----------------------------------------------------------")
    print("5.2. RESET_BUNDLE_COUNTERS")
    print("-----------------------------------------------------------")
    
    TestUtils.reset_counters("BUNDLE")
    '''
    print("-----------------------------------------------------------")
    print("5.3. RESET_ALL_COUNTERS")
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
#adu_delivery_nominal("xxx")
#set_line_delay(0)

