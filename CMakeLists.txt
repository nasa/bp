##################################################################
#
# cFS Bundle Protocol Agent (BP) module CMake build recipe
#
##################################################################

project(CFS_BP C)

include_directories(fsw/custom)
include_directories(fsw/src)

set(APP_SRC_FILES
    fsw/src/bp_app.c
    fsw/src/bp_flow.c
    fsw/src/bp_cla_bundle_io.c
    fsw/src/bp_storage.c
    fsw/src/bp_global.c
    fsw/custom/bp_semcfg.c
    fsw/custom/bp_storecfg.c
    fsw/custom/bp_tlmcfg.c
)

set(APP_TABLE_FILES
    fsw/tables/bp_flowtable.c
)

# Create the app module
add_cfe_app(bp ${APP_SRC_FILES})
add_cfe_tables(bp ${APP_TABLE_FILES})

target_include_directories(bp PUBLIC fsw/inc)

# Indicate that this app depends on BPLIB
add_cfe_app_dependency(bp bplib iodriver)

if (ENABLE_UNIT_TESTS)
    add_cfe_coverage_test(bp ALL
        ut-coverage/coveragetest_bp_setup.c
        ${APP_SRC_FILES}
    )
endif (ENABLE_UNIT_TESTS)
