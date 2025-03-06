#pragma once

#include <vector>

#include "Event.h"

#ifdef VR_WIN32
#include "../Win32/Win32EventQueue.h"
#elif VR_UWP
#include "../UWP/UWPEventQueue.h"
#elif VR_COCOA
#include "../Cocoa/CocoaEventQueue.h"
#elif VR_XCB
#include "../XCB/XCBEventQueue.h"
#elif VR_XLIB
#include "../XLib/XLibEventQueue.h"
#elif VR_ANDROID
#include "../Android/AndroidEventQueue.h"
#elif VR_UIKIT
#include "../UIKit/UIKitEventQueue.h"
#elif VR_WASM
#include "../WASM/WASMEventQueue.h"
#elif VR_NOOP
#include "../Noop/NoopEventQueue.h"
#endif