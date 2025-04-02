/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/entities/model.h"
#include "engine/world.h"
#include "engine/entity.h"
#include "engine/renderer.h"
#include "engine/scene.h"

#include <chrono>
#include <cassert>

namespace vr {

	World::World()
		: m_world(nullptr)
		, m_camera(nullptr)
	{
	}

	World::~World()
	{
	}

    void World::update()
    {
        if (m_world == nullptr)
        {
            m_world = shared_from_this();
        }

        static double lastTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double currentTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double frameMsCpu = (currentTime - lastTime) * 1000.0f;
        lastTime = currentTime;
        double dt = frameMsCpu * 0.001; 

        m_sdTotal.pushSample(float(frameMsCpu));

        auto startTime = std::chrono::high_resolution_clock::now();

        for (uint32_t ii = 0; ii < m_entities.size(); ++ii)
        {
            m_entities[ii]->preUpdate(dt);
            m_entities[ii]->update(dt);
            m_entities[ii]->postUpdate(dt);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> updateDuration = endTime - startTime;

        m_sdGame.pushSample(float(updateDuration.count() * 1000.0f));
    }

	void World::render(std::shared_ptr<Renderer> _renderer)
	{
		assert(m_camera != nullptr); 
		_renderer->render(m_world, m_camera);
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
