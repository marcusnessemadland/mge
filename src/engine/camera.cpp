/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/camera.h"
#include "engine/world.h"

namespace mge
{
	Camera::Camera(Projection::Enum _mode)
		: m_projMode(_mode)
		, m_fov(70.0f)
		, m_near(0.01f)
		, m_far(100000.0f)
		, m_position(Vec3(0.0f, 0.0f, -1.0f))
		, m_target(Vec3(0.0f, 0.0f, 0.0f))
		, m_up(Vec3(0.0f, 1.0f, 0.0f))
	{
	}

	std::shared_ptr<Camera> createCamera(Projection::Enum _mode)
	{
		return std::make_shared<Camera>(_mode);
	}

	Projection::Enum Camera::getProjectionMode()
	{
		return m_projMode;
	}

	void Camera::setFOV(float _fov)
	{
		m_fov = _fov;
	}

	float Camera::getFOV()
	{
		return m_fov;
	}

	void Camera::setClipping(float _near, float _far)
	{
		m_near = _near;
		m_far = _far;
	}

	float Camera::getNear()
	{
		return m_near;
	}

	float Camera::getFar()
	{
		return m_far;
	}

	void Camera::setPosition(const Vec3& _position)
	{
		m_position = _position;
	}

	Vec3 Camera::getPosition()
	{
		return m_position;
	}

	void Camera::setTarget(const Vec3& _target)
	{
		m_target = _target;
	}

	Vec3 Camera::getTarget()
	{
		return m_target;
	}

	void Camera::setUp(const Vec3& _up)
	{
		m_up = _up;
	}

	Vec3 Camera::getUp()
	{
		return m_up;
	}

} // namespace mge