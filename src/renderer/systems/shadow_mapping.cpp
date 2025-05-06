/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "shadow_mapping.h"

#include "engine/settings.h"
#include "engine/world.h"
#include "engine/mesh.h"
#include "engine/objects/model.h"
#include "engine/objects/scene.h"
#include "engine/components/mesh_component.h"

#include "../common_resources.h"
#include "../shaders/shadowmap.h"

#include "../bgfx_utils.h"
#include <bgfx/embedded_shader.h>
#include <bx/math.h>

namespace mge
{
	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_shadowmap),
		BGFX_EMBEDDED_SHADER(fs_shadowmap),

		BGFX_EMBEDDED_SHADER_END()
	};

	void ShadowMapping::createFramebuffer()
	{
		const Settings& settings = getSettings();

		m_framebuffer = bgfx::createFrameBuffer(
			settings.renderer.shadowMapRes,
			settings.renderer.shadowMapRes,
			bgfx::TextureFormat::D16,
			BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL);
	}

	void ShadowMapping::destroyFramebuffer()
	{
		if (isValid(m_framebuffer))
		{
			// Textures are destroyed with it
			bgfx::destroy(m_framebuffer);
		}
	}

	void ShadowMapping::submit(std::shared_ptr<Model> _model)
	{
		float mtx[16];
		bx::mtxSRT(mtx, _model->getPosition(), _model->getRotation(), _model->getScale());

		if (std::shared_ptr<MeshComponent> meshComp = _model->getComponent<MeshComponent>())
		{
			std::shared_ptr<Mesh> mesh = meshComp->m_mesh;

			for (auto& submesh : mesh->m_submeshes)
			{
				bgfx::setTransform(mtx);
				bgfx::setVertexBuffer(0, mesh->m_vbh);
				bgfx::setIndexBuffer(submesh->m_ibh);
				bgfx::submit(m_view, m_program);
			}
		}
	}

	ShadowMapping::ShadowMapping(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common)
		: m_view(_view)
		, m_common(_common)
	{
		bgfx::setViewName(_view, "Shadow Mapping");

		const bgfx::RendererType::Enum type = bgfx::getRendererType();

		m_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_shadowmap"), 
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_shadowmap"), 
			true
		);

		// Don't create framebuffer until first render call.
		m_framebuffer.idx = bgfx::kInvalidHandle;
	}

	ShadowMapping::~ShadowMapping()
	{
		destroyFramebuffer();

		bgfx::destroy(m_program);
	}

	void ShadowMapping::render(std::shared_ptr<World> _world)
	{
		// Begin timer
		m_sd.begin();

		//
		if (m_common->firstFrame)
		{
			destroyFramebuffer();
			createFramebuffer();
		}

		// Light View Proj
		const Vec3 lightDir = _world->m_directionalLight;
		float lightView[16];
		float lightProj[16];

		const bx::Vec3 at = { 0.0f,  0.0f,   0.0f };
		const bx::Vec3 eye = { lightDir.x, lightDir.y, lightDir.z };
		bx::mtxLookAt(lightView, eye, at);

		const bgfx::Caps* caps = bgfx::getCaps();
		const float area = 30.0f;
		bx::mtxOrtho(lightProj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, caps->homogeneousDepth, bx::Handedness::Right);

		// Set view 
		bgfx::setViewFrameBuffer(m_view, m_framebuffer);
		bgfx::setViewRect(m_view, 0, 0, m_common->width, m_common->height);
		bgfx::setViewClear(m_view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewTransform(m_view, lightView, lightProj);
		bgfx::setState(
			m_view,
			0
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA);

		// Submit
		for (auto& entity : _world->m_objects)
		{
			if (std::shared_ptr<Scene> scene = std::dynamic_pointer_cast<Scene>(entity))
			{
				for (auto& pair : scene->m_models)
				{
					submit(pair.second);
				}
			}

			if (std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(entity))
			{
				submit(model);
			}
		}

		// End timer
		m_sd.pushSample(m_sd.end());
	}
}