/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#if defined(VR_WIN32)
#include <Windows.h>
#elif defined(VR_XCB)
#include <xcb/xcb.h>
#elif defined(VR_XLIB)
#include <X11/Xlib.h>
#endif

namespace vr
{
    struct State
    {
    #if defined(VR_WIN32)

        HINSTANCE hInstance;
        HINSTANCE hPrevInstance;
        LPSTR lpCmdLine;
        int nCmdShow;

        State()
        {
        }

        State(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
            : hInstance(hInstance), hPrevInstance(hPrevInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow)
        {
        }

    #elif defined(VR_COCOA) || defined(VR_UIKIT)

        int argc;
        const char** argv;
        void* application;

        State(int argc, const char** argv, void* application)
            : argc(argc), argv(argv), application(application)
        {
        }
    #elif defined(VR_XCB)
        int argc;
        const char** argv;
        xcb_connection_t* connection;
        xcb_screen_t* screen;
        State(int argc, const char** argv, xcb_connection_t* connection,
                  xcb_screen_t* screen)
            : argc(argc), argv(argv), connection(connection), screen(screen)
        {
        }

    #elif defined(VR_ANDROID)

        android_app* app;

        State(android_app* app) : app(app) {}

    #else

        int argc;
        const char** argv;

        State(int argc, const char** argv) : argc(argc), argv(argv) {}

    #endif
    };
}
