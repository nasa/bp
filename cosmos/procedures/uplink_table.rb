load_utility("DTNFSW-1/procedures/dtnfsw_update_table.rb")
load_utility("DTNFSW-1/procedures/load_new_table.rb")

tbl_bin_file = ask("Table binary file name: ")
tbl_def_file = tbl_bin_file.split(/[.]/)[0] + "_def.txt"

printf("tbl def file: %s", tbl_def_file)

all_values = Array.new
value_input = ask('Comma separated name and value (Ex: CONTACT_0_CLA_IN_PORT, 4501). "Done" when done')
while value_input.upcase != 'DONE'
    value_split = value_input.split(/[,]/)
    value_split[0] = value_split[0].strip
    value_split[1] = value_split[1].strip

    IsInt = Integer(value_split[1]) rescue false
    if IsInt
        value_split[1] = value_split[1].to_f
    end

    all_values.push(value_split)

    value_input = ask('Comma separated name and value (Ex: CONTACT_0_CLA_IN_PORT, 4501). "Done" when done')
end

dtnfsw_update_table("DTNFSW-1",
                    tbl_def_file,
                    tbl_bin_file,
                    all_values,
                    "#{tbl_bin_file}")

ask("In a shell run, `cp <cosmos location>/plugins/DEFAULT/targets_modified/#{tbl_bin_file} <dtn-cfs location>/build/exe/cpu1/cf`, then press enter to continue")

load_new_table("/cf/#{tbl_bin_file}")