#pragma once

#include "../core.hpp"


namespace engine{
	class Scene;

	namespace ECS::systems{
		class CircleRenderer;
		class TriangleRenderer;
		class Sprite;
	}

	class ENGINE_API BatchRenderer{
		public:
			BatchRenderer(Scene *scene);
			~BatchRenderer() = default;

			void render();
		
		private:
			Scene *scene;

			Ref<ECS::systems::CircleRenderer> circleRendererSystem;
			Ref<ECS::systems::Sprite> spriteSystem;
			Ref<ECS::systems::TriangleRenderer> triangleRendererSystem;
	};
}