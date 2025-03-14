/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#ifdef VR_WIN32

#include "engine/window_win32.h"
#include "engine/eventqueue_win32.h"

#include "vulkan-renderer.h"

#include "Shobjidl.h"
#include "dwmapi.h"

#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

enum Style : DWORD
{
    windowed = WS_OVERLAPPEDWINDOW,
    aero_borderless = WS_POPUP | WS_THICKFRAME,
    basicBorderless = WS_CAPTION | WS_OVERLAPPED | WS_THICKFRAME |
                      WS_MINIMIZEBOX | WS_MAXIMIZEBOX
};

HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));

namespace vr
{
    Window::Window(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags)
        : mEventQueue(NULL)
    {
        WindowDesc desc;
        desc.visible = true;
        desc.name = _name;
        desc.title = _name;
        desc.x = _x;
        desc.y = _y;
        desc.width = _width;
        desc.height = _height;
        desc.centered = (_flags & VR_WINDOW_CENTERED) != 0;
        desc.resizable = (_flags & VR_WINDOW_RESIZABLE) != 0;
        desc.movable = (_flags & VR_WINDOW_MOVABLE) != 0;
        desc.closable = (_flags & VR_WINDOW_CLOSABLE) != 0;
        desc.minimizable = (_flags & VR_WINDOW_MINIMIZABLE) != 0;
        desc.maximizable = (_flags & VR_WINDOW_MAXIMIZABLE) != 0;
        desc.canFullscreen = (_flags & VR_WINDOW_CANFULLSCREEN) != 0;
        
        const State& xwinState = getState();

        hinstance = xwinState.hInstance;
        HINSTANCE hPrevInstance = xwinState.hPrevInstance;
        LPSTR lpCmdLine = xwinState.lpCmdLine;
        int nCmdShow = xwinState.nCmdShow;

        mDesc = desc;

        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = Window::WindowProcStatic;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = WS_EX_NOPARENTNOTIFY;
        wndClass.hInstance = hinstance;
        wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = hBrush;
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = mDesc.name.c_str();
        wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

        RegisterClassEx(&wndClass);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        if (mDesc.fullscreen)
        {
            DEVMODE dmScreenSettings;
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth = screenWidth;
            dmScreenSettings.dmPelsHeight = screenHeight;
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields =
                DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if ((desc.width != screenWidth) && (desc.height != screenHeight))
            {
                if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) !=
                    DISP_CHANGE_SUCCESSFUL)
                {
                    // Stay in Windowed mode
                }
            }
        }

        DWORD dwExStyle = 0;
        DWORD dwStyle = 0;

        if (mDesc.fullscreen)
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            if (mDesc.frame)
            {
                dwStyle = Style::windowed;
            }
            else
            {
                dwStyle = Style::basicBorderless;
            }
        }

        // Store the current thread's DPI-awareness context
        DPI_AWARENESS_CONTEXT previousDpiContext = SetThreadDpiAwarenessContext(
            DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        RECT windowRect;
        windowRect.left = mDesc.x;
        windowRect.top = mDesc.y;
        windowRect.right = mDesc.fullscreen ? (uint64_t)screenWidth : (uint64_t)desc.width;
        windowRect.bottom =
            mDesc.fullscreen ? (uint64_t)screenHeight : (uint64_t)desc.height;

        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        _windowBeingCreated = this;
        hwnd = CreateWindowEx(0, mDesc.name.c_str(), mDesc.title.c_str(), dwStyle,
            0, 0, windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top, NULL, NULL,
            hinstance, NULL);

        BOOL isNCRenderingEnabled{ TRUE };
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_ENABLED,
            &isNCRenderingEnabled, sizeof(isNCRenderingEnabled));

        if (!mDesc.fullscreen)
        {
            // Adjust size to match DPI
            int iDpi = GetDpiForWindow(hwnd);
            if (iDpi != USER_DEFAULT_SCREEN_DPI)
            {
                windowRect.bottom =
                    MulDiv(windowRect.bottom, iDpi, USER_DEFAULT_SCREEN_DPI);
                windowRect.right =
                    MulDiv(windowRect.right, iDpi, USER_DEFAULT_SCREEN_DPI);
            }
            uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
            uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;

            // Center on screen
            SetWindowPos(hwnd, 0, x, y, windowRect.right, windowRect.bottom, 0);
        }

        if (mDesc.visible)
        {
            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd);
            SetFocus(hwnd);
        }

        static const DWM_BLURBEHIND blurBehind{ {0}, {TRUE}, {NULL}, {TRUE} };
        DwmEnableBlurBehindWindow(hwnd, &blurBehind);
        static const MARGINS shadow_state[2]{ {0, 0, 0, 0}, {1, 1, 1, 1} };
        DwmExtendFrameIntoClientArea(hwnd, &shadow_state[0]);

        RegisterWindowMessage("TaskbarButtonCreated");
        HRESULT hrf =
            CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                IID_ITaskbarList3, (LPVOID*)&mTaskbarList);
        setProgress(0.0f);
    }

    Window::~Window()
    {
        if (hwnd != NULL)
        {
            close();
        }
    }

    bool Window::isClosed()
    {
        if (mEventQueue != NULL)
        {
            mEventQueue->update();
        }

        return hwnd == NULL;
    }

    void Window::minimize() { ShowWindow(hwnd, SW_MINIMIZE); }

    void Window::maximize()
    {
        if (!IsZoomed(hwnd))
        {
            ShowWindow(hwnd, SW_MAXIMIZE);
        }
        else
        {
            ShowWindow(hwnd, SW_RESTORE);
        }
    }

    void Window::fullscreen()
    {
        // @todo
    }

    void Window::close()
    {
        if (hwnd != NULL)
        {
            DestroyWindow(hwnd);
            hwnd = NULL;
        }
    }

    void Window::trackEventsAsync(
        const std::function<void(const Event e)>& fun)
    {
        mCallback = fun;
    }

    void Window::setProgress(float progress)
    {
        uint32_t max = 10000;
        uint32_t cur = (uint32_t)(progress * (float)max);
        mTaskbarList->SetProgressValue(hwnd, cur, max);
    }

    void Window::showMouse(bool show) { ShowCursor(show ? TRUE : FALSE); }

    float Window::getDpiScale() const
    {
        int currentDpi = GetDpiForWindow(hwnd);
        int defaultDpi = USER_DEFAULT_SCREEN_DPI;

        return static_cast<float>(currentDpi) / static_cast<float>(defaultDpi);
    }

    std::string Window::getTitle() const
    {
        char str[1024];
        memset(str, 0, sizeof(char) * 1024);
        GetWindowTextA(hwnd, str, 1024);
        std::string outStr = std::string(str);
        return outStr;
    }

    void Window::setTitle(std::string title)
    {
        mDesc.title = title;
        SetWindowText(hwnd, mDesc.title.c_str());
    }

    void Window::setPosition(uint32_t x, uint32_t y)
    {
        SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        mDesc.x = x;
        mDesc.y = y;
    }

    void Window::setSize(uint32_t width, uint32_t height)
    {
        RECT rect, frame, border;
        GetWindowRect(hwnd, &rect);
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

        border.left = frame.left - rect.left;
        border.top = frame.top - rect.top;
        border.right = rect.right - frame.right;
        border.bottom = rect.bottom - frame.bottom;

        int titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) +
            GetSystemMetrics(SM_CXPADDEDBORDER));

        SetWindowPos(hwnd, NULL, -1, -1, width + border.right + border.left, height + border.top + border.bottom + titlebarHeight,
                     SWP_NOMOVE | SWP_NOREDRAW);
    }

    // clang-format off
    uint32_t Window::getBackgroundColor()
    {
        return mBackgroundColor;
    }

    void Window::setBackgroundColor(uint32_t color)
    {
        mBackgroundColor = color;
    }

    // clang-format on

    UVec2 Window::getPosition() const
    {
        RECT lpRect;
        GetWindowRect(hwnd, &lpRect);

        UVec2 vec;
        vec.x = (uint32_t)lpRect.left;
        vec.y = (uint32_t)lpRect.top;
        return vec;
    }

    UVec2 Window::getWindowSize() const
    {
        RECT lpRect;
        DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lpRect, sizeof(lpRect));
       // int titlebarHeight = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));
        return UVec2(static_cast<uint32_t>(lpRect.right - lpRect.left), static_cast<uint32_t>(lpRect.bottom - lpRect.top ));
    }

    UVec2 Window::getCurrentDisplaySize() const
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        UVec2 r = UVec2(static_cast<uint32_t>(screenWidth),
                        static_cast<uint32_t>(screenHeight));
        return r;
    }

    UVec2 Window::getCurrentDisplayPosition() const
    {
        WINDOWPLACEMENT lpwndpl = {0};
        GetWindowPlacement(hwnd, &lpwndpl);
        UVec2 r = UVec2(
            static_cast<uint32_t>(lpwndpl.ptMinPosition.x), 
            static_cast<uint32_t>(lpwndpl.ptMinPosition.y));
        return r;
    }

    void Window::setMousePosition(uint32_t x, uint32_t y) { SetCursorPos(x, y); }

    HINSTANCE Window::getHinstance() { return hinstance; }

    HWND Window::getNativeHandle() const { return hwnd; }

    void Window::executeEventCallback(const Event e)
    {
        if (mCallback) mCallback(e);
    }

    LRESULT CALLBACK Window::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam,
                                              LPARAM lparam)
    {
        Window* _this;
        if (_windowBeingCreated != NULL)
        {
            _hwndMap.emplace(hwnd, _windowBeingCreated);
            _windowBeingCreated->hwnd = hwnd;
            _this = _windowBeingCreated;
            _windowBeingCreated = NULL;
        }
        else
        {
            auto existing = _hwndMap.find(hwnd);
            _this = existing->second;
        }

        return _this->WindowProc(msg, wparam, lparam);
    }

    LRESULT Window::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        MSG message;
        message.hwnd = hwnd;
        message.lParam = lparam;
        message.wParam = wparam;
        message.message = msg;
        message.time = 0;

        if (mEventQueue != NULL)
        {
            LRESULT result = mEventQueue->pushEvent(message, this);
            if (result > 0) return result;
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    std::shared_ptr<Window> createWindow(const char* _name, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags)
    {
        return std::make_shared<Window>(_name, _x, _y, _width, _height, _flags);
    }
}

#endif // VR_WIN32