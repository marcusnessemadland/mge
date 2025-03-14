/*
 * Copyright 2021 Alain Galvan. All rights reserved.
 * License: https://github.com/alaingalvan/CrossWindow/blob/master/license.md
 */

#ifdef VR_NOOP

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

#endif // VR_NOOP