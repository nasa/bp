require 'dtnfsw_globals.rb'
require 'dtnfsw_cmd_tlm_list.rb'
require 'unix_time_conversion_epoch_offset.rb'

# Output a CCSDS Command Packet with the GSFC Secondary Header (Command Code and Checksum fields)
def dtnfsw_cmd_hdr(target_name, cmd_name, cpu_num, func_code, pkt_desc)
    cmdPacket = String.new
    cmdPacket << "COMMAND #{target_name} #{cmd_name} #{$dtnfsw_globals_endianness} \"#{pkt_desc}\" \n"
    cmdPacket << "   APPEND_ID_PARAMETER CCSDS_STREAMID 16 UINT MIN_UINT16 MAX_UINT16 #{get_dtnfsw_pkt_msg_id(cmd_name, cpu_num)} \"CCSDS Packet Identification\" BIG_ENDIAN \n"
    cmdPacket << "      FORMAT_STRING \"0x%04X\"\n"
    cmdPacket << "   APPEND_PARAMETER CCSDS_SEQUENCE 16 UINT MIN_UINT16 MAX_UINT16 0xC000 \"CCSDS Packet Sequence Control\" BIG_ENDIAN \n"
    cmdPacket << "      FORMAT_STRING \"0x%04X\"\n"
    cmdPacket << "   APPEND_PARAMETER CCSDS_LENGTH 16 UINT MIN_UINT16 MAX_UINT16 1 \"CCSDS Packet Data Length\" BIG_ENDIAN \n"
    cmdPacket << "   APPEND_PARAMETER CCSDS_FC 8 UINT MIN_UINT8 MAX_UINT8 #{func_code} \"CCSDS Command Function Code\" \n"
    cmdPacket << "   APPEND_PARAMETER CCSDS_CHECKSUM 8 UINT MIN_UINT8 MAX_UINT8 0 \"CCSDS Command Checksum\" \n"
    return cmdPacket
end

# Output a CCSDS Telemetry Packet with the GSFC Secondary Header (timestamp)
def dtnfsw_tlm_hdr(target_name, tlm_name, cpu_num, pkt_desc)
    tlmPacket = String.new
    tlmPacket << "TELEMETRY #{target_name} #{tlm_name} #{$dtnfsw_globals_endianness} \"#{pkt_desc}\" \n"
    tlmPacket << "     APPEND_ID_ITEM CCSDS_STREAMID       16 UINT #{get_dtnfsw_pkt_msg_id(tlm_name, cpu_num)}  \"CCSDS Packet Identification\" BIG_ENDIAN \n"
    tlmPacket << "      FORMAT_STRING \"0x%04X\"\n"
    tlmPacket << "     APPEND_ITEM CCSDS_SEQUENCE 16 UINT \"CCSDS Packet Sequence Control\" BIG_ENDIAN \n"
    tlmPacket << "      FORMAT_STRING \"0x%04X\"\n"
    tlmPacket << "     APPEND_ITEM    CCSDS_LENGTH         16 UINT         \"CCSDS Packet Data Length\" BIG_ENDIAN \n"
    tlmPacket << "     APPEND_ITEM    SECONDS              32 UINT         \"CCSDS Telemetry Secondary Header (seconds)\" BIG_ENDIAN \n"
    tlmPacket << "     APPEND_ITEM    SUBSECS              16 UINT         \"CCSDS Telemetry Secondary Header (subseconds)\" BIG_ENDIAN \n"
    if $dtnfsw_globals_eds_enabled == false
    tlmPacket << "     APPEND_ITEM    SPARE_FOR_64_ALIGN   32 UINT         \"Spare padding for 64-bit alignment\" \n"
    end
    tlmPacket << "     ITEM PACKET_TIME 0 0 DERIVED \"Ruby time based on SECONDS and SUBSECS\" \n"
    tlmPacket << "       READ_CONVERSION unix_time_conversion_epoch_offset.rb SECONDS SUBSECS \n"
    return tlmPacket
end

# Output a cFS Table Header
# usage: put this into your table def.txt files, before the start of your table-specific content
# <%= dtnfsw_tbl_hdr('Contacts_Table', 'Contacts Config Table', 'BPNODE', 'ContactsTable') %>
def dtnfsw_tbl_hdr(tbl_name, tbl_description, cfs_app_name, cfs_tbl_name)
    tblPacket = String.new
    tblPacket << "TABLE #{tbl_name} #{$dtnfsw_globals_endianness} KEY_VALUE \" #{tbl_description} \" \n"
    tblPacket << "    APPEND_PARAMETER \"Content_Type\" 32 UINT 0 0xFFFFFFFF 0x63464531 \"Content Type\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Sub_Type\" 32 UINT 0 0xFFFFFFFF 0x00000008 \"Sub-Type\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Length\" 32 UINT 0 0xFFFFFFFF 0xdeadbeef \"Length\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Spacecraft_ID\" 32 UINT 0 0xFFFFFFFF 0x00000042 \"Spacecraft ID\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Processor_ID\" 32 UINT 0 0xFFFFFFFF 0 \"Processor ID\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Process_ID\" 32 UINT 0 0xFFFFFFFF 0 \"Application ID\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"CreateTime_Seconds\" 32 UINT 0 0xFFFFFFFF 0 \"Create Time: Seconds\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"CreateTime_Subseconds\" 32 UINT 0 0xFFFFFFFF 0 \"Create Time: SubSeconds\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Description\" 256 STRING \"#{tbl_description}\" \n"
    tblPacket << "    APPEND_PARAMETER \"reserved\" 32 UINT 0 0xFFFFFFFF 0 \"Reserved\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"byteOffset\" 32 UINT 0 0xFFFFFFFF 0 \"Offset\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"numBytes\" 32 UINT 0 0xFFFFFFFF 0xcafef00d \"Number of Bytes\" BIG_ENDIAN \n"
    tblPacket << "      FORMAT_STRING \"0x%08X\" \n"
    tblPacket << "    APPEND_PARAMETER \"Table_Name\" 320 STRING \"#{cfs_app_name}.#{cfs_tbl_name}\" \n"
    tblPacket << "\n"
    return tblPacket
end
