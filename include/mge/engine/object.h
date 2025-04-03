/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"
#include "engine/component.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace mge
{
	class World;

	/// World Object.
	///
	class Object : public std::enable_shared_from_this<Object>
	{
		friend class World;

		void preUpdate(double _dt);
		void postUpdate(double _dt);

	public:
		Object();
		~Object();

		/// Set the position of the object.
		/// 
		/// @param[in] _position The new position of the object.
		void setPosition(const Vec3& _position);

		/// Get the current position of the object.
		/// 
		/// @returns The current position of the object.
		/// 
		Vec3 getPosition() const;

		/// Set the rotation of the object using Euler angles.
		/// 
		/// @param[in] _euler The new rotation as Euler angles (pitch, yaw, roll).
		/// 
		void setRotation(const Vec3& _euler);

		/// Set the rotation of the object using a quaternion.
		/// 
		/// @param[in] _rotation The new rotation as a quaternion.
		/// 
		void setRotation(const Quat& _rotation);

		/// Get the current rotation of the object as a quaternion.
		/// 
		/// @returns The current rotation of the object.
		/// 
		Quat getRotation() const;

		/// Set the scale of the object.
		/// 
		/// @param[in] _scale The new scale of the object.
		/// 
		void setScale(const Vec3& _scale);

		/// Get the current scale of the object.
		/// 
		/// @returns The current scale of the object.
		/// 
		Vec3 getScale() const;

		/// Add component of type.
		/// 
		/// @param[in] _args Component arguments.
		/// 
		template<typename T, typename... Args>
		void addComponent(Args&&... _args);  

		/// Get component of type.
		/// 
		/// @returns Shared Component.
		/// 
		template<typename T>
		std::shared_ptr<T> getComponent() const;  

	protected:
		virtual void update(double _dt) {};

	private:
		std::unordered_map<std::string, std::shared_ptr<Component>> m_components;
		Vec3 m_position;
		Quat m_rotation;
		Vec3 m_scale;
	};

	/// Create a world object of type.
	/// 
	/// @param[in] _world Shared world that will own this object.
	/// @param[in] _args Object arguments.
	/// 
	/// @returns Shared Object.
	/// 
	template<typename T, typename... Args>
	std::shared_ptr<T> createObject(std::shared_ptr<World> _world, Args&&... _args); 

} // namespace mge

#include "inline/object.inl"
