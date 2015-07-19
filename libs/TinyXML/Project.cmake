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
	set (${PROJECT_NAME}_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(${PROJECT_NAME}_OUT_DIR ${CMAKE_CURRENT_LIST_DIR}/bin)

	set(EXECUTABLE_OUTPUT_PATH ${${PROJECT_NAME}_OUT_DIR} CACHE PATH "Build directory" FORCE)
	set(LIBRARY_OUTPUT_PATH ${${PROJECT_NAME}_OUT_DIR} CACHE PATH "Build directory" FORCE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUT_DIR})
	set(CMAKE_BUILD_FILES_DIRECTORY ${${PROJECT_NAME}_OUT_DIR})
	set(CMAKE_BUILD_DIRECTORY ${${PROJECT_NAME}_OUT_DIR})
	set(CMAKE_BINARY_DIR  ${${PROJECT_NAME}_OUT_DIR})
	SET(EXECUTABLE_OUTPUT_PATH ${${PROJECT_NAME}_OUT_DIR})
	SET(LIBRARY_OUTPUT_PATH ${${PROJECT_NAME}_OUT_DIR})
	SET(CMAKE_CACHEFILE_DIR ${${PROJECT_NAME}_OUT_DIR})
endmacro ()

# Linker settings
macro (make_executable)
	set (${PROJECT_NAME}_SOURCES)
	
	# Determinate platforms
	define_platform ()
	# Make project files hierarhy
	define_directories ()
	define_source_files ()

	# Make static library
	add_library (${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

	if (OS_IPHONE)
		# SDK
		set(IOS_SDK "iphoneos")
		set(CMAKE_OSX_SYSROOT ${IOS_SDK})
	
		# Arhitecture
		set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")

		# Compiler settings
		set (CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++11")
		set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
		set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -stdlib=libc++ -g -Wall")

		set_property (TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "7.0")

		target_link_libraries (${PROJECT_NAME} "-framework Foundation")
		target_link_libraries (${PROJECT_NAME} "-framework UIKit")
		target_link_libraries (${PROJECT_NAME} "-framework CoreGraphics")
		target_link_libraries (${PROJECT_NAME} "-framework GLKit")
		target_link_libraries (${PROJECT_NAME} "-framework OpenGLES")

	elseif (OS_MAC)

		# TODO
	elseif (OS_ANDROID)

		# TODO
	elseif (OS_WINDOWS)

		# TODO
	elseif (OS_LINUX)

		# TODO
	endif ()

endmacro ()