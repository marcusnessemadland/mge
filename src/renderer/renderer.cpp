/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/renderer.h"
#include "engine/camera.h"
#include "engine/material.h"
#include "engine/vertex.h"
#include "vertexpos.h"

#include "vulkan-renderer.h"
#include "common_resources.h"
#include "gbuffer.h"
#include "tonemapping.h"

#include "bgfx_utils.h"
#include <bgfx/bgfx.h>

#include <algorithm>

namespace vr
{
	void Renderer::update(const World* _world, const Camera* _camera)
	{
		const bgfx::Caps* caps = bgfx::getCaps();

		// Update world
		if (m_world == NULL)
		{
			m_world = _world;
		}

		// Update resolution upon resize
		UVec2 size = m_window->getWindowSize();
		if (m_common->width != size.x ||
			m_common->height != size.y)
		{
			m_common->width = size.x;
			m_common->height = size.y;

			if (!m_window->isClosed())
			{
				bgfx::reset(m_common->width, m_common->height);

				m_common->firstFrame = true;
			}
		}

		// Update camera
		if (_camera != NULL)
		{
			bx::mtxLookAt(
				m_common->view,
				toBgfxVec(_camera->m_position),
				toBgfxVec(_camera->m_position + _camera->m_forward),
				toBgfxVec(_camera->m_up)
			);

			if (_camera->m_projMode == Projection::Perspective)
			{
				bx::mtxProj(
					m_common->proj,
					_camera->m_fov,
					(float)m_common->width / (float)m_common->height,
					_camera->m_near,
					_camera->m_far,
					caps->homogeneousDepth,
					bx::Handedness::Left
				);
			}
			else
			{
				float halfWidth = m_common->width * 0.5f;
				float halfHeight = m_common->height * 0.5f;
				bx::mtxOrtho(
					m_common->proj,
					-halfWidth, 
					halfWidth,
					-halfHeight, 
					halfHeight,
					_camera->m_near,
					_camera->m_far,
					0.0f,
					caps->homogeneousDepth,
					bx::Handedness::Left
				);
			}
		}
	}

	void Renderer::postUpdate()
	{
		m_common->firstFrame = false;
	}

	void Renderer::render(const World* _world, const Camera* _camera)
	{
		// Update common resources before rendering
		update(_world, _camera);

		// Render
		m_gbuffer->render(_world);
		m_tonemapping->render();

		// Update common resources after rendering
		postUpdate();

		// Swap
		bgfx::frame();
	}

	Renderer::Renderer(std::shared_ptr<Window> _window, RendererType::Enum _type)
		: m_window(_window)
		, m_world(NULL)
	{
		// Common 
		UVec2 size = m_window->getWindowSize();
		m_common = std::make_unique<CommonResources>();
		m_common->width = size.x;
		m_common->height = size.y;

		// Type
		bgfx::RendererType::Enum type = bgfx::RendererType::Count;
		if (_type == RendererType::OpenGL) type = bgfx::RendererType::OpenGL;
		if (_type == RendererType::Vulkan) type = bgfx::RendererType::Vulkan;
		if (_type == RendererType::Direct3D12) type = bgfx::RendererType::Direct3D12;

		// Init
		bgfx::Init init;
		init.type = type;
		init.vendorId = BGFX_PCI_ID_NONE;
		init.platformData.nwh = _window->getNativeHandle();
		init.platformData.ndt = NULL;
		init.platformData.type = bgfx::NativeWindowHandleType::Default;
		init.resolution.width = m_common->width;
		init.resolution.height = m_common->height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);

		// Techniques
		m_gbuffer = std::make_unique<GBuffer>(0, m_common.get());
		m_tonemapping = std::make_unique<ToneMapping>(1, m_common.get(), m_gbuffer.get());

		// Layouts
		Vertex::init();
		VertexPos::init();

		// Utils
		bgfx::initBgfxUtils();
	}

	Renderer::~Renderer()
	{
		// Utils
		bgfx::shutdownBgfxUtils();

		// Techniques
		m_common.reset();
		m_gbuffer.reset();
		m_tonemapping.reset();

		// Shutdown
		bgfx::shutdown();
	}

	std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, RendererType::Enum _type)
	{
		return std::make_shared<Renderer>(_window, _type);
	}

} // namespace vr