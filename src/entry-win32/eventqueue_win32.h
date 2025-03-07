/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#pragma once

#include <Windows.h>

#include "entry/event.h"

#include <queue>

namespace vr
{
    class Window;

    class EventQueue
    {
    public:
        EventQueue();

        void update();

        const Event& front();

        void pop();

        bool empty();

        size_t size();

        enum class ProcessingMode
        {
            Poll,
            Dispatch,
            ProcessingModeMax
        };
        void setProcessingMode(ProcessingMode mode);

        friend class Window;

    protected:
        LRESULT pushEvent(MSG msg, Window* window);

        ProcessingMode processingMode = ProcessingMode::Poll;
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