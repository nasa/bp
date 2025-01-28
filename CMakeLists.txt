project(CFS_BPNODE C)

set(APP_SRC_FILES
  # App files
  fsw/src/bpnode_app.c
  fsw/src/bpnode_utils.c
  fsw/src/bpnode_adu_out.c
  fsw/src/bpnode_adu_in.c
  fsw/src/bpnode_cla_in.c
  fsw/src/bpnode_cla_out.c
  fsw/src/bpnode_gen_worker.c

  # FWP files
  fsw/fwp/fwp_timep.c
  fsw/fwp/fwp_tablep.c
  fsw/fwp/fwp_evp.c
  fsw/fwp/fwp_adup.c
  fsw/fwp/fwp_perflogp.c
  fsw/fwp/fwp_dp.c
  fsw/fwp/fwp_tlmp.c
  fsw/fwp/fwp_utils.c
)

set(APP_TABLE_FILES
  fsw/tables/bpnode_adup_config_tbl.c
  fsw/tables/bpnode_channel_config_tbl.c
  fsw/tables/bpnode_contacts_tbl.c
  fsw/tables/bpnode_crs_tbl.c
  fsw/tables/bpnode_custodian_auth_policy_tbl.c
  fsw/tables/bpnode_custody_auth_policy_tbl.c
  fsw/tables/bpnode_mib_config_pn_tbl.c
  fsw/tables/bpnode_mib_config_ps_tbl.c
  fsw/tables/bpnode_reportto_eid_auth_policy_tbl.c
  fsw/tables/bpnode_src_auth_policy_tbl.c
  fsw/tables/bpnode_src_latency_policy_tbl.c
  fsw/tables/bpnode_storage_tbl.c
)

# Create the app module
add_cfe_app(bpnode ${APP_SRC_FILES})

target_include_directories(bpnode PUBLIC fsw/inc)
target_include_directories(bpnode PUBLIC fsw/fwp)
target_include_directories(bpnode PUBLIC fsw/src)

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
