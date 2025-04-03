/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include <bgfx/bgfx.h>

#include <memory>
#include <string>

namespace mge
{
    /// Texture.
    ///
    class Texture
    {
        friend class Scene;
        friend class GBuffer;

    public:
        Texture();
        Texture(const char* _filePath);
        ~Texture();

        /// Create a texture.
        /// 
        /// @remark Not supported. @todo Implement...
        /// 
        /// @returns Shared Texture.
        /// 
        friend std::shared_ptr<Texture> createTexture();

        /// Load a texture.
        /// 
        /// @param[in] _filepath Path of texture file.
        /// 
        /// @remark If texture is already loaded, will just return reference to existing texture.
        /// 
        /// @returns Shared Texture.
        /// 
        friend std::shared_ptr<Texture> loadTexture(const char* _filepath);

    private:
        std::string m_filepath;
        bgfx::TextureHandle m_th;
    };

} // namespace mge