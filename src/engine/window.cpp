/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/window.h"

#include "mge.h" // _main_

namespace mge
{
    Window::Window(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags)
		: quit(false)
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
        window = SDL_CreateWindow(_title, _width, _height, _flags);

        SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION, "1", SDL_HINT_OVERRIDE);
    }

    Window::~Window()
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

    std::shared_ptr<Window> createWindow(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags)
    {
        return std::make_shared<Window>(_title, _width, _height, _flags);
    }

    bool Window::isClosed()
    {
		SDL_Event event;
        SDL_Update update;

        // Events
		while (SDL_PollEvent(&event))
		{
            if (event.type == SDL_EVENT_QUIT)
                quit = true;

            auto it = pushEvents.find(event.type);
            if (it != pushEvents.end())
            {
                for (auto& callback : it->second) 
                {
                    callback(event);
                }
            }
		}

        // Updates
        auto keyboard = updateEvents.find(SDL_UPDATE_KEYBOARD);
        if (keyboard != updateEvents.end())
        {
            update.keyboard.down = SDL_GetKeyboardState(nullptr);

            for (auto& callback : keyboard->second)
            {
                callback(update);
            }
        }
        auto mouse = updateEvents.find(SDL_UPDATE_MOUSE);
        if (mouse != updateEvents.end())
        {
            float x, y;
            SDL_GetMouseState(&x, &y);
            update.mouse.x = (int32_t)x;
            update.mouse.y = (int32_t)y;

            float xrel, yrel;
            SDL_GetRelativeMouseState(&xrel, &yrel);
            update.mouse.xrel = (int32_t)xrel;
            update.mouse.yrel = (int32_t)yrel;

            for (auto& callback : mouse->second)
            {
                callback(update);
            }
        }

        return quit;
    }

    void Window::registerEvent(uint32_t _type, EventCallback _callback)
    {
        pushEvents[_type].push_back(_callback);
    }

    void Window::registerUpdate(uint32_t _type, UpdateCallback _callback)
    {
        updateEvents[_type].push_back(_callback);
    }

    void Window::setCursorVisible(bool _visible)
    {
        if (_visible)
        {
            SDL_ShowCursor();
        }
        else
        {
            SDL_HideCursor();
        }
    }

    void Window::setCursorLock(bool _lock)
    {
        SDL_SetWindowRelativeMouseMode(window, _lock);
    }

    void Window::setCursorPos(int32_t _x, int32_t _y)
    {
        SDL_WarpMouseInWindow(window, (float)_x, (float)_y);
    }

	void Window::setFullscreen(bool _fullscreen)
	{
        SDL_SetWindowFullscreen(window, _fullscreen);
	}

    uint32_t Window::getWidth()
    {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        return w;
    }

    uint32_t Window::getHeight()
    {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        return h;
    }

    void* Window::getNativeHandle()
    {
#if defined(SDL_PLATFORM_WIN32)
        return (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(SDL_PLATFORM_MACOS)
        return (__bridge NSWindow*) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif defined(SDL_PLATFORM_LINUX)
        if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
        {
            return (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        }
        else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
        {
            /*struct wl_surface *surface*/ return (struct wl_surface*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        }
#endif
    }

    void* Window::getNativeDisplayHandle()
    {
#if defined(SDL_PLATFORM_WIN32)
        return nullptr;
#elif defined(SDL_PLATFORM_MACOS)
        return nullptr;
#elif defined(SDL_PLATFORM_LINUX)
        if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
        {
            return (Display*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        }
        else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
        {
            /*struct wl_display *display*/ return (struct wl_display*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        }
#endif
    }

} // namespace mge

int main(int _argc, const char** _argv)
{
    _main_(_argc, _argv);
    return 0;
}