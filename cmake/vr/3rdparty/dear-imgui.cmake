# Ensure the directory exists
if(NOT IS_DIRECTORY ${VR_DIR})
	message(SEND_ERROR "Could not load vulkan-renderer, directory does not exist. ${VR_DIR}")
	return()
endif()

if(NOT DEAR_IMGUI_LIBRARIES)
	file(
		GLOB #
		DEAR_IMGUI_SOURCES #
		${VR_DIR}/3rdparty/dear-imgui/*.cpp #
		${VR_DIR}/3rdparty/dear-imgui/*.h #
		${VR_DIR}/3rdparty/dear-imgui/*.inl #
	)
	set(DEAR_IMGUI_INCLUDE_DIR ${VR_DIR}/3rdparty)
endif()
