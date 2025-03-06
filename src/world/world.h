#pragma once

#include <memory>

namespace vr
{
	class Framebuffer;

	class World
	{
	public:
		void update();
		void render(std::shared_ptr<Framebuffer>& _framebuffer);
	};

}
