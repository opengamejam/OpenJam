cmake_minimum_required (VERSION 2.8)
include(ExternalProject)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

project (OpenJam)
set(VERSION "1.0.0")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

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

macro (get_sources NAME SRC_PATH)

    # search sources
    set (${NAME}_SOURCE_PATTERNS  ${SRC_PATH}/*.h 
                                  ${SRC_PATH}/*.hpp 
                                  ${SRC_PATH}/*.cpp 
                                  ${SRC_PATH}/*.c
                                  ${SRC_PATH}/*.cc 
                                  ${SRC_PATH}/*.m 
                                  ${SRC_PATH}/*.mm)

    set (${NAME}_DIR_LIST)
    file (GLOB_RECURSE ${NAME}_SCAN_LIST ${${NAME}_SOURCE_PATTERNS})
    foreach (FILE_PATH ${${NAME}_SCAN_LIST})
        get_filename_component (DIR_PATH ${FILE_PATH} PATH)
        file (RELATIVE_PATH REL_PATH ${SRC_PATH} ${DIR_PATH})
        list (APPEND ${NAME}_DIR_LIST ${DIR_PATH})

        list (APPEND ${NAME}_SRCS ${FILE_PATH}) 

        set (GROUP_NAME "\\")
        if (NOT ${REL_PATH} STREQUAL "")
            string (REPLACE "/" "\\\\" GROUP_NAME ${REL_PATH})
        endif ()
        source_group (${GROUP_NAME} FILES ${FILE_PATH})
    endforeach ()
    
    list (LENGTH ${NAME}_DIR_LIST ${NAME}_LEN)
    if (${NAME}_LEN GREATER 1)
        list (REMOVE_DUPLICATES ${NAME}_DIR_LIST)
    endif ()
    
    # add includes
    include_directories (${SRC_PATH})
    foreach (DIR_PATH ${${NAME}_DIR_LIST})
        include_directories (${DIR_PATH})
    endforeach ()

endmacro ()

macro (link_library TARGET_NAME LIB_NAME)
	if(UNIX)
	  	set(prefix "lib")
	  	set(suffix ".a")
	else()
		set(prefix "")
	  	set(suffix ".lib")
	endif()

	target_link_libraries(${TARGET_NAME} ${INSTALL_LIB_DIR}/${prefix}${LIB_NAME}${suffix})

endmacro()

set(OPENJAM_LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

#============================================================================
# OpenJam
#============================================================================

define_Platform()
get_sources(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INSTALL_INC_DIR})

set (CMAKE_CXX_FLAGS "-std=gnu++11 -fno-rtti -fno-exceptions -stdlib=libc++ -g -Wall")
if (OS_IPHONE)
	# SDK
	set(CMAKE_OSX_SYSROOT "iphoneos")
	# Architecture
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
	# Compiler settings
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD 	"gnu++11")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY 			"libc++")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
	# IOS compiler cannot pass cmake compiler checking
	set (CMAKE_C_COMPILER_WORKS 1)
	set (CMAKE_CXX_COMPILER_WORKS 1)

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
	# Architecture		
	set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	# Compiler settings
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD 	"gnu++11")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY 			"libc++")

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
elseif (OS_KOS)
	# Compiler settings
	set (CMAKE_C_COMPILER "/opt/toolchains/dc/gcc-4.9.1/sh-elf") 
	set (CMAKE_C_COMPILER_ARG1 "") 
	set (CMAKE_C_COMPILER_ID "GNU") 
	set (CMAKE_C_PLATFORM_ID "Dreamcast")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer -c -fmessage-length=0 -ml -m4-single-only -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c -fmessage-length=0 -fno-operator-names")
	set (CMAKE_LINK_FLAGS "-nodefaultlibs -L/opt/toolchains/dc/kos/lib/dreamcast -L/opt/toolchains/dc/kos/addons/lib/dreamcast -Wl,--gc-sections -Wl,-T/opt/toolchains/dc/kos/utils/ldscripts/shlelf.xc -ml -m4-single-only -Wl,-Ttext=0x8c010000")

	add_definitions (-DOS_KOS)
	add_definitions (-DRENDER_OGL1_3)
endif ()


if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")
endif()

if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL)
    install(FILES ${PROJECT_NAME}_SRCS DESTINATION "${INSTALL_INC_DIR}")
endif()

#============================================================================
# zlib
#============================================================================

if(UNIX)
    set(ZLIB_NAME z)
else()
    set(ZLIB_NAME zlibstatic)
endif()
set(ZLIB_DIR ${OPENJAM_LIB_DIR}/zlib)

set(ZLIB_C_FLAGS " -Wno-error-implicit-function-declaration")

ExternalProject_Add(zlib
    SOURCE_DIR ${ZLIB_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}/zlib
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    PATCH_COMMAND COMMAND ${CMAKE_COMMAND} -E copy ${OPENJAM_LIB_DIR}/patches/zlib/CMakeLists.txt ${OPENJAM_LIB_DIR}/zlib/CMakeLists.txt
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} 
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_C_FLAGS=${ZLIB_C_FLAGS} 
    		   -DCMAKE_XCODE_EFFECTIVE_PLATFORMS=${CMAKE_XCODE_EFFECTIVE_PLATFORMS} 
    		   -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
)

#============================================================================
# minizip
#============================================================================

set(MINIZIP_DIR ${ZLIB_DIR}/contrib/minizip)

set(MINIZIP_HDRS
    ${MINIZIP_DIR}/crypt.h
    ${MINIZIP_DIR}/ioapi.h
    ${MINIZIP_DIR}/mztools.h
    ${MINIZIP_DIR}/unzip.h
    ${MINIZIP_DIR}/zip.h
)

set(MINIZIP_SRCS
    ${MINIZIP_DIR}/ioapi.c
    ${MINIZIP_DIR}/mztools.c
    ${MINIZIP_DIR}/unzip.c
    ${MINIZIP_DIR}/zip.c
)

if(WIN32)
    set(MINIZIP_HDRS ${MINIZIP_HDRS} ${MINIZIP_DIR}/iowin32.h)
    set(MINIZIP_SRCS ${MINIZIP_SRCS} ${MINIZIP_DIR}/iowin32.c)
endif()

add_library(minizip ${MINIZIP_SRCS} ${MINIZIP_HDRS})
add_dependencies(minizip zlib)
link_library(minizip ${ZLIB_NAME})
target_include_directories(minizip PUBLIC ${INSTALL_INC_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${MINIZIP_DIR})
add_dependencies(${PROJECT_NAME} minizip)
target_link_libraries(${PROJECT_NAME} minizip)

#============================================================================
# TinyXML
#============================================================================

set(TINYXML_DIR ${OPENJAM_LIB_DIR}/TinyXML)

ExternalProject_Add(TinyXML
    SOURCE_DIR ${TINYXML_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}/TinyXML
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} 
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_CXX_COMPILER_WORKS=${CMAKE_CXX_COMPILER_WORKS}
)

add_dependencies(${PROJECT_NAME} TinyXML)
link_library(${PROJECT_NAME} TinyXML)

#============================================================================
# glm
#============================================================================

set(GLM_DIR ${OPENJAM_LIB_DIR}/glm)

ExternalProject_Add(glm
    SOURCE_DIR ${GLM_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}/glm
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} 
    		   -DGLM_STATIC_LIBRARY_ENABLE=ON 
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_CXX_COMPILER_WORKS=${CMAKE_CXX_COMPILER_WORKS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_DIR})
add_dependencies(${PROJECT_NAME} glm)

#============================================================================
# examples
#============================================================================

set(TESTAPP_PROJECT TestApp)

set (${TESTAPP_PROJECT}_RES_DIR ${CMAKE_CURRENT_LIST_DIR}/examples/media)
if (OS_IPHONE)
	set (${TESTAPP_PROJECT}_RES_DIR
         ${${TESTAPP_PROJECT}_RES_DIR}
         ${CMAKE_CURRENT_LIST_DIR}/examples/platform/ios/Images.xcassets)
endif()
get_sources(${TESTAPP_PROJECT} ${CMAKE_CURRENT_LIST_DIR}/examples/src)

add_executable(${TESTAPP_PROJECT} MACOSX_BUNDLE ${${TESTAPP_PROJECT}_SRCS} ${${TESTAPP_PROJECT}_RES_DIR})
add_dependencies(${TESTAPP_PROJECT} ${PROJECT_NAME})
target_link_libraries(${TESTAPP_PROJECT} ${PROJECT_NAME})
target_include_directories(${TESTAPP_PROJECT} PUBLIC ${GLM_DIR})

if (OS_IPHONE)
	target_link_libraries (${TESTAPP_PROJECT} "-framework Foundation")
	target_link_libraries (${TESTAPP_PROJECT} "-framework UIKit")
	target_link_libraries (${TESTAPP_PROJECT} "-framework CoreGraphics")
	target_link_libraries (${TESTAPP_PROJECT} "-framework GLKit")
	target_link_libraries (${TESTAPP_PROJECT} "-framework OpenGLES")

	set (MACOSX_BUNDLE_GUI_IDENTIFIER "com.openjam.\${PRODUCT_NAME:identifier}")

	set_target_properties (${TESTAPP_PROJECT} 
						   PROPERTIES 
						   XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 	"7.0"
						   XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY 		"1,2"
						   XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME 		"AppIcon"
						   XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME 		"Brand Assets"
						   MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/examples/platform/ios/Info.plist"
						   RESOURCE "${${TESTAPP_PROJECT}_RES_DIR}")
elseif (OS_MAC)
	target_link_libraries (${TESTAPP_PROJECT} "-framework OpenGL")
	target_link_libraries (${TESTAPP_PROJECT} "-framework Cocoa")

	set (MACOSX_BUNDLE_GUI_IDENTIFIER "com.openjam.\${PRODUCT_NAME:identifier}")
	set (MACOSX_BUNDLE_BUNDLE_NAME ${TESTAPP_PROJECT})

	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.8")

	# Window
	#set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
	set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")
	#set_target_properties(${TESTAPP_PROJECT} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/src/platform/osx/InfoOSX.plist)
	# Copy resources
	add_custom_command (TARGET ${TESTAPP_PROJECT} POST_BUILD
                	  	COMMAND ${CMAKE_COMMAND} -E copy_directory ${${TESTAPP_PROJECT}_RES_DIR} "\${TARGET_BUILD_DIR}/\${FULL_PRODUCT_NAME}/media"
                	  	COMMENT "Copying resource files and directories to OSX App Bundle...")
endif()



