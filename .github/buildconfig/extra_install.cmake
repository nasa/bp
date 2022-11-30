
# The SCH LAB and BP Flow tables need to access the right headers
target_include_directories(${TGTNAME}_sch_lab_table_sch_lab_table PRIVATE
    $<TARGET_PROPERTY:cf,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:bp,INCLUDE_DIRECTORIES>
)
target_include_directories(${TGTNAME}_bp_bp_flowtable PRIVATE
    $<TARGET_PROPERTY:cf,INCLUDE_DIRECTORIES>
)

# Install CFE script and tinycbor into filesystem
# For tinycbor this just grabs the .so from /usr/local
get_filename_component(TINYCBOR_SO /usr/local/lib/libtinycbor.so REALPATH)
install(
    FILES ${TINYCBOR_SO}
    DESTINATION ${TGTNAME}/${INSTALL_SUBDIR} 
    RENAME tinycbor.so
)
install(
    FILES ${MISSION_DEFS}/cfe_es_startup.scr
    DESTINATION ${TGTNAME}/${INSTALL_SUBDIR}
)
	
