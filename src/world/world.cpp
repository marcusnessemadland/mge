/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/world.h"
#include "engine/entity.h"
#include "engine/camera.h"
#include "engine/renderer.h"
#include "engine/scene.h"

#include <cassert>

namespace vr {

	World::World()
		: m_camera(NULL)
	{
	}

	void World::update()
	{
		for (auto& scene : m_scenes)
		{
			scene->update();
		}
	}

	void World::render(std::shared_ptr<Renderer> _renderer)
	{
		assert(m_camera != NULL); 
		_renderer->render(this, m_camera);
	}

	std::shared_ptr<World> createWorld()
	{
		return std::make_shared<World>();
	}

} // namespace vr
