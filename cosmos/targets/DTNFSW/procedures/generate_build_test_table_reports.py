#######################################################################
## generate_build_test_table_reports
##
## The binary files will be overwritten, so copy them to bin/reports/
#######################################################################

prompt("*** CAUTION - the binary files will be overwritten, copy them to bin/reports/")

#bin_dir = "DTNFSW-1/tables/target_node/bin/reports/"
bin_dir = "DTNFSW-1/tables/bin/reports/"
config_dir = "DTNFSW-1/tables/config/"

# Contacts table
def_file = "bpnode_contacts_def.txt"
for tbl_file in [
        "contact_nominal.tbl", 
        "contact_rx_only.tbl", 
        "cont_erate_lim.tbl",
        "cont_irate_lim.tbl", 
        "cont_inv_dest.tbl", 
        "cont_dup_dest.tbl", 
    ]:
    table = table_create_report(bin_dir+tbl_file, config_dir+def_file)
    #print(table)

# Channel table
def_file = "bpnode_channel_def.txt"
for tbl_file in [
        "chan_bad_crc.tbl", 
        "chan_dup_serv.tbl", 
        "chan_bad_sspdst.tbl", 
        "chan_bad_ssprep.tbl", 
        "chan_no_extblk.tbl", 
        "chan0_psize_10.tbl", 
    ]:
    table = table_create_report(bin_dir+tbl_file, config_dir+def_file)

# MIB PN table
def_file = "bpnode_mib_pn_def.txt"
for tbl_file in [
        "mib_short_life.tbl",
        "mib_pn_blen_0.tbl",
        "mib_pn_blen_200.tbl",
        "mib_pn_invplen.tbl",
    ]:
    table = table_create_report(bin_dir+tbl_file, config_dir+def_file)
