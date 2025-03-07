/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <stdint.h>

namespace vr 
{
    struct UVec2
    {
        UVec2(uint32_t x = 0, uint32_t y = 0) : x(x), y(y) {}

        uint32_t x;
        uint32_t y;

        template <typename T> UVec2 operator*(T b) const
        {
            UVec2 self = *this;
            self.x = static_cast<uint32_t>(static_cast<T>(self.x) * b);
            self.y = static_cast<uint32_t>(static_cast<T>(self.y) * b);
            return self;
        }
        template <typename T> UVec2 operator/(T b) const
        {
            UVec2 self = *this;
            self.x = static_cast<uint32_t>(static_cast<T>(self.x) / b);
            self.y = static_cast<uint32_t>(static_cast<T>(self.y) / b);
            return self;
        }
    };

} // namespace vr