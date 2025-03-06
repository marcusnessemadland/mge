#pragma once

#include <bx/platform.h>

#ifdef BX_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace xwin
{
struct XWinState
{

#if defined(BX_PLATFORM_WINDOWS)

    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    LPSTR lpCmdLine;
    int nCmdShow;

    XWinState(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
              int nCmdShow)
        : hInstance(hInstance), hPrevInstance(hPrevInstance),
          lpCmdLine(lpCmdLine), nCmdShow(nCmdShow)
    {
    }

#else

    int argc;
    const char** argv;

    XWinState(int argc, const char** argv) : argc(argc), argv(argv) {}

#endif
    XWinState() {}
};
}
