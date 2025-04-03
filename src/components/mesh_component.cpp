/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/components/mesh_component.h"

namespace mge
{
	MeshComponent::MeshComponent(std::shared_ptr<Mesh> _mesh)
		: m_mesh(_mesh) 
	{
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::preUpdate(double _dt)
	{
	}

	void MeshComponent::postUpdate(double _dt)
	{
	}

} // namespace mge