/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/mesh.h"
#include "engine/material.h"

namespace vr
{
	Mesh::Mesh(const Primitive::Enum _primitive, const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices)
		: m_primitive(_primitive)
		, m_vertices(_vertices)
		, m_indices(_indices)
		, m_material(nullptr)
	{
		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(m_vertices.data(), (uint32_t)(sizeof(Vertex) * m_vertices.size())),
			Vertex::ms_layout
		);

		m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(m_indices.data(), (uint32_t)(sizeof(uint32_t) * m_indices.size())),
			BGFX_BUFFER_INDEX32
		);
	}

	Mesh::~Mesh()
	{
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
	}

	void Mesh::setMaterial(std::shared_ptr<Material> _material)
	{
		m_material = _material;
	}

	std::shared_ptr<Mesh> createMesh(const Primitive::Enum _primitive, const std::vector<Vertex>& _vertices, const std::vector<uint32_t>& _indices)
	{
		return std::make_shared<Mesh>(_primitive, _vertices, _indices);
	}

} // namespace vr