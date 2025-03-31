/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#include "engine/component.h"

namespace vr
{
	std::shared_ptr<Entity> Component::getOwner()
	{
		return m_owner;
	}

} // namespace vr