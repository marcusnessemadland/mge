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
	/// Vertex data layout
	///
	struct Vertex
	{
		friend class Renderer;
		friend class Mesh;

		Vec3 position;
		Vec3 normal;
		Vec3 tangent;
		Vec3 bitangent;
		Vec2 texcoord;
		float weights[4];
		float displacement;
		uint8_t indices[4];

	private:
		static void init();
		static bgfx::VertexLayout ms_layout;
	};

} // namespace mge
