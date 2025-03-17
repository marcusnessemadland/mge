/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <memory>
#include <vector>

namespace vr
{
	class Renderer;
	class Camera;
	class Model;

	class World
	{
		friend class Renderer;
		friend class GBuffer;
		friend class Camera;
		friend class Model;
		friend class Scene;

	public:
		World();

		/// 
		friend std::shared_ptr<World> createWorld();

		/// 
		void update();

		/// 
		void render(std::shared_ptr<Renderer> _renderer);

	private:
		Camera* m_camera;
		std::vector<Model*> m_models;
		std::vector<Scene*> m_scenes;
	};

}
