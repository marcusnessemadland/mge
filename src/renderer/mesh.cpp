/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/mesh.h"

namespace mge
{
	SubMesh::SubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material)
		: m_indices(_indices), m_material(_material)
	{
		m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(m_indices.data(), (uint32_t)(sizeof(uint32_t) * m_indices.size())),
			BGFX_BUFFER_INDEX32
		);
	}

	SubMesh::~SubMesh()
	{
		bgfx::destroy(m_ibh);
	}

	std::shared_ptr<SubMesh> createSubMesh(const std::vector<uint32_t>& _indices, std::shared_ptr<Material> _material)
	{
		return std::make_shared<SubMesh>(_indices, _material);
	}

	void SubMesh::setMaterial(std::shared_ptr<Material> _material)
	{
		m_material = _material;
	}

	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes)
		: m_vertices(_vertices)
		, m_submeshes(_submeshes)
	{
		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(m_vertices.data(), (uint32_t)(sizeof(Vertex) * m_vertices.size())),
			Vertex::ms_layout
		);
	}

	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices)
		: m_vertices(_vertices)
	{
		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(m_vertices.data(), (uint32_t)(sizeof(Vertex) * m_vertices.size())),
			Vertex::ms_layout
		);
		m_submeshes.push_back(createSubMesh(_indices));
	}

	Mesh::~Mesh()
	{
		bgfx::destroy(m_vbh);
	}

	std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<std::shared_ptr<SubMesh>>& _submeshes)
	{
		return std::make_shared<Mesh>(_vertices, _submeshes);
	}

	std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices)
	{
		return std::make_shared<Mesh>(_vertices, _indices);
	}

	void Mesh::setMaterial(std::shared_ptr<Material> _material)
	{
		for (auto& sub : m_submeshes)
		{
			sub->setMaterial(_material);
		}
	}

	void Mesh::setMaterial(std::shared_ptr<Material> _material, uint32_t _idx)
	{
		m_submeshes[_idx]->setMaterial(_material);
	}

} // namespace mge