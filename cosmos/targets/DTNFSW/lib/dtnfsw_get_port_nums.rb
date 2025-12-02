# DTN FSW Utility to Get Tlm / Cmd Ports

require 'dtnfsw_cmd_tlm_list.rb' # for cpu_num_or_nil
require 'dtnfsw_globals.rb'

# -------------------------------------------------------------------------------------------------
# Get the cmd port based on the cpu_num
# -------------------------------------------------------------------------------------------------
def dtnfsw_get_port_num_cmd(cpu_num_input)
  cpu_num = cpu_num_or_nil(cpu_num_input)
  port_num = 1234
  port_num_str = String.new
  if (cpu_num == nil)
      port_num_str << "dtnfsw_get_port_num_cmd error: could not convert #{cpu_num_input} to an integer"
  elsif ((cpu_num < 1) && (cpu_num > 8))
      port_num_str << "dtnfsw_get_port_num_cmd error: should be between 1 and 8 (inclusive). instead got: #{cpu_num}."
  else
      port_num = port_num + (cpu_num - 1)
      port_num_str << sprintf("%d", port_num)
  end
  return port_num_str
end


# -------------------------------------------------------------------------------------------------
# Get the tlm port based on the cpu_num
# -------------------------------------------------------------------------------------------------
def dtnfsw_get_port_num_tlm(cpu_num_input)
  cpu_num = cpu_num_or_nil(cpu_num_input)
  port_num = 2234
  port_num_str = String.new
  if (cpu_num == nil)
      port_num_str << "dtnfsw_get_port_num_tlm error: could not convert #{cpu_num_input} to an integer"
  elsif ((cpu_num < 1) && (cpu_num > 8))
      port_num_str << "dtnfsw_get_port_num_tlm error: should be between 1 and 8 (inclusive). instead got: #{cpu_num}."
  else
      port_num = port_num + (cpu_num - 1)
      port_num_str << sprintf("%d", port_num)
  end
  return port_num_str
end

# -------------------------------------------------------------------------------------------------
# Get the CLA in port based on the target name and the contact ID
# -------------------------------------------------------------------------------------------------
def dtnfsw_get_cla_in_port(target_name, contact_id)
    port_num = 0
    port_num_base = 4501
  
    cpu_num = dtnfsw_cpu_num_from_target_name(target_name)
  
    if ((cpu_num != nil) && (contact_id >= 0) && (contact_id < $dtnfsw_globals_num_contacts))
      port_num = port_num_base + (cpu_num - 1)    
    end
  
    return port_num  
  end
  

# -------------------------------------------------------------------------------------------------
# Get the CLA out port based on the target name and the contact ID
# -------------------------------------------------------------------------------------------------
def dtnfsw_get_cla_out_port(target_name, contact_id)
  port_num = 0
  port_num_base = 4551

  cpu_num = dtnfsw_cpu_num_from_target_name(target_name)

  if ((cpu_num != nil) && (contact_id >= 0) && (contact_id < $dtnfsw_globals_num_contacts))
    port_num = port_num_base + (cpu_num - 1)    
  end

  return port_num  
end
