# To prevent this warning: https://cmake.org/cmake/help/git-stage/policy/CMP0072.html
if(POLICY CMP0072)
	cmake_policy(SET CMP0072 NEW)
endif()

# Ensure the directory exists
if(NOT IS_DIRECTORY ${VR_DIR})
	message(SEND_ERROR "Could not load vulkan-renderer, directory does not exist. ${VR_DIR}")
	return()
endif()

# Quick platform detection
if(WIN32)
    set(PLATFORM_API "Win32")
else()
    set(PLATFORM_API "Noop")
endif()

# Grab the vulkan-renderer source files
file(
	GLOB_RECURSE
	VR_SOURCES
	${VR_DIR}/src/entry/CrossWindow.cpp
    ${VR_DIR}/src/entry/CrossWindow.h
    ${VR_DIR}/src/entry/Common/*.cpp
    ${VR_DIR}/src/entry/Common/*.mm
    ${VR_DIR}/src/entry/Common/*.h
    ${VR_DIR}/src/entry/Main/Main.h
	${VR_DIR}/src/entry/Main/${PLATFORM_API}Main/*.cpp
	${VR_DIR}/src/entry/${PLATFORM_API}/*.cpp
    ${VR_DIR}/src/entry/${PLATFORM_API}/*.mm
    ${VR_DIR}/src/entry/${PLATFORM_API}/*.h
	${VR_DIR}/src/renderer/*.cpp
	${VR_DIR}/src/renderer/*.h
	${VR_DIR}/include/vulkan-renderer/*.h
)

# Create the vulkan-renderer target
if(VR_LIBRARY_TYPE STREQUAL STATIC)
	add_library(vulkan-renderer STATIC ${VR_SOURCES})
else()
	add_library(vulkan-renderer SHARED ${VR_SOURCES})
	target_compile_definitions(vulkan-renderer PUBLIC VR_SHARED_LIB_BUILD=1)
endif()

# Special Visual Studio Flags
if(MSVC)
	target_compile_definitions(vulkan-renderer PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()

# Includes
target_include_directories(
	vulkan-renderer PRIVATE ${DIRECTX_HEADERS} ${VR_DIR}/3rdparty ${VR_DIR}/3rdparty/khronos
	PUBLIC $<BUILD_INTERFACE:${VR_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# vulkan-renderer depends on bx 
target_link_libraries(vulkan-renderer PUBLIC bx)

# Put in a "vulkan-renderer" folder in Visual Studio
set_target_properties(vulkan-renderer PROPERTIES FOLDER "vulkan-renderer")

# Preserve folder structure in Visual Studio
foreach(source_file ${VR_SOURCES})
    get_filename_component(source_path "${source_file}" PATH)
    file(RELATIVE_PATH source_path "${VR_DIR}" "${source_path}")
    string(REPLACE "/" "\\" source_path "${source_path}")
    source_group("${source_path}" FILES "${source_file}")
endforeach()
