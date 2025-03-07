/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <memory>

namespace vr
{
	class Renderer;

	class World
	{
	public:
		void update();
		void render(std::shared_ptr<Renderer>& _renderer);

	private:

	};

}
