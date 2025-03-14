# Project Info
cmake_minimum_required(VERSION 3.10.2) #3.18
project(vulkan-renderer)
cmake_policy(VERSION 3.18)


# CMake Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================

# Options
set(VR_API AUTO CACHE STRING "A more specific platform selector to choose from, choose the exact OS API to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP.")
set_property(
    CACHE
    VR_API PROPERTY
    STRINGS AUTO WIN32 UWP COCOA UIKIT XCB XLIB MIR WAYLAND ANDROID WASM NOOP
)

set(VR_OS AUTO CACHE STRING "Optional: Choose the OS to build for, defaults to AUTO, but can be WINDOWS, MACOS, LINUX, ANDROID, IOS, WASM.") 
set_property(
    CACHE
    VR_OS PROPERTY
    STRINGS AUTO WINDOWS MACOS LINUX ANDROID IOS WASM NOOP
)

if( NOT (VR_OS STREQUAL "AUTO") AND VR_API STREQUAL "AUTO")
    if(VR_OS STREQUAL "WINDOWS")
        set(VR_API "WIN32")
    elseif(VR_OS STREQUAL "MACOS")
        set(VR_API "COCOA")
    elseif(VR_OS STREQUAL "LINUX")
        set(VR_API "XLIB")
    elseif(VR_OS STREQUAL "ANDROID")
        set(VR_API "ANDROID")
    elseif(VR_OS STREQUAL "IOS")
        set(VR_API "UIKIT")
    elseif(VR_OS STREQUAL "WASM")
        set(VR_API "WASM")
    elseif(VR_OS STREQUAL "NOOP")
        set(VR_API "NOOP")
    else()
        message( SEND_ERROR "VR_OS can only be either AUTO, NOOP, WINDOWS, MACOS, LINUX, ANDROID, IOS, or WASM.")
    endif()
endif()

if(VR_API STREQUAL "AUTO")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(VR_API "WIN32" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(VR_API "COCOA" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    set(VR_API "XCB" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    endif()
endif()

set(VR_API_PATH "Noop")

if(VR_API STREQUAL "WIN32")
    set(VR_API_PATH "win32")
elseif(VR_API STREQUAL "UWP")
    set(VR_API_PATH "uwp")
elseif(VR_API STREQUAL "COCOA")
    set(VR_API_PATH "cocoa")
elseif(VR_API STREQUAL "UIKIT")
    set(VR_API_PATH "uikit")
elseif(VR_API STREQUAL "XCB")
    set(VR_API_PATH "xcb")
elseif(VR_API STREQUAL "XLIB")
    set(VR_API_PATH "xlib")
elseif(VR_API STREQUAL "ANDROID")
    set(VR_API_PATH "android")
elseif(VR_API STREQUAL "WASM")
    set(VR_API_PATH "wasm")
elseif(VR_API STREQUAL "NOOP")
    set(VR_API_PATH "noop")
else()
    message( SEND_ERROR "VR_API can only be either AUTO, NOOP, WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, or WASM.")
endif()

message( STATUS "Building Vulkan Renderer for " ${VR_API_PATH} )

# =============================================================

# Sources
file(GLOB_RECURSE FILE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    #
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.sc
)

# Solution Filters
foreach(source IN LISTS FILE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    #string(REPLACE "src" "" source_path_final "${source_path_msvc}")
    #source_group("${source_path_final}" FILES "${source}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

# Main files
file(GLOB_RECURSE MAIN_SOURCES RELATIVE
    ${CMAKE_HOME_DIRECTORY}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/main_${VR_API_PATH}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/main_${VR_API_PATH}.mm
)

set(XMAIN_SOURCES ${MAIN_SOURCES} CACHE STRING "Global Variable - The source files for the currently active protocol.")

# =============================================================

# Vulkan Renderer Functions
function(vr_setup versionMajor versionMinor versionPatch versionRevision companyName iconPath)
    # @TODO - implement
    message("Warning: vr_setup has not yet been implemented.")
endfunction()

function(vr_add_executable targetProject targetSources)
    message("Creating Vulkan Renderer executable:")

    foreach(source IN LISTS XMAIN_SOURCES)
        source_group("" FILES "${source}")
    endforeach()
    set(VR_FILES "${XMAIN_SOURCES}" "${targetSources}")

    if(VR_API STREQUAL "WIN32" OR VR_API STREQUAL "UWP")
        add_executable(
            ${targetProject}
            WIN32
            "${VR_FILES}"
        )
    elseif(VR_API STREQUAL "COCOA" OR VR_API STREQUAL "UIKIT")
        add_executable(
            ${targetProject}
            MACOSX_BUNDLE
            ${VR_FILES}

        )
    elseif(VR_API STREQUAL "XCB" OR VR_API STREQUAL "XLIB")
        add_executable(
            ${targetProject}
            ${VR_FILES}
        )
    elseif(VR_API STREQUAL "ANDROID")
        add_executable(
            ${targetProject}
            ${VR_FILES}
        )
    elseif(VR_API STREQUAL "WASM")
        add_executable(
            ${targetProject}
            ${VR_FILES}
        )
    elseif(VR_API STREQUAL "NOOP")
        add_executable(
            ${targetProject}
            ${VR_FILES}
        )
    endif()

    target_compile_definitions(
		${targetProject}
		PRIVATE 
        VR_${VR_API}=1
        "-D_CRT_SECURE_NO_WARNINGS" #
		"-D__STDC_FORMAT_MACROS" #
	)

    set_property(
        TARGET 
        ${targetProject} 
        PROPERTY 
        VS_DEBUGGER_WORKING_DIRECTORY 
        "${CMAKE_CURRENT_SOURCE_DIR}/runtime")

endfunction()

# =============================================================

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

# =============================================================

# Vulkan Renderer Dependencies
if(VR_API STREQUAL "COCOA")
  add_definitions("-x objective-c++")
  find_library(COCOA_LIBRARY Cocoa)
  find_library(QUARTZ_LIBRARY QuartzCore)
  target_link_libraries(
    ${PROJECT_NAME}
    ${COCOA_LIBRARY}
    ${QUARTZ_LIBRARY}
)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"
)
elseif(VR_API STREQUAL "UIKIT")
    add_definitions("-x objective-c++")
    # Locate system libraries on iOS
    find_library(UIKIT UIKit)
    find_library(METALKIT_LIBRARY MetalKit)

    find_library(FOUNDATION Foundation)
    find_library(MOBILECORESERVICES MobileCoreServices)
    find_library(CFNETWORK CFNetwork)
    find_library(SYSTEMCONFIGURATION SystemConfiguration)
    
    target_link_libraries(
    ${PROJECT_NAME}
    ${UIKIT}
    ${METALKIT_LIBRARY}
    ${FOUNDATION}
    ${CFNETWORK}
    ${SYSTEMCONFIGURATION}
    )
    set_target_properties(
    ${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"
    )
    # Turn on ARC
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
elseif(VR_API STREQUAL "XLIB")
    find_package(X11 REQUIRED)
    if(X11_FOUND)
        message("Found X11 Libraries.")
        message("X11 includes = ${X11_INCLUDE_DIR}")
        message("X11 Libraries = ${X11_LIBRARIES}")
        target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
    endif()
elseif(VR_API STREQUAL "XCB")
    find_package(X11 REQUIRED)
    message("Found XCB Libraries.")
    message("XCB Include Path = ${X11_xcb_INCLUDE_PATH}")
    message("XCB Lib = ${X11_xcb_LIB}")
    target_link_libraries(${PROJECT_NAME} ${X11_xcb_LIB})
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_xcb_INCLUDE_PATH})
endif()

# =============================================================

add_subdirectory(3rdparty/bgfx)
target_link_libraries(${PROJECT_NAME} PUBLIC bgfx bx bimg bimg_decode)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bgfx/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bgfx/bx/include
    )

# =============================================================

include(CMakeParseArguments)

include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/bgfx/cmake/bgfx/util/ConfigureDebugging.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bgfx/cmake/bgfxToolUtils.cmake)

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

# =============================================================

# Preprocessor Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC VR_${VR_API}=1 NOMINMAX)

# Put in a "vulkan-renderer" folder in Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "vulkan-renderer")
