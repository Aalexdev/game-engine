#pragma once

#include <engine/engine.hpp>
#include "EditorEvents.hpp"

namespace engine{
	class EntityEvent : public EditorEvent{
		public:
			inline Entity getEntity() const {return entity;}

			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_ENTITY);
		
		protected:
			EntityEvent(Entity entity) : entity{entity}{}
			Entity entity;
	};

	class EntityCreatedEvent : public EntityEvent{
		public:
			EntityCreatedEvent(Entity entity) : EntityEvent(entity){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_ENTITY_CREATED);
	};

	class EntityRenamedEvent : public EntityEvent{
		public:
			EntityRenamedEvent(Entity entity) : EntityEvent(entity){}

			const std::string& getNewName(){
				ENGINE_ASSERT(entity, "invalide entity");
				return entity.getTag();
			}

			
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_ENTITY_RENAMED);
	};

	// this event is called just before the entity get removed and not after to avoid errors
	class EntityRemovedEvent : public EntityEvent{
		public:
			EntityRemovedEvent(Entity entity) : EntityEvent(entity){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_ENTITY_REMOVED);
	};

	class EntitySelectedEvent : public EntityEvent{
		public:
			EntitySelectedEvent(Entity entity) : EntityEvent(entity){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_ENTITY_SELECTED);
	};
}