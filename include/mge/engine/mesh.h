/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "vertex.h"

#include <bgfx/bgfx.h>

#include <memory>
#include <vector>

namespace mge
{
	class Material;

	/// Sub Mesh.
	/// 
	class SubMesh
	{
		friend class Scene;
		friend class GBuffer;
		friend class ShadowMapping;

	public:
		SubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material = nullptr);
		~SubMesh();

		/// Create a Sub Mesh.
		/// 
		/// @param[in] _indices List of indices to shared vertices in parent mesh.
		/// @param[in] _material Material to be used to render this sub mesh. 
		/// 
		/// @returns Shared Sub Mesh.
		/// 
		friend std::shared_ptr<SubMesh> createSubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material = nullptr);

		/// Set the material for this sub mesh.
		/// 
		/// @param[in] _material Shared material to be used for rendering this sub mesh.
		/// 
		void setMaterial(std::shared_ptr<Material> _material);

	private:
		bgfx::IndexBufferHandle m_ibh;
		std::vector<uint32_t> m_indices;
		std::shared_ptr<Material> m_material;
	};

	/// Mesh.
	/// 
	class Mesh
	{
		friend class Scene;
		friend class GBuffer;
		friend class ShadowMapping;

	public:
		Mesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes);
		Mesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);
		~Mesh();

		/// Create a mesh from a list of vertices and sub-meshes.
		/// 
		/// @param[in] _vertices List of shared mesh vertices.
		/// @param[in] _submeshes List of shared sub-meshes.
		/// 
		/// @returns Shared Mesh.
		/// 
		friend std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes);

		/// Create a mesh from a list of vertices and indices.
		/// 
		/// @param[in] _vertices List of mesh vertices.
		/// @param[in] _indices List of indices defining mesh faces.
		/// 
		/// @remark This will create a single Sub Mesh with given indices.
		/// 
		/// @returns Shared Mesh.
		/// 
		friend std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices);

		/// Set the material for the entire mesh.
		/// 
		/// @param[in] _material Shared material to be applied to the mesh.
		/// 
		/// @remark This will be applied to every single Sub Mesh.
		/// 
		void setMaterial(std::shared_ptr<Material> _material);

		/// Set the material for a specific Sub Mesh.
		/// 
		/// @param[in] _material Shared material to be applied.
		/// @param[in] _idx Index of the Sub Mesh.
		/// 
		void setMaterial(std::shared_ptr<Material> _material, uint32_t _idx);

	private:
		bgfx::VertexBufferHandle m_vbh;
		std::vector<Vertex> m_vertices;
		std::vector<std::shared_ptr<SubMesh>> m_submeshes;
	};

} // namespace mge
