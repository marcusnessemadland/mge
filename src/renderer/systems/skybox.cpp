/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "skybox.h"
#include "gbuffer.h"

#include "engine/world.h"
#include "engine/texture.h"

#include "../samplers.h"
#include "../vertexpostex.h"
#include "../common_resources.h"
#include "../shaders/skybox.h"

#include <bgfx/embedded_shader.h>
#include <bx/math.h>

namespace mge
{
	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_skybox),
		BGFX_EMBEDDED_SHADER(fs_skybox),

		BGFX_EMBEDDED_SHADER_END()
	};

	void Skybox::setScreenQuad()
	{
		if (3 == bgfx::getAvailTransientVertexBuffer(3, VertexPosTex::ms_layout))
		{
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 3, VertexPosTex::ms_layout);
			VertexPosTex* vertex = (VertexPosTex*)vb.data;

			const float zz = 0.0f;

			const float minx = -1.0f;
			const float maxx = 1.0f;
			const float miny = 0.0f;
			const float maxy = 1.0f * 2.0f;

			const float texelHalfW = 0.0f / m_common->width;
			const float texelHalfH = 0.0f / m_common->height;
			const float minu = -1.0f + texelHalfW;
			const float maxu = 1.0f + texelHalfW;

			float minv = texelHalfH;
			float maxv = 2.0f + texelHalfH;

			if (bgfx::getCaps()->originBottomLeft)
			{
				std::swap(minv, maxv);
				minv -= 1.0f;
				maxv -= 1.0f;
			}

			vertex[0].position.x = minx;
			vertex[0].position.y = miny;
			vertex[0].position.z = zz;
			vertex[0].texcoord.x = minu;
			vertex[0].texcoord.y = minv;

			vertex[1].position.x = maxx;
			vertex[1].position.y = miny;
			vertex[1].position.z = zz;
			vertex[1].texcoord.x = maxu;
			vertex[1].texcoord.y = minv;

			vertex[2].position.x = maxx;
			vertex[2].position.y = maxy;
			vertex[2].position.z = zz;
			vertex[2].texcoord.x = maxu;
			vertex[2].texcoord.y = maxv;

			bgfx::setVertexBuffer(0, &vb);
		}
	}

	Skybox::Skybox(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common, std::shared_ptr<GBuffer> _gbuffer)
		: m_view(_view)
		, m_common(_common)
		, m_gbuffer(_gbuffer)
	{
		bgfx::setViewName(_view, "Skybox");

		const bgfx::RendererType::Enum type = bgfx::getRendererType();

		m_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_skybox"),
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_skybox"),
			true
		);

		u_cameraMtx = bgfx::createUniform("u_cameraMtx", bgfx::UniformType::Mat4);
		s_skyboxCubemap = bgfx::createUniform("s_skyboxCubemap", bgfx::UniformType::Sampler);
		s_gbufferDepth = bgfx::createUniform("s_gbufferDepth", bgfx::UniformType::Sampler);
	}

	Skybox::~Skybox()
	{
		bgfx::destroy(m_program);

		bgfx::destroy(u_cameraMtx);
		bgfx::destroy(s_skyboxCubemap);
		bgfx::destroy(s_gbufferDepth);
	}

	void Skybox::render(std::shared_ptr<World> _world)
	{
		// Begin timer
		m_sd.begin();

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth, bx::Handedness::Left);

		bgfx::setViewFrameBuffer(m_view, BGFX_INVALID_HANDLE); // Render on top of previous
		bgfx::setViewRect(m_view, 0, 0, m_common->width, m_common->height);
		bgfx::setViewTransform(m_view, nullptr, proj);

		float cameraMtx[16];
		Vec3 forward = m_common->cameraDirection;  
		forward.y = forward.y * -1.0f;
		forward.z = forward.z * -1.0f;
		const Vec3 right = normalize(cross({ 0.0f, 1.0f, 0.0f }, forward));
		const Vec3 up = normalize(cross(forward, right));
		cameraMtx[0]  = right.x;
		cameraMtx[1]  = right.y;
		cameraMtx[2]  = right.z;
		cameraMtx[3]  = 0.0f;
		cameraMtx[4]  = up.x;
		cameraMtx[5]  = up.y;
		cameraMtx[6]  = up.z;
		cameraMtx[7]  = 0.0f;
		cameraMtx[8]  = forward.x;
		cameraMtx[9]  = forward.y;
		cameraMtx[10] = forward.z;
		cameraMtx[11] = 0.0f;
		cameraMtx[12] = 0.0f;
		cameraMtx[13] = 0.0f;
		cameraMtx[14] = 0.0f;
		cameraMtx[15] = 1.0f;
		bgfx::setUniform(u_cameraMtx, cameraMtx);

		bgfx::setTexture(Samplers::DeferredDepth, s_gbufferDepth, bgfx::getTexture(m_gbuffer->m_framebuffer, GBufferAttachment::Depth));
		bgfx::setTexture(Samplers::SkyboxCubemap, s_skyboxCubemap, _world->m_environment[Environment::Skybox]->m_th);
		bgfx::setState(0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_EQUAL);
		setScreenQuad();
		bgfx::submit(m_view, m_program);

		// End timer
		m_sd.pushSample(m_sd.end());
	}

} // namespace mge