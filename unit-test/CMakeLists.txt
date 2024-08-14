##################################################################
#
# Coverage Unit Test build recipe
#
# This CMake file contains the recipe for building the BPNode unit tests.
# It is invoked from the parent directory when unit tests are enabled.
#
##################################################################

add_cfe_coverage_stubs("bpnode_internal"
  utilities/bpnode_test_utils.c
  stubs/bpnode_global_stubs.c
  stubs/bpnode_app_stubs.c
  stubs/bpnode_cmds_stubs.c
  stubs/bpnode_dispatch_stubs.c
  stubs/bpnode_utils_stubs.c
  stubs/fwp_timep_stubs.c
  stubs/fwp_tablep_stubs.c
)

# Link with the cfe core stubs and unit test assert libs
target_link_libraries(coverage-bpnode_internal-stubs ut_core_api_stubs ut_assert)

# Include and expose unit test utilities, fsw/inc, and fsw/src includes
target_include_directories(coverage-bpnode_internal-stubs PUBLIC utilities)
target_include_directories(coverage-bpnode_internal-stubs PUBLIC ../fsw/inc)
target_include_directories(coverage-bpnode_internal-stubs PUBLIC ../fsw/src)
target_include_directories(coverage-bpnode_internal-stubs PUBLIC ../fsw/fwp)

# Generate a dedicated "testrunner" executable for each test file
# Accomplish this by cycling through all the app's source files, there must be
# a *_tests file for each
foreach(SRCFILE ${APP_SRC_FILES})

    # Get the base sourcefile name as a module name without path or the
    # extension, this will be used as the base name of the unit test file.
    get_filename_component(UNIT_NAME "${SRCFILE}" NAME_WE)

    # Use the module name to make the test name by adding _tests to the end
    set(TESTS_NAME "${UNIT_NAME}_tests")

    # Make the test sourcefile name with unit test path and extension
    set(TESTS_SOURCE_FILE "${PROJECT_SOURCE_DIR}/unit-test/${TESTS_NAME}.c")

    add_cfe_coverage_test(bpnode "${UNIT_NAME}" "${TESTS_SOURCE_FILE}" "${CFS_BPNODE_SOURCE_DIR}/${SRCFILE}")

    add_cfe_coverage_dependency(bpnode "${UNIT_NAME}" bpnode_internal)

endforeach()

# The bpnode uses library functions provided by bplib so must be linked
# with the bplib stub library (this is mainly just an example of how this
# can be done).
add_cfe_coverage_dependency(bpnode bpnode_cmds bplib)
