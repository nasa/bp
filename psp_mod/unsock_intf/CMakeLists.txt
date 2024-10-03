
# Pseudo-terminal interface module
add_psp_module(unsock_intf unsock_intf.c)
target_include_directories(unsock_intf PRIVATE $<TARGET_PROPERTY:iodriver,INTERFACE_INCLUDE_DIRECTORIES>)
