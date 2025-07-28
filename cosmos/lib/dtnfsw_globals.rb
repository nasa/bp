# DTN FSW Globals

# -------------------------------------------------------------------------------------------------
# Defines the format of our DTNFSW target names
# -------------------------------------------------------------------------------------------------
def dtnfsw_target_name_from_cpu_num(cpu_num)
  return "DTNFSW-#{cpu_num}"
end

# -------------------------------------------------------------------------------------------------
# Gets the CPU number of the provided DTNFSW target name
# -------------------------------------------------------------------------------------------------
def dtnfsw_cpu_num_from_target_name(target_name)
  return Integer(target_name.delete("DTNFSW\-") || '')
rescue ArgumentError
    nil
end

# -------------------------------------------------------------------------------------------------
# EDS Enabled / Disabled Flag
# Must be boolean (`true` or `false`)
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_eds_enabled = false

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "true" or "false"
def dtnfsw_globals_set_eds_enabled(eds_enabled_input_string)
  if eds_enabled_input_string == "true"
    $dtnfsw_globals_eds_enabled = true
  else
    $dtnfsw_globals_eds_enabled = false
  end
end


# -------------------------------------------------------------------------------------------------
# FSW Target Memory Size
# Must be `32` or `64`
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_mem_size = 32

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "32" or "64"
def dtnfsw_globals_set_mem_size(mem_size_input_string)
  if mem_size_input_string == "64"
    $dtnfsw_globals_mem_size = 64
  else
    $dtnfsw_globals_mem_size = 32
  end
end


# -------------------------------------------------------------------------------------------------
# FSW Target's Processor Endianness
# Must be "LITTLE_ENDIAN" or "BIG_ENDIAN"
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_endianness = "LITTLE_ENDIAN"

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "true" or "false"
def dtnfsw_globals_set_endianness(endianness_input_string)
  if endianness_input_string == "BIG_ENDIAN"
    $dtnfsw_globals_endianness = "BIG_ENDIAN"
  else
    $dtnfsw_globals_endianness = "LITTLE_ENDIAN"
  end
end

# -------------------------------------------------------------------------------------------------
# Maximum number of channels BPNode is configured for
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_channels = 2

# -------------------------------------------------------------------------------------------------
# Maximum number of contacts BPNode is configured for
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_contacts = 1

# -------------------------------------------------------------------------------------------------
# Maximum number of EID patterns that can map to a source MIB set index
# Ref: BPLIB_MAX_NUM_EID_PATTERNS_PER_MIB_SET
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_eid_patterns_per_mib_set = 4

# -------------------------------------------------------------------------------------------------
# Maximum number of Destination EID Patterns per Contact BPNode is configured for
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_contact_eids = 3

# -------------------------------------------------------------------------------------------------
# Maximum number of authorized Source EID Patterns BPNode is configured for
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_auth_source_eids = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of authorized Custodian EID Patterns BPNode is configured for
# Ref: BPLIB_MAX_AUTH_CUSTODIAN_EIDS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_auth_custodian_eids = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of authorized Custody Source EID Patterns BPNode is configured for
# Ref: BPLIB_MAX_AUTH_CUSTODY_SOURCE_EIDS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_auth_custody_source_eids = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of authorized Report-To EID patterns BPNode is configured for
# Ref: BPLIB_MAX_AUTH_REPORT_TO_EIDS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_auth_report_to_eids = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of latency source table_entries BPNode is configured for
# Ref: BPLIB_MAX_NUM_LATENCY_POLICY_SETS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_latency_source_tbl_entries = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of EID patterns per latency policy table_entry
# This value is currently fixed at '1', because its not an array
# Need to update FSW to add something like "BPLIB_MAX_NUM_EIDS_PER_LATENCY_SET"
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_eids_per_src_latency_tbl_entry = 1

# -------------------------------------------------------------------------------------------------
# Maximum number of Compressed Reporting Signal (CRS) table entries BPNode is configured for
# Ref: BPLIB_MAX_NUM_CRS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_crs_tbl_entries = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of extension blocks BPNode is configured to support
# Ref: BPLIB_MAX_NUM_EXTENSION_BLOCK
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_extension_blks = 4

# -------------------------------------------------------------------------------------------------
# Maximum number of canonical blocks BPNode is configured to support
# Ref: BPLIB_MAX_NUM_EXTENSION_BLOCK + 1
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_canonical_blks = $dtnfsw_globals_num_extension_blks + 1

# -------------------------------------------------------------------------------------------------
# Maximum number of Destination EID patterns used per BPNode contact config table entry
# Ref: BPLIB_MAX_CONTACT_DEST_EIDS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_dest_eid_pats_per_contact = 3

# -------------------------------------------------------------------------------------------------
# Maximum number of source  for MIB Config per source
# Ref: BPLIB_MAX_NUM_MIB_SETS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_max_num_mib_sets = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of EID patterns for each MIB PS Cfg Table Entry
# Ref: BPLIB_MAX_NUM_MIB_PS_EID_PATTERNS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_eid_patterns_per_mib_ps_tbl_entry = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of storage allocation table entries
# Ref: BPLIB_MAX_NUM_STORE_SET
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_stor_alloc_tbl_entries = 10

# -------------------------------------------------------------------------------------------------
# Maximum number of EID patterns for each storage allocation entries
# Ref: BPLIB_MAX_NUM_STORE_EIDS
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_num_eid_patterns_per_stor_alloc_tbl_entry = 10

# -------------------------------------------------------------------------------------------------
# Destination EID's node number for contact 0 egress
# Ref: BPNODE_EID_NODE_NUM_FOR_CONTACT_0
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_contact_0_dest_eid_node = 200

# -------------------------------------------------------------------------------------------------
# Destination EID's service number for contact 0 egress
# Ref: BPNODE_EID_SERVICE_NUM_FOR_CONTACT_0
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_contact_0_dest_eid_service = 64

# -------------------------------------------------------------------------------------------------
# Destination EID's node number for egress to the local node
# Ref: BPLIB_EID_INSTANCE.Node
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_channel_dest_eid_node = 100

# -------------------------------------------------------------------------------------------------
# Destination EID's service number for channel 1 egress
# Ref: BPNODE_EID_SERVICE_NUM_FOR_CHANNEL_1
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_channel_1_dest_eid_service = 53

# -------------------------------------------------------------------------------------------------
# Service number for the local BPNode instance
# Ref: BPLIB_EID_INSTANCE.Service
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_local_service_num = 0

# -------------------------------------------------------------------------------------------------
# Size of BPLib's memory pool, in bytes
# Ref: BPNODE_MEM_POOL_LEN
# -------------------------------------------------------------------------------------------------
$dtnfsw_globals_mem_pool_len = 8000000