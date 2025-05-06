/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/world.h"
#include "engine/renderer.h"
#include "engine/objects/scene.h"

#include <chrono>

namespace mge 
{
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

        auto startTime = std::chrono::high_resolution_clock::now();

        static double lastTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double currentTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double frameMsCpu = (currentTime - lastTime) * 1000.0f;
        m_sdTotal.pushSample(float(frameMsCpu));

        lastTime = currentTime;
        double dt = frameMsCpu * 0.001; 

        for (uint32_t ii = 0; ii < m_objects.size(); ++ii)
        {
            m_objects[ii]->preUpdate(dt);
            m_objects[ii]->update(dt);
            m_objects[ii]->postUpdate(dt);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> updateDuration = endTime - startTime;

        m_sdGame.pushSample(float(updateDuration.count() * 1000.0f));
    }

	void World::render(std::shared_ptr<Renderer> _renderer)
	{
		_renderer->render(m_world, m_camera);
	}

    void World::setCamera(std::shared_ptr<Camera> _camera)
    {
        m_camera = _camera;
    }

    void World::setEnvironment(std::shared_ptr<Texture> _cubemap, Environment::Enum _type)
    {
        m_environment[_type] = _cubemap;
    }

    void World::setDirectionalLight(const Vec3& _directionalLight)
    {
        m_directionalLight = _directionalLight;
    }

	std::shared_ptr<World> createWorld()
	{
		return std::make_shared<World>();
	}

} // namespace mge
