/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/entity.h"

#include <memory>
#include <vector>

namespace vr
{
	class World;
	class Mesh;

	class Model : public Entity
	{
		friend class Renderer;
		friend class GBuffer;

	public:
		Model(std::shared_ptr<World> _world);
		~Model();

		///
		friend std::shared_ptr<Model> createModel(std::shared_ptr<World> _world);

		/// 
		void addMesh(std::shared_ptr<Mesh> _mesh);

	private:
		std::vector<Mesh*> m_meshes;
	};

} // namespace vr