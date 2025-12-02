def adu_delivery_errors(self):

    print("###################################################################")
    print("### ADU Delivery test - Errors")
    print("###################################################################")

    from dtntools.dtncla.udp import UdpTxSocket
    load_utility('DTNFSW-1/procedures/load_new_table.py')
    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
       
    global rqmnt_status

    # Initialize requirement status
    rqmnt_status = {
        "DTN.6.01032":"U", 
        "DTN.6.01060":"U", 
        "DTN.6.03151":"U", 
        "DTN.6.12452":"U", 
        #"DTN.6.12596":"U", 
        "DTN.6.13051":"U", 
        "DTN.6.19180":"U", 
        "DTN.6.19190":"U", 
        "DTN.6.19360":"U", 
        "DTN.6.19390":"U", 
        "DTN.6.19410":"U", 

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
           " - chan_bad_sspdst.tbl\n" +
           " - chan_bad_ssprep.tbl\n" +
           " - chan_dup_serv.tbl\n" +
           " - chan_bad_crc.tbl\n"
          )

    # Start channels 0 and 1
    cmd("DTNFSW-1 BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
    cmd("DTNFSW-1 BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
    cmd("DTNFSW-1 BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    cmd("DTNFSW-1 BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_0 == 'STARTED'", 10)
    wait_check(f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'", 10)

    print("--------------------------------------------------------------------")
    print(" 1. Channel configuration errors")
    print("--------------------------------------------------------------------")
   
    #No Channel Available   DTN.6.19090, DTN.6.19210, DTN.6.12431, DTN.6.12451, DTN.6.12596
    #Duplicate Channel DTN.6.12452, DTN.6.12596
    #Local EID Invalid DTN.6.12470, DTN.6.12596, DTN.6.01060 - set in MIB PN table
    #Local EID in Use DTN.6.12470, DTN.6.01032
    #Foreign EID Invalid 
    
    #DTN.6.12596 If BPA Payload Interface reports an error indication on any 
    #add-application action, AA Node Configuration shall mark as available that ADU Proxy and its path.
    #- Currently BPLib_PI_AddApplication returns success always
    
    print("...................................................................")
    print(" 1.1 CrcType 0")
    print("...................................................................")
    status = TestUtils.validate_invalid_table('/cf/chan_bad_crc.tbl') 
    for rqmnt in ["DTN.6.03151", "DTN.6.13051"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Verify channels maintain same state after table load failure
    if f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_0 == 'STARTED'" and \
       f"{target} BPNODE_CHAN_CON_STAT_HK CHAN_STAT_STATE_1 == 'STARTED'":
       status = "P"
    else:
       print("ERROR - Channel state(s) changed after table load failure")
       status = "F"
        
    #for rqmnt in ["DTN.6.12596"]: #TBD
    #    TestUtils.set_requirement_status(rqmnt, status)


    print("...................................................................")
    print(" 1.2 Duplicate channel - same local EID")
    print("...................................................................")
    # Local EID: node number set in MIB PN table, service number in channel table
    
    status = TestUtils.validate_invalid_table('/cf/chan_dup_serv.tbl') 
    for rqmnt in ["DTN.6.01032", "DTN.6.03151", "DTN.6.12452", "DTN.6.13051"]:
        TestUtils.set_requirement_status(rqmnt, status)
        
    '''
    # If table is accepted, channel ADD should get rejected:
    load_new_table('/cf/chan_dup_dest.tbl')
    load_new_table('/cf/chan_dup_serv.tbl')
    status = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0", INVALID_CMD_TYPE)
    status = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1", INVALID_CMD_TYPE)
    for rqmnt in ["DTN.6.12452"]:
        TestUtils.set_requirement_status(rqmnt, status)

    # Load valid channel table, verify channel can be added/started
    load_new_table('/cf/bpnode_channel.tbl')
    status = "P"
    stat = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 0")
    if stat = "F" status = "F"
    status = TestUtils.send_command("BPNODE_CMD_START_APPLICATION with CHAN_ID 0")
    if stat = "F" status = "F"
    stat = TestUtils.send_command("BPNODE_CMD_ADD_APPLICATION with CHAN_ID 1")
    if stat = "F" status = "F"
    status = TestUtils.send_command("BPNODE_CMD_START_APPLICATION with CHAN_ID 1")
    if stat = "F" status = "F"
    for rqmnt in ["DTN.6.12596"]:
        TestUtils.set_requirement_status(rqmnt, status)
    '''

    print("...................................................................")
    print(" 1.3 Invalid destination/report EID - bad SSP")
    print("...................................................................")
    rqmnt = "DTN.6.01060"
    status = "P"
    
    if TestUtils.validate_invalid_table('/cf/chan_bad_sspdst.tbl') == "F" or \
       TestUtils.validate_invalid_table('/cf/chan_bad_ssprep.tbl') == "F": 
           status = "F"
    
    TestUtils.set_requirement_status(rqmnt, status)


    print("=================================================================")
    print("2. Reset Counters - ERROR/BUNDLE")
    print("=================================================================")
    
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
#adu_delivery_errors("xxx")
#set_line_delay(0)
