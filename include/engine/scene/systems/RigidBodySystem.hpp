#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"


namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class ENGINE_API RigidBody : public System{
		public:
			RigidBody(Scene *scene) : scene{scene}{}
			~RigidBody() = default;

			virtual void init() override;

			void update(Timestep ts, uint32_t velocityIteration, uint32_t positionIteration);
			void start();
		
		private:
			Scene *scene;
	};
}