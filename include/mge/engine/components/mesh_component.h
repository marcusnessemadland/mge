/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/component.h"

#include <memory>

namespace mge
{
	class Mesh;

	/// Mesh Component
	/// 
	class MeshComponent : public Component
	{
		friend class Scene;
		friend class GBuffer;
		friend class ShadowMapping;

	public:
		MeshComponent(std::shared_ptr<Mesh> _mesh);
		~MeshComponent();

	protected:
		void preUpdate(double _dt) override;
		void postUpdate(double _dt) override;

	private:
		std::shared_ptr<Mesh> m_mesh;
	};

} // namespace mge