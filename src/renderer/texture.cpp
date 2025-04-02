/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/texture.h"

#include "bgfx_utils.h"

namespace vr
{
    Texture::Texture(const char* _filePath)
        : m_filepath(_filePath)
    {
        m_th = bgfx::loadTexture(_filePath);
    }

    Texture::Texture()
        : m_th(BGFX_INVALID_HANDLE)
    {
        // @todo
    }

    Texture::~Texture()
    {
        if (isValid(m_th))
        {
            bgfx::destroy(m_th);
        }
    }

    std::shared_ptr<Texture> createTexture()
    {
        return std::make_shared<Texture>();
    }

    std::shared_ptr<Texture> loadTexture(const char* _filepath)
    {
        if (_filepath)
        {
            return std::make_shared<Texture>(_filepath);
        }
        else
        {
            return nullptr;
        }
    }

} // namespace vr