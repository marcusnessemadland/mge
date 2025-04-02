/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/window.h"

#include <bgfx/bgfx.h>

#include <memory>

namespace vr
{
	class Window;

	struct CommonResources;

	class Imgui
	{
		void keyDown(const SDL_Event& _event);
		void mouseButtonDown(const SDL_Event& _event);
		void mouseButtonUp(const SDL_Event& _event);
		void mouseWheel(const SDL_Event& _event);
		void mouseMotion(const SDL_Event& _event);

	public:
		Imgui(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<Window> _window);
		~Imgui();

		void render();

	private:
		bgfx::ViewId m_view;
		std::shared_ptr<CommonResources> m_common;
		std::shared_ptr<Window> m_window;

		bool m_show;

		int32_t mouseX, mouseY;
		int32_t scroll;
		uint8_t button;
	};

} // namespace vr