#pragma once

#include "EditorEvents.hpp"

namespace engine{
	class RuntimeStartedEvent : public EditorEvent{
		public:
			RuntimeStartedEvent(){}
			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_RUNTIME);
			EDITOR_EVENT_CLASS_TYPE(EDITOR_RUNTIME_START);
	};

	class RuntimePausedEvent : public EditorEvent{
		public:
			RuntimePausedEvent(){}
			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_RUNTIME);
			EDITOR_EVENT_CLASS_TYPE(EDITOR_RUNTIME_PAUSE);
	};

	class RuntimePlayedEvent : public EditorEvent{
		public:
			RuntimePlayedEvent(){}
			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_RUNTIME);
			EDITOR_EVENT_CLASS_TYPE(EDITOR_RUNTIME_PLAY);
	};

	class RuntimeStoppedEvent : public EditorEvent{
		public:
			RuntimeStoppedEvent(){}
			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_RUNTIME);
			EDITOR_EVENT_CLASS_TYPE(EDITOR_RUNTIME_STOP);
	};
}