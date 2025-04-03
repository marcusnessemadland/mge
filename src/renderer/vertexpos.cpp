/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "vertexpos.h"

namespace mge
{
	void VertexPos::init()
	{
		ms_layout
			.begin() 
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	};

	bgfx::VertexLayout VertexPos::ms_layout;

} // namespace mge

