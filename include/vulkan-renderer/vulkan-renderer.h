#pragma once

#include "defines.h"

// Entry
#ifdef VR_WIN32
#include "entry/Win32/Win32Window.h"
#elif VR_NOOP
#include "entry/Noop/NoopWindow.h"
#endif

// Renderer
#include "renderer/renderer.h"
#include "renderer/framebuffer.h"

// World
#include "world/world.h"

// Entity
#include "entity/camera.h"

/// 
void _main_(int argc, const char** argv);

namespace vr
{
	///
	struct Projection
	{
		enum Enum
		{
			Perspective,
			Orthographic,
		};
	};

	/// 
	std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);

	///
	std::shared_ptr<EventQueue> createEventQueue(std::shared_ptr<Window>& _window);

	///
	std::shared_ptr<Framebuffer> createFramebuffer(std::shared_ptr<Window>& _window);

	///
	std::shared_ptr<World> createWorld();

	/// 
	std::shared_ptr<Camera> createCamera(std::shared_ptr<World>& _world, Projection::Enum _projection);
}