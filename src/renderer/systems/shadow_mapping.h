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
    class World;
    class Model;

    struct CommonResources;

    class ShadowMapping
    {
        void createFramebuffer();
        void destroyFramebuffer();

        void submit(std::shared_ptr<Model> _model);

    public:
        ShadowMapping(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common);
        ~ShadowMapping();

        void render(std::shared_ptr<World> _world);

    public:
        SampleData m_sd;

    private:
        bgfx::ViewId m_view;
        std::shared_ptr<CommonResources> m_common;

        bgfx::ProgramHandle m_program;
        bgfx::FrameBufferHandle m_framebuffer;
    };

} // namespace mge