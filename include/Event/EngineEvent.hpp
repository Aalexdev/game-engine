#pragma once

#include <engine/engine.hpp>
#include "EditorEvents.hpp"

namespace engine{
	class EngineEvent : public EditorEvent{
		public:
			EngineEvent(Event &e) : event{e} {}
			Event &getEvent() {return event;}

			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_ENGINE);
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_ENGINE);

		private:
			Event &event;
	};
}