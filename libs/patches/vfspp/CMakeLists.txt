cmake_minimum_required(VERSION 2.8)
include(ExternalProject)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

project(vfspp)
set(VERSION "1.0.0")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(LIB_PREFIX "")
    set(LIB_SUFFIX ".lib")
else()
    set(LIB_PREFIX "lib")
    set(LIB_SUFFIX ".a")
endif()

macro (link_library TARGET_NAME LIB_NAME DIR)
	target_link_libraries(${TARGET_NAME} ${CMAKE_INSTALL_PREFIX}${DIR}${LIB_NAME}/lib/${LIB_PREFIX}${LIB_NAME}${LIB_SUFFIX})
endmacro()

#============================================================================
# zlib
#============================================================================

if(UNIX)
    set(ZLIB_NAME z)
else()
    set(ZLIB_NAME zlibstatic)
endif()
set(ZLIB_DIR ${CMAKE_SOURCE_DIR}/external/zlib)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(ZLIB_C_FLAGS " -Wno-error-implicit-function-declaration")
endif()

ExternalProject_Add(zlib
    SOURCE_DIR ${ZLIB_DIR}
    PREFIX ${CMAKE_INSTALL_PREFIX}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    PATCH_COMMAND COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/patches/zlib/CMakeLists.txt ${CMAKE_SOURCE_DIR}/external/zlib/CMakeLists.txt
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/${ZLIB_NAME}
               -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
               -DCMAKE_C_FLAGS=${ZLIB_C_FLAGS} 
               -DCMAKE_XCODE_EFFECTIVE_PLATFORMS=${CMAKE_XCODE_EFFECTIVE_PLATFORMS} 
               -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DCMAKE_AR=${CMAKE_AR}
               -DCMAKE_RANLIB=${CMAKE_RANLIB}
               -DCMAKE_LINKER=${CMAKE_LINKER}
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

target_include_directories(minizip PUBLIC ${CMAKE_INSTALL_PREFIX}/${ZLIB_NAME}/include)
set_target_properties(minizip PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/minizip/lib
                                         ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_INSTALL_PREFIX}/minizip/lib
                                         ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_INSTALL_PREFIX}/minizip/lib
                                         ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_INSTALL_PREFIX}/minizip/lib
                                         ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_INSTALL_PREFIX}/minizip/lib)

#============================================================================
# vfspp
#============================================================================

set(LIBVFSPP_PUBLIC_HDRS_DIR include)
set(LIBVFSPP_PRIVATE_HDRS_DIR src)
set(LIBVFSPP_SRCS_DIR src)

set(LIBVFSPP_PUBLIC_HDRS
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CFileInfo.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CMemoryFile.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CMemoryFileSystem.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CNativeFile.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CNativeFileSystem.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CVirtualFileSystem.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CZipFile.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/CZipFileSystem.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/Global.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/IFile.h
    ${LIBVFSPP_PUBLIC_HDRS_DIR}/IFileSystem.h
)

set(LIBVFSPP_PRIVATE_HDRS
    ${LIBVFSPP_PRIVATE_HDRS_DIR}/CStringUtils.h
)

set(LIBVFSPP_SRCS
    ${LIBVFSPP_SRCS_DIR}/CFileInfo.cpp
    ${LIBVFSPP_SRCS_DIR}/CMemoryFile.cpp
    ${LIBVFSPP_SRCS_DIR}/CMemoryFileSystem.cpp
    ${LIBVFSPP_SRCS_DIR}/CNativeFile.cpp
    ${LIBVFSPP_SRCS_DIR}/CNativeFileSystem.cpp
    ${LIBVFSPP_SRCS_DIR}/CStringUtils.cpp
    ${LIBVFSPP_SRCS_DIR}/CVirtualFileSystem.cpp
    ${LIBVFSPP_SRCS_DIR}/CZipFile.cpp
    ${LIBVFSPP_SRCS_DIR}/CZipFileSystem.cpp
)

add_library(${PROJECT_NAME} STATIC ${LIBVFSPP_SRCS} ${LIBVFSPP_HDRS})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib
                                                     ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib
                                                     ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib
                                                     ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib
                                                     ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBVFSPP_PUBLIC_HDRS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/${ZLIB_NAME}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${MINIZIP_DIR})
add_dependencies(${PROJECT_NAME} minizip)

add_definitions (-DIOAPI_NO_64) # minizip

# Installation
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL)
    install(FILES ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib/${LIB_PREFIX}${PROJECT_NAME}${LIB_SUFFIX}
            DESTINATION "${INSTALL_LIB_DIR}")
endif()
if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL)
    install(FILES ${LIBVFSPP_PUBLIC_HDRS} DESTINATION "${INSTALL_INC_DIR}")
endif()

#============================================================================
# Example binaries
#============================================================================

if(WITH_EXAMPLES)
    set(EXAMPLES_DIR examples)

    add_executable(example ${EXAMPLES_DIR}/example.cpp)
    add_dependencies(example ${PROJECT_NAME})
    link_library(example ${PROJECT_NAME} /)
    link_library(example minizip /)
    link_library(example ${ZLIB_NAME} /)
    #todo link minizip and libz
    target_include_directories(example PUBLIC ${LIBVFSPP_PUBLIC_HDRS_DIR})
endif()