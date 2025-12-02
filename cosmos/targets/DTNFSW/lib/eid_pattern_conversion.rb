# encoding: ascii-8bit

# Copyright 2022 Ball Aerospace & Technologies Corp.
# All Rights Reserved.
#
# This program is free software; you can modify and/or redistribute it
# under the terms of the GNU Affero General Public License
# as published by the Free Software Foundation; version 3 with
# attribution addendums as found in the LICENSE.txt
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.

# Modified by OpenC3, Inc.
# All changes Copyright 2022, OpenC3, Inc.
# All Rights Reserved
#
# This file may also be used under the terms of a commercial license
# if purchased from OpenC3, Inc.

require 'openc3/conversions/conversion'

module OpenC3
  # Converts an EID struct (either regular or pattern) to a human-readable string
  class EidPatternConversion < Conversion
    # Initializes the time item to grab from the packet
    #
    # @param scheme [String] The telemetry item in the packet representing the EID scheme
    # @param format [String] The telemetry item in the packet representing the EID format
    # @param max_alloc [String] The telemetry item in the packet representing the max allocator 
    # @param max_node [String] The telemetry item in the packet representing the max node number
    # @param max_serv [String] The telemetry item in the packet representing the max service number 
    # @param min_alloc [String] The telemetry item in the packet representing the min allocator
    # @param min_node [String] The telemetry item in the packet representing the min node number
    # @param min_serv [String] The telemetry item in the packet representing the min service number
    def initialize(scheme, format, max_alloc, max_node, max_serv, 
                                   min_alloc=nil, min_node=nil,  min_serv=nil)
      super()
      @scheme_item_name = scheme
      @scheme_type = 'RAW'.to_sym
      @format_item_name = format
      @format_type = 'RAW'.to_sym
      @max_alloc_item_name = max_alloc
      @max_alloc_type = 'RAW'.to_sym
      @min_alloc_item_name = min_alloc
      @min_alloc_type = 'RAW'.to_sym
      @max_node_item_name = max_node
      @max_node_type = 'RAW'.to_sym
      @min_node_item_name = min_node
      @min_node_type = 'RAW'.to_sym
      @max_serv_item_name = max_serv
      @max_serv_type = 'RAW'.to_sym
      @min_serv_item_name = min_serv
      @min_serv_type = 'RAW'.to_sym

      @converted_type = :STRING
      @converted_bit_size = 0
    end

    def get_one_field(max_item_name, max_type, min_item_name, min_type, packet, buffer)
      if min_item_name == nil
        return "#{packet.read(max_item_name, max_type, buffer)}"
      elsif packet.read(min_item_name, min_type, buffer) == packet.read(max_item_name, max_type, buffer)
        return "#{packet.read(max_item_name, max_type, buffer)}"
      else
        return "#{packet.read(min_item_name, min_type, buffer)}-#{packet.read(max_item_name, max_type, buffer)}"
      end
    end

    # @param (see Conversion#call)
    # @return [String] EID pattern as a string
    def call(value, packet, buffer)
      # DTN scheme (only for dtn:none)
      if packet.read(@scheme_item_name, @scheme_type, buffer) == 1
        return "dtn:none"
      # Three digit IPN SSP format
      elsif packet.read(@format_item_name, @format_type, buffer) == 3
        return "ipn:#{get_one_field(@max_alloc_item_name, @max_alloc_type, @min_alloc_item_name, @min_alloc_type, packet, buffer)}." +
               "#{get_one_field(@max_node_item_name, @max_node_type, @min_node_item_name, @min_node_type, packet, buffer)}." +
               "#{get_one_field(@max_serv_item_name, @max_serv_type, @min_serv_item_name, @min_serv_type, packet, buffer)}"
      # Two digit IPN SSP format
      else
        return "ipn:#{get_one_field(@max_node_item_name, @max_node_type, @min_node_item_name, @min_node_type, packet, buffer)}." +
               "#{get_one_field(@max_serv_item_name, @max_serv_type, @min_serv_item_name, @min_serv_type, packet, buffer)}"
      end
    end
  end
end
