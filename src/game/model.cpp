/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/entities/model.h"
#include "engine/components/mesh_component.h"
#include "engine/mesh.h"
#include "engine/world.h"

namespace vr
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	std::shared_ptr<Model> createModel(std::shared_ptr<World> _world)
	{
		return _world->makeEntity<Model>();
	}

	void Model::addMesh(std::shared_ptr<Mesh> _mesh)
	{
		addComponent<MeshComponent>(_mesh);
	}

} // namespace vr


