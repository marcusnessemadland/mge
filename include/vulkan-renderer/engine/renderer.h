/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <bgfx/bgfx.h>

#include <stdint.h>
#include <memory>
#include <vector>

namespace vr
{
	struct RendererType
	{
		enum Enum
		{
			Auto,
			OpenGL,
			Direct3D12,
			Vulkan,
		};
	};

	class Window;
	class Camera;
	class Model;
	class World;

	struct CommonResources;
	class GBuffer;

	class Renderer
	{
		friend class World;
		friend class GBuffer;
		friend class ToneMapping;

		void update(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);
		void postUpdate();
		void render(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera);

	public:
		Renderer(std::shared_ptr<Window> _window, RendererType::Enum _type);
		~Renderer();

		/// 
		friend std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, RendererType::Enum _type);

	private:
		std::shared_ptr<Window> m_window;

		std::shared_ptr<CommonResources> m_common;
		std::shared_ptr<GBuffer> m_gbuffer;
		std::shared_ptr<ToneMapping> m_tonemapping;

		std::shared_ptr<World> m_world;
	};
}
