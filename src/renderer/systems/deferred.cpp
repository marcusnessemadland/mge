/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "deferred.h"
#include "gbuffer.h"

#include "../common_resources.h"
#include "../samplers.h"
#include "../vertexpos.h"
#include "../shaders/light_ambient.h"
#include "../shaders/light_directional.h"

#include "engine/renderer.h"
#include "engine/settings.h"

#include <bgfx/embedded_shader.h>
#include <bx/bx.h>

namespace mge
{
	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_light_ambient),
		BGFX_EMBEDDED_SHADER(fs_light_ambient),
		BGFX_EMBEDDED_SHADER(vs_light_directional),
		BGFX_EMBEDDED_SHADER(fs_light_directional),

		BGFX_EMBEDDED_SHADER_END()
	};

	void Deferred::createFramebuffer()
	{
		bgfx::TextureHandle textures[2];
		uint8_t attachments = 0;

		const uint64_t samplerFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
			BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

		bgfx::TextureFormat::Enum format =
			hdr ? bgfx::TextureFormat::RGBA16F : bgfx::TextureFormat::BGRA8; // BGRA is often faster (internal GPU format)
		textures[attachments++] =
			bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, format, BGFX_TEXTURE_RT | samplerFlags);

		if (depth)
		{
			bgfx::TextureFormat::Enum depthFormat = findDepthFormat(BGFX_TEXTURE_RT_WRITE_ONLY | samplerFlags);
			assert(depthFormat != bgfx::TextureFormat::Enum::Count);
			textures[attachments++] = bgfx::createTexture2D(
				bgfx::BackbufferRatio::Equal, false, 1, depthFormat, BGFX_TEXTURE_RT_WRITE_ONLY | samplerFlags);
		}

		bgfx::FrameBufferHandle fb = bgfx::createFrameBuffer(attachments, textures, true);
	}

	void Deferred::destroyFramebuffer()
	{
	}

	void Deferred::createDepthTexture()
	{
		const uint64_t flags = BGFX_SAMPLER_MIN_POINT |
			BGFX_SAMPLER_MAG_POINT |
			BGFX_SAMPLER_MIP_POINT |
			BGFX_SAMPLER_U_CLAMP |
			BGFX_SAMPLER_V_CLAMP;

		m_depth = bgfx::createTexture2D(
			bgfx::BackbufferRatio::Equal, 
			false, 
			1,
			bgfx::TextureFormat::D32F, 
			flags | BGFX_TEXTURE_BLIT_DST);
	}

	void Deferred::destroyDepthTexture()
	{
		if (isValid(m_depth))
		{
			bgfx::destroy(m_depth);
		}
	}

	void Deferred::createScreenBuffer()
	{
		constexpr float b = -1.0f;
		constexpr float t = 3.0f;
		constexpr float l = -1.0f;
		constexpr float r = 3.0f;

		const VertexPos vertices[3] = {
			{Vec3(l, b, 0.0f)},
			{Vec3(r, b, 0.0f)},
			{Vec3(l, t, 0.0f)} };

		m_vbh = bgfx::createVertexBuffer(bgfx::copy(&vertices, sizeof(vertices)), VertexPos::ms_layout);
	}

	void Deferred::destroyScreenBuffer()
	{
		if (isValid(m_vbh))
		{
			bgfx::destroy(m_vbh);
		}
	}

	Deferred::Deferred(bgfx::ViewId _view0, bgfx::ViewId _view1, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer)
		: m_view0(_view0)
		, m_view1(_view1)
		, m_common(_common)
		, m_gbuffer(_gbuffer)
	{
		bgfx::setViewName(_view0, "Deferred Shading (Ambient)");
		bgfx::setViewName(_view1, "Deferred Shading (Directional)");

		const bgfx::RendererType::Enum type = bgfx::getRendererType();

		m_programAmbient = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_light_ambient"),
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_light_ambient"),
			true
		);
		m_programDirectional = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_light_directional"),
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_light_directional"),
			true
		);

		s_texDiffuseA           = bgfx::createUniform("s_texDiffuseA", bgfx::UniformType::Sampler);
		s_texNormal				= bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
		s_texF0Metallic			= bgfx::createUniform("s_texF0Metallic", bgfx::UniformType::Sampler);
		s_texEmissiveOcclusion	= bgfx::createUniform("s_texEmissiveOcclusion", bgfx::UniformType::Sampler);
		s_texDepth				= bgfx::createUniform("s_texDepth", bgfx::UniformType::Sampler);

		// Don't create screen vertex buffer until first render call.
		m_vbh.idx = bgfx::kInvalidHandle;
	}

	Deferred::~Deferred()
	{
		destroyScreenBuffer();

		bgfx::destroy(m_programAmbient);
		bgfx::destroy(m_programDirectional);

		bgfx::destroy(s_texDiffuseA);
		bgfx::destroy(s_texNormal);
		bgfx::destroy(s_texF0Metallic);
		bgfx::destroy(s_texEmissiveOcclusion);
		bgfx::destroy(s_texDepth);
	}

	void Deferred::render()
	{
		// Begin timer
		m_sd.begin();

		if (m_common->firstFrame)
		{
			destroyDepthTexture();
			createDepthTexture();

			destroyScreenBuffer();
			createScreenBuffer();
		}

		// Set view 
		bgfx::setViewClear(m_view0, BGFX_CLEAR_COLOR);
		bgfx::setViewRect(m_view0, 0, 0, m_common->width, m_common->height);
		bgfx::setViewFrameBuffer(m_view0, m_framebuffer);
		bgfx::setViewTransform(m_view0, m_common->view, m_common->proj);

		bgfx::blit(m_view0, m_depth, 0, 0, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::Depth));

		bgfx::setTexture(Samplers::DeferredDiffuseA, s_texDiffuseA, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::DiffuseRoughness));
		bgfx::setTexture(Samplers::DeferredDiffuseA, s_texNormal, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::EncodedNormal));
		bgfx::setTexture(Samplers::DeferredDiffuseA, s_texF0Metallic, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::FresnelMetallic));
		bgfx::setTexture(Samplers::DeferredDiffuseA, s_texEmissiveOcclusion, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::EmissiveOcclusion));
		bgfx::setTexture(Samplers::DeferredDiffuseA, s_texDepth, m_depth);

		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_GREATER | BGFX_STATE_CULL_CW);
		bgfx::submit(m_view0, m_programAmbient, 0, ~BGFX_DISCARD_BINDINGS);

		// End timer
		m_sd.pushSample(m_sd.end());
	}
}