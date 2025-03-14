/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#ifdef VR_WIN32

#pragma once

#include "engine/event.h"

#include <windows.h>
#include <queue>
#include <memory>

namespace vr
{
    struct ProcessingMode
    {
        enum Enum
        {
            Poll,
            Dispatch,
            ProcessingModeMax
        };
    };

    class Window;

    class EventQueue
    {
        friend class Window;

        void update();
        void setProcessingMode(ProcessingMode::Enum mode);
        LRESULT pushEvent(MSG msg, Window* window);

    public:
        EventQueue(Window* _window);

        /// 
        friend std::shared_ptr<EventQueue> createEventQueue(std::shared_ptr<Window> _window);

        /// 
        const Event& front();

        /// 
        void pop();

        /// 
        bool empty();

        /// 
        size_t size();

    private:
        ProcessingMode::Enum processingMode;
        bool initialized;

        uint32_t prevMouseX;
        uint32_t prevMouseY;

        std::queue<Event> mQueue;

        /**
         * Virtual Key Codes in Win32 are an uint8_t:
         * https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
         */
        typedef Key::Enum Win32VKeyToDigitalInputMap[1 << (8 * sizeof(uint8_t))];

        Win32VKeyToDigitalInputMap mVKeyToDigitalInputMap =
        {
            /*NONE 0x00*/ Key::Count,
            /*VK_LBUTTON 0x01*/ Key::Count,
            /*VK_RBUTTON 0x02*/ Key::Count,
            /*VK_CANCEL 0x03*/ Key::Count
        };
    };

} // namespace vr

#endif // VR_WIN32