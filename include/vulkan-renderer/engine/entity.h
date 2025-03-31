/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"
#include "engine/component.h"

#include <memory>
#include <string>
#include <utility>
#include <type_traits>
#include <unordered_map>
#include <typeinfo>

namespace vr
{
	class World;
	class Component;

	// @todo Rename Object / Actor or something else
	class Entity : public std::enable_shared_from_this<Entity>
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
			static_assert(std::is_base_of<Component, T>::value, 
				"T must be a subclass of Component");

			const char* name = typeid(T).name();

			auto it = m_components.find(name);
			if (it != m_components.end())
			{
				// @todo Assert
				return;
			}

			m_components[name] = std::make_shared<T>(std::forward<Args>(_args)...);
			m_components[name]->m_owner = shared_from_this();
		}

		template<typename T>
		std::shared_ptr<T> getComponent() const
		{
			static_assert(std::is_base_of<Component, T>::value,
				"T must be a subclass of Component");

			const char* name = typeid(T).name();

			auto it = m_components.find(name);
			if (it != m_components.end())
			{
				return std::static_pointer_cast<T>(it->second);
			}

			return nullptr;
		}

	private:
		Vec3 m_position;
		Quat m_rotation;
		Vec3 m_scale;

		std::unordered_map<std::string, std::shared_ptr<Component>> m_components;
	};

	/// 
	template<typename T, typename... Args>
	std::shared_ptr<T> createEntity(std::shared_ptr<World> _world, Args&&... _args)
	{
		return _world->makeEntity<T>(std::forward<Args>(_args)...);
	}

} // namespace vr