# Workflow-specific targets.cmake file
# ------------------------------------
#
# This is the cfs configuration used by the build-run workflow for BP
# To implement a full file transfer, it must include all dependencies
# (bplib, cf, iodriver/unsock, tinycbor) and two cpu configurations.

SET(MISSION_NAME "BpTestWorkflow")
SET(SPACECRAFT_ID 0xb9)

# This defines two CPUs - one that will send the file, other to receive it
SET(MISSION_CPUNAMES tx rx)

# This shortcut compiles the same set of apps for all CPUs,
# but there is no equivalent for PSP_MODULELIST at this time
list(APPEND MISSION_GLOBAL_APPLIST cf bp bplib ci_lab to_lab sch_lab)
set(GLOBAL_PSP_MODULELIST iodriver unsock_intf)

SET(tx_PROCESSORID 1)
SET(tx_PSP_MODULELIST ${GLOBAL_PSP_MODULELIST})
SET(tx_PLATFORM default)

SET(rx_PROCESSORID 2)
SET(rx_PSP_MODULELIST ${GLOBAL_PSP_MODULELIST})
SET(rx_PLATFORM default)
