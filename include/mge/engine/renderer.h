/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"
#include "engine/sampledata.h"

#include <bgfx/bgfx.h>

#include <stdint.h>
#include <memory>
#include <vector>

namespace vr
{
	struct BgfxCallback;
	class Window;
	class Camera;
	class Model;
	class World;

	struct CommonResources;
	class GBuffer;
	class Imgui;
	class ToneMapping;

	class Renderer
	{
		friend class World;
		friend class GBuffer;
		friend class ToneMapping;

		void update(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);
		void postUpdate();
		void render(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);

	public:
		Renderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type);
		~Renderer();

		/// 
		friend std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type);

	private:
		std::shared_ptr<Window> m_window;

		std::shared_ptr<CommonResources> m_common;
		std::shared_ptr<GBuffer> m_gbuffer;
		std::shared_ptr<ToneMapping> m_tonemapping;
		std::shared_ptr<Imgui> m_imgui;

		std::shared_ptr<World> m_world;
		
		std::unique_ptr<BgfxCallback> m_callback;

		SampleData m_sdCpu;
		SampleData m_sdGpu;
	};
}
