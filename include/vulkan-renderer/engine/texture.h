/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <bgfx/bgfx.h>

#include <memory>

namespace vr
{
    class Texture
    {
        friend class Renderer;
        friend class GBuffer;

    public:
        Texture();
        Texture(const char* _filePath);
        ~Texture();

        /// @todo Not yet implemented
        friend std::shared_ptr<Texture> createTexture();

        /// 
        friend std::shared_ptr<Texture> loadTexture(const char* _filepath);

    private:
        bgfx::TextureHandle m_th;
    };
}