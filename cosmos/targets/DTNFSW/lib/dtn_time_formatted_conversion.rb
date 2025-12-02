require 'cosmos/conversions/conversion'

module Cosmos
  # Convert a monotonic time counter and a correlation factor to DTN time and then convert
  # to a Ruby Time object (using the Unix epoch)
  class DtnTimeFormattedConversion < Conversion
    def initialize(monotonic_time, correlation_factor, monotonic_time_type = 'RAW', cf_type = 'RAW')
        super()
        @monotonic_time = monotonic_time
        @correlation_factor = correlation_factor
        @monotonic_time_type = monotonic_time_type.to_sym
        @cf_type = cf_type.to_sym

        @epoch_conversion = 946684800000  # Milliseconds between DTN epoch and Unix epoch

        @converted_bit_size = 0
        @converted_type = :RUBY_TIME
      end
      
    def call(value, packet, buffer)
      if packet.read(@correlation_factor, @cf_type, buffer) == 0
        return Time.at(0).sys
      else
        unix_time = packet.read(@monotonic_time, @monotonic_time_type, buffer) + 
                    packet.read(@correlation_factor, @cf_type, buffer) + @epoch_conversion
      
        return Time.at(unix_time / 1000, unix_time % 1000, :millisecond).sys
      end
    end

  end
end 
