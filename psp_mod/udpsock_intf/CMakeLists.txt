# Pseudo-terminal interface module
add_psp_module(udpsock_intf udpsock_intf.c)
target_include_directories(udpsock_intf PRIVATE $<TARGET_PROPERTY:iodriver,INTERFACE_INCLUDE_DIRECTORIES>)
