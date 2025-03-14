/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

namespace vr
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		/// 
		void setPosition(const Vec3& _position);

		/// 
		Vec3 getPosition() const;

		/// 
		void setRotation(const Vec3& _euler);

		/// 
		void setRotation(const Quat& _rotation);

		/// 
		Quat getRotation() const;

		/// 
		void setScale(const Vec3& _scale);

		/// 
		Vec3 getScale() const;

	private:
		Vec3 m_position;
		Quat m_rotation;
		Vec3 m_scale;
	};

} // namespace vr