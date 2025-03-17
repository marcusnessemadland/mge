/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "maya-bridge/shared_buffer.h"
#include "maya-bridge/shared_data.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace vr
{
	class World;
	class Model;

	/// Can only hold models
	class Scene
	{
		friend class World;

		void update();

		void modelAdded(const mb::Model& _model);
		void modelRemoved(const mb::Model& _model);
		void modelChanged(const mb::Model& _model);

	public:
		Scene(std::shared_ptr<World> _world);
		Scene(std::shared_ptr<World> _world, const char* _filepath);
		~Scene();

		/// 
		friend std::shared_ptr<Scene> createScene(std::shared_ptr<World> _world);

		/// 
		friend std::shared_ptr<Scene> loadScene(std::shared_ptr<World> _world, const char* _filepath);

		/// 
		void save(const char* _filepath);

	private:
		std::shared_ptr<World> m_world;

		std::unordered_map<std::string, std::shared_ptr<Model>> m_models;

		mb::SharedBuffer* m_writeBuffer;
		mb::SharedBuffer* m_readBuffer;
		mb::SharedData m_shared;
	};
}
