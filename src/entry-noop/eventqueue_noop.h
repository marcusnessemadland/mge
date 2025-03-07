#pragma once

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