/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/entities/model.h"
#include "engine/world.h"
#include "engine/entity.h"
#include "engine/renderer.h"
#include "engine/scene.h"

#include <bgfx/bgfx.h> // time

#include <cassert>

namespace vr {

	World::World()
		: m_camera(nullptr)
	{
	}

	void World::update()
	{
		const bgfx::Stats* stats = bgfx::getStats();

		const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
		const double frameMsCpu = double(stats->cpuTimeFrame) * toMsCpu;
		m_sampleData.pushSample(float(frameMsCpu));

		double dt = frameMsCpu * 0.001f; // In seconds

		for (uint32_t ii = 0; ii < m_entities.size(); ++ii)
		{
			m_entities[ii]->preUpdate(dt);
			m_entities[ii]->update(dt);
			m_entities[ii]->postUpdate(dt);
		}
	}

	void World::render(std::shared_ptr<Renderer> _renderer)
	{
		assert(m_camera != nullptr); 
		_renderer->render(shared_from_this(), m_camera);
	}

	std::shared_ptr<Camera> World::makeCamera(Projection::Enum _mode)
	{
		m_camera = std::make_shared<Camera>(_mode);
		return m_camera;
	}

	std::shared_ptr<World> createWorld()
	{
		return std::make_shared<World>();
	}

} // namespace vr
