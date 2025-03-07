/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "defines.h"

// Entry
#include "entry/state.h"
#ifdef VR_WIN32
#include "entry-win32/window_win32.h"
#include "entry-win32/eventqueue_win32.h"
#elif VR_NOOP
#include "entry-noop/window_noop.h"
#include "entry-noop/eventqueue_noop.h"
#endif

// Renderer
#include "renderer/renderer.h"

// World
#include "world/world.h"

// Game
#include "game/camera.h"

// API
namespace vr
{
	/// 
	std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);

	///
	std::shared_ptr<EventQueue> createEventQueue(std::shared_ptr<Window>& _window);

	///
	std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window>& _window);

	///
	std::shared_ptr<World> createWorld();

	/// 
	std::shared_ptr<Camera> createCamera(std::shared_ptr<World>& _world, Projection::Enum _projection);

} // namespace vr

#if defined(VR_WIN32)
#define MainArgs                                                               \
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
#define MainArgsVars hInstance, hPrevInstance, lpCmdLine, nCmdShow
#elif defined(VR_ANDROID)
#define MainArgs android_app* app
#define MainArgsVars app
#elif defined(VR_COCOA) || defined(VR_UIKIT)
#define MainArgs int argc, const char *argv[], void *application
#define MainArgsVars argc, argv, application
#elif defined(VR_XCB)
#define MainArgs                                                               \
    int argc, const char *argv[], xcb_connection_t *connection,                \
        xcb_screen_t *screen
#define MainArgsVars argc, argv, connection, screen
#elif defined(VR_XLIB) || defined(VR_MIR) || defined(VR_WAYLAND) ||      \
    defined(VR_WASM) || defined(VR_NOOP)
#define MainArgs int argc, const char *argv[]
#define MainArgsVars argc, argv
#endif

namespace vr
{
	bool init(MainArgs);

	const State& getState();

} // namespace vr

void _main_(int argc, const char** argv);