/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "vulkan-renderer/vulkan-renderer.h"

namespace vr
{
	std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags)
	{
		std::shared_ptr<Window> window = std::make_shared<Window>(_name, _x, _y, _width, _height, _flags);
		return window;
	}

	std::shared_ptr<EventQueue> createEventQueue(std::shared_ptr<Window>& _window)
	{
		std::shared_ptr<EventQueue> event = std::make_shared<EventQueue>();
		_window->setEventQueue(event.get()); // @todo Not a fan of this
		return event;
	}

	std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window>& _window)
	{
		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(_window.get());
		return renderer;
	}

	std::shared_ptr<World> createWorld()
	{
		std::shared_ptr<World> world = std::make_shared<World>();
		return world;
	}

	std::shared_ptr<Camera> createCamera(std::shared_ptr<World>& _world, Projection::Enum _projection)
	{
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		return camera;
	}

} // namespace vr

namespace vr
{
	State g_state;

	bool init(MainArgs)
	{
		g_state = State(MainArgsVars);
		return true;
	}

	const State& getState()
	{
		return g_state;
	}

} // namespace vr