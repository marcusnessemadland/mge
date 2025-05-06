/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

namespace mge
{
	struct CommonResources
	{
		CommonResources()
			: firstFrame(true)
			, view()
			, proj()
			, width(1280)
			, height(720)
		{
		}

		bool firstFrame;

		float view[16];
		float proj[16];
		Vec3 cameraDirection;

		uint16_t width;
		uint16_t height;
	};

} // namespace mge

