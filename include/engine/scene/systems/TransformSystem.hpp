#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"

namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class ENGINE_API Transform : public System{
		public:
			Transform(Scene *scene);
			virtual void init() override;

			void update();
		
		private:
			Scene *scene;
	};
}