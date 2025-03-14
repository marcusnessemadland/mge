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
	class VertexPos
	{
		friend class Renderer;
		friend class ToneMapping;

		static void init();

	public:
		Vec3 position;

	private:
		static bgfx::VertexLayout ms_layout;
	};

} // namespace vr
