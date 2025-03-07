/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#pragma once

#include "entry/windowdesc.h"
#include "entry/event.h"
#include "entry/state.h"

#include <Windows.h>
#include <unordered_map>

#include <functional>
#include <vector>
#include <memory>

struct ITaskbarList3;

/**
 * Currently Win32Window uses the Win32 windowing protocol for the best
 * backwards compatibility possible.
 *
 * WinTouch is limited to the Windows 8 pointer API.
 *
 * Events -
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
 *
 */

namespace vr
{
    class EventQueue;

    ///
    struct Projection
    {
        enum Enum
        {
            Perspective,
            Orthographic,
        };
    };

    /// 
    class Window
    {
    public:
        Window(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);

        ~Window();

        // Initialize this window with the Win32 API.
        bool create(const WindowDesc& desc);

        // 
        bool isClosed();

        //
        void setEventQueue(EventQueue* _eventQueue);

        // Get the title of this window.
        std::string getTitle() const;

        // Set the title of this window.
        void setTitle(std::string title);

        // Get the position of this window in display space.
        UVec2 getPosition() const;

        // Set the position of this window based on the top left corner.
        void setPosition(uint32_t x, uint32_t y);

        // Show or hide the mouse cursor.
        void showMouse(bool show);

        // Set the mouse position in display space.
        void setMousePosition(uint32_t x, uint32_t y);

        // Get this window's size in pixels.
        UVec2 getWindowSize() const;

        // Get the size of the current display this window is in.
        UVec2 getCurrentDisplaySize() const;

        // returns the current top left corner this window is located in
        UVec2 getCurrentDisplayPosition() const;

        // Get the DPI scaling for the current window
        float getDpiScale() const;

        // Set the size of this window.
        void setSize(uint32_t width, uint32_t height);

        // Get the background color of this window, as a 8 bit per channel RGBA
        // value. (eg. 0xff0000ff is red with alpha at 1.0).
        uint32_t getBackgroundColor();

        // Set the background color of this window. This will be the clear color
        // when resizing, etc.
        void setBackgroundColor(uint32_t color);

        // Request that this window be minimized.
        void minimize();

        // Request that this window be maximized.
        void maximize();

        // Request that this window be closed.
        void close();

        // Track events asynchronously for this window
        void trackEventsAsync(const std::function<void(const Event e)>& fun);

        // Windows Only Functions:

        // Set progress visible in the taskbar
        void setProgress(float progress);

        // Get this window's application Handle.
        HINSTANCE getHinstance();

        // Get this window hwnd handle.
        HWND getHwnd() const;

        /**
         * It's possible to define regions in the window as part of the titlebar,
         * a help region, maximize, minimize buttons, and much more.
         */

        // The hit rectangle's type (eg. this rectangle is a titlebar)
        enum class HitRectType : size_t
        {
            None,
            TitleBar,
            Maximize,
            Minimize,
            Close,
            Help,
            HitRectTypeMax
        };
        // A Win32 special hit region, can be a titlebar, maximize button, etc.
        struct HitRect
        {
            UVec2 position;
            UVec2 size;
            HitRectType type;
        };
        std::vector<HitRect> hitRects;

        friend class EventQueue;

        // Get this Window's descriptor object.
        const WindowDesc* getDesc();

      protected:
        // Executes an event callback asynchronously, use this for non-blocking
        // events (resizing while rendering, etc.)
        void executeEventCallback(const Event e);

        std::function<void(const Event e)> mCallback;

        static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam,
                                                 LPARAM lparam);

        LRESULT WindowProc(UINT msg, WPARAM wparam, LPARAM lparam);

        // This window handle
        HWND hwnd = NULL;

        // This window's application instance.
        HINSTANCE hinstance = NULL;

        // CrossWindow's event queue
        EventQueue* mEventQueue;

        // CrossWindow's descriptor object
        WindowDesc mDesc;

        // Win32 Window State
        WNDCLASSEX wndClass = {0};

        // Win32 Window Size/Position
        RECT windowRect = {0};

        // Win32 Screen State
        DEVMODE dmScreenSettings = {0};

        // Win32 Window Behavior
        DWORD dwExStyle = 0x0;
        DWORD dwStyle = 0x0;

        // Win32 Taskbar Interface
        ITaskbarList3* mTaskbarList;

        uint32_t mBackgroundColor = 0xFFFFFFFF;
    };

    static thread_local Window* _windowBeingCreated = NULL;
    static thread_local std::unordered_map<HWND, Window*> _hwndMap = {};

}
