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

		void update(const World* _world, const Camera* _camera);
		void postUpdate();
		void render(const World* _world, const Camera* _camera);

	public:
		Renderer(std::shared_ptr<Window> _window, RendererType::Enum _type);
		~Renderer();

		/// 
		friend std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, RendererType::Enum _type);

	private:
		std::shared_ptr<Window> m_window;

		std::unique_ptr<CommonResources> m_common;
		std::unique_ptr<GBuffer> m_gbuffer;
		std::unique_ptr<ToneMapping> m_tonemapping;

		const World* m_world;
	};
}
