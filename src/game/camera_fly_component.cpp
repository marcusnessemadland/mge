/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/components/camera_fly_component.h"
#include "engine/camera.h"
#include "engine/window.h"

namespace vr
{
    Vec3 CameraFlyComponent::getMoveInput()
    {
        Vec3 input(0.0f, 0.0f, 0.0f);
        if (m_window->isKeyDown(SDL_SCANCODE_W))
        {
            input.z -= 1.0f;
        }
        if (m_window->isKeyDown(SDL_SCANCODE_S))
        {
            input.z += 1.0f;
        }
        if (m_window->isKeyDown(SDL_SCANCODE_D))
        {
            input.x += 1.0f;
        }
        if (m_window->isKeyDown(SDL_SCANCODE_A))
        {
            input.x -= 1.0f;
        }
        input = normalize(input);

        if (m_window->isKeyDown(SDL_SCANCODE_LSHIFT))
        {
            input = input * 3.0f;
        }
        return input;
    }

    Vec2 CameraFlyComponent::getLookInput()
    {
        Vec2 delta;
        m_window->getRelativeMouseState(&delta.x, &delta.y);
        return delta * -1.0f;
    }

	CameraFlyComponent::CameraFlyComponent(std::shared_ptr<Window> _window, std::shared_ptr<Camera> _camera)
		: m_window(_window)
		, m_camera(_camera)
		, m_pitch(0.0f)
		, m_yaw(0.0f)
		, m_smoothPitch(0.0f)
		, m_smoothYaw(0.0f)
		, m_position(_camera->getPosition())
		, m_smoothPosition(_camera->getPosition())
	{
        m_window->setCursorVisible(false);
	}

	CameraFlyComponent::~CameraFlyComponent()
	{
        m_window->setCursorVisible(true);
	}

	void CameraFlyComponent::preUpdate(double _dt)
	{
		 float dt = static_cast<float>(_dt);

        const float speed = 5000.0f;
        const float sensitivity = 0.1f;
        const float smoothFactorPosition = 3.0f;  // Lower values will make the smoothing slower (more inertia)
        const float smoothFactorRotation = 10.0f;  // Higher value means faster interpolation

        Vec3 moveInput = getMoveInput() * speed * dt;
        Vec2 lookInput = getLookInput() * sensitivity;

        m_yaw += lookInput.x;
        m_pitch -= lookInput.y;
        m_pitch = clampf(m_pitch, -70.0f, 70.0f);

        m_smoothYaw = lerpf(m_smoothYaw, m_yaw, smoothFactorRotation * dt);
        m_smoothPitch = lerpf(m_smoothPitch, m_pitch, smoothFactorRotation * dt);

        Vec3 forward;
        forward.x = cosf(toRad(m_smoothPitch)) * sinf(toRad(m_smoothYaw));
        forward.y = sinf(toRad(m_smoothPitch));
        forward.z = cosf(toRad(m_smoothPitch)) * cosf(toRad(m_smoothYaw));
        forward = normalize(forward);

        Vec3 right = cross(forward, Vec3(0.0f, 1.0f, 0.0f));

        Vec3 targetVelocity = forward * moveInput.z + right * -moveInput.x;

        m_velocity = lerp(m_velocity, targetVelocity, smoothFactorPosition * dt);

        m_position = m_position + (m_velocity * dt);
        m_smoothPosition = m_position;

        // Set camera position and target
        m_camera->setPosition(m_smoothPosition);
        m_camera->setTarget(m_smoothPosition + forward);
	}

	void CameraFlyComponent::postUpdate(double _dt)
	{
	}

} // namespace vr