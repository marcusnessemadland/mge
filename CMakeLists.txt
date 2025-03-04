cmake_minimum_required(VERSION 3.10.2)
project(vulkan-renderer)
cmake_policy(SET CMP0054 NEW)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Makefile generators on apple need this flag to compile mixed objective/c++
if(APPLE AND NOT XCODE)
	set(CMAKE_CXX_FLAGS "-ObjC++")
endif()

if(MSVC AND (MSVC_VERSION GREATER_EQUAL 1914))
	add_compile_options("/Zc:__cplusplus")
	add_compile_options("/Zc:preprocessor")
endif()

include(CMakeDependentOption)

# BX
option(BX_AMALGAMATED "Amalgamated bx build for faster compilation" OFF)
option(BX_CONFIG_DEBUG "Log debug messages (default: on in debug)" OFF)

if(NOT BX_DIR)
	set(BX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bx" CACHE STRING "Location of bx.")
elseif(NOT IS_ABSOLUTE "${BX_DIR}")
	get_filename_component(BX_DIR "${BX_DIR}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
file(TO_CMAKE_PATH "${BX_DIR}" BX_DIR)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bx)

# Vulkan Renderer
option(VR_BUILD_SAMPLES "Build vulkan renderer samples" ON)

if(NOT VR_DIR)
	set(VR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vulkan-renderer" CACHE STRING "Location of vulkan-renderer.")
elseif(NOT IS_ABSOLUTE "${VR_DIR}")
	get_filename_component(VR_DIR "${VR_DIR}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
file(TO_CMAKE_PATH "${VR_DIR}" VR_DIR)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmake/vulkan-renderer)


