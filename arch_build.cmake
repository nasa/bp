###########################################################
#
# BP App platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the BP configuration
set(BP_PLATFORM_CONFIG_FILE_LIST
  bp_internal_cfg.h
  bp_platform_cfg.h
  bp_msgids.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, msgids come generated from the EDS tool
  set(BP_CFGFILE_SRC_bp_msgids "${CMAKE_CURRENT_LIST_DIR}/config/bp_eds_msg_topicids.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BP_CFGFILE ${BP_PLATFORM_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${BP_CFGFILE}" NAME_WE)
  if (DEFINED BP_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE "${BP_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE "${CMAKE_CURRENT_LIST_DIR}/config/default_${BP_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${BP_CFGFILE}"
    FALLBACK_FILE       ${DEFAULT_SOURCE}
  )
endforeach()
