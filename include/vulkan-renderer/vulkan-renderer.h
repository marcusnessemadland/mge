/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "defines.h"

#include "engine/renderer.h"
#include "engine/world.h"
#include "engine/camera.h"
#include "engine/state.h"
#include "engine/mesh.h"
#include "engine/model.h"
#include "engine/material.h"
#include "engine/texture.h"

#ifdef VR_WIN32
#include "engine/window_win32.h"
#include "engine/eventqueue_win32.h"
#elif VR_NOOP
#include "engine/window_noop.h"
#include "engine/eventqueue_noop.h"
#endif

void _main_(int argc, const char** argv);

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


