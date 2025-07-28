def bundle_creation_errors(self):
    
    ###################################################################
    ## Bundle creation test - errors
    ###################################################################

    load_utility ("DTNFSW-1/procedures/build_tests/test_globals.py")
    load_utility ("DTNFSW-1/procedures/build_tests/test_utils.py")
      
    global rqmnt_status
  
    ## Initialize requirement status
    rqmnt_status = {
        "DTN.6.02414":"U", 
        "DTN.6.12451":"U",
        "DTN.6.12644":"U",
        "DTN.6.19160":"U", 
        "DTN.6.19170":"U",
        "DTN.6.19180":"U",
        "DTN.6.19190":"U",
        "DTN.6.26195":"U", 

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
    chan_stat_pkt  = "BPNODE_CHAN_CON_STAT_HK"
           
    # Error Event IDs
    BPLIB_NC_ADD_ALL_APPS_ERR_EID   = 540
    BPLIB_NC_ADD_APP_ERR_EID        = 541
    BPLIB_NC_RM_APP_ERR_EID         = 562
    BPLIB_NC_START_ALL_APPS_ERR_EID = 572
    BPLIB_NC_START_APP_ERR_EID      = 573
    BPLIB_NC_STOP_APP_ERR_EID       = 574

    add_app    = "BPNODE_CMD_ADD_APPLICATION"
    start_app  = "BPNODE_CMD_START_APPLICATION"
    stop_app   = "BPNODE_CMD_STOP_APPLICATION"
    remove_app = "BPNODE_CMD_REMOVE_APPLICATION"
    
    
    print("-----------------------------------------------------------------")
    print(" Start with Applications 1 and 2 in ADDED state")
    print("-----------------------------------------------------------------")
       
    match tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0"):
        case "ADDED":
            print("!!! Application 1 already in ADDED state")
        case "STARTED":
            cmd(f"{target} {stop_app} with CHAN_ID 0")
            cmd(f"{target} {remove_app} with CHAN_ID 0")
            cmd(f"{target} {add_app} with CHAN_ID 0")
        case "STOPPED":
            cmd(f"{target} {remove_app} with CHAN_ID 0")
            cmd(f"{target} {add_app} with CHAN_ID 0")
        case "REMOVED":
            cmd(f"{target} {add_app} with CHAN_ID 0")
        case _:
            print("!!! Application 1 state unknown")
    
    match tlm(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1"):
        case "ADDED":
            print("!!! Application 2 already in ADDED state")
        case "STARTED":
            cmd(f"{target} {stop_app} with CHAN_ID 1")
            cmd(f"{target} {remove_app} with CHAN_ID 1")
            cmd(f"{target} {add_app} with CHAN_ID 1")
        case "STOPPED":
            cmd(f"{target} {remove_app} with CHAN_ID 1")
            cmd(f"{target} {add_app} with CHAN_ID 1")
        case "REMOVED":
            cmd(f"{target} {add_app} with CHAN_ID 1")
        case _:
            print("!!! Application 2 state unknown")
     
    wait (2)
    
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_0 == 'ADDED'", 6)
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'ADDED'", 6)
    
    ##***********************************************************************

    print("=================================================================")
    print(" 1. Invalid channel tests")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print(" 1.1 Add Application with invalid channel ID")
    print("-----------------------------------------------------------")
    TestUtils.send_command(f"{add_app} with CHAN_ID 2", TestUtils.INVALID_CMD_TYPE)
    status = TestUtils.verify_event("BPNODE", BPLIB_NC_ADD_APP_ERR_EID, "ERROR")
    rqmnt = "DTN.6.12451"
    TestUtils.set_requirement_status(rqmnt, status)
    
    print("-----------------------------------------------------------")
    print(" 1.2 Start Application with invalid channel ID")
    print("-----------------------------------------------------------")
    TestUtils.send_command(f"{start_app} with CHAN_ID 2", TestUtils.INVALID_CMD_TYPE)
    status = TestUtils.verify_event("BPNODE", BPLIB_NC_START_APP_ERR_EID, "ERROR")
    rqmnt = "DTN.6.12644"
    TestUtils.set_requirement_status(rqmnt, status)

    print("-----------------------------------------------------------")
    print(" 1.3 Stop Application with invalid channel ID")
    print("-----------------------------------------------------------")
    TestUtils.send_command(f"{stop_app} with CHAN_ID 2", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_STOP_APP_ERR_EID, "ERROR")

    print("-----------------------------------------------------------")
    print(" 1.4 Remove Application with invalid channel ID")
    print("-----------------------------------------------------------")
    TestUtils.send_command(f"{remove_app} with CHAN_ID 2", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_RM_APP_ERR_EID, "ERROR")

    ##***********************************************************************

    print("=================================================================")
    print(" 2. Add-application Directive error tests")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print(" 2.1 Add Application 2 in STARTED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {start_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STARTED'", 6)

    TestUtils.send_command(f"{add_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_ADD_APP_ERR_EID, "ERROR")

    print("-----------------------------------------------------------")
    print(" 2.2 Add Application 2 in STOPPED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {stop_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STOPPED'", 6)

    TestUtils.send_command(f"{add_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_ADD_APP_ERR_EID, "ERROR")
    
    # Remove Application 2  
    cmd(f"{target} {remove_app} with CHAN_ID 1")
    
    ##***********************************************************************

    print("=================================================================")
    print(" 3. Start-application Directive error tests")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print(" 3.1 Start Application 2 in STARTED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {add_app} with CHAN_ID 1")
    cmd(f"{target} {start_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STARTED'", 6)
    
    TestUtils.send_command(f"{start_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_START_APP_ERR_EID, "ERROR")

    print("-----------------------------------------------------------")
    print(" 3.2 Start Application 2 in REMOVED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {stop_app} with CHAN_ID 1")
    cmd(f"{target} {remove_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'REMOVED'", 6)

    TestUtils.send_command(f"{start_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    status = TestUtils.verify_event("BPNODE", BPLIB_NC_START_APP_ERR_EID, "ERROR")
    rqmnt = "DTN.6.12644"
    TestUtils.set_requirement_status(rqmnt, status)
    
    ##***********************************************************************

    print("=================================================================")
    print(" 4. Stop-application Directive error tests")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print(" 4.1 Stop Application 2 in STOPPED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {add_app} with CHAN_ID 1")
    cmd(f"{target} {start_app} with CHAN_ID 1")
    cmd(f"{target} {stop_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STOPPED'", 6)

    TestUtils.send_command(f"{stop_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_STOP_APP_ERR_EID, "ERROR")

    print("-----------------------------------------------------------")
    print(" 4.2 Stop Application 2 in REMOVED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {remove_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'REMOVED'", 6)

    TestUtils.send_command(f"{stop_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_STOP_APP_ERR_EID, "ERROR")

    ##***********************************************************************    

    print("=================================================================")
    print(" 5. Remove-application Directive error tests")
    print("=================================================================")

    print("-----------------------------------------------------------")
    print(" 5.1 Remove Application 2 in STARTED state")
    print("-----------------------------------------------------------")
    cmd(f"{target} {add_app} with CHAN_ID 1")
    cmd(f"{target} {start_app} with CHAN_ID 1")
    wait_check(f"{target} {chan_stat_pkt} CHAN_STAT_STATE_1 == 'STARTED'", 6)

    TestUtils.send_command(f"{remove_app} with CHAN_ID 1", TestUtils.INVALID_CMD_TYPE)
    TestUtils.verify_event("BPNODE", BPLIB_NC_RM_APP_ERR_EID, "ERROR")

    ##***********************************************************************

    print("=================================================================")
    print(" 6. Reset Counters Directives")
    print("=================================================================")
    
    print("-----------------------------------------------------------")
    print(" 6.1 RESET_COUNTER")
    print("-----------------------------------------------------------")

    TestUtils.reset_counter("BUNDLE_AGENT_REJECTED_DIRECTIVE_COUNT")
    TestUtils.reset_counter("ADU_COUNT_RECEIVED")
    TestUtils.reset_counter("BUNDLE_COUNT_GENERATED_ACCEPTED")
    
    print("-----------------------------------------------------------")
    print("6.2 RESET_ALL_COUNTERS")
    print("-----------------------------------------------------------")
    
    TestUtils.reset_counters("ALL")

    ##***********************************************************************


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

#bundle_creation_errors("xxx")
