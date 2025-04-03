/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/component.h"

namespace mge
{
	std::shared_ptr<Object> Component::getOwner()
	{
		return m_owner;
	}

} // namespace mge