cmake_minimum_required (VERSION 2.8)
include(ExternalProject)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

project (OpenJam)
set(VERSION "1.0.0")

set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT DEFINED PLATFORM)
    message (FATAL_ERROR "Please specify the target platform: Windows, Linux, Android, macOS, iOS, tvOS, watchOS. -DPLATFORM=macOS")
endif()

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

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

if(${PLATFORM} STREQUAL "Windows")
  	set(LIB_PREFIX "")
  	set(LIB_SUFFIX ".lib")
else()
	set(LIB_PREFIX "lib")
  	set(LIB_SUFFIX ".a")
endif()

macro (link_library TARGET_NAME LIB_NAME DIR)
	target_link_libraries(${TARGET_NAME} ${CMAKE_INSTALL_PREFIX}${DIR}${LIB_NAME}/lib/${LIB_PREFIX}${LIB_NAME}${LIB_SUFFIX})
endmacro()

set(OPENJAM_LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

#============================================================================
# OpenJam
#============================================================================

get_sources(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INSTALL_INC_DIR})

if (${PLATFORM} STREQUAL "macOS")
	# SDK
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	# Architecture		
	set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	# Compiler settings
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -stdlib=libc++ -x objective-c")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -x objective-c++")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD 	"c++11")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY 			"libc++")

	add_definitions (-DOS_MAC)
	add_definitions (-DRENDER_OGL1_3)
	add_definitions (-DRENDER_OGL1_5)
	add_definitions (-DRENDER_OGL2_0)
	add_definitions (-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

	# TODO
elseif (${PLATFORM} STREQUAL "Android")

	# TODO
elseif (${PLATFORM} STREQUAL "Windows")
	add_definitions (-DOS_WINDOWS)
	add_definitions (-DRENDER_OGLES2_0)
elseif (${PLATFORM} STREQUAL "Linux")

	# TODO
elseif (${PLATFORM} STREQUAL "KOS")
	set(CMAKE_OSX_SYSROOT /)
	add_definitions (-DGLM_HAS_CXX11_STL=0)
	add_definitions (-DGLM_STATIC_ASSERT_NULL=1)
	add_definitions (-DOS_KOS)
	add_definitions (-DRENDER_OGL1_3)
endif ()

# Install
if (NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")
endif()

if (NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL)
    install(FILES ${PROJECT_NAME}_SRCS DESTINATION "${INSTALL_INC_DIR}")
endif()

#============================================================================
# tinyxml
#============================================================================

set (TINYXML_DIR ${OPENJAM_LIB_DIR}/tinyxml)

ExternalProject_Add(tinyxml
    SOURCE_DIR ${TINYXML_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/tinyxml
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_CXX_COMPILER_WORKS=${CMAKE_CXX_COMPILER_WORKS}
			   -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    		   -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    		   -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    		   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    		   -DCMAKE_AR=${CMAKE_AR}
    		   -DCMAKE_RANLIB=${CMAKE_RANLIB}
    		   -DCMAKE_LINKER=${CMAKE_LINKER}
			   -DCMAKE_OBJCOPY=${CMAKE_OBJCOPY}
			   -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    		   -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
			   -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
			   -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
			   -DCMAKE_CROSSCOMPILING=TRUE
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/tinyxml/include)
add_dependencies(${PROJECT_NAME} tinyxml)

#============================================================================
# glm
#============================================================================

set(GLM_DIR ${OPENJAM_LIB_DIR}/glm)

ExternalProject_Add(glm
    SOURCE_DIR ${GLM_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    PATCH_COMMAND COMMAND ${CMAKE_COMMAND} -E copy ${OPENJAM_LIB_DIR}/patches/glm/setup.hpp ${OPENJAM_LIB_DIR}/glm/glm/detail/setup.hpp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/glm
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_CXX_COMPILER_WORKS=${CMAKE_CXX_COMPILER_WORKS}
			   -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    		   -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    		   -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    		   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    		   -DCMAKE_AR=${CMAKE_AR}
    		   -DCMAKE_RANLIB=${CMAKE_RANLIB}
    		   -DCMAKE_LINKER=${CMAKE_LINKER}
			   -DPLATFORM=${PLATFORM}
               -DKOS_BASE_DIR=${KOS_BASE_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/glm/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_DIR})
add_dependencies(${PROJECT_NAME} glm)

#============================================================================
# vfspp
#============================================================================

set (VFSPP_DIR ${OPENJAM_LIB_DIR}/vfspp)

ExternalProject_Add(vfspp
    SOURCE_DIR ${VFSPP_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/vfspp
    		   -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
    		   -DCMAKE_CXX_COMPILER_WORKS=${CMAKE_CXX_COMPILER_WORKS}
			   -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    		   -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    		   -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    		   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    		   -DCMAKE_AR=${CMAKE_AR}
    		   -DCMAKE_RANLIB=${CMAKE_RANLIB}
    		   -DCMAKE_LINKER=${CMAKE_LINKER}
    		   -DPLATFORM=${PLATFORM}
               -DKOS_BASE_DIR=${KOS_BASE_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/vfspp/include)
add_dependencies(${PROJECT_NAME} vfspp)

#============================================================================
# examples
#============================================================================

set(TESTAPP_PROJECT TestApp)

set (${TESTAPP_PROJECT}_RES_DIR ${CMAKE_SOURCE_DIR}/examples/media)
if (${PLATFORM} STREQUAL "iOS")
	set (${TESTAPP_PROJECT}_RES_DIR
         ${${TESTAPP_PROJECT}_RES_DIR}
         ${CMAKE_SOURCE_DIR}/examples/platform/ios/Images.xcassets)
endif()
get_sources(${TESTAPP_PROJECT} ${CMAKE_SOURCE_DIR}/examples/src)

if (${PLATFORM} STREQUAL "iOS" OR ${PLATFORM} STREQUAL "macOS")
	add_executable(${TESTAPP_PROJECT} MACOSX_BUNDLE ${${TESTAPP_PROJECT}_SRCS} ${${TESTAPP_PROJECT}_RES_DIR})
elseif (${PLATFORM} STREQUAL "Windows")
	add_executable(${TESTAPP_PROJECT} WIN32 ${${TESTAPP_PROJECT}_SRCS} ${${TESTAPP_PROJECT}_RES_DIR})
else()
	add_executable(${TESTAPP_PROJECT} ${${TESTAPP_PROJECT}_SRCS} ${${TESTAPP_PROJECT}_RES_DIR})
endif()

add_dependencies(${TESTAPP_PROJECT} ${PROJECT_NAME})
target_link_libraries(${TESTAPP_PROJECT} ${PROJECT_NAME})
target_include_directories(${TESTAPP_PROJECT} PUBLIC ${GLM_DIR})

# vfspp	
link_library(${TESTAPP_PROJECT} vfspp /)
link_library(${TESTAPP_PROJECT} minizip /vfspp/)
link_library(${TESTAPP_PROJECT} z /vfspp/)
# tinyxml
link_library(${TESTAPP_PROJECT} tinyxml /)

if (${PLATFORM} STREQUAL "iOS")
	add_definitions (-DOS_IPHONE)
	add_definitions (-DRENDER_OGLES1_1)
	add_definitions (-DRENDER_OGLES2_0)
	add_definitions (-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

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
						   XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME 	"Brand Assets"
						   MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/examples/platform/ios/Info.plist"
						   RESOURCE "${${TESTAPP_PROJECT}_RES_DIR}")
elseif (${PLATFORM} STREQUAL "macOS")
	target_link_libraries (${TESTAPP_PROJECT} "-framework OpenGL")
	target_link_libraries (${TESTAPP_PROJECT} "-framework Cocoa")

	set (MACOSX_BUNDLE_GUI_IDENTIFIER "com.openjam.\${PRODUCT_NAME:identifier}")
	set (MACOSX_BUNDLE_BUNDLE_NAME ${TESTAPP_PROJECT})

	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.8")

	# Window
	#set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
	set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")
	#set_target_properties(${TESTAPP_PROJECT} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/platform/osx/InfoOSX.plist)
	# Copy resources
	add_custom_command (TARGET ${TESTAPP_PROJECT} POST_BUILD
                	  	COMMAND ${CMAKE_COMMAND} -E copy_directory ${${TESTAPP_PROJECT}_RES_DIR} "\${TARGET_BUILD_DIR}/\${FULL_PRODUCT_NAME}/media"
                	  	COMMENT "Copying resource files and directories to OSX App Bundle...")
elseif (${PLATFORM} STREQUAL "KOS")
	set(CMAKE_OSX_SYSROOT /)
	set(CMAKE_CXX_LINK_FLAGS "")#-Wl,--start-group -lkallisti -lc -lgcc -lstdc++ -lm -Wl,--end-group") #to avoid adding Wl,-search_paths_first
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lkosutils -lstdc++ -Wl,--start-group -lkallisti -lc -lgcc -Wl,--end-group")

	target_link_libraries(${TESTAPP_PROJECT} libGL.a) # kos-ports/libGL
	target_link_libraries(${TESTAPP_PROJECT} libkallisti.a) # kos
	
	#target_link_libraries(${TESTAPP_PROJECT} libc.a) # gcc
	#target_link_libraries(${TESTAPP_PROJECT} libm.a)  # gcc

	add_definitions (-DOS_KOS)
endif()



