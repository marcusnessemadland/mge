/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <memory>

namespace mge
{
	/// Camera projection mode.
	///
	struct Projection
	{
		enum Enum
		{
			Perspective,
			Orthographic,
		};
	};

	/// Camera data. 
	/// @todo This could be turned into an object.
	/// 
	class Camera
	{
	public:
		Camera(Projection::Enum _mode);

		/// Create and set as active camera for world.
		/// 
		/// @param[in] _world Shared world that will own this camera.
		/// @param[in] _mode Projection mode.
		/// 
		/// @returns Shared Camera.
		/// 
		friend std::shared_ptr<Camera> createCamera(Projection::Enum _mode);

		/// Get the projection mode of the camera.
		/// 
		/// @returns Camera projection mode.
		/// 
		Projection::Enum getProjectionMode();

		/// Set the field of view (FOV) of the camera.
		/// 
		/// @param[in] _fov The desired field of view (in degrees).
		/// 
		void setFOV(float _fov);

		/// Get the current field of view (FOV) of the camera.
		/// 
		/// @returns Current field of view (in degrees).
		/// 
		float getFOV();

		/// Set the near and far clipping planes of the camera.
		/// 
		/// @param[in] _near The distance to the near clipping plane.
		/// @param[in] _far The distance to the far clipping plane.
		/// 
		void setClipping(float _near, float _far);

		/// Get the near clipping plane distance of the camera.
		/// 
		/// @returns The distance to the near clipping plane.
		/// 
		float getNear();

		/// Get the far clipping plane distance of the camera.
		/// 
		/// @returns The distance to the far clipping plane.
		/// 
		float getFar();

		/// Set the position of the camera.
		/// 
		/// @param[in] _position The new position of the camera.
		/// 
		void setPosition(const Vec3& _position);

		/// Get the current position of the camera.
		/// 
		/// @returns The position of the camera.
		Vec3 getPosition();

		/// Set the target that the camera is looking at.
		/// 
		/// @param[in] _target The new target position for the camera.
		/// 
		void setTarget(const Vec3& _target);

		/// Get the current target the camera is looking at.
		/// 
		/// @returns The target position.
		/// 
		Vec3 getTarget();

		/// Set the "up" vector for the camera.
		/// 
		/// @param[in] _up The new up direction for the camera.
		/// 
		void setUp(const Vec3& _up);

		/// Get the current up direction of the camera.
		/// 
		/// @returns The up direction.
		/// 
		Vec3 getUp();

	private:
		Projection::Enum m_projMode;
		float m_fov;
		float m_near;
		float m_far;
		Vec3 m_position;
		Vec3 m_target;
		Vec3 m_up;
	};

} // namespace mge
