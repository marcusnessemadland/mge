/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/camera.h"
#include "engine/sampledata.h"

#include <memory>
#include <vector>
#include <utility>

namespace vr
{
	class Renderer;
	class Camera;
	class Model;

	class World : public std::enable_shared_from_this<World>
	{
		friend class Renderer;
		friend class GBuffer;
		friend class Camera;
		friend class Model;
		friend class Scene;

	public:
		World();

		/// 
		friend std::shared_ptr<World> createWorld();

		/// 
		void update();

		/// 
		void render(std::shared_ptr<Renderer> _renderer);

	public:
		template<typename T, typename... Args>
		std::shared_ptr<T> makeEntity(Args&&... _args) // @todo Move to .impl
		{
			static_assert(std::is_base_of<Entity, T>::value,
				"T must be a subclass of Entity");
			std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(_args)...);
			m_entities.push_back(entity);
			return entity;
		}
		std::shared_ptr<Camera> makeCamera(Projection::Enum _mode);

	private:
		std::shared_ptr<World> m_world;
		std::shared_ptr<Camera> m_camera; // @todo Turn into entity
		std::vector<std::shared_ptr<Entity>> m_entities;

		SampleData m_sampleData;
	};

} // namespace vr
