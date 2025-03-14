/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#ifdef VR_NOOP

#include "../Event.h"

namespace vr
{
	class EventQueue
	{
	public:
		EventQueue();

		void update();

		const Event& front();

		void pop();

		bool empty();

	protected:
		Event e = Event(EventType::EventTypeMax, nullptr);
	};

}

#endif // VR_NOOP