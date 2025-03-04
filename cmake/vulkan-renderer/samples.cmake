#
file(
    GLOB_RECURSE
	SOURCE_FILES
    ${VR_DIR}/../samples/00-sponza/*.h
    ${VR_DIR}/../samples/00-sponza/*.cpp
)
add_executable(00-sponza ${SOURCE_FILES})
target_link_libraries(00-sponza PRIVATE vulkan-renderer)
target_include_directories(00-sponza PRIVATE ${VR_DIR} ${BX_DIR}/include)

# Put in a "samples" folder in Visual Studio
set_target_properties(00-sponza PROPERTIES FOLDER "samples")
