def dtnfsw_update_table(target_name, definition_file, binary_file, values_list, output_file)
    require 'openc3/tools/table_manager/table_config'

    def_dir = target_name+"/tables/config"
    bin_dir = target_name+"/tables/bin"

    # Get the definition file
    def_file = get_target_file("#{def_dir}/#{definition_file}")

    # Get the table name in COSMOS from definition file
    table_name_cosmos = def_file.read().split(' ')[1].strip

    # Access the internal TableConfig to process the definition
    config = OpenC3::TableConfig.process_file(def_file.path())

    # Grab the table by the table name in COSMOS
    table = config.table(table_name_cosmos)

    # Get the binary file contents
    bin_file = get_target_file("#{bin_dir}/#{binary_file}")
    table_bin = bin_file.read()

    # Copy binary file contents to table buffer for update
    table.buffer = table_bin

    # Update individual items in the table
    values_list.each do |item|
        table.write(item[0], item[1])
    end

    # Finally write table buffer (the binary) back to storage
    put_target_file("#{output_file}", table.buffer)
  end