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
    class World;

    struct CommonResources;
    class GBuffer;

    class Skybox
    {
        friend class Imgui;

        void setScreenQuad();

    public:
        Skybox(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer);
        ~Skybox();

        void render(std::shared_ptr<World> _world);

    public:
        SampleData m_sd;

    private:
        bgfx::ViewId m_view;
        std::shared_ptr<CommonResources> m_common;
        std::shared_ptr<GBuffer> m_gbuffer;

        bgfx::ProgramHandle m_program;
        bgfx::UniformHandle u_cameraMtx;
        bgfx::UniformHandle s_skyboxCubemap;
        bgfx::UniformHandle s_gbufferDepth;
    };

} // namespace mge