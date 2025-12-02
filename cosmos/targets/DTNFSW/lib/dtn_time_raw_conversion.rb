require 'cosmos/conversions/conversion'

module Cosmos
  # Convert a monotonic time counter and a correlation factor to DTN time
  class DtnTimeRawConversion < Conversion
    def initialize(monotonic_time, correlation_factor, monotonic_time_type = 'RAW', cf_type = 'RAW')
        super()
        @monotonic_time = monotonic_time
        @correlation_factor = correlation_factor
        @monotonic_time_type = monotonic_time_type.to_sym
        @cf_type = cf_type.to_sym

        @converted_bit_size = 64
        @converted_type = :FLOAT
      end
      
    def call(value, packet, buffer)
      dtn_time = 0
      if packet.read(@correlation_factor, @cf_type, buffer) != 0
        dtn_time = packet.read(@monotonic_time, @monotonic_time_type, buffer) + 
                    packet.read(@correlation_factor, @cf_type, buffer)
      end
      
      return dtn_time.to_f
    end
  end
end 
