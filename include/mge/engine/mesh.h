/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "vertex.h"

#include <bgfx/bgfx.h>

#include <memory>
#include <vector>

namespace vr
{
	class Material;
	class Entity;

	class SubMesh
	{
		friend class Renderer;
		friend class GBuffer;
		friend class Scene;

	public:
		SubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material = NULL);
		~SubMesh();

		/// 
		friend std::shared_ptr<SubMesh> createSubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material = NULL);

		/// 
		void setMaterial(std::shared_ptr<Material> _material);

	private:
		bgfx::IndexBufferHandle m_ibh;
		std::vector<uint32_t> m_indices;
		std::shared_ptr<Material> m_material;
	};

	class Mesh
	{
		friend class Renderer;
		friend class GBuffer;
		friend class Scene;

	public:
		Mesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes);
		Mesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);
		~Mesh();

		///
		friend std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes);

		///
		friend std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);

		///
		void setMaterial(std::shared_ptr<Material> _material);
		
		/// _idx = subMesh idx
		void setMaterial(std::shared_ptr<Material> _material, uint32_t _idx);

	private:
		bgfx::VertexBufferHandle m_vbh;
		std::vector<Vertex> m_vertices;
		std::vector<std::shared_ptr<SubMesh>> m_submeshes;
	};

} // namespace vr
