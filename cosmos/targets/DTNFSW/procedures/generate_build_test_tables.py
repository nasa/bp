#######################################################################
# generate_build_test_tables
#
# Script(s) may need to be edited for different instances
#######################################################################

def generate_contact_tables():
    print("Generating contact tables ...")

    # contact definition and default binary files
    def_file = "bpnode_contacts_def.txt"
    bin_file = "bpnode_contacts.tbl"

    ## Edit as needed for local/test cosmos instance
    CONT_0_OUT_ADDR="10.2.8.43" # local
    bin_dir=""

    #CONT_0_OUT_ADDR = "10.2.8.151" # test
    #bin_dir = "target_node/"


    ## contact_nominal.tbl
    out_file = bin_dir+"contact_nominal.tbl"
    values_list = [
           ["CONT_0_OUT_ADDR",CONT_0_OUT_ADDR],
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## contact_rx_only.tbl
    out_file = bin_dir+"contact_rx_only.tbl"
    values_list = [
           ["CONT_0_DEST_ID_PAT_0_MAXNODE",0], 
           ["CONT_0_DEST_ID_PAT_0_MINNODE",0], 
           ["CONT_0_DEST_ID_PAT_0_MAXSERVICE",0], 
           ["CONT_0_DEST_ID_PAT_0_MINSERVICE",0], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## cont_irate_lim.tbl
    out_file = bin_dir+"cont_irate_lim.tbl"
    values_list = [
           ["CONT_0_DEST_ID_PAT_0_MAXNODE",0], 
           ["CONT_0_DEST_ID_PAT_0_MINNODE",0], 
           ["CONT_0_DEST_ID_PAT_0_MAXSERVICE",0], 
           ["CONT_0_DEST_ID_PAT_0_MINSERVICE",0], 
           ["CONT_0_INGRESS_BITS_PER_CYCLE",1000],
        ]   
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## cont_erate_lim.tbl
    out_file = bin_dir+"cont_erate_lim.tbl"
    values_list = [
           ["CONT_0_OUT_ADDR",CONT_0_OUT_ADDR],
           ["CONT_0_EGRESS_BITS_PER_CYCLE",10000],
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)

    '''
    CONT_0_DEST_ID_PAT_0_SCHEME	
    CONT_0_DEST_ID_PAT_0_IPNSSPFORMAT	
    CONT_0_DEST_ID_PAT_0_MAXALLOCATOR	
    CONT_0_DEST_ID_PAT_0_MINALLOCATOR	
    CONT_0_DEST_ID_PAT_0_MAXNODE	
    CONT_0_DEST_ID_PAT_0_MINNODE	
    CONT_0_DEST_ID_PAT_0_MAXSERVICE	
    CONT_0_DEST_ID_PAT_0_MINSERVICE	

    CONT_0_DEST_ID_PAT_1_SCHEME	
    CONT_0_DEST_ID_PAT_1_IPNSSPFORMAT	
    CONT_0_DEST_ID_PAT_1_MAXALLOCATOR	
    CONT_0_DEST_ID_PAT_1_MINALLOCATOR	
    CONT_0_DEST_ID_PAT_1_MAXNODE	
    CONT_0_DEST_ID_PAT_1_MINNODE	
    CONT_0_DEST_ID_PAT_1_MAXSERVICE	
    CONT_0_DEST_ID_PAT_1_MINSERVICE	

    CONT_0_DEST_ID_PAT_2_SCHEME	
    CONT_0_DEST_ID_PAT_2_IPNSSPFORMAT	
    CONT_0_DEST_ID_PAT_2_MAXALLOCATOR	
    CONT_0_DEST_ID_PAT_2_MINALLOCATOR	
    CONT_0_DEST_ID_PAT_2_MAXNODE	
    CONT_0_DEST_ID_PAT_2_MINNODE	
    CONT_0_DEST_ID_PAT_2_MAXSERVICE	
    CONT_0_DEST_ID_PAT_2_MINSERVICE	

    CONT_0_CLA_TYPE	
    CONT_0_IN_ADDR	
    CONT_0_OUT_ADDR	
    CONT_0_IN_PORT	
    CONT_0_OUT_PORT	
    CONT_0_DEST_LTP_ENG_ID	
    CONT_0_SEND_BYTES_PER_CYCLE	
    CONT_0_RECV_BYTES_PER_CYCLE	
    CONT_0_RETRANSMIT_TIMEOUT	
    CONT_0_CS_TIME_TRIGGER	
    CONT_0_CS_SIZE_TRIGGER
    CONT_0_INGRESS_BITS_PER_CYCLE 
    CONT_0_EGRESS_BITS_PER_CYCLE 
    '''


def generate_channel_tables():
    print("Generating channel_tables ...")

    # channel definition and default binary files
    def_file = "bpnode_channel_def.txt"
    bin_file = "bpnode_channel.tbl"

    ## chan_no_extblk.tbl
    out_file = "chan_no_extblk.tbl"
    values_list = [
           ["CHAN_0_PREVNODEBLKCONFIG_INCLUDEBLOCK","FALSE"], 
           ["CHAN_0_AGEBLKCONFIG_INCLUDEBLOCK","FALSE"], 
           ["CHAN_0_PREVNODEBLKCONFIG_INCLUDEBLOCK","FALSE"], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## chan_bad_crc.tbl
    out_file = "chan_bad_crc.tbl"
    values_list = [
           ["CHAN_0_CRC_TYPE",0], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## chan_dup_serv.tbl
    out_file = "chan_dup_serv.tbl"
    values_list = [
           ["CHAN_1_LOCAL_SERVICE_NUMBER",42], 
        ]   
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## chan_bad_sspdst.tbl
    out_file = "chan_bad_sspdst.tbl"
    values_list = [
           ["CHAN_0_DESTINATION_EID_ALLOCATOR",1],
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## chan_bad_ssprep.tbl
    out_file = "chan_bad_ssprep.tbl"
    values_list = [
           ["CHAN_0_REPORT_TO_EID_IPNSSPFORMAT",3],
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)

    ## chan0_psize_10.tbl
    out_file = "chan0_psize_10.tbl"
    values_list = [
           ["CHAN_0_MAX_BUNDLE_PAYLOAD_SIZE",10],
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)

    '''
    CHAN_0_ADD_AUTOMATICALLY                   
    CHAN_0_REQUEST_CUSTODY                     
    CHAN_0_ADU_WRAPPING                        
    CHAN_0_ADU_UNWRAPPING                      
    CHAN_0_REG_STATE                           
    CHAN_0_HOP_LIMIT                           
    CHAN_0_CRC_TYPE                            
    CHAN_0_SPARE                               
    CHAN_0_INGRESS_BITS_PER_CYCLE              
    CHAN_0_EGRESS_BITS_PER_CYCLE               
    CHAN_0_LOCAL_SERVICE_NUMBER                
    CHAN_0_MAX_BUNDLE_PAYLOAD_SIZE             
    CHAN_0_BUNDLE_PROC_FLAGS                   
    CHAN_0_LIFETIME                            
    CHAN_0_DESTINATION_EID_SCHEME              
    CHAN_0_DESTINATION_EID_IPNSSPFORMAT        
    CHAN_0_DESTINATION_EID_ALLOCATOR           
    CHAN_0_DESTINATION_EID_NODE                
    CHAN_0_DESTINATION_EID_SERVICE             
    CHAN_0_REPORT_TO_EID_SCHEME                
    CHAN_0_REPORT_TO_EID_IPNSSPFORMAT          
    CHAN_0_REPORT_TO_EID_ALLOCATOR             
    CHAN_0_REPORT_TO_EID_NODE                  
    CHAN_0_REPORT_TO_EID_SERVICE               
    CHAN_0_PREVNODEBLKCONFIG_INCLUDEBLOCK      
    CHAN_0_PREVNODEBLKCONFIG_CRC_TYPE          
    CHAN_0_PREVNODEBLKCONFIG_SPARE             
    CHAN_0_PREVNODEBLKCONFIG_BLOCK_NUM         
    CHAN_0_PREVNODEBLKCONFIG_BLOCK_PROC_FLAGS  
    CHAN_0_AGEBLKCONFIG_INCLUDEBLOCK           
    CHAN_0_AGEBLKCONFIG_CRC_TYPE               
    CHAN_0_AGEBLKCONFIG_SPARE                  
    CHAN_0_AGEBLKCONFIG_BLOCK_NUM              
    CHAN_0_AGEBLKCONFIG_BLOCK_PROC_FLAGS       
    CHAN_0_HOPCOUNTBLKCONFIG_INCLUDEBLOCK      
    CHAN_0_HOPCOUNTBLKCONFIG_CRC_TYPE          
    CHAN_0_HOPCOUNTBLKCONFIG_SPARE             
    CHAN_0_HOPCOUNTBLKCONFIG_BLOCK_NUM         
    CHAN_0_HOPCOUNTBLKCONFIG_BLOCK_PROC_FLAGS  
    CHAN_0_PAYLOADBLKCONFIG_INCLUDEBLOCK       
    CHAN_0_PAYLOADBLKCONFIG_CRC_TYPE           
    CHAN_0_PAYLOADBLKCONFIG_SPARE              
    CHAN_0_PAYLOADBLKCONFIG_BLOCK_NUM          
    CHAN_0_PAYLOADBLKCONFIG_BLOCK_PROC_FLAGS   
    CHAN_1_ADD_AUTOMATICALLY                   
    CHAN_1_REQUEST_CUSTODY                     
    CHAN_1_ADU_WRAPPING                        
    CHAN_1_ADU_UNWRAPPING                      
    CHAN_1_REG_STATE                           
    CHAN_1_HOP_LIMIT                           
    CHAN_1_CRC_TYPE                            
    CHAN_1_SPARE                               
    CHAN_1_INGRESS_BITS_PER_CYCLE              
    CHAN_1_EGRESS_BITS_PER_CYCLE               
    CHAN_1_LOCAL_SERVICE_NUMBER                
    CHAN_1_MAX_BUNDLE_PAYLOAD_SIZE             
    CHAN_1_BUNDLE_PROC_FLAGS                   
    CHAN_1_LIFETIME                            
    CHAN_1_DESTINATION_EID_SCHEME              
    CHAN_1_DESTINATION_EID_IPNSSPFORMAT        
    CHAN_1_DESTINATION_EID_ALLOCATOR           
    CHAN_1_DESTINATION_EID_NODE                
    CHAN_1_DESTINATION_EID_SERVICE             
    CHAN_1_REPORT_TO_EID_SCHEME                
    CHAN_1_REPORT_TO_EID_IPNSSPFORMAT          
    CHAN_1_REPORT_TO_EID_ALLOCATOR             
    CHAN_1_REPORT_TO_EID_NODE                  
    CHAN_1_REPORT_TO_EID_SERVICE               
    CHAN_1_PREVNODEBLKCONFIG_INCLUDEBLOCK      
    CHAN_1_PREVNODEBLKCONFIG_CRC_TYPE          
    CHAN_1_PREVNODEBLKCONFIG_SPARE             
    CHAN_1_PREVNODEBLKCONFIG_BLOCK_NUM         
    CHAN_1_PREVNODEBLKCONFIG_BLOCK_PROC_FLAGS  
    CHAN_1_AGEBLKCONFIG_INCLUDEBLOCK           
    CHAN_1_AGEBLKCONFIG_CRC_TYPE               
    CHAN_1_AGEBLKCONFIG_SPARE                  
    CHAN_1_AGEBLKCONFIG_BLOCK_NUM              
    CHAN_1_AGEBLKCONFIG_BLOCK_PROC_FLAGS       
    CHAN_1_HOPCOUNTBLKCONFIG_INCLUDEBLOCK      
    CHAN_1_HOPCOUNTBLKCONFIG_CRC_TYPE          
    CHAN_1_HOPCOUNTBLKCONFIG_SPARE             
    CHAN_1_HOPCOUNTBLKCONFIG_BLOCK_NUM         
    CHAN_1_HOPCOUNTBLKCONFIG_BLOCK_PROC_FLAGS  
    CHAN_1_PAYLOADBLKCONFIG_INCLUDEBLOCK       
    CHAN_1_PAYLOADBLKCONFIG_CRC_TYPE           
    CHAN_1_PAYLOADBLKCONFIG_SPARE              
    CHAN_1_PAYLOADBLKCONFIG_BLOCK_NUM          
    CHAN_1_PAYLOADBLKCONFIG_BLOCK_PROC_FLAGS   
    '''
    

def generate_mib_tables():
    print("Generating mib_tables ...")

    # MIB definition and default binary files
    def_file = "bpnode_mib_pn_def.txt"
    bin_file = "bpnode_mib_pn.tbl"

    ## mib_pn_blen_0.tbl
    out_file = "mib_pn_blen_0.tbl"
    values_list = [
           ["PARAM_SET_MAX_PAYLOAD_LENGTH",0], 
           ["PARAM_SET_MAX_BUNDLE_LENGTH",0], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)
    

    ## mib_pn_blen_200.tbl
    out_file = "mib_pn_blen_200.tbl"
    values_list = [
           ["PARAM_SET_MAX_PAYLOAD_LENGTH",100], 
           ["PARAM_SET_MAX_BUNDLE_LENGTH",200], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## mib_pn_invplen.tbl
    out_file = "mib_pn_invplen.tbl"
    values_list = [
           ["PARAM_SET_MAX_PAYLOAD_LENGTH",4096], 
           ["PARAM_SET_MAX_BUNDLE_LENGTH",4000], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)


    ## mib_short_life.tbl
    out_file = "mib_short_life.tbl"
    values_list = [
           ["PARAM_SET_MAX_LIFETIME",60000], 
        ]
    TestUtils.update_table(def_file, bin_file, values_list, out_file)

    '''
    INSTANCE_EID_SCHEME                 
    INSTANCE_EID_IPNSSPFORMAT           
    INSTANCE_EID_ALLOCATOR              
    INSTANCE_EID_NODE                   
    INSTANCE_EID_SERVICE                
    PARAM_BUNDLE_SIZE_NO_FRAGMENT       
    PARAM_SET_MAX_SEQUENCE_NUM          
    PARAM_SET_MAX_PAYLOAD_LENGTH        
    PARAM_SET_MAX_BUNDLE_LENGTH         
    PARAM_SET_NODE_DTN_TIME             
    PARAM_SET_BEHAVIOR_EVENT_REPORTING  
    PARAM_SET_MAX_LIFETIME            
    '''
#######################################################################

# main
   
load_utility('DTNFSW-1/procedures/build_tests/test_utils.py')

prompt("CAUTION - Script may need to be edited for different instances")

set_line_delay(0)

table = ask("Which table(s)?\n\n- all\n- contact\n- channel\n- mib")

table = table.upper()

if table == 'CONTACT' or table == 'ALL':
    generate_contact_tables()

if table == 'CHANNEL' or table == 'ALL':
    generate_channel_tables()

if table == 'MIB' or table == 'ALL':
    generate_mib_tables()


