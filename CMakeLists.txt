cmake_minimum_required (VERSION 2.8.11)

# Project name
project (OpenJam)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/") 
include (Project)
include (Dependencies)

# Make library with linker settings
make_executable ()
