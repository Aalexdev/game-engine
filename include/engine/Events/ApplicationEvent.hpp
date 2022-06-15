#pragma once

#include "../pch.hpp"
#include "Event.hpp"

namespace engine{
	class ENGINE_API AppTickEvent : public Event{
		public:
			AppTickEvent(Application *app) : Event(app) {}
			EVENT_CLASS_TYPE(EVENT_APP_TICK);
			EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
	};
}