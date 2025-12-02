# This file contains String Generation Utilities for COSMOS Parameter Definition

# Generates set of table parameters to represent the BPLib_EID_t struct
def dtnfsw_gen_tbl_param_eid_t(field_name_prefix)
  tblSnippet = String.new
  tblSnippet << "      APPEND_PARAMETER #{field_name_prefix}_Scheme 64 UINT 1 2 2 \n"
  tblSnippet << "       STATE DTN_SCHEME 1 \n"
  tblSnippet << "       STATE IPN_SCHEME 2 \n"
  tblSnippet << "      APPEND_PARAMETER #{field_name_prefix}_IpnSspFormat 64 UINT 2 3 2 \n"
  tblSnippet << "       STATE IPN_SSP_FORMAT_TWO_DIGIT 2 \n"
  tblSnippet << "       STATE IPN_SSP_FORMAT_THREE_DIGIT 3 \n"
  tblSnippet << "      APPEND_PARAMETER #{field_name_prefix}_Allocator 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "      APPEND_PARAMETER #{field_name_prefix}_Node 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "      APPEND_PARAMETER #{field_name_prefix}_Service 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  return tblSnippet
end

# Generates set of table parameters to represent the BPLib_EID_PatternMatch_t struct
def dtnfsw_gen_tbl_param_eid_pattern_t(field_name_prefix)
  tblSnippet = String.new
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_Scheme 64 UINT 1 2 2 \n"
  tblSnippet << "       STATE DTN_SCHEME 1 \n"
  tblSnippet << "       STATE IPN_SCHEME 2 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_IpnSspFormat 64 UINT 2 3 2 \n"
  tblSnippet << "       STATE IPN_SSP_FORMAT_TWO_DIGIT 2 \n"
  tblSnippet << "       STATE IPN_SSP_FORMAT_THREE_DIGIT 3 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MaxAllocator 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MinAllocator 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MaxNode 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MinNode 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MaxService 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_MinService 64 UINT 0 0xFFFFFFFFFFFFFFFF 0 \n"
  return tblSnippet
end

def dtnfsw_gen_cmd_param_eid_pattern_t(field_name_prefix)
  return dtnfsw_gen_tbl_param_eid_pattern_t(field_name_prefix)
end

# Generates set of telemetry parameters to represent the BPLib_EID_PatternMatch_t struct
def dtnfsw_gen_tlm_param_eid_pattern_t(field_name_prefix)
  tlmSnippet = String.new
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_Scheme 64 UINT ''\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_IpnSspFormat 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MaxAllocator 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MinAllocator 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MaxNode 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MinNode 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MaxService 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_MinService 64 UINT\n"
  tlmSnippet << "    ITEM #{field_name_prefix}_EidStr 0 0 DERIVED  \n"
  tlmSnippet << "      READ_CONVERSION eid_pattern_conversion.rb #{field_name_prefix}_Scheme " +
                              "#{field_name_prefix}_IpnSspFormat #{field_name_prefix}_MaxAllocator " +
                              "#{field_name_prefix}_MaxNode #{field_name_prefix}_MaxService " +
                              "#{field_name_prefix}_MinAllocator #{field_name_prefix}_MinNode " +
                              "#{field_name_prefix}_MinService \n"  
  return tlmSnippet
end

# Generates set of telemetry parameters to represent the BPLib_EID_t struct
def dtnfsw_gen_tlm_param_eid_t(field_name_prefix)
  tlmSnippet = String.new
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_Scheme 64 UINT ''\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_IpnSspFormat 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_Allocator 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_Node 64 UINT\n"
  tlmSnippet << "    APPEND_ITEM #{field_name_prefix}_Service 64 UINT\n"
  tlmSnippet << "    ITEM #{field_name_prefix}_EidStr 0 0 DERIVED  \n"
  tlmSnippet << "      READ_CONVERSION eid_pattern_conversion.rb #{field_name_prefix}_Scheme " +
                              "#{field_name_prefix}_IpnSspFormat #{field_name_prefix}_Allocator " +
                              "#{field_name_prefix}_Node #{field_name_prefix}_Service \n"  
  return tlmSnippet
end


# Generates set of table parameters to represent the BPLib_PI_CanBlkConfig_t struct
def dtnfsw_gen_tbl_param_can_blk_cfg_t(field_name_prefix)
  tblSnippet = String.new
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_IncludeBlock 8 UINT 0 1 0 \n"
  tblSnippet << "            STATE FALSE 0 \n"
  tblSnippet << "            STATE TRUE  1 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_Crc_Type 8 UINT 0 2 1 \n"
  tblSnippet << "            STATE NONE   0 \n"
  tblSnippet << "            STATE CRC_16 1 \n"
  tblSnippet << "            STATE CRC_32 2 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_Spare 16 UINT 0 0 0 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_Block_Num 32 UINT 0  0xFFFFFFFF 1 \n"
  tblSnippet << "    APPEND_PARAMETER #{field_name_prefix}_Block_Proc_Flags 64 UINT 0 0xFFFFFFFF 1 \n"
  tblSnippet << "        FORMAT_STRING \"0x%016X\" \n"
  return tblSnippet
end


