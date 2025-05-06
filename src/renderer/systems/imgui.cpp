/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "imgui.h"

#include "../imgui/imgui.h"
#include "../common_resources.h"

#include "engine/window.h"
#include "engine/settings.h"

#include "shadow_mapping.h"
#include "gbuffer.h"
#include "skybox.h"
#include "tone_mapping.h"

namespace mge
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

	Imgui::Imgui(bgfx::ViewId _view,
		std::shared_ptr<CommonResources> _common,
		std::shared_ptr<Window> _window)
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

		m_window->registerEvent(SDL_EVENT_KEY_DOWN, [this](const SDL_Event& event)
		{
			this->keyDown(event);
		});

		m_window->registerEvent(SDL_EVENT_MOUSE_BUTTON_DOWN, [this](const SDL_Event& event)
		{
			this->mouseButtonDown(event);
		});

		m_window->registerEvent(SDL_EVENT_MOUSE_BUTTON_UP, [this](const SDL_Event& event)
		{
			this->mouseButtonUp(event);
		});

		m_window->registerEvent(SDL_EVENT_MOUSE_WHEEL, [this](const SDL_Event& event)
		{
			this->mouseWheel(event);
		});

		m_window->registerEvent(SDL_EVENT_MOUSE_MOTION, [this](const SDL_Event& event)
		{
			this->mouseMotion(event);
		});
	}

	Imgui::~Imgui()
	{
		imguiDestroy();
	}

	void Imgui::render(std::shared_ptr<Renderer> _renderer)
	{
		// Begin timer
		m_sd.begin();

		// Show imgui menu
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

			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Modern Graphics Engine", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
			{
				Settings& settings = getSettings();

				// Scene
				if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Settings::Scene& scene = settings.scene;
					// display scene triangles vertex buffers etc
					// display scene hiarchy? how many objects vs rendered objects
					// instancing etc?
				}

				// Camera
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Settings::Camera& camera = settings.camera;
					// camera settings, fov, speed, sensitivity, lag etc.
				}

				// Renderer Settings
				if (ImGui::CollapsingHeader("Renderer"))
				{
					Settings::Renderer& renderer = settings.renderer;
					
					// actual render system settings
					// probe res, shadow map size, etc
				}

				// Profiling
				if (ImGui::CollapsingHeader("Profiling", ImGuiTreeNodeFlags_DefaultOpen))
				{
					std::shared_ptr<ShadowMapping> shadowmap = _renderer->m_shadowmapping;
					if (ImGui::TreeNodeEx("Shadow Mapping", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"Shadow Mapping", shadowmap->m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					std::shared_ptr<GBuffer> gbuffer = _renderer->m_gbuffer;
					if (ImGui::TreeNodeEx("GBuffer", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"GBuffer", gbuffer->m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					std::shared_ptr<Skybox> skybox = _renderer->m_skybox;
					if (ImGui::TreeNodeEx("Skybox", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"Skybox", skybox->m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					std::shared_ptr<ToneMapping> tonemapping = _renderer->m_tonemapping;
					if (ImGui::TreeNodeEx("Tone Mapping", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"Tone Mapping", tonemapping->m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					if (ImGui::TreeNodeEx("ImGui", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"ImGui", m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					ImGui::Separator();

					if (ImGui::TreeNodeEx("Total", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"Total", _renderer->m_sd.getAverage()))
					{
						ImGui::TreePop();
					}

					if (ImGui::TreeNodeEx("Total (with frame)", ImGuiTreeNodeFlags_Leaf, "%-35s: %.2f ms",
						"Total (with frame)", _renderer->m_sdCpu.getAverage()))
					{
						ImGui::TreePop();
					}
				}

				// Debugging
				if (ImGui::CollapsingHeader("Debugging", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Settings::Debugging& debugging = settings.debugging;

					const char* debugOptions[] = {
						"None",
						"DiffuseRoughness",
						"Normal",
						"FresnelMetallic",
						"EmissiveOcclusion",
						"Depth"
					};

					ImGui::Combo("Buffer Debug Mode", reinterpret_cast<int*>(&debugging.buffer), debugOptions, IM_ARRAYSIZE(debugOptions));
				}
			}
			ImGui::End();

			imguiEndFrame();
		}

		// End timer
		m_sd.pushSample(m_sd.end());
	}
}