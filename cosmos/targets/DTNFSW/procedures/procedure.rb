# Script Runner test script
cmd("DTNFSW EXAMPLE")
wait_check("DTNFSW STATUS BOOL == 'FALSE'", 5)
