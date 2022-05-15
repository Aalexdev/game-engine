#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"
#include "../components/CameraComponent.hpp"

namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class ENGINE_API Camera : public System{
		public:
			Camera(Scene *scene);
			virtual void init() override;

			/**
			 * @brief update the cameras transformation and return the primary camera (the camera with the higher priority)
			 * @return nullptr if the is no camera in the scene
			 */
			SceneCamera* update();

			void updateViewport(uint32_t width, uint32_t height);
		
		private:
			Scene *scene;
	};
}