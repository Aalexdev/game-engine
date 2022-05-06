#pragma once

#include <engine/engine.hpp>
#include "EditorEvents.hpp"

namespace engine{
	template<typename T> // T the component type
	class ComponentEvent : public EditorEvent{
		public:

			T& getComponent() const {return component;}
			Entity getEntity() const {return entity;}

			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_ENTITY | EDITOR_EVENT_CATEGORY_COMPONENT);
		
		protected:
			ComponentEvent(Entity entity, T& component) : entity{entity}, component{component}{}

			T& component;
			Entity entity;
	};

	template<typename T>
	class ComponentAddedEvent : public ComponentEvent<T>{
		public:
			ComponentAddedEvent(Entity entity, T& component) : ComponentEvent<T>(entity, component){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_COMPONENT_ADDED);
	};

	template<typename T>
	class ComponentRemovedEvent : public ComponentEvent<T>{
		public:
			ComponentRemovedEvent(Entity entity, T& component) : ComponentEvent<T>(entity, component){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_COMPONENT_REMOVED);
	};

	template<typename T>
	class ComponentCallEvent : public ComponentEvent<T>{
		public:
			ComponentCallEvent(Entity entity, T& component, int id=0) : ComponentEvent<T>(entity, component), id{id}{}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_COMPONENT_CALL);

			inline int getId() const {return id;}
		
		private:
			int id;
	};
}