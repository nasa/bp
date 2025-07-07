# Workflow-specific targets.cmake file
# ------------------------------------
#
# This is the cFS configuration used by the build-run workflow for BPNode

SET(MISSION_NAME "DTN")
SET(SPACECRAFT_ID 0xb9)

# This defines two CPUs - one that will send the file, other to receive it
SET(MISSION_CPUNAMES tx rx)

# This shortcut compiles the same set of apps for all CPUs,
# but there is no equivalent for PSP_MODULELIST at this time
list(APPEND MISSION_GLOBAL_APPLIST cf bpnode bplib ci_lab to_lab sch_lab)
set(GLOBAL_PSP_MODULELIST udpsock_intf)

SET(cpu1_PROCESSORID 1)
SET(cpu1_PSP_MODULELIST ${GLOBAL_PSP_MODULELIST})
SET(cpu1_PLATFORM default)
