
# The SCH LAB and BP Flow tables need to access the right headers
target_include_directories(sch_lab.table INTERFACE
    $<TARGET_PROPERTY:cf,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:bpnode,INCLUDE_DIRECTORIES>
)

install(
    FILES ${MISSION_DEFS}/cfe_es_startup.scr
    DESTINATION ${TGTNAME}/${INSTALL_SUBDIR}
)
	
