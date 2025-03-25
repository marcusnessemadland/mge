/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <bgfx/bgfx.h>

#include <memory>
#include <string>

namespace vr
{
    class Texture
    {
        friend class Renderer;
        friend class GBuffer;
        friend class Scene;

    public:
        Texture();
        Texture(const char* _filePath);
        ~Texture();

        /// @todo Not yet implemented
        friend std::shared_ptr<Texture> createTexture();

        /// 
        friend std::shared_ptr<Texture> loadTexture(const char* _filepath);

    private:
        std::string m_filepath;
        bgfx::TextureHandle m_th;
    };
}