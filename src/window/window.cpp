/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/window.h"
#include "vulkan-renderer.h"

#ifdef SDL_PLATFORM_WIN32

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    FILE* pCout = nullptr;
    FILE* pCerr = nullptr;
    freopen_s(&pCout, "CONOUT$", "w+", stdout);
    freopen_s(&pCerr, "CONOUT$", "w+", stderr);
#endif

    int argc = 0;
    char** argv = 0;

    LPWSTR* commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (NULL == commandLineArgs)
    {
        argc = 0;
    }

    if (argc > 0)
    {
        argv = (char**)malloc(sizeof(char*) * argc);
        if (argv == NULL)
        {
            argc = 0;
        }
        else
        {
            for (int iii = 0; iii < argc; iii++)
            {
                size_t wideCharLen = wcslen(commandLineArgs[iii]);
                size_t numConverted = 0;

                argv[iii] = (char*)malloc(sizeof(char) * (wideCharLen + 1));
                if (argv[iii] != NULL)
                {
                    wcstombs_s(&numConverted, argv[iii], wideCharLen + 1,
                        commandLineArgs[iii], wideCharLen + 1);
                }
            }
        }
    }
    else
    {
        argv = NULL;
    }

    _main_(argc, (const char**)argv);

    // Free up the items we had to allocate for the command line arguments.
    if (argc > 0 && argv != NULL)
    {
        for (int iii = 0; iii < argc; iii++)
        {
            char* arg = argv[iii];
            if (arg != NULL)
            {
                free(argv[iii]);
            }
        }
        free(argv);
    }

#ifdef _DEBUG
    FreeConsole();
    if (pCout != nullptr) fclose(pCout);
    if (pCerr != nullptr) fclose(pCerr);
#endif

    return 0;
}

#endif // SDL_PLATFORM_WIN32

namespace vr
{
	Window::Window(const char* _title, uint32_t _width, uint32_t _height, SDL_WindowFlags _flags)
	{
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
        window = SDL_CreateWindow(_title, _width, _height, _flags);
        running = true;

        SDL_SetWindowRelativeMouseMode(window, true); 
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

	void* Window::getNativeHandle()
	{
#if defined(SDL_PLATFORM_WIN32)
		if (HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL))
		{
			return hwnd;
		}
#endif

		return nullptr;
	}

	bool Window::isClosed()
	{
        SDL_Event event;
        while (SDL_PollEvent(&event)) // Poll all events
        {
            if (event.type == SDL_EVENT_QUIT) // Check for window close event
            {
                running = false;
            }
        }

		return !running;
	}

    bool Window::isKeyDown(SDL_Scancode _code)
    {
        return SDL_GetKeyboardState(NULL)[_code];
    }

    void Window::getRelativeMouseState(float* _x, float* _y)
    {
        SDL_GetRelativeMouseState(_x, _y);
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

    void Window::setFullscreen(bool _fullscreen)
    {
        SDL_SetWindowFullscreen(window, _fullscreen);
    }

    SDL_WindowFlags Window::getFlags()
    {
        return SDL_GetWindowFlags(window);
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

} // namespace vr

