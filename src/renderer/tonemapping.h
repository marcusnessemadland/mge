/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <bgfx/bgfx.h>

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
        ToneMapping(bgfx::ViewId _view, const CommonResources* _common, const GBuffer* _gbuffer);
        ~ToneMapping();

        void render();

    private:
        bgfx::ViewId m_view;
        const CommonResources* m_common;
        const GBuffer* m_gbuffer;

        bgfx::ProgramHandle m_program;
        bgfx::UniformHandle m_sampler;
        bgfx::VertexBufferHandle m_vbh;
    };

} // namespace vr