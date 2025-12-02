# -------------------------------------------------------------------------------------------------
# Set screen title colors for target number
# -------------------------------------------------------------------------------------------------
def dtnfsw_target_num_color(dtnfsw_target_name)
    case (dtnfsw_target_name)
    when "DTNFSW-5"
        target_color = "GREY"
    when "DTNFSW-4"
        target_color = "PURPLE"
    when "DTNFSW-3"
        target_color = "BLUE"
    when "DTNFSW-2"
        target_color = "BROWN"
    else # (default, usually "DTNFSW-1")
        target_color = "GREEN"
    end
    return target_color
end
