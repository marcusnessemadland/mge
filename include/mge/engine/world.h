/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/environment.h"
#include "engine/camera.h"
#include "engine/sampledata.h"

#include <memory>
#include <vector>

namespace mge
{
	class Renderer;
	class Object;
	class Texture;

	/// World.
	/// 
	class World : public std::enable_shared_from_this<World>
	{
		friend class Renderer;
		friend class GBuffer;
		friend class Skybox;
		friend class ShadowMapping;

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

		/// Set active camera to use for rendering.
		/// 
		/// @param[in] _camera The camera to activate. 
		/// 
		void setCamera(std::shared_ptr<Camera> _camera);

		/// Set the environment maps for rendering.
		/// 
		/// @param[in] _cubemap The cube map to use for this environment type.
		/// @param[in] _type The rendering pipeline type for this cubemap.
		/// 
		void setEnvironment(std::shared_ptr<Texture> _cubemap, Environment::Enum _type);

		/// Set directional light.
		/// 
		/// @param[in] _directionalLight The direction of the sun light.
		/// 
		void setDirectionalLight(const Vec3& _directionalLight);

	public:
		/// Should only be used internally.
		template<typename T, typename... Args>
		std::shared_ptr<T> makeObject(Args&&... _args);

	private:
		std::shared_ptr<World> m_world; // Ref to shared self
		 
		std::shared_ptr<Camera> m_camera; 
		std::shared_ptr<Texture> m_environment[Environment::Count];
		Vec3 m_directionalLight; // @todo Turn into class with more settings?

		std::vector<std::shared_ptr<Object>> m_objects;

		SampleData m_sdTotal;
		SampleData m_sdGame;
	};

} // namespace mge

#include "inline/world.inl"
