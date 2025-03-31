/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "tonemapping.h"
#include "gbuffer.h"
#include "common_resources.h"
#include "vertexpos.h"

#include "engine/renderer.h"

#include "shaders/tonemap.h"

#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <optick.h>

namespace vr
{
	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_tonemap),
		BGFX_EMBEDDED_SHADER(fs_tonemap),

		BGFX_EMBEDDED_SHADER_END()
	};

	void ToneMapping::createScreenBuffer()
	{
		constexpr float b = -1.0f;
		constexpr float t =  3.0f; 
		constexpr float l = -1.0f;
		constexpr float r =  3.0f;

		const VertexPos vertices[3] = {
			{Vec3(l, b, 0.0f)}, 
			{Vec3(r, b, 0.0f)}, 
			{Vec3(l, t, 0.0f)}};

		m_vbh = bgfx::createVertexBuffer(bgfx::copy(&vertices, sizeof(vertices)), VertexPos::ms_layout);
	}

	void ToneMapping::destroyScreenBuffer()
	{
		if (isValid(m_vbh))
		{
			bgfx::destroy(m_vbh);
		}
	}

	ToneMapping::ToneMapping(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer)
		: m_view(_view)
		, m_common(_common)
		, m_gbuffer(_gbuffer)
	{
		bgfx::setViewName(_view, "Tone Mapping");

		const bgfx::RendererType::Enum type = bgfx::getRendererType();

		m_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_tonemap"), 
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_tonemap"), 
			true
		);
		m_sampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

		// Don't create screen vertex buffer until first render call.
		m_vbh.idx = bgfx::kInvalidHandle;
	}

	ToneMapping::~ToneMapping()
	{
		destroyScreenBuffer();

		bgfx::destroy(m_program);
		bgfx::destroy(m_sampler);
	}

	void ToneMapping::render()
	{
		OPTICK_EVENT();

		if (m_common->firstFrame)
		{
			destroyScreenBuffer();
			createScreenBuffer();
		}

		// Set view 
		bgfx::setViewClear(m_view, BGFX_CLEAR_NONE);
		bgfx::setViewRect(m_view, 0, 0, m_common->width, m_common->height);
		bgfx::setViewFrameBuffer(m_view, BGFX_INVALID_HANDLE);

		// Submit
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		bgfx::setTexture(0, m_sampler, bgfx::getTexture(m_gbuffer->m_framebuffer, 0));
		bgfx::setVertexBuffer(0, m_vbh);
		
		bgfx::submit(m_view, m_program);
	}
}