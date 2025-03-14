/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "vertex.h"

#include <bgfx/bgfx.h>

#include <memory>
#include <vector>

namespace vr
{
	struct Primitive
	{
		enum Enum
		{
			Triangles,
		};
	};

	class Material;

	class Mesh
	{
		friend class Renderer;
		friend class GBuffer;

	public:
		Mesh(const Primitive::Enum _primitive, const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);
		~Mesh();

		///
		friend std::shared_ptr<Mesh> createMesh(const Primitive::Enum _primitive, const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);
	
		///
		void setMaterial(std::shared_ptr<Material> _material);

	private:
		Primitive::Enum m_primitive;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		std::shared_ptr<Material> m_material;
	};

} // namespace vr
