/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "vulkan-renderer/vulkan-renderer.h"

namespace vr
{
	State g_state;

	bool init(MainArgs)
	{
		g_state = State(MainArgsVars);
		return true;
	}

	const State& getState()
	{
		return g_state;
	}

} // namespace vr