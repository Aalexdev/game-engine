#pragma once

#include "../engine.hpp"
#include "Entity.hpp"

namespace engine{
	class ENGINE_API ScriptableEntity : public Entity{
		public:
			ScriptableEntity() = default;
			ScriptableEntity(entt::entity entity, Scene *scene) : Entity(entity, scene) {}
			virtual ~ScriptableEntity() = default;

			virtual void OnCreate() {};
			virtual void OnDestroy() {};

			// runtime
			virtual void OnUpdate(Timestep dt) {};
	};
}