/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "vertexpostex.h"

namespace mge
{
	void VertexPosTex::init()
	{
		ms_layout
			.begin() 
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	};

	bgfx::VertexLayout VertexPosTex::ms_layout;

} // namespace mge

