/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <memory>

namespace vr
{
	class Entity;

	class Component
	{
		friend class Entity;

	public:
		/// 
		std::shared_ptr<Entity> getOwner();

	protected:
		/// update that happens before entity update
		virtual void preUpdate(double _dt) = 0;

		/// update that happens after entity update
		virtual void postUpdate(double _dt) = 0;

	private:
		std::shared_ptr<Entity> m_owner;
	};

} // namespace vr