#include "renderer.h"

#include <vulkan/vulkan.hpp>

namespace vr
{
	SomeData createSomeData()
	{
		PFN_vkCreateDebugReportCallbackEXT createDebugReportCallbackEXT;

		SomeData data = { bx::Vec3(bx::InitNone) };
		return data;
	}

} // namespace vr