/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/sampledata.h"

#include <bgfx/bgfx.h>

#include <memory>

namespace mge
{
	struct BgfxCallback;
	struct CommonResources;

	class Window;
	class Camera;
	class GBuffer;
	class ToneMapping;
	class Imgui;

	/// Renderer.
	/// 
	class Renderer : public std::enable_shared_from_this<Renderer>
	{
		friend class World;
		friend class Imgui;

		void dbgTextPrintStats(const bgfx::Stats* _stats);

		void update(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);
		void postUpdate();
		void render(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);

	public:
		Renderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type);
		~Renderer();

		/// Create the renderer.
		/// 
		/// @param[in] _window The window context to render to.
		/// @param[in] _type The graphics API to use.
		/// 
		/// @returns Shared Renderer.
		/// 
		friend std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type);

	public:
		SampleData m_sd;
		SampleData m_sdCpu;
		SampleData m_sdGpu;

	private:
		std::shared_ptr<Window> m_window;
		std::shared_ptr<World> m_world;

		std::unique_ptr<BgfxCallback> m_callback;

		std::shared_ptr<CommonResources> m_common;
		std::shared_ptr<GBuffer> m_gbuffer;
		std::shared_ptr<ToneMapping> m_tonemapping;
		std::shared_ptr<Imgui> m_imgui;
	};

} // namespace mge
