macro (define_source_files)

	set (SOURCE_PATTERNS 	${${PROJECT_NAME}_SOURCES_DIR}/*.h 
							${${PROJECT_NAME}_SOURCES_DIR}/*.hpp 
							${${PROJECT_NAME}_SOURCES_DIR}/*.cpp 
							${${PROJECT_NAME}_SOURCES_DIR}/*.c 
							${${PROJECT_NAME}_SOURCES_DIR}/*.m 
							${${PROJECT_NAME}_SOURCES_DIR}/*.mm)

	set (${PROJECT_NAME}_DIR_LIST)
    file (GLOB_RECURSE ${PROJECT_NAME}_SCAN_LIST ${SOURCE_PATTERNS})
    foreach (FILE_PATH ${${PROJECT_NAME}_SCAN_LIST})
        get_filename_component (DIR_PATH ${FILE_PATH} PATH)
        file (RELATIVE_PATH REL_PATH ${${PROJECT_NAME}_SOURCES_DIR}/ ${DIR_PATH})
        list (APPEND ${PROJECT_NAME}_DIR_LIST ${DIR_PATH})

        list (APPEND ${PROJECT_NAME}_SOURCES ${FILE_PATH}) 

        set (GROUP_NAME "\\")
        if (NOT ${REL_PATH} STREQUAL "")
        	string (REPLACE "/" "\\\\" GROUP_NAME ${REL_PATH})
        endif ()
        source_group (${GROUP_NAME} FILES ${FILE_PATH})
    endforeach ()
    
    list (LENGTH ${PROJECT_NAME}_DIR_LIST ${LIB}_LEN)
    if (${PROJECT_NAME}_LEN GREATER 1)
    	list (REMOVE_DUPLICATES ${PROJECT_NAME}_DIR_LIST)
    endif ()
    
    # add includes
    include_directories (${${PROJECT_NAME}_SOURCES_DIR})
    foreach (DIR_PATH ${${PROJECT_NAME}_DIR_LIST})
    	include_directories (${DIR_PATH})
    endforeach ()

endmacro ()
