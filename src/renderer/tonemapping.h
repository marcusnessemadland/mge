/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include <bgfx/bgfx.h>

#include <memory>

namespace vr
{
    class Renderer;

    struct CommonResources;
    class GBuffer;

    class ToneMapping
    {
        void createScreenBuffer();
        void destroyScreenBuffer();

    public:
        ToneMapping(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer);
        ~ToneMapping();

        void render();

    private:
        bgfx::ViewId m_view;
        std::shared_ptr<CommonResources> m_common;
        std::shared_ptr<GBuffer> m_gbuffer;

        bgfx::ProgramHandle m_program;
        bgfx::UniformHandle m_sampler;
        bgfx::VertexBufferHandle m_vbh;
    };

} // namespace vr