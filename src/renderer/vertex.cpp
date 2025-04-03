/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/vertex.h"

namespace mge
{
	void Vertex::init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Weight, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 1, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Indices, 4, bgfx::AttribType::Uint8)
			.end();
	};

	bgfx::VertexLayout Vertex::ms_layout;

} // namespace mge

