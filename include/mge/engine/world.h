/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/camera.h"
#include "engine/sampledata.h"

#include <memory>
#include <vector>

namespace mge
{
	class Renderer;
	class Object;

	/// World.
	/// 
	class World : public std::enable_shared_from_this<World>
	{
		friend class Renderer;
		friend class GBuffer;

	public:
		World();
		~World();

		/// Create a world.
		/// 
		/// @returns Shared Material.
		/// 
		friend std::shared_ptr<World> createWorld();

		/// Update all world objects.
		/// 
		void update();

		/// Render the world with given renderer.
		/// 
		/// @param[in] _renderer The renderer to use.
		/// 
		void render(std::shared_ptr<Renderer> _renderer);

	public:
		/// Should only be used internally.
		template<typename T, typename... Args>
		std::shared_ptr<T> makeObject(Args&&... _args);

		/// Should only be used internally.
		std::shared_ptr<Camera> makeCamera(Projection::Enum _mode);

	private:
		std::shared_ptr<World> m_world;
		std::shared_ptr<Camera> m_camera; // @todo This could be turned into an object.
		std::vector<std::shared_ptr<Object>> m_objects;

		SampleData m_sdTotal;
		SampleData m_sdGame;
	};

} // namespace mge

#include "inline/world.inl"
