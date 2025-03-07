#include "NoopEventQueue.h"

namespace vr
{
  EventQueue::EventQueue()
  {
  }

  void EventQueue::update()
  {
  }

  const Event& EventQueue::front()
  {
    return e;
  }

  void EventQueue::pop()
  {
  }
  bool EventQueue::empty()
  {
	  return false;
  }
}