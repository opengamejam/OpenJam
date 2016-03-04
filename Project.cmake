# Determinate platform
macro (define_platform)

	set (PLATFORM "" CACHE STRING "User defined platform")

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	    if (${PLATFORM} STREQUAL "IOS")
	    	set (OS_IPHONE True)
	    else ()
	    	set (OS_MAC True)
		endif ()
	elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
		if (${PLATFORM} STREQUAL "ANDROID")
	    	set (OS_ANDROID True)
	    else ()
	    	set (OS_WINDOWS True)
		endif ()
	elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		if (${PLATFORM} STREQUAL "ANDROID")
	    	set (OS_ANDROID True)
	    else ()
	    	set (OS_LINUX True)
		endif ()
	endif ()

endmacro ()

# Setup build directories
macro (define_directories)
	set (${PROJECT_NAME}_SOURCES_DIR	${CMAKE_CURRENT_LIST_DIR}/src)
	set (${PROJECT_NAME}_LIBS_DIR 		${CMAKE_CURRENT_LIST_DIR}/libs)
	set (${PROJECT_NAME}_OUT_DIR 		${CMAKE_CURRENT_LIST_DIR}/bin/${PLATFORM})
	set (${PROJECT_NAME}_RES_DIR 		${CMAKE_CURRENT_LIST_DIR}/media)

	set (EXECUTABLE_OUTPUT_PATH 		${${PROJECT_NAME}_OUT_DIR} CACHE PATH "Build directory" FORCE)
	set (LIBRARY_OUTPUT_PATH 			${${PROJECT_NAME}_OUT_DIR} CACHE PATH "Build directory" FORCE)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUT_DIR})
	set (CMAKE_BUILD_FILES_DIRECTORY 	${${PROJECT_NAME}_OUT_DIR})
	set (CMAKE_BUILD_DIRECTORY 			${${PROJECT_NAME}_OUT_DIR})
	set (CMAKE_BINARY_DIR  				${${PROJECT_NAME}_OUT_DIR})
	set (EXECUTABLE_OUTPUT_PATH 		${${PROJECT_NAME}_OUT_DIR})
	set (LIBRARY_OUTPUT_PATH 			${${PROJECT_NAME}_OUT_DIR})
	set (CMAKE_CACHEFILE_DIR 			${${PROJECT_NAME}_OUT_DIR})

	link_directories (${${PROJECT_NAME}_OUT_DIR})
endmacro ()

macro (define_lib LIB LIB_PATH TYPE)

    # search sources
    set (${LIB}_SOURCES_DIR ${LIB_PATH}/src)
    set (${LIB}_SOURCE_PATTERNS     ${${LIB}_SOURCES_DIR}/*.h 
                                    ${${LIB}_SOURCES_DIR}/*.hpp 
                                    ${${LIB}_SOURCES_DIR}/*.cpp 
                                    ${${LIB}_SOURCES_DIR}/*.c 
                                    ${${LIB}_SOURCES_DIR}/*.m 
                                    ${${LIB}_SOURCES_DIR}/*.mm)

    set (${LIB}_DIR_LIST)
    file (GLOB_RECURSE ${LIB}_SCAN_LIST ${${LIB}_SOURCE_PATTERNS})
    foreach (FILE_PATH ${${LIB}_SCAN_LIST})
        get_filename_component (DIR_PATH ${FILE_PATH} PATH)
        file (RELATIVE_PATH REL_PATH ${${LIB}_SOURCES_DIR} ${DIR_PATH})
        list (APPEND ${LIB}_DIR_LIST ${DIR_PATH})

        list (APPEND ${LIB}_SOURCES ${FILE_PATH}) 

        set (GROUP_NAME "\\")
        if (NOT ${REL_PATH} STREQUAL "")
            string (REPLACE "/" "\\\\" GROUP_NAME ${REL_PATH})
        endif ()
        source_group (${GROUP_NAME} FILES ${FILE_PATH})
    endforeach ()
    
    list (LENGTH ${LIB}_DIR_LIST ${LIB}_LEN)
    if (${LIB}_LEN GREATER 1)
        list (REMOVE_DUPLICATES ${LIB}_DIR_LIST)
    endif ()
    
    # add includes
    include_directories (${${LIB}_SOURCES_DIR})
    foreach (DIR_PATH ${${LIB}_DIR_LIST})
        include_directories (${DIR_PATH})
    endforeach ()


    if (${TYPE} STREQUAL "LIB")
        target_link_libraries(${PROJECT_NAME} ${LIB})
        add_library (${LIB} STATIC ${${LIB}_SCAN_LIST})
    elseif (${TYPE} STREQUAL "EXELIB")
        add_library (${LIB} STATIC ${${LIB}_SCAN_LIST})
    elseif (${TYPE} STREQUAL "FRAMEWORK")
        if (OS_IPHONE)
            target_link_libraries (${PROJECT_NAME} "-framework ${LIB}")
        elseif (OS_MAC)
            target_link_libraries (${PROJECT_NAME} "-framework ${LIB}")
        endif ()
    elseif (${TYPE} STREQUAL "EXE")
        if (OS_IPHONE)
            add_executable (${LIB} MACOSX_BUNDLE ${${LIB}_SCAN_LIST})
        elseif (OS_MAC)
            add_executable (${LIB} MACOSX_BUNDLE ${${LIB}_SCAN_LIST})
        else ()
            add_executable (${LIB} ${${LIB}_SCAN_LIST})
        endif ()
    endif ()

endmacro ()

# Linker settings
macro (make_executable)
	
	# Determinate platforms
	define_platform ()
	define_directories ()

	define_dependencies_list ()

	if (OS_IPHONE)
		# SDK
		set(CMAKE_OSX_SYSROOT 									"iphoneos")
		# Arhitecture
		set (CMAKE_OSX_ARCHITECTURES 							"$(ARCHS_STANDARD)")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS 					"-iphoneos;-iphonesimulator")
		# Compiler settings
		set (CMAKE_XCODE_ATTRIBUTE_GCC_VERSION 					"com.apple.compilers.llvm.clang.1_0")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD 	"gnu++11")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY 			"libc++")
		set (CMAKE_C_FLAGS 										"-x objective-c")
		set (CMAKE_CXX_FLAGS 									"-std=gnu++11 -stdlib=libc++ -g -Wall -x objective-c++")

		set_property (TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 	"7.0")
		set_property (TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY 		"1,2")

		add_definitions (-DOS_IPHONE)
		add_definitions (-DRENDER_OGLES1_1)
		add_definitions (-DRENDER_OGLES2_0)
		add_definitions (-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

	elseif (OS_MAC)
		# SDK
		set(XCODE_ATTRIBUTE_SDKROOT macosx)
		set(CMAKE_OSX_SYSROOT macosx)
		# Arhitecture		
		set (CMAKE_OSX_ARCHITECTURES 							"$(ARCHS_STANDARD)")
		# Compiler settings
		set (CMAKE_XCODE_ATTRIBUTE_GCC_VERSION 					"com.apple.compilers.llvm.clang.1_0")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD 	"gnu++11")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY 			"libc++")
		set (CMAKE_C_FLAGS 										"-x objective-c")
		set (CMAKE_CXX_FLAGS 									"-std=gnu++11 -stdlib=libc++ -g -Wall -x objective-c++")

		set (MACOSX_BUNDLE_GUI_IDENTIFIER 						"com.openjam.\${PRODUCT_NAME:identifier}")
		set (MACOSX_BUNDLE_BUNDLE_NAME ${PRODUCT_NAME})

		set (CMAKE_OSX_DEPLOYMENT_TARGET "10.8")

		add_definitions (-DOS_MAC)
		add_definitions (-DRENDER_OGL1_3)
		add_definitions (-DRENDER_OGL1_5)
		add_definitions (-DRENDER_OGL2_0)
		add_definitions (-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

		# TODO
	elseif (OS_ANDROID)

		# TODO
	elseif (OS_WINDOWS)
		add_definitions (-DOS_WINDOWS)
		add_definitions (-DRENDER_OGLES2_0)
	elseif (OS_LINUX)

		# TODO
	endif ()

endmacro ()