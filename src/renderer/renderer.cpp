/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "renderer/renderer.h"

#include <vulkan-renderer/vulkan-renderer.h>

#include <bgfx/bgfx.h>

namespace vr
{
	Renderer::Renderer(const Window* _window)
	{
		const UVec2 size = _window->getCurrentDisplaySize();
		m_width = size.x;
		m_height = size.y;

		bgfx::Init init;
		init.type = bgfx::RendererType::OpenGL;
		init.vendorId = BGFX_PCI_ID_NONE;
		init.platformData.nwh = _window->getHwnd();
		init.platformData.ndt = NULL;
		init.platformData.type = bgfx::NativeWindowHandleType::Default;
		init.resolution.width = m_width;
		init.resolution.height = m_height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);
	}

	Renderer::~Renderer()
	{
		bgfx::shutdown();
	}

	void Renderer::render()
	{
		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);

		bgfx::touch(0);

		// Swap
		bgfx::frame();
	}

} // namespace vr