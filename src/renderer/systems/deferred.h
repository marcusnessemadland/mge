/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/sampledata.h"

#include <bgfx/bgfx.h>

#include <memory>

namespace mge
{
    class Renderer;

    struct CommonResources;
    class GBuffer;

    class Deferred
    {
        void createFramebuffer();
        void destroyFramebuffer();

        void createDepthTexture();
        void destroyDepthTexture();

        void createScreenBuffer();
        void destroyScreenBuffer();

    public:
        Deferred(bgfx::ViewId _view0, bgfx::ViewId _view1, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer);
        ~Deferred();

        void render();

    public:
        SampleData m_sd;

    private:
        bgfx::ViewId m_view0;
        bgfx::ViewId m_view1;
        std::shared_ptr<CommonResources> m_common;
        std::shared_ptr<GBuffer> m_gbuffer;

        bgfx::ProgramHandle m_programAmbient;
        bgfx::ProgramHandle m_programDirectional;
        bgfx::UniformHandle s_texDiffuseA;
        bgfx::UniformHandle s_texNormal;
        bgfx::UniformHandle s_texF0Metallic;
        bgfx::UniformHandle s_texEmissiveOcclusion;
        bgfx::UniformHandle s_texDepth;
        bgfx::TextureHandle m_depth;
        bgfx::VertexBufferHandle m_vbh;
        bgfx::FrameBufferHandle m_framebuffer;
    };

} // namespace mge