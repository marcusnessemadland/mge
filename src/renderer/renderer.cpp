#include "vulkan-renderer/renderer.h"

#include <vulkan-local/vulkan.h>

namespace vr
{
	SomeData createSomeData()
	{
		PFN_vkCreateDebugReportCallbackEXT createDebugReportCallbackEXT;

		SomeData data = { bx::Vec3(bx::InitNone) };
		return data;
	}

} // namespace vr