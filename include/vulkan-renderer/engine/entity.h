/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <memory>
#include <vector>
#include <utility>
#include <type_traits>

namespace vr
{
	class World;
	class ComponentI;

	class Entity
	{
		friend class World;
		friend class GBuffer;
		friend class Scene;

		void preUpdate(double _dt);
		void postUpdate(double _dt);

	public:
		Entity();
		~Entity();

		/// 
		virtual void update(double _dt) {};

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

		///
		template<typename T, typename... Args>
		void addComponent(Args&&... _args)
		{
			static_assert(std::is_base_of<ComponentI, T>::value, 
				"T must be a subclass of ComponentI");
			m_components.push_back(std::make_shared<T>(std::forward<Args>(_args)...));
		}

		template<typename T>
		std::vector<std::shared_ptr<T>> getAllComponents() const
		{
			static_assert(std::is_base_of<ComponentI, T>::value,
				"T must be a subclass of ComponentI");

			std::vector<std::shared_ptr<T>> matchingComponents;
			for (const auto& component : m_components)
			{
				auto castedComponent = std::dynamic_pointer_cast<T>(component);
				if (castedComponent)
				{
					matchingComponents.push_back(castedComponent);
				}
			}
			return matchingComponents;
		}

	private:
		Vec3 m_position;
		Quat m_rotation;
		Vec3 m_scale;

		std::vector<std::shared_ptr<ComponentI>> m_components;
	};

	/// 
	template<typename T, typename... Args>
	std::shared_ptr<T> createEntity(std::shared_ptr<World> _world, Args&&... _args)
	{
		return _world->makeEntity<T>(std::forward<Args>(_args)...);
	}

} // namespace vr