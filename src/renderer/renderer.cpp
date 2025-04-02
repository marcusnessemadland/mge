/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/renderer.h"
#include "engine/camera.h"
#include "engine/material.h"
#include "engine/vertex.h"
#include "vertexpos.h"

#include "mge.h"
#include "common_resources.h"
#include "gbuffer.h"
#include "tonemapping.h"
#include "imgui.h"

#include "bgfx_utils.h"
#include <bgfx/bgfx.h>
#include <bx/timer.h>

#include <algorithm>

namespace vr
{
	void Renderer::update(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera)
	{
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
		m_common->firstFrame = false;
	}

	void Renderer::render(std::shared_ptr<World> _world, std::shared_ptr<Camera> _camera)
	{
		const bgfx::Stats* stats = bgfx::getStats();

		const uint32_t x = stats->textWidth - 40;

		const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
		const double toMsGpu = 1000.0 / stats->gpuTimerFreq;
		m_sdCpu.pushSample(float(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu);
		m_sdGpu.pushSample(float(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu);

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(x, 1, 0x8a, " cpu(game):    ");
		bgfx::dbgTextPrintf(x + 15, 1, 0x8a, "%.2f ms [%.2f ms] ", _world->m_sdGame.getAverage(), _world->m_sdGame.getMax());

		bgfx::dbgTextPrintf(x, 2, 0x8a, " cpu(render):  ");
		bgfx::dbgTextPrintf(x + 15, 2, 0x8a, "%.2f ms [%.2f ms] ", m_sdCpu.getAverage(), m_sdCpu.getMax());

		bgfx::dbgTextPrintf(x, 3, 0x8a, " gpu:          ");
		bgfx::dbgTextPrintf(x + 15, 3, 0x8a, "%.2f ms [%.2f ms] ", m_sdGpu.getAverage(), m_sdGpu.getMax());

		float framerate = 1000.0f / _world->m_sdTotal.getAverage();
		bgfx::dbgTextPrintf(x, 4, framerate < 60 ? 0x8c : 0x8a, " framerate:     ");
		bgfx::dbgTextPrintf(x + 15, 4, framerate < 60 ? 0x8c : 0x8a, "%.2f fps        ", framerate);

		float textures = (float)stats->textureMemoryUsed / (1024.0f * 1024.0f);
		bgfx::dbgTextPrintf(x, 5, textures > 1454 ? 0x8c : 0x8a, " textures:     ");
		bgfx::dbgTextPrintf(x + 15, 5, textures > 1454 ? 0x8c : 0x8a, "%.2f / 1454 MiB ", textures);
		
		// Update common resources before rendering
		update(_world, _camera);

		// Render
		m_gbuffer->render(_world);
		m_tonemapping->render();
		m_imgui->render();

		// Update common resources after rendering
		postUpdate();

		// Swap
		bgfx::frame();
	}

	Renderer::Renderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type)
		: m_window(_window)
		, m_world(nullptr)
	{
		// Common 
		m_common = std::make_unique<CommonResources>();
		m_common->width = m_window->getWidth();
		m_common->height = m_window->getHeight();

		// Callback
		m_callback = std::make_unique<BgfxCallback>();

		// Init
		bgfx::Init init;
		init.type = _type;
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
		m_imgui = std::make_shared<Imgui>(255, m_common, m_window);

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
		m_imgui.reset();

		// Shutdown
		bgfx::shutdown();
	}

	std::shared_ptr<Renderer> createRenderer(std::shared_ptr<Window> _window, bgfx::RendererType::Enum _type)
	{
		return std::make_shared<Renderer>(_window, _type);
	}

} // namespace vr