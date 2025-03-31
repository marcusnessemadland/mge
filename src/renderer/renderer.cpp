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
#include <bx/timer.h>
#include <optick.h>

#include <algorithm>

namespace vr
{
	void Renderer::update(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera)
	{
		OPTICK_EVENT();

		const bgfx::Caps* caps = bgfx::getCaps();

		// Update world
		if (m_world == nullptr)
		{
			m_world = _world;
		}

		// Update resolution upon resize
		uint32_t w = m_window->getWidth();
		uint32_t h = m_window->getHeight();

		if (m_common->width != w ||
			m_common->height != h)
		{
			m_common->width = w;
			m_common->height = h;

			if (!m_window->isClosed())
			{
				bgfx::reset(m_common->width, m_common->height);

				m_common->firstFrame = true;
			}
		}

		// Update camera
		if (_camera != nullptr)
		{
			bx::mtxLookAt(
				m_common->view,
				toBgfxVec(_camera->m_position),
				toBgfxVec(_camera->m_target),
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
					bx::Handedness::Right // Matches Autodesk Maya
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
					bx::Handedness::Right // Matches Autodesk Maya
				);
			}
		}
	}

	void Renderer::postUpdate()
	{
		OPTICK_EVENT();

		m_common->firstFrame = false;
	}

	void Renderer::render(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera)
	{
		OPTICK_EVENT();

		//
		bgfx::dbgTextClear();

		SampleData& sampleData = _world->m_sampleData;
		bgfx::dbgTextPrintf(1, 1, 0xf, "min: %.3fms", sampleData.getMin());
		bgfx::dbgTextPrintf(1, 2, 0xf, "max: %.3fms", sampleData.getMax());
		bgfx::dbgTextPrintf(1, 3, 0xf, "avg: %.3fms", sampleData.getAverage());
		bgfx::dbgTextPrintf(1, 4, 0xf, "framerate: %.2f fps", 1000.0f / sampleData.getAverage());
		bgfx::dbgTextPrintf(1, 5, 0xf, "gfx: %s", bgfx::getRendererName(bgfx::getRendererType()));

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
		, m_world(nullptr)
	{
		// Common 
		m_common = std::make_unique<CommonResources>();
		m_common->width = m_window->getWidth();
		m_common->height = m_window->getHeight();

		// Type
		bgfx::RendererType::Enum type = bgfx::RendererType::Count;
		if (_type == RendererType::OpenGL) type = bgfx::RendererType::OpenGL;
		if (_type == RendererType::Vulkan) type = bgfx::RendererType::Vulkan;
		if (_type == RendererType::Direct3D12) type = bgfx::RendererType::Direct3D12;

		// Callback
		m_callback = std::make_unique<BgfxCallback>();

		// Init
		bgfx::Init init;
		init.type = type;
		init.vendorId = BGFX_PCI_ID_NONE;
		init.callback = m_callback.get();
		init.platformData.nwh = _window->getNativeHandle();
		init.platformData.ndt = nullptr;
		init.platformData.type = bgfx::NativeWindowHandleType::Default;
		init.resolution.width = m_common->width;
		init.resolution.height = m_common->height;
		init.resolution.reset = BGFX_RESET_NONE;
		bgfx::init(init);

		bgfx::setDebug(BGFX_DEBUG_TEXT);

		// Techniques
		m_gbuffer = std::make_shared<GBuffer>(0, m_common);
		m_tonemapping = std::make_shared<ToneMapping>(1, m_common, m_gbuffer);

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