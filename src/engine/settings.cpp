/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/settings.h"

namespace mge
{
	static Settings s_settings;

	Settings& getSettings()
	{
		return s_settings;
	}

} // namespace mge