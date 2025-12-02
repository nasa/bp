require 'cosmos/interfaces/protocols/protocol'
module OpenC3
  class ProtocolCcsdsLength < Protocol
    SIZE_CCSDS_HDR_BYTES = 6
    ONE_FOR_DATA_STARTS_AT_0 = 1
    def write_packet(packet)
      # Update the length
      new_packet_length = packet.length - SIZE_CCSDS_HDR_BYTES - ONE_FOR_DATA_STARTS_AT_0
      packet.write('CCSDS_LENGTH', new_packet_length)
      # Return the updated packet
      return packet
    end
  end
end
