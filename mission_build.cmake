###########################################################
#
# BP App mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the BP configuration
set(BP_MISSION_CONFIG_FILE_LIST
  bp_fcncodes.h
  bp_interface_cfg.h
  bp_msg.h
  bp_msgstruct.h
  bp_topicids.h
  bp_tbl.h
  bp_tblstruct.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, these files come generated from the EDS tool
  set(BP_CFGFILE_SRC_bp_interface_cfg "bp_eds_designparameters.h")
  set(BP_CFGFILE_SRC_bp_fcncodes      "bp_eds_cc.h")
  set(BP_CFGFILE_SRC_bp_msgstruct     "bp_eds_typedefs.h")
  set(BP_CFGFILE_SRC_bp_tblstruct     "bp_eds_typedefs.h")
  set(BP_CFGFILE_SRC_bp_topicids      "bp_eds_designparameters.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BP_CFGFILE ${BP_MISSION_CONFIG_FILE_LIST})
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
