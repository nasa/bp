project(CFS_BPNODE C)

set(APP_SRC_FILES
  # App files
  fsw/src/bpnode_app.c
  fsw/src/bpnode_cmds.c
  fsw/src/bpnode_utils.c

  # FWP files
  fsw/fwp/fwp_timep.c
  fsw/fwp/fwp_tablep.c
)

if (CFE_EDS_ENABLED_BUILD)
  list(APPEND APP_SRC_FILES
    fsw/src/bpnode_eds_dispatch.c
  )
else()
  list(APPEND APP_SRC_FILES
    fsw/src/bpnode_dispatch.c
  )
endif()

set(APP_TABLE_FILES
  fsw/tables/crs_tbl.c
  fsw/tables/adup_config_tbl.c
  fsw/tables/contacts_tbl.c
  fsw/tables/custodian_auth_policy_tbl.c
  fsw/tables/custody_auth_policy_tbl.c
  fsw/tables/channel_config_tbl.c
  fsw/tables/reportto_eid_auth_policy_tbl.c
  fsw/tables/src_auth_policy_tbl.c
  fsw/tables/src_latency_policy_tbl.c
  fsw/tables/storage_tbl.c
  fsw/tables/mib_config_pn_tbl.c
  fsw/tables/mib_config_ps_tbl.c
)

# Create the app module
add_cfe_app(bpnode ${APP_SRC_FILES})

target_include_directories(bpnode PUBLIC fsw/inc)
target_include_directories(bpnode PUBLIC fsw/src)
target_include_directories(bpnode PUBLIC fsw/fwp)

# Include bplib
add_cfe_app_dependency(bpnode bplib)

# Add table
add_cfe_tables(bpnode ${APP_TABLE_FILES})

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if (ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)
