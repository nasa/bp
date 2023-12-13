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

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BP_CFGFILE ${BP_PLATFORM_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${BP_CFGFILE}" NAME_WE)
  if (DEFINED BP_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${BP_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${BP_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${BP_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
