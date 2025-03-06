#pragma once

#ifdef VR_WIN32
#include "entry/Win32/Win32Window.h"
#elif VR_UWP
#include "entry/UWP/UWPWindow.h"
#elif VR_COCOA
#include "entry/Cocoa/CocoaWindow.h"
#elif VR_XCB
#include "entry/XCB/XCBWindow.h"
#elif VR_XLIB
#include "entry/XLib/XLibWindow.h"
#elif VR_ANDROID
#include "entry/Android/AndroidWindow.h"
#elif VR_UIKIT
#include "entry/UIKit/UIKitWindow.h"
#elif VR_WASM
#include "entry/WASM/WASMWindow.h"
#elif VR_NOOP
#include "entry/Noop/NoopWindow.h"
#endif

/**
 * Window states.
 *
 */
#define VR_WINDOW_CENTERED			UINT32_C(0x00010000) //!< Enable stencil test, less.
#define VR_WINDOW_RESIZABLE			UINT32_C(0x00020000) //!< Enable stencil test, less or equal.
#define VR_WINDOW_MOVABLE           UINT32_C(0x00030000) //!< Enable stencil test, equal.
#define VR_WINDOW_CLOSABLE          UINT32_C(0x00040000) //!< Enable stencil test, greater or equal.
#define VR_WINDOW_MINIMIZABLE       UINT32_C(0x00050000) //!< Enable stencil test, greater.
#define VR_WINDOW_MAXIMIZABLE       UINT32_C(0x00060000) //!< Enable stencil test, not equal.
#define VR_WINDOW_CANFULLSCREEN     UINT32_C(0x00070000) //!< Enable stencil test, never.
/// Default state is enable all
#define VR_WINDOW_DEFAULT (0 \
	| VR_WINDOW_CENTERED \
	| VR_WINDOW_RESIZABLE \
	| VR_WINDOW_MOVABLE \
	| VR_WINDOW_CLOSABLE \
	| VR_WINDOW_MINIMIZABLE \
	| VR_WINDOW_MAXIMIZABLE \
	| VR_WINDOW_CANFULLSCREEN \
	)

namespace vr
{
	/// 
	std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);

	///
	std::shared_ptr<EventQueue> createEventQueue(const std::shared_ptr<Window> _window);
}