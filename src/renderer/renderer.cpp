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

#include "bgfx_utils.h"
#include "common_resources.h"
#include "gbuffer.h"
#include "tonemapping.h"
#include "imgui.h"

namespace mge
{
	void Renderer::dbgTextPrintStats(const bgfx::Stats* _stats)
	{
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		const uint32_t x = _stats->textWidth - 40;

		const float toMsCpu = float(1000.0 / _stats->cpuTimerFreq);
		const float toMsGpu = float(1000.0 / _stats->gpuTimerFreq);
		m_sdCpu.pushSample(float(_stats->cpuTimeEnd - _stats->cpuTimeBegin) * toMsCpu);
		m_sdGpu.pushSample(float(_stats->gpuTimeEnd - _stats->gpuTimeBegin) * toMsGpu);

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(x, 1, 0x8a, " cpu(game):    ");
		bgfx::dbgTextPrintf(x + 15, 1, 0x8a, "%.2f ms [%.2f ms] ", m_world->m_sdGame.getAverage(), m_world->m_sdGame.getMax());

		bgfx::dbgTextPrintf(x, 2, 0x8a, " cpu(render):  ");
		bgfx::dbgTextPrintf(x + 15, 2, 0x8a, "%.2f ms [%.2f ms] ", m_sdCpu.getAverage(), m_sdCpu.getMax());

		bgfx::dbgTextPrintf(x, 3, 0x8a, " gpu:          ");
		bgfx::dbgTextPrintf(x + 15, 3, 0x8a, "%.2f ms [%.2f ms] ", m_sdGpu.getAverage(), m_sdGpu.getMax());

		float framerate = 1000.0f / m_world->m_sdTotal.getAverage();
		bgfx::dbgTextPrintf(x, 4, framerate < 60 ? 0x8c : 0x8a, " framerate:     ");
		bgfx::dbgTextPrintf(x + 15, 4, framerate < 60 ? 0x8c : 0x8a, "%.2f fps        ", framerate);

		float textures = (float)_stats->textureMemoryUsed / (1024.0f * 1024.0f);
		bgfx::dbgTextPrintf(x, 5, textures > 1454 ? 0x8c : 0x8a, " textures:     ");
		bgfx::dbgTextPrintf(x + 15, 5, textures > 1454 ? 0x8c : 0x8a, "%.2f / 1454 MiB ", textures);
	}

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

		// Matches Autodesk Maya
		bx::Handedness::Enum handedness = bx::Handedness::Right;

		// Update camera
		if (_camera != nullptr)
		{
			bx::mtxLookAt(
				m_common->view,
				toBgfxVec(_camera->getPosition()),
				toBgfxVec(_camera->getTarget()),
				toBgfxVec(_camera->getUp())
			);

			if (_camera->getProjectionMode() == Projection::Perspective)
			{
				bx::mtxProj(
					m_common->proj,
					_camera->getFOV(),
					(float)m_common->width / (float)m_common->height,
					_camera->getNear(),
					_camera->getFar(),
					caps->homogeneousDepth,
					handedness
				);
			}
			else
			{
				const float halfWidth = m_common->width * 0.5f;
				const float halfHeight = m_common->height * 0.5f;

				bx::mtxOrtho(
					m_common->proj,
					-halfWidth, 
					halfWidth,
					-halfHeight, 
					halfHeight,
					_camera->getNear(),
					_camera->getFar(),
					0.0f,
					caps->homogeneousDepth,
					handedness
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
		// Push Stats
		const bgfx::Stats* stats = bgfx::getStats();
		m_sdCpu.pushSample(float(stats->cpuTimeEnd - stats->cpuTimeBegin) * float(1000.0 / stats->cpuTimerFreq));
		m_sdGpu.pushSample(float(stats->gpuTimeEnd - stats->gpuTimeBegin) * float(1000.0 / stats->gpuTimerFreq));

		// Update common resources before rendering
		update(_world, _camera);

		// Begin timer
		m_sd.begin();

		// Render
		m_gbuffer->render(_world);
		m_tonemapping->render();
		m_imgui->render(shared_from_this());

		// End timer
		m_sd.pushSample(m_sd.end());

		// Update common resources after rendering
		postUpdate();

		// Print Stats
		dbgTextPrintStats(stats);

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
		init.platformData.ndt = _window->getNativeDisplayHandle();
		init.platformData.type = bgfx::NativeWindowHandleType::Default;
		init.resolution.width = m_common->width;
		init.resolution.height = m_common->height;
		init.resolution.reset = BGFX_RESET_NONE;
		bgfx::init(init);

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

} // namespace mge