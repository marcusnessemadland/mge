/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace vr
{
    class Window;

    /// Event type.
    ///
    struct EventType
    {
        enum Enum
        {
            None = 0,

            Close,
            Create,
            Focus,
            Paint,
            Resize,
            DPI,
            Keyboard,
            MouseMove,
            MouseRaw,
            MouseWheel,
            Mouse,
            Touch,
            Gamepad,
            DropFile,
            HoverFile,
            EventTypeMax,

            Count
        };
    };

    /// Button press state.
    /// 
    struct ButtonState
    {
        enum Enum
        {
            Pressed,
            Released,
            ButtonStateMax,

            Count
        };
    };

    /// Key type.
    ///
    struct Key 
    {
        enum Enum
        {
            Escape = 0,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            Num0,
            Minus,
            Equals,
            Back,
            Tab,
            Q,
            W,
            E,
            R,
            T,
            Y,
            U,
            I,
            O,
            P,
            LBracket,
            RBracket,
            Enter,
            LControl,
            A,
            S,
            D,
            F,
            G,
            H,
            J,
            K,
            L,
            Semicolon,
            Colon,
            Apostrophe,
            Quotation,
            Grave,
            LShift,
            Backslash,
            Z,
            X,
            C,
            V,
            B,
            N,
            M,
            Comma,
            Period,
            Slash,
            RShift,
            Multiply,
            LAlt,
            Space,
            Capital,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            Numlock,
            Scroll,
            Numpad7,
            Numpad8,
            Numpad9,
            Subtract,
            Numpad4,
            Numpad5,
            Numpad6,
            Add,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad0,
            Decimal,
            F11,
            F12,
            Numpadenter,
            RControl,
            Divide,
            sysrq,
            RAlt,
            Pause,
            Home,
            Up,
            PgUp,
            Left,
            Right,
            End,
            Down,
            PgDn,
            Insert,
            Del,
            LWin,
            RWin,
            Apps,

            Count
        };
    };

    /// Mouse type.
    ///
    struct Mouse
    {
        enum Enum
        {
            Left,
            Right,
            Middle,
            Button4,
            Button5,

            Count
        };
    };

    /// Gamepad Button pressed enum
    /// 
    struct GamepadButton
    {
        enum Enum
        {
            DPadUp,
            DPadDown,
            DPadLeft,
            DPadRight,
            StartButton,
            BackButton,
            LThumbClick,
            RThumbClick,
            LShoulder,
            RShoulder,
            AButton,
            BButton,
            XButton,
            YButton,

            Count
        };
    };

    /// Gamepad analog stick enum
    /// 
    struct AnalogInput
    {
        enum Enum
        {
            AnalogLeftTrigger,
            AnalogRightTrigger,
            AnalogLeftStickX,
            AnalogLeftStickY,
            AnalogRightStickX,
            AnalogRightStickY,

            AnalogMouseX,
            AnalogMouseY,
            AnalogMouseScroll,

            Count
        };
    };

    /// Focus data passed with Focus events.
    ///
    struct FocusData
    {
        bool focused; //!< True if focused, false if not

        FocusData(bool _focused);

        static const EventType::Enum type = EventType::Focus;
    };

    /// Resize data passed with Resize events.
    ///
    struct ResizeData
    {
        uint32_t width;  //!< New width of window viewport
        uint32_t height; //!< New height of window viewport
	    bool resizing;   //!< In the process of resizing

        ResizeData(uint32_t _width, uint32_t _height, bool _resizing);

        static const EventType::Enum type = EventType::Resize;
    };

    /// DPI data passed with DPI events.
    ///
    struct DpiData
    {
        float scale; //!< DPI Scale (currently not in use @todo)

        DpiData(float _scale);

        static const EventType::Enum type = EventType::DPI;
    };

    /// The state of modifier keys such as ctrl, alt, shift, and the windows/command
    /// buttons. Pressed is true, released is false;
    ///
    struct ModifierState
    {
        bool ctrl;  //!< Ctrl key
        bool alt;   //!< Alt key
        bool shift; //!< Shift key
        bool meta;  //!< Meta buttons such as the Windows button or Mac's Command button

        ModifierState(bool _ctrl = false, bool _alt = false, bool _shift = false,
                      bool _meta = false);
    };

    typedef const char charElm[256];
    typedef const charElm KeyToCharMap[static_cast<size_t>(Key::Count)];
    typedef Key::Enum CharToKeyMap[static_cast<size_t>(Key::Count)];

    /// Converts a key to a string for serialization.
    /// 
    const char* convertKeyToString(Key::Enum _key);

    /// Converts a string name to a Key for deserialization.
    /// 
    Key::Enum convertStringToKey(const char* _str);

    /// Data sent during keyboard events.
    ///
    struct KeyboardData
    {
        Key::Enum key;           //!< Key type
        ButtonState::Enum state; //!< Button press state
        ModifierState modifiers; //!< Modifier state

        static const EventType::Enum type = EventType::Keyboard;

        KeyboardData(Key::Enum _key, ButtonState::Enum _state, ModifierState _modifiers);
    };

    /// Raw mouse data.
    ///
    struct MouseRawData
    {
        int deltax; //!< Delta on x axis
        int deltay; //!< Delta on y axis

        static const EventType::Enum type = EventType::MouseRaw;

        MouseRawData(int _deltax, int _deltay);
    };

    /// The event data passed with mouse events click, mouse moving events
    /// 
    struct MouseMoveData
    {
        uint32_t x;       //!< Current x position relative to active window
        uint32_t y;       //!< Current y position relative to active window
        uint32_t screenx; //!< Current global x position
        uint32_t screeny; //!< Current in global y position
        int deltax;       //!< Change in x relative to previous event, used for FPS motion
        int deltay;       //!< Change in y relative to previous event, used for FPS motion

        static const EventType::Enum type = EventType::MouseMove;

        MouseMoveData(uint32_t _x, uint32_t _y, uint32_t _screenx, uint32_t _screeny,
                      int _deltax, int _deltay);
    };

    /// Data passed with mouse input events
    /// 
    struct MouseData
    {
        Mouse::Enum button;      //!< Mouse type
        ButtonState::Enum state; //!< Button press state
        ModifierState modifiers; //!< Modifier state

        static const EventType::Enum type = EventType::Mouse;

        MouseData(Mouse::Enum _button, ButtonState::Enum _state, ModifierState _modifiers);
    };

    /// Data passed with mouse wheel events
    /// 
    struct MouseWheelData
    {
        double delta;            //!< Delta on wheel axis
        ModifierState modifiers; //!< Modifier state

        static const EventType::Enum type = EventType::MouseWheel;

        MouseWheelData(double _delta, ModifierState _modifiers);
    };

    /// Touch point data
    /// 
    struct TouchPoint
    {
        uint64_t id;      //!< A unique id for each touch point
        uint32_t screenX; //!< touch coordinate relative to whole screen origin in pixels
        uint32_t screenY; //!< touch coordinate relative to whole screen origin in pixels
        uint32_t clientX; //!< touch coordinate relative to window in pixels
        uint32_t clientY; //!< touch coordinate relative to window in pixels
        bool isChanged;   //!< Did the touch point change
    };

    /// Data passed for touch events
    /// 
    struct TouchData
    {
        uint32_t numTouches;    //!< Number of touches
        TouchPoint touches[32]; //!< Touch point array, max 32

        static const EventType::Enum type = EventType::Touch;
    };

    typedef const char* AnalogToStringMap[static_cast<size_t>(AnalogInput::Count)];

    /// Data passed for gamepad events
    ///
    struct GamepadData
    {
        bool connected; //!< If the gamepad is connected or not
        size_t index; //!< Gamepad Index
        const char* id; //!< String id of the brand of the gamepad
        const char* mapping; //!< String id that lays out controller mapping (Southpaw, etc.)
        double axis[64]; //!< Analog Axis input data, such as joysticks, normalized range [-1, 1]
        uint32_t numAxes; //!< The number of analog axes
        double analogButton[64]; //!< Analog gamepad buttons like triggers, bound to [0, 1]
        bool digitalButton[64]; //!< Digital gamepad buttons
        uint32_t numButtons; //!< Number of digital buttons and analog buttons

        static const EventType::Enum type = EventType::Gamepad;
    };

    /// SDL does something similar:
    /// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlevent.html
    /// 
    union EventData 
    {
        EventData() {}
        ~EventData() {}

        FocusData focus;
        ResizeData resize;
        DpiData dpi;
        KeyboardData keyboard;
        MouseMoveData mouseMove;
        MouseData mouseInput;
        MouseWheelData mouseWheel;
        TouchData touch;
        GamepadData gamepad;
        MouseRawData mouseRaw;
    };

    struct Event
    {
        Event(EventType::Enum _type = EventType::None, Window* _window = NULL);
        Event(FocusData _data, Window* _window = NULL);
        Event(ResizeData _data, Window* _window = NULL);
        Event(KeyboardData _data, Window* _window = NULL);
        Event(MouseMoveData _data, Window* _window = NULL);
        Event(MouseRawData _data, Window* _window = NULL);
        Event(MouseData _data, Window* _window = NULL);
        Event(MouseWheelData _data, Window* _window = NULL);
        Event(TouchData _data, Window* _window = NULL);
        Event(GamepadData _data, Window* _window = NULL);
        Event(DpiData _data, Window* _window = NULL);
        ~Event();

        bool operator==(const Event& _other) const
        {
            return type == _other.type && window == _other.window;
        }

        EventType::Enum type; //!< The event's type
        Window* window;       //!< Pointer to a window
        EventData data;       //!< Inner data of the event
    };

} // namespace vr
