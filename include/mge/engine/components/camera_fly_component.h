/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/component.h"
#include "engine/math.h"

#include <memory>

namespace vr
{
	class Window;
	class Camera;

	class CameraFlyComponent : public Component
	{
	public:
		CameraFlyComponent(std::shared_ptr<Camera> _camera);
		~CameraFlyComponent();

		/// 
		void setMoveInput(const Vec3& _input);

		/// 
		void setLookInput(const Vec2& _input);

	protected:
		void preUpdate(double _dt) override;
		void postUpdate(double _dt) override;

	private:
		std::shared_ptr<Window> m_window;
		std::shared_ptr<Camera> m_camera;

		Vec3 m_moveInput;
		Vec2 m_lookInput;

		Vec3 m_velocity;

		float m_pitch, m_yaw;
		float m_smoothPitch, m_smoothYaw;

		Vec3 m_position;
		Vec3 m_smoothPosition;
	};

} // namespace vr