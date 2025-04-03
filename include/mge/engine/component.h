/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include <memory>

namespace mge
{
	/// Object Component.
	///
	class Component
	{
		friend class Object;

	public:
		/// Get the owner of this component.
		/// 
		/// @returns Shared owner.
		///
		std::shared_ptr<Object> getOwner();

	protected:
		virtual void preUpdate(double _dt) = 0;
		virtual void postUpdate(double _dt) = 0;

	private:
		std::shared_ptr<Object> m_owner;
	};

} // namespace mge