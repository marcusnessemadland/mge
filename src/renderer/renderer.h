/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <stdint.h>

namespace vr
{
	class Window;

	class Renderer
	{
	public:
		Renderer(const Window* _window);
		~Renderer();

		void render();

	private:
		uint32_t m_width, m_height;
	};
}
