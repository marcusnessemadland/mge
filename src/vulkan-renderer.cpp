#include "vulkan-renderer/vulkan-renderer.h"

namespace vr
{
	std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags)
	{
		WindowDesc desc;
		desc.visible = true;
		desc.name = _name;
		desc.title = _name;
		desc.x = _x;
		desc.y = _y;
		desc.width = _width;
		desc.height = _height;

		desc.centered = (_flags & VR_WINDOW_CENTERED) != 0;
		desc.resizable = (_flags & VR_WINDOW_RESIZABLE) != 0;
		desc.movable = (_flags & VR_WINDOW_MOVABLE) != 0;
		desc.closable = (_flags & VR_WINDOW_CLOSABLE) != 0;
		desc.minimizable = (_flags & VR_WINDOW_MINIMIZABLE) != 0;
		desc.maximizable = (_flags & VR_WINDOW_MAXIMIZABLE) != 0;
		desc.canFullscreen = (_flags & VR_WINDOW_CANFULLSCREEN) != 0;

		std::shared_ptr<Window> window = std::make_shared<Window>();
		window->create(desc);
		return window;
	}

	std::shared_ptr<EventQueue> createEventQueue(const std::shared_ptr<Window> _window)
	{
		std::shared_ptr<EventQueue> event = std::make_shared<EventQueue>();
		_window->setEventQueue(event.get());
		return event;
	}
}