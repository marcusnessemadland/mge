/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "../renderer/imgui.h"
#include "../renderer/imgui/imgui.h"
#include "engine/window.h"

namespace vr
{
	void Imgui::keyDown(const SDL_Event& _event)
	{
		if (_event.key.key == SDLK_F1 && _event.key.down)
		{
			m_show = !m_show;
		}
	}

	void Imgui::mouseButtonDown(const SDL_Event& _event)
	{
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			button |= (1 << ImGuiMouseButton_Left);
		}
		if (_event.button.button == SDL_BUTTON_RIGHT)
		{
			button |= (1 << ImGuiMouseButton_Right);
		}
		if (_event.button.button == SDL_BUTTON_MIDDLE)
		{
			button |= (1 << ImGuiMouseButton_Middle);
		}
	}

	void Imgui::mouseButtonUp(const SDL_Event& _event)
	{
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			button &= ~(1 << ImGuiMouseButton_Left);
		}
		if (_event.button.button == SDL_BUTTON_RIGHT)
		{
			button &= ~(1 << ImGuiMouseButton_Right);
		}
		if (_event.button.button == SDL_BUTTON_MIDDLE)
		{
			button &= ~(1 << ImGuiMouseButton_Middle);
		}
	}

	void Imgui::mouseWheel(const SDL_Event& _event)
	{
		scroll += (int32_t)_event.wheel.y;
	}

	void Imgui::mouseMotion(const SDL_Event& _event)
	{
		mouseX = (int32_t)_event.motion.x;
		mouseY = (int32_t)_event.motion.y;
	}

	Imgui::Imgui(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<Window> _window)
		: m_view(_view)
		, m_common(_common)
		, m_window(_window)
		, m_show(false)
		, mouseX(0)
		, mouseY(0)
		, scroll(0)
		, button(0)
	{
		imguiCreate();

		_window->registerEvent(SDL_EVENT_KEY_DOWN, [this](const SDL_Event& event)
		{
			this->keyDown(event);
		});

		_window->registerEvent(SDL_EVENT_MOUSE_BUTTON_DOWN, [this](const SDL_Event& event)
		{
			this->mouseButtonDown(event);
		});

		_window->registerEvent(SDL_EVENT_MOUSE_BUTTON_UP, [this](const SDL_Event& event)
		{
			this->mouseButtonUp(event);
		});

		_window->registerEvent(SDL_EVENT_MOUSE_WHEEL, [this](const SDL_Event& event)
		{
			this->mouseWheel(event);
		});

		_window->registerEvent(SDL_EVENT_MOUSE_MOTION, [this](const SDL_Event& event)
		{
			this->mouseMotion(event);
		});
	}

	Imgui::~Imgui()
	{
		imguiDestroy();
	}

	void Imgui::render()
	{
		if (m_show)
		{
			imguiBeginFrame(
				mouseX,
				mouseY,
				button,
				scroll,
				m_window->getWidth(),
				m_window->getHeight(),
				-1,
				m_view);

			ImGui::ShowDemoWindow();

			imguiEndFrame();
		}
	}
}