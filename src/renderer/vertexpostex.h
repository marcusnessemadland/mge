/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <bgfx/bgfx.h>

#include <stdint.h>

namespace mge
{
	/// 
	class VertexPosTex
	{
		friend class Renderer;
		friend class Skybox;

		static void init();

	public:
		Vec3 position;
		Vec2 texcoord;

	private:
		static bgfx::VertexLayout ms_layout;
	};

} // namespace mge
