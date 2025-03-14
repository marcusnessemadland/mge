/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/event.h"

#include <unordered_map>

namespace vr
{
    Event::Event(EventType::Enum _type, Window* _window) 
        : type(_type), window(_window) 
    {
    }

    Event::Event(FocusData _d, Window* _window)
        : type(EventType::Focus), window(_window)
    {
        data.focus = _d;
    }

    Event::Event(ResizeData _d, Window* _window)
        : type(EventType::Resize), window(_window)
    {
        data.resize = _d;
    }

    Event::Event(KeyboardData _d, Window* _window)
        : type(EventType::Keyboard), window(_window)
    {
        data.keyboard = _d;
    }

    Event::Event(MouseRawData _d, Window* _window)
        : type(EventType::MouseRaw), window(_window)
    {
        data.mouseRaw = _d;
    }

    Event::Event(MouseMoveData _d, Window* _window)
        : type(EventType::MouseMove), window(_window)
    {
        data.mouseMove = _d;
    }

    Event::Event(MouseData _d, Window* _window)
        : type(EventType::Mouse), window(_window)
    {
        data.mouseInput = _d;
    }

    Event::Event(MouseWheelData _d, Window* _window)
        : type(EventType::MouseWheel), window(_window)
    {
        data.mouseWheel = _d;
    }

    Event::Event(TouchData _d, Window* _window)
        : type(EventType::Touch), window(_window)
    {
        data.touch = _d;
    }

    Event::Event(GamepadData _d, Window* _window)
        : type(EventType::Gamepad), window(_window)
    {
        data.gamepad = _d;
    }

    Event::Event(DpiData _d, Window* _window) : type(EventType::DPI), window(_window)
    {
        data.dpi = _d;
    }

    Event::~Event() 
    {
    }

    ResizeData::ResizeData(uint32_t _width, uint32_t _height, bool _resizing)
        : width(_width), height(_height), resizing(_resizing)
    {
    }

    /// A map of the Keys enum to chars for matching keyboard event data.
    /// Convenient for converting Key(s) to strings for serialization
    /// Though for certain platforms that natively support this
    /// (Mac OS, iOS, WebAssembly) we should opt to use those functions.
    /// 
    static KeyToCharMap s_keyToCharMap = 
    {
        "\x1B", "1", "2",  "3",  "4", "5", "6", "7", "8", "9", "0", "-", "=", "\b",
        "\t", "Q", "W",  "E",  "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\r",
        "",   "A", "S",  "D",  "F", "G", "H", "J", "K", "L", ";", ":", "'", "\"",
        "`",  "",  "\\", "Z",  "X", "C", "V", "B", "N", "M", ",", ".", "/", "",
        "*",  "",  " ",  "",   "",  "",  "",  "",  "",  "",  "",  "",  "",  "",
        "",   "",  "7",  "8",  "9", "-", "4", "5", "6", "+", "1", "2", "3", "0",
        ".",  "",  "",   "\r", "",  "/", "",  "",  "",  "",  "",  "",  "",  "",
        "",   "",  "",   "",   "",  "",  "",  ""
    };

    static std::unordered_map<const char*, Key::Enum> s_charToKeyMap = 
    {
        {"\x1B", Key::Escape},
        {"1", Key::Num1},
        {"2", Key::Num2},
        {"3", Key::Num3},
        {"4", Key::Num4},
        {"5", Key::Num5},
        {"6", Key::Num6},
        {"7", Key::Num7},
        {"8", Key::Num8},
        {"9", Key::Num9},
        {"0", Key::Num0},
        {"-", Key::Subtract},
        {"=", Key::Equals},
        {"\b", Key::Back},
        {"\t", Key::Tab},
        {"Q", Key::Q},
        {"W", Key::W},
        {"E", Key::E},
        {"R", Key::R},
        {"T", Key::T},
        {"Y", Key::Y},
        {"U", Key::U},
        {"I", Key::I},
        {"O", Key::O},
        {"P", Key::P},
        {"[", Key::LBracket},
        {"]", Key::RBracket},
        {"\r", Key::Enter},
        {"A", Key::A},
        {"S", Key::S},
        {"D", Key::D},
        {"F", Key::F},
        {"G", Key::G},
        {"H", Key::H},
        {"J", Key::J},
        {"K", Key::K},
        {"L", Key::L},
        {";", Key::Semicolon},
        {":", Key::Colon},
        {"'", Key::Apostrophe},
        {"\"", Key::Semicolon},
        {"`", Key::Grave},
        {"Left Shift", Key::LShift},
        {"\\", Key::Backslash},
        {"Z", Key::Z},
        {"X", Key::X},
        {"C", Key::C},
        {"V", Key::V},
        {"B", Key::B},
        {"N", Key::N},
        {"M", Key::M},
        {".", Key::Period},
        {"/", Key::Slash},
        {"Right Shift", Key::RShift},
        {"*", Key::Multiply},
        {"Left Alt", Key::LAlt},
        {" ", Key::Space},
        {"Captial", Key::Capital},
        {"F1", Key::F1},
        {"F2", Key::F2},
        {"F3", Key::F3},
        {"F4", Key::F4},
        {"F5", Key::F5},
        {"F6", Key::F6},
        {"F7", Key::F7},
        {"F8", Key::F8},
        {"F9", Key::F9},
        {"F10", Key::F10},
        {"Numlock", Key::Numlock},
    };

    const char* convertKeyToString(Key::Enum _key)
    {
        return s_keyToCharMap[static_cast<size_t>(_key)];
    }

    Key::Enum convertStringToKey(const char* _str)
    {
        Key::Enum k = Key::Count;
        auto itr = s_charToKeyMap.find(_str);
        if (itr != s_charToKeyMap.end())
        {
            k = (*itr).second;
        }
        return k;
    }

    FocusData::FocusData(bool _focused) 
        : focused(_focused)
    {
    }

    ModifierState::ModifierState(bool _ctrl, bool _alt, bool _shift, bool _meta)
        : ctrl(_ctrl), alt(_alt), shift(_shift), meta(_meta)
    {
    }

    MouseMoveData::MouseMoveData(uint32_t _x, uint32_t _y, uint32_t _screenx, uint32_t _screeny, int _deltax, int _deltay)
        : x(_x), y(_y), screenx(_screenx), screeny(_screeny), deltax(_deltax), deltay(_deltay)
    {
    }

    MouseData::MouseData(Mouse::Enum _button, ButtonState::Enum _state, ModifierState _modifiers)
        : button(_button), state(_state), modifiers(_modifiers)
    {
    }

    KeyboardData::KeyboardData(Key::Enum _key, ButtonState::Enum _state, ModifierState _modifiers)
        : key(_key), state(_state), modifiers(_modifiers)
    {
    }

    MouseWheelData::MouseWheelData(double _delta, ModifierState _modifiers)
        : delta(_delta), modifiers(_modifiers)
    {
    }

    MouseRawData::MouseRawData(int _deltax, int _deltay)
        : deltax(_deltax), deltay(_deltay)
    {
    }

    DpiData::DpiData(float _scale)
        : scale(_scale)
    {
    }

} // namespace vr
