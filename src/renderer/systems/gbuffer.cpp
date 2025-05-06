/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "gbuffer.h"

#include "../common_resources.h"
#include "../samplers.h"
#include "../bgfx_utils.h"

#include "../shaders/geometry.h"

#include "engine/objects/model.h"
#include "engine/objects/scene.h"
#include "engine/components/mesh_component.h"
#include "engine/world.h"
#include "engine/mesh.h"
#include "engine/renderer.h"
#include "engine/material.h"
#include "engine/texture.h"
#include "engine/settings.h"

#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/math.h>

namespace mge
{
	static const bgfx::EmbeddedShader s_embeddedShaders[] =
	{
		BGFX_EMBEDDED_SHADER(vs_geometry),
		BGFX_EMBEDDED_SHADER(fs_geometry),

		BGFX_EMBEDDED_SHADER_END()
	};

	void GBuffer::createFramebuffer()
	{
		const uint32_t width = m_common->width;
		const uint32_t height = m_common->height;

		const uint64_t flags = BGFX_SAMPLER_MIN_POINT |
							   BGFX_SAMPLER_MAG_POINT |
							   BGFX_SAMPLER_MIP_POINT |
							   BGFX_SAMPLER_U_CLAMP |
							   BGFX_SAMPLER_V_CLAMP;

		// @todo D32F format might not be available at all platforms. 
		// Consider a func for 'getAvailableDepthFormat'
		bgfx::TextureHandle textures[GBufferAttachment::Count] =
		{
			bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | flags),
			bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RG16F, BGFX_TEXTURE_RT | flags),
			bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | flags),
			bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | flags),
			bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D32F,  BGFX_TEXTURE_RT | flags)
		};
		m_framebuffer = bgfx::createFrameBuffer(GBufferAttachment::Count, textures, true);
	}

	void GBuffer::destroyFramebuffer()
	{
		if (isValid(m_framebuffer))
		{
			// Textures are destroyed with it
			bgfx::destroy(m_framebuffer); 
		}
	}

	void GBuffer::setUniforms()
	{
		// Normal matrix
		// https://github.com/graphitemaster/normals_revisited#the-details-of-transforming-normals
		float identity[16];
		bx::mtxIdentity(identity);

		float normalMat[16];
		bx::mtxAdjugate(normalMat, identity);

		float normalMat3[9];
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				normalMat3[i * 3 + j] = normalMat[i * 4 + j];
			}
		}
		bgfx::setUniform(m_normalMatrixUniform, normalMat3);
	}

	void GBuffer::setMaterial(std::shared_ptr<Material> _material)
	{
		bgfx::setUniform(m_baseColorFactorUniform, &_material->baseColorFactor);

		float factorValues[4] = {
			_material->metallicFactor, 
			_material->roughnessFactor,
			_material->normalScale, 
			_material->occlusionStrength
		};
		bgfx::setUniform(m_metRoughNorOccFactorUniform, factorValues);

		float emissiveFactor[4] = {
			_material->emissiveFactor.x,
			_material->emissiveFactor.y,
			_material->emissiveFactor.z,
			0.0f
		};
		bgfx::setUniform(m_emissiveFactorUniform, &_material->emissiveFactor);

		float hasTexturesValues[4] = { 
			0.0f, 
			0.0f, 
			0.0f, 
			0.0f 
		};

		const uint32_t hasTexturesMask = 0
			| ((setTextureOrDefault(Samplers::BaseColor, m_baseColorSampler, _material->baseColorTexture) ? 1 : 0) << 0)
			| ((setTextureOrDefault(Samplers::Metal, m_metallicSampler, _material->metallicTexture) ? 1 : 0) << 1)
			| ((setTextureOrDefault(Samplers::Roughness, m_roughnessSampler, _material->roughnessTexture) ? 1 : 0) << 2)
			| ((setTextureOrDefault(Samplers::Normal, m_normalSampler, _material->normalTexture) ? 1 : 0) << 3)
			| ((setTextureOrDefault(Samplers::Occlusion, m_occlusionSampler, _material->occlusionTexture) ? 1 : 0) << 4)
			| ((setTextureOrDefault(Samplers::Emissive, m_emissiveSampler, _material->emissiveTexture) ? 1 : 0) << 5);
		hasTexturesValues[0] = (float)hasTexturesMask;

		bgfx::setUniform(m_hasTexturesUniform, hasTexturesValues);
	}

	bool GBuffer::setTextureOrDefault(uint8_t stage, bgfx::UniformHandle uniform, std::shared_ptr<Texture> texture)
	{
		bool valid = texture != nullptr && bgfx::isValid(texture->m_th);
		if (valid)
		{
			bgfx::setTexture(stage, uniform, texture->m_th);
		}
		else
		{
			bgfx::setTexture(stage, uniform, m_defaultTexture);
		}

		return valid;
	}

	void GBuffer::submit(std::shared_ptr<Model> _model)
	{
		float mtx[16];
		bx::mtxSRT(mtx, _model->getPosition(), _model->getRotation(), _model->getScale());

		if (std::shared_ptr<MeshComponent> meshComp = _model->getComponent<MeshComponent>())
		{
			std::shared_ptr<Mesh> mesh = meshComp->m_mesh;

			for (auto& submesh : mesh->m_submeshes)
			{
				// State
				uint64_t state = 0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS;

				// Material
				if (submesh->m_material)
				{
					setMaterial(submesh->m_material);
					if (submesh->m_material != nullptr)
					{
						if (submesh->m_material->blend)
						{
							state |= BGFX_STATE_BLEND_ALPHA;
						}
						if (!submesh->m_material->doubleSided)
						{
							state |= BGFX_STATE_CULL_CW;
						}
					}
				}

				// Uniforms
				setUniforms();

				// Submit
				bgfx::setState(state);
				bgfx::setTransform(mtx);
				bgfx::setVertexBuffer(0, mesh->m_vbh);
				bgfx::setIndexBuffer(submesh->m_ibh);
				bgfx::submit(m_view, m_program);
			}
		}
	}

	GBuffer::GBuffer(bgfx::ViewId _view, std::shared_ptr<CommonResources> _common)
		: m_view(_view)
		, m_common(_common)
	{
		bgfx::setViewName(_view, "GBuffer Generation");

		const bgfx::RendererType::Enum type = bgfx::getRendererType();

		// Programs
		m_program = bgfx::createProgram(
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_geometry"),
			bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_geometry"),
			true
		);

		// Uniforms
		m_defaultTexture			  = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8);
		m_normalMatrixUniform         = bgfx::createUniform("u_normalMatrix", bgfx::UniformType::Mat3);
		m_baseColorFactorUniform	  = bgfx::createUniform("u_baseColorFactor", bgfx::UniformType::Vec4);
		m_metRoughNorOccFactorUniform = bgfx::createUniform("u_metallicRoughnessNormalOcclusionFactor", bgfx::UniformType::Vec4);
		m_emissiveFactorUniform		  = bgfx::createUniform("u_emissiveFactorVec", bgfx::UniformType::Vec4);
		m_hasTexturesUniform		  = bgfx::createUniform("u_hasTextures", bgfx::UniformType::Vec4);

		// Samplers
		m_baseColorSampler = bgfx::createUniform("s_texBaseColor", bgfx::UniformType::Sampler);
		m_metallicSampler  = bgfx::createUniform("s_texMetallic", bgfx::UniformType::Sampler);
		m_roughnessSampler = bgfx::createUniform("s_texRoughness", bgfx::UniformType::Sampler);
		m_normalSampler    = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
		m_occlusionSampler = bgfx::createUniform("s_texOcclusion", bgfx::UniformType::Sampler);
		m_emissiveSampler  = bgfx::createUniform("s_texEmissive", bgfx::UniformType::Sampler);

		// Don't create framebuffer until first render call.
		m_framebuffer.idx = bgfx::kInvalidHandle;
	}

	GBuffer::~GBuffer()
	{
		destroyFramebuffer();

		bgfx::destroy(m_program);
		bgfx::destroy(m_normalMatrixUniform);
		bgfx::destroy(m_baseColorFactorUniform);
		bgfx::destroy(m_metRoughNorOccFactorUniform);
		bgfx::destroy(m_emissiveFactorUniform);
		bgfx::destroy(m_hasTexturesUniform);

		bgfx::destroy(m_baseColorSampler);
		bgfx::destroy(m_metallicSampler);
		bgfx::destroy(m_roughnessSampler);
		bgfx::destroy(m_normalSampler);
		bgfx::destroy(m_occlusionSampler);
		bgfx::destroy(m_emissiveSampler);
		bgfx::destroy(m_defaultTexture);
	}

	void GBuffer::render(std::shared_ptr<World> _world)
	{
		// Begin timer
		m_sd.begin();

		// Recreate gbuffer upon reset. 
		if (m_common->firstFrame)
		{
			destroyFramebuffer();
			createFramebuffer();
		}

		// Set view 
		bgfx::setViewFrameBuffer(m_view, m_framebuffer);
		bgfx::setViewRect(m_view, 0, 0, m_common->width, m_common->height);
		bgfx::setViewClear(m_view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewTransform(m_view, m_common->view, m_common->proj);

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