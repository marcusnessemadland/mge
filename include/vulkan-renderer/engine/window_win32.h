/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#ifdef VR_WIN32

#pragma once

#include "engine/windowdesc.h"
#include "engine/event.h"
#include "engine/state.h"
#include "engine/math.h"

#include <windows.h>
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
    class Window
    {
        friend class EventQueue;
        friend class Renderer;

        HINSTANCE getHinstance();
        HWND getNativeHandle() const;
        void executeEventCallback(const Event e);
        LRESULT WindowProc(UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        
    public:
        Window(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);
        ~Window();

        ///  
        friend std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags);

        /// 
        bool isClosed();

        /// Get the title of this window.
        std::string getTitle() const;

        /// Set the title of this window.
        void setTitle(std::string title);

        /// Get the position of this window in display space.
        UVec2 getPosition() const;

        /// Set the position of this window based on the top left corner.
        void setPosition(uint32_t x, uint32_t y);

        /// Show or hide the mouse cursor.
        void showMouse(bool show);

        /// Set the mouse position in display space.
        void setMousePosition(uint32_t x, uint32_t y);

        /// Get this window's size in pixels.
        UVec2 getWindowSize() const;

        /// Get the size of the current display this window is in.
        UVec2 getCurrentDisplaySize() const;

        /// returns the current top left corner this window is located in
        UVec2 getCurrentDisplayPosition() const;

        /// Get the DPI scaling for the current window
        float getDpiScale() const;

        /// Set the size of this window.
        void setSize(uint32_t width, uint32_t height);

        // Get the background color of this window, as a 8 bit per channel RGBA
        // value. (eg. 0xff0000ff is red with alpha at 1.0).
        uint32_t getBackgroundColor();

        // Set the background color of this window. This will be the clear color
        // when resizing, etc.
        void setBackgroundColor(uint32_t color);

        /// Request that this window be minimized.
        void minimize();

        /// Request that this window be maximized.
        void maximize();

        /// Request that this window be fullscreen.
        void fullscreen();

        /// Request that this window be closed.
        void close();

        /// Track events asynchronously for this window
        void trackEventsAsync(const std::function<void(const Event e)>& _func);

        /// Set progress visible in the taskbar
        void setProgress(float _progress);

    private:
        std::function<void(const Event e)> mCallback;

        HWND hwnd = NULL;
        HINSTANCE hinstance = NULL;
        EventQueue* mEventQueue;
        WindowDesc mDesc;
        WNDCLASSEX wndClass = { 0 };
        RECT windowRect = { 0 };
        DEVMODE dmScreenSettings = { 0 };
        DWORD dwExStyle = 0x0;
        DWORD dwStyle = 0x0;
        ITaskbarList3* mTaskbarList;
        uint32_t mBackgroundColor = 0xFFFFFFFF;

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
        struct HitRect
        {
            UVec2 position;
            UVec2 size;
            HitRectType type;
        };
        std::vector<HitRect> hitRects;
    };

    static thread_local Window* _windowBeingCreated = NULL;
    static thread_local std::unordered_map<HWND, Window*> _hwndMap = {};

}

#endif // VR_WIN32