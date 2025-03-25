/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

namespace vr
{
	class ComponentI
	{
	public:
		/// update that happens before entity update
		virtual void preUpdate(double _dt) = 0;

		/// update that happens after entity update
		virtual void postUpdate(double _dt) = 0;
	};

} // namespace vr