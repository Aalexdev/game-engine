#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"


namespace engine{
	class Scene;
	class Entity;
}

namespace engine::ECS::systems{
	class ENGINE_API RigidBody : public System{
		public:
			RigidBody(Scene *scene) : scene{scene}{}
			~RigidBody() = default;

			virtual void init() override;

			void update(Timestep ts, uint32_t velocityIteration, uint32_t positionIteration);
			void start();

			void renderCollisions(engine::Entity entity); // render the collisions of the given entity
			void renderJoints(engine::Entity entity); // render the joints connected to the entity
			void renderCollisions(); // render all collisions
		
		private:
			void connectJoints(engine::Entity entity);

			Scene *scene;
	};
}