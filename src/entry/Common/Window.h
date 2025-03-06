#pragma once

#include <bx/platform.h>

#ifdef BX_PLATFORM_WINDOWS
#include "../Win32/Win32Window.h"
#else
#include "../Noop/NoopWindow.h"
#endif

#include <memory>

namespace xwin
{
typedef std::shared_ptr<Window> WindowPtr;
typedef std::weak_ptr<Window> WindowWeakPtr;
}