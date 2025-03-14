/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <string>

namespace vr
{
    /// Initial window description
    /// 
    struct WindowDesc
    {
        uint32_t x = 0;              //!< Window X position based on Top Left
        uint32_t y = 0;              //!< Window Y position based on Top Left
        uint32_t width = 1920;       //!< Window Width
        uint32_t height = 1080;      //!< Window Height
        uint32_t minWidth = 0;       //!< Window minimum width
        uint32_t minHeight = 0;      //!< Window minimum height
        uint32_t maxWidth = 0xFFFF;  //!< Window maximum width
        uint32_t maxHeight = 0xFFFF; //!< Window maximum height
        
        bool centered = true;      //!< Should this window be centered on creation?
        bool resizable = true;     //!< Whether this window is resizable
        bool movable = true;       //!< Whether this window is movable
        bool closable = true;      //!< Whether this window is closable
        bool minimizable = true;   //!< Whether this window is minimizable
        bool maximizable = true;   //!< whether this window is maximizable
        bool canFullscreen = true; //!< Whether this window can be fullscreened
        
        uint32_t backgroundColor = 0xFFFFFFFF; //!< Hexidecimal background color (0xffffffff = white)
        bool transparent = false;              //!< makes this window transparent
        bool frame = true;                     //!< does this window feature a frame?
        bool hasShadow = true;                 //!< if this window has a shadow
        
        bool visible = true;     //!< Is this window visible?
        bool fullscreen = false; //!< Is this window full screen?
        bool modal = false;      //!< Is this window a modal?
        
        std::string title = "CrossWindow"; //!< Window Title
        std::string name = "XWinApp";      //!< Window name ID
        std::string iconPath = "";         //!< Icon Path
    };

}
