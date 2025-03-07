/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "world/world.h"
#include "renderer/renderer.h"

namespace vr {

	void World::update()
	{
	}

	void World::render(std::shared_ptr<Renderer>& _renderer)
	{
		_renderer->render();
	}

} // namespace vr
