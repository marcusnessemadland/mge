# To prevent this warning: https://cmake.org/cmake/help/git-stage/policy/CMP0072.html
if(POLICY CMP0072)
	cmake_policy(SET CMP0072 NEW)
endif()

# Ensure the directory exists
if(NOT IS_DIRECTORY ${VR_DIR})
	message(SEND_ERROR "Could not load vulkan-renderer, directory does not exist. ${VR_DIR}")
	return()
endif()

# Grab the vulkan-renderer source files
file(
	GLOB_RECURSE
	VR_SOURCES
	${VR_DIR}/src/*.cpp
	${VR_DIR}/src/*.h
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