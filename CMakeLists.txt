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
	MESSAGE("Link: " ${INSTALL_LIB_DIR}/${prefix}${LIB_NAME}${suffix})

endmacro()

set(OPENJAM_LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

#============================================================================
# OpenJam
#============================================================================

define_Platform()
get_sources(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src)

set (CMAKE_CXX_FLAGS "-std=gnu++11 -fno-rtti -fno-exceptions -stdlib=libc++ -g -Wall")
if (OS_IPHONE)
	# SDK
	set(CMAKE_OSX_SYSROOT "iphoneos")
	# Arhitecture
	set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
	# Compiler settings
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

	add_definitions (-DOS_IPHONE)
	add_definitions (-DRENDER_OGLES1_1)
	add_definitions (-DRENDER_OGLES2_0)
	add_definitions (-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

elseif (OS_MAC)
	# SDK
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	# Arhitecture		
	set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	# Compiler settings
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

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

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INSTALL_INC_DIR})


#============================================================================
# zlib
#============================================================================

if(UNIX)
    set(ZLIB_NAME z)
else()
    set(ZLIB_NAME zlibstatic)
endif()
set(ZLIB_DIR ${OPENJAM_LIB_DIR}/zlib)

ExternalProject_Add(zlib
    SOURCE_DIR ${ZLIB_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}/zlib
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
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
target_link_libraries(minizip ${ZLIB_NAME})
target_include_directories(minizip PUBLIC ${INSTALL_INC_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${MINIZIP_DIR})
add_dependencies(${PROJECT_NAME} minizip)
link_library(${PROJECT_NAME} minizip)

#============================================================================
# TinyXML
#============================================================================

set(TINYXML_DIR ${OPENJAM_LIB_DIR}/TinyXML)

ExternalProject_Add(TinyXML
    SOURCE_DIR ${TINYXML_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}/TinyXML
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
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
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DGLM_STATIC_LIBRARY_ENABLE=ON
)

add_dependencies(${PROJECT_NAME} glm)

