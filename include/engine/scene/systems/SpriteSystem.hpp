#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"
#include "../../dataStruct/Set.hpp"

namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class ENGINE_API Sprite : public System{
		public:
			Sprite(Scene *scene) : scene{scene}{}
			virtual void init() override;

			void update();
		
		private:
			Scene *scene;
	};
}