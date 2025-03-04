#
file(
	GLOB_RECURSE 
	SOURCE_FILES
    ${VR_DIR}/*.h
    ${VR_DIR}/*.cpp
)
add_library(vulkan-renderer ${SOURCE_FILES})
target_link_libraries(vulkan-renderer PUBLIC bx)
target_include_directories(vulkan-renderer PRIVATE ${VR_DIR} ${BX_DIR}/include)

#
find_package(Vulkan REQUIRED)
target_link_libraries(vulkan-renderer PRIVATE Vulkan::Vulkan)
