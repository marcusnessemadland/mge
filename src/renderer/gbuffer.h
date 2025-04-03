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
    struct GBufferAttachment 
    {
        // @todo Consider computing Fresnel reflectance in shader instead of storing it
        enum Enum
        {
            DiffuseRoughness,  // .rgb = Diffuse color, .a = Roughness squared (remapped roughness)
            EncodedNormal,     // .rg  = Encoded normal (compressed normal vector)
            FresnelMetallic,   // .rgb = Fresnel reflectance at normal incidence, .a = metallic factor
            EmissiveOcclusion, // .rgb = Emissive radiance, .a = Ambient occlusion multiplier
            Depth,             // .r   = Depth value

            Count
        };
    };

    class World;
    class Model;
    class Scene;
    class Material;
    class Texture;

	struct CommonResources;

	class GBuffer
	{
        friend class ToneMapping;

        void createFramebuffer();
        void destroyFramebuffer();

        void setUniforms();
        void setMaterial(std::shared_ptr<Material> _material);
        bool setTextureOrDefault(uint8_t stage, bgfx::UniformHandle uniform, std::shared_ptr<Texture> texture);
        void submit(std::shared_ptr<Model> _model);

	public:
		GBuffer(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common);
		~GBuffer();

		void render(std::shared_ptr<World> _world);

    public:
        SampleData m_sd;

	private:
		bgfx::ViewId m_view;
        std::shared_ptr<CommonResources> m_common;

        bgfx::FrameBufferHandle m_framebuffer;
		bgfx::ProgramHandle m_program;
        bgfx::TextureHandle m_defaultTexture;
        bgfx::UniformHandle m_normalMatrixUniform;
        bgfx::UniformHandle m_baseColorFactorUniform;
        bgfx::UniformHandle m_metRoughNorOccFactorUniform;
        bgfx::UniformHandle m_emissiveFactorUniform;
        bgfx::UniformHandle m_hasTexturesUniform;
        bgfx::UniformHandle m_multipleScatteringUniform;
        bgfx::UniformHandle m_baseColorSampler;
        bgfx::UniformHandle m_metallicSampler;
        bgfx::UniformHandle m_roughnessSampler;
        bgfx::UniformHandle m_normalSampler;
        bgfx::UniformHandle m_occlusionSampler;
        bgfx::UniformHandle m_emissiveSampler;
	};

} // namespace mge