/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <bgfx/bgfx.h>

#include <stdint.h>

namespace vr
{
	/// 
	class Vertex
	{
		friend class Renderer;
		friend class Mesh;

		static void init();

	public:
		Vec3 position;
		Vec3 normal;
		Vec3 tangent;
		Vec3 bitangent;
		Vec2 texcoord;
		float weights[4];
		float displacement;
		uint8_t indices[4];

	private:
		static bgfx::VertexLayout ms_layout;
	};

} // namespace vr
