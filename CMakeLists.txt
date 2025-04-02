# Project Info
cmake_minimum_required(VERSION 3.10.2) #3.18
project(vulkan-renderer)
cmake_policy(VERSION 3.18)

# CMake Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
file(GLOB_RECURSE FILE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    #
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.sc

    #
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/maya-bridge/include/maya-bridge/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/**.cpp #
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/**.h #
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/**.inl #
)

# Solution Filters
foreach(source IN LISTS FILE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# Finalize Library
add_library(
    ${PROJECT_NAME}
    ${FILE_SOURCES}
    )

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan-renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

# 3rdparties
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
    )

# 3rdparty (bgfx) STATIC
set(BGFX_BUILD_TOOLS_GEOMETRY OFF)
set(BGFX_BUILD_TOOLS_TEXTURE OFF)
set(BGFX_BUILD_EXAMPLES OFF)
set(BGFX_BUILD_EXAMPLE_COMMON OFF)
set(BGFX_LIBRARY_TYPE "STATIC")
target_compile_definitions(${PROJECT_NAME} PUBLIC BGFX_CONFIG_PROFILER=1)

add_subdirectory(3rdparty/bgfx)
target_link_libraries(${PROJECT_NAME} PUBLIC bgfx bx bimg bimg_decode)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bgfx/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bgfx/bx/include
    )
set_target_properties(bgfx PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx")
set_target_properties(bimg PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx")
set_target_properties(bimg_decode PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx")
set_target_properties(bimg_encode PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx")
set_target_properties(bx PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx")
set_target_properties(fcpp PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/3rdparty")
set_target_properties(glslang PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/3rdparty")
set_target_properties(glsl-optimizer PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/3rdparty")
set_target_properties(spirv-cross PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/3rdparty")
set_target_properties(spirv-opt PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/3rdparty")
set_target_properties(bin2c PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/tools")
set_target_properties(shaderc PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/tools")
set_target_properties(tools PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/tools")
set_target_properties(example-common PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/examples")
set_target_properties(examples PROPERTIES FOLDER "vulkan-renderer/3rdparty/bgfx/examples")

# 3rdparty (sdl) STATIC
set(SDL_SHARED_DEFAULT OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_subdirectory(3rdparty/sdl EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sdl/include
    )
set_target_properties(SDL_uclibc PROPERTIES FOLDER "vulkan-renderer/3rdparty/sdl")
set_target_properties(SDL3-shared PROPERTIES FOLDER "vulkan-renderer/3rdparty/sdl")

# 3rdparty (optick) SHARED
add_subdirectory(3rdparty/optick)
target_link_libraries(${PROJECT_NAME} PUBLIC OptickCore)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/optick/src
    )
set_target_properties(OptickCore PROPERTIES FOLDER "vulkan-renderer/3rdparty/optick")

# 3rdparty (maya-bridge) INCLUDED
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/maya-bridge/include/
    )

# 3rdparty (imgui) INCLUDED
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
    )

# Shaders
include(CMakeParseArguments)
include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/bgfx/cmake/bgfx/util/ConfigureDebugging.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfxToolUtils.cmake)

# Shader (Deferred)
bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/vs_deferred.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders
  AS_HEADERS
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/fs_deferred.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders
  AS_HEADERS
)

# Shader (Tonemap)
bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/vs_tonemap.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders
  AS_HEADERS
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/fs_tonemap.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders
  AS_HEADERS
)

# Preprocessor Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)

# Put in a "vulkan-renderer" folder in Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "vulkan-renderer")
