/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/camera.h"
#include "engine/world.h"

#include <bgfx/bgfx.h>

namespace vr
{
	Camera::Camera(World* _world, Projection::Enum _mode)
		: m_projMode(_mode)
		, m_fov(70.0f)
		, m_near(0.01f)
		, m_far(10000.0f)
		, m_position(0.0f, 2.0f, -10.0f)
		, m_forward(0.0f, 0.0f, 1.0f)
		, m_up(0.0f, 1.0f, 0.0f)
	{
		_world->m_camera = this;
	}

	void Camera::setFOV(float _fov)
	{
		m_fov = _fov;
	}

	void Camera::setClipping(float _near, float _far)
	{
		m_near = _near;
		m_far = _far;
	}

	std::shared_ptr<Camera> createCamera(std::shared_ptr<World> _world, Projection::Enum _mode)
	{
		return std::make_shared<Camera>(_world.get(), _mode);
	}

} // namespace vr