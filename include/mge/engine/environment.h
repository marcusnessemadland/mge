/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

namespace mge
{
	/// Environment Type.
	///
	struct Environment
	{
		enum Enum
		{
			Skybox,   //!< Apply this map to the sky
			Diffuse,  //!< Apply this map to the diffuse sky visibility on objects
			Specular, //!< Apply this map to the specular sky visibility on objects

			Count
		};
	};

} // namespace mge