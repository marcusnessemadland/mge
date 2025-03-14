/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/entity.h"
#include "engine/math.h"

#include <memory>

namespace vr
{
	class World;

	///
	struct Projection
	{
		enum Enum
		{
			Perspective,
			Orthographic,
		};
	};

	/// 
	class Camera : public Entity
	{
		friend class Renderer;

	public:
		Camera(World* _world, Projection::Enum _mode);

		/// 
		friend std::shared_ptr<Camera> createCamera(std::shared_ptr<World> _world, Projection::Enum _mode);

		/// 
		void setFOV(float _fov);
		
		/// 
		void setClipping(float _near, float _far);

	private:
		Projection::Enum m_projMode;
		float m_fov;
		float m_near;
		float m_far;
		Vec3 m_position;
		Vec3 m_forward;
		Vec3 m_up;
	};

} // namespace vr
