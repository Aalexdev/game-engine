#pragma once

#include "../core.hpp"
#include "../renderer/Camera.hpp"

namespace engine{
	class ENGINE_API SceneCamera{
		public:
			SceneCamera(){}

			inline void setZoomLevel(float zoom) {zoomLevel = zoom; updateProjection();}
			inline float getZoomLevel() const {return zoomLevel;}

			inline void setAspectRatio(float aspec) {aspecRatio = aspec; updateProjection();}
			inline float getAspectRatio() const {return aspecRatio;}

			inline void setViewportSize(uint32_t width, uint32_t height){setAspectRatio(static_cast<float>(width) / static_cast<float>(height));}

			inline void setPosition(const glm::vec2 &position) {camera.setPosition(position);}
			inline const glm::vec2& getPosition() const {return camera.getPosition();}

			inline void setRotation(float rotation) {camera.setRotation(rotation);}
			inline float getRotation() const {return camera.getRotation();}

			inline Camera& getCamera() {return camera;}

			inline const glm::mat4 &getViewMatrix() const {return camera.getViewMatrix();}
			inline const glm::mat4 &getProjectionMatrix() const {return camera.getProjectionMatrix();}
			inline const glm::mat4 &getViewProjectionMatrix() const {return camera.getViewProjection();}

			void updateView();
		
		private:
			void updateProjection();

			Camera camera;
			float zoomLevel = 1.f;
			float aspecRatio = 1.f;
	};
}