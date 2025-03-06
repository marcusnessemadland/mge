#pragma once

#include "State.h"

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

const XWinState& getXWinState();
}
