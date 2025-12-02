require 'cosmos/interfaces/protocols/protocol'
module OpenC3
  class ProtocolEvsLog < Protocol
    def initialize(allow_empty_data = nil)
      super(allow_empty_data)
    end

    def read_packet(packet)
      # Need to make sure packet is identified and defined
      target_names = nil
      target_names = @interface.tlm_target_names if @interface

      # Telemetry packet items:
      #   "PACKET_TIMESECONDS", "PACKET_TIMEFORMATTED", "RECEIVED_TIMESECONDS",    "RECEIVED_TIMEFORMATTED",
      #   "RECEIVED_COUNT",     "PACKET_TIME",          "CCSDS_STREAMID",          "CCSDS_SEQUENCE",
      #   "CCSDS_LENGTH",       "SECONDS",              "SUBSECS",                 "PACKET_ID_APP_NAME",
      #   "PACKET_ID_EVENT_ID", "PACKET_ID_EVENT_TYPE", "PACKET_ID_SPACECRAFT_ID", "PACKET_ID_PROCESSOR_ID",
      #   "MESSAGE"
      identified_packet = System.telemetry.identify_and_define_packet(packet, target_names)

      if identified_packet
        if (identified_packet.packet_name == "CFE_EVS_LONG_EVENT_MSG")
          # Get the time for the packet, but convert it to a string and cut off the STCF
          packetTime = identified_packet.read_item(identified_packet.get_item("PACKET_TIME")).to_s
          packetTime = packetTime[0..-7]

          spacecraftID = identified_packet.read_item(identified_packet.get_item("PACKET_ID_SPACECRAFT_ID"))
          processorID  = identified_packet.read_item(identified_packet.get_item("PACKET_ID_PROCESSOR_ID"))
          appName      = identified_packet.read_item(identified_packet.get_item("PACKET_ID_APP_NAME"))
          eventID      = identified_packet.read_item(identified_packet.get_item("PACKET_ID_EVENT_ID"))
          message      = identified_packet.read_item(identified_packet.get_item("MESSAGE"))

          # Type of event, can be any of ("DEBUG"/1, "INFO"/2, "ERROR"/3, "CRITICAL"/4)
          eventType = identified_packet.read_item(identified_packet.get_item("PACKET_ID_EVENT_TYPE"))

          # General format for the event message in the COSMOS logger
          eventFormat = "#{packetTime} #{eventType} #{spacecraftID} #{processorID} #{appName} ID=#{eventID} #{message}"

          case eventType
          when "DEBUG", 1
            Logger.debug "#{eventFormat}"
          when "INFO", 2
            Logger.info "#{eventFormat}"
          when "ERROR", 3
            Logger.error "#{eventFormat}"
          when "CRITICAL", 4
            Logger.fatal "#{eventFormat}"
          else
            Logger.warn "Could not find event type for event, '#{eventFormat}'"
          end
        end
      else
        Logger.error "Could not identify packet"
      end

      return super(packet)
    end
  end
end
