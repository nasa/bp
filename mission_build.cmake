###########################################################
#
# BPNode mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the BPNode configuration
set(BPNODE_MISSION_CONFIG_FILE_LIST
  bpnode_fcncodes.h
  bpnode_interface_cfg.h
  bpnode_mission_cfg.h
  bpnode_perfids.h
  bpnode_msg.h
  bpnode_msgdefs.h
  bpnode_msgstruct.h
  bpnode_tbl.h
  bpnode_tbldefs.h
  bpnode_tblstruct.h
  bpnode_topicids.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, these files come generated from the EDS tool
  set(BPNODE_CFGFILE_SRC_bpnode_interface_cfg "bpnode_eds_designparameters.h")
  set(BPNODE_CFGFILE_SRC_bpnode_tbldefs       "bpnode_eds_typedefs.h")
  set(BPNODE_CFGFILE_SRC_bpnode_tblstruct     "bpnode_eds_typedefs.h")
  set(BPNODE_CFGFILE_SRC_bpnode_msgdefs       "bpnode_eds_typedefs.h")
  set(BPNODE_CFGFILE_SRC_bpnode_msgstruct     "bpnode_eds_typedefs.h")
  set(BPNODE_CFGFILE_SRC_bpnode_fcncodes      "bpnode_eds_cc.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BPNODE_CFGFILE ${BPNODE_MISSION_CONFIG_FILE_LIST})
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
