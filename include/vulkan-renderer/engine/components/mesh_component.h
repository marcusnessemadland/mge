/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/component.h"

#include <memory>

namespace vr
{
	class Mesh;

	class MeshComponent : public Component
	{
		friend class GBuffer;
		friend class Scene;

	public:
		MeshComponent(std::shared_ptr<Mesh> _mesh) : m_mesh(_mesh) {}
		~MeshComponent() {}

	protected:
		void preUpdate(double _dt) override {}
		void postUpdate(double _dt) override {}

	private:
		std::shared_ptr<Mesh> m_mesh;
	};

} // namespace vr