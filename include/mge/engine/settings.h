/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

#include <stdint.h>

namespace mge
{
	struct Settings
	{
		struct Scene
		{

		} scene;

		struct Camera
		{

		} camera;

		struct Renderer
		{
			Renderer()
				: shadowMapRes(512)
			{
			}

			uint32_t shadowMapRes;

		} renderer;

		struct Debugging
		{
			Debugging()
				: buffer(BufferDebug::None)
			{}

			enum BufferDebug
			{
				None = 0,
				DiffuseRoughness,  
				Normal,     
				FresnelMetallic,   
				EmissiveOcclusion, 
				Depth,    

			} buffer;

		} debugging;
	};

	Settings& getSettings();

} // namespace mge

