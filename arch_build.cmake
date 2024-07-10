###########################################################
#
# BPNode platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the BPNode configuration
set(BPNODE_PLATFORM_CONFIG_FILE_LIST
  bpnode_internal_cfg.h
  bpnode_platform_cfg.h
  bpnode_perfids.h
  bpnode_msgids.h
)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BPNODE_CFGFILE ${BPNODE_PLATFORM_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${BPNODE_CFGFILE}" NAME_WE)
  if (DEFINED BPNODE_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${BPNODE_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${BPNODE_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${BPNODE_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
