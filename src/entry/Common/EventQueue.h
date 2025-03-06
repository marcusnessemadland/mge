#pragma once

#include <vector>

#include "Event.h"

#include <bx/platform.h>

#ifdef BX_PLATFORM_WINDOWS
#include "../Win32/Win32EventQueue.h"
#else
#include "../Noop/NoopEventQueue.h"
#endif