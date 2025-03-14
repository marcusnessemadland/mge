/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/model.h"
#include "engine/mesh.h"
#include "engine/world.h"

namespace vr
{
	Model::Model(std::shared_ptr<World> _world)
	{
		_world->m_models.push_back(this);
	}

	Model::~Model()
	{
	}

	std::shared_ptr<Model> createModel(std::shared_ptr<World> _world)
	{
		return std::make_shared<Model>(_world);
	}

	void Model::addMesh(std::shared_ptr<Mesh> _mesh)
	{
		m_meshes.push_back(_mesh.get());
	}

} // namespace vr


