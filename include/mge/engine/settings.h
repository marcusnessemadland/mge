/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include "engine/math.h"

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
				: multipleScatteringEnabled(true)
				, whiteFurnaceEnabled(true)
			{}

			bool multipleScatteringEnabled;
			bool whiteFurnaceEnabled;

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

