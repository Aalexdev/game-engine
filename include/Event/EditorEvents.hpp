#pragma once

#include <engine/engine.hpp>

// event system inspired by engine/Events/Event.hpp

namespace engine{

	enum class EditorEventType{
		EDITOR_EVENT_NONE = 0,
		EDITOR_EVENT_ENTITY_SELECTED, // when an entity is selected, can be also used to select an entity
		EDITOR_EVENT_ENTITY_CREATED, // when a entity is created (right after the entity get created in the registry)
		EDITOR_EVENT_ENTITY_REMOVED, // when a entity is removed (right before the entity get erease from the registry)
		EDITOR_EVENT_ENTITY_RENAMED, // when a entity is renamed
		
		EDITOR_EVENT_COMPONENT_ADDED, // when a component is added to an entity
		EDITOR_EVENT_COMPONENT_REMOVED, // when a component is removed from an entity
		EDITOR_EVENT_COMPONENT_CALL, // a custom component call

		EDITOR_EVENT_CONTENT_BROWSER_NEW_SELECTION, // when a file or a directory is selected
		EDITOR_EVENT_CONTENT_BROWSER_FILE_OPENED, // when a file is opened
		EDITOR_EVENT_CONTENT_BROWSER_DIRECTORY_OPENED, // when a directory is opened in the content browser panel
		EDITOR_EVENT_CONTENT_BROWSER_CREATED, // when a file or a directory is created
		EDITOR_EVENT_CONTENT_BROWSER_REMOVED, // when a file or a directory is removed
		EDITOR_EVENT_CONTENT_BROWSER_RENAMED, // when a file or a directory is renamed
		EDITOR_EVENT_CONTENT_BROWSER_FILE_OPTION_POPUP_OPENED, // when a file is right clicked
		EDITOR_EVENT_CONTENT_BROWSER_DIRECTORY_OPTION_POPUP_OPEND, // when a directory is right clicked
		EDITOR_EVENT_CONTENT_BROWSER_VOID_OPTION_POPUP_OPENED, // when an empty space is right clicked

		EDITOR_EVENT_PROJECT_CREATED,
		EDITOR_EVENT_PROJECT_OPENED,
		EDITOR_EVENT_PROJECT_RENAMED,

		EDITOR_RUNTIME_START, // start the runtime from zero
		EDITOR_RUNTIME_PAUSE, // pause the runtime, do nothing if the runtime has not started
		EDITOR_RUNTIME_PLAY, // play the runtime after being paused, do nothing is the runtime has not started
		EDITOR_RUNTIME_STOP, // stop the runtime, do nithing if the runtime has not started

		EDITOR_EVENT_ENGINE,
	};

	enum EditorEventCategory{
		EDITOR_EVENT_CATEGORY_NONE = 0,
		EDITOR_EVENT_CATEGORY_ENTITY = 1 << 0, // any event relatted with an entity
		EDITOR_EVENT_CATEGORY_COMPONENT = 1 << 1, // on a entity component call
		EDITOR_EVENT_CATEGORY_CONTENT_BROWSER = 1 << 2, // any content browser change
		EDITOR_EVENT_CATEGORY_PROJECT = 1 << 3,
		EDITOR_EVENT_CATEGORY_ENGINE = 1 << 4,
		EDITOR_EVENT_CATEGORY_RUNTIME = 1 << 5,
	};

	#define EDITOR_EVENT_CLASS_TYPE(type)  static EditorEventType getStaticType() {return EditorEventType::type;} \
									virtual EditorEventType getEventType() const override {return getStaticType();} \
									virtual const char * getName() const override {return #type;}

	#define EDITOR_EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override {return category;}

	class EditorEvent{
		friend class EditorEventDispatcher;

		public:
			virtual EditorEventType getEventType() const = 0;
			virtual const char* getName() const = 0;
			virtual int getCategoryFlags() const = 0;
			virtual std::string toString() const {return getName();}

			inline bool isInCategory(EditorEventCategory category){
				return getCategoryFlags() & category;
			}

			inline bool isHandled() const {return handled;}
		
		private:
			bool handled = false;
	};

	class EditorEventDispatcher{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

		public:
			EditorEventDispatcher(EditorEvent &event) : event{event} {}

			template<typename T>
			bool dispatch(EventFn<T> func){
				if (event.getEventType() == T::getStaticType()){
					event.handled = func(*(T*)&event);
					return true;
				}
				return false;
			}
		
		private:
			EditorEvent &event;
	};

	inline std::ostream& operator<<(std::ostream& os, const EditorEvent &e){
		return os << e.toString();
	}
}

#include "ContentBrowserEvent.hpp"
#include "ComponentEvent.hpp"
#include "EntityEvent.hpp"
#include "EngineEvent.hpp"
#include "RuntimeEvent.hpp"

namespace engine{
	using EditorEventCallbackFn = std::function<void(EditorEvent&)>;
}