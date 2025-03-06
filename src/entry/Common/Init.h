#pragma once

#include "State.h"

#include <bx/platform.h>

#if defined(BX_PLATFORM_WINDOWS)
#define MainArgs                                                               \
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
#define MainArgsVars hInstance, hPrevInstance, lpCmdLine, nCmdShow
#endif

namespace xwin
{
bool init(MainArgs);

const XWinState& getXWinState();
}
