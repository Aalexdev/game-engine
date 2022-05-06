#pragma once

#include "../core.hpp"

#include <libs/glm/glm.hpp>

namespace engine{

	// the engine is 2d only, therefore there is only an orthographic camera
	class ENGINE_API Camera{
		public:
			Camera();
			Camera(const glm::mat4& projection);
			~Camera() {}

			inline void setPosition(const glm::vec2 &position) {this->position = position;}
			inline const glm::vec2 &getPosition() const {return position;}

			// rotation in radians
			inline void setRotation(float rotation) {this->rotation = rotation;}
			inline float getRotation() const {return rotation;}

			void orthographic(float left, float right, float bottom, float top, float near, float far);
			void update();

			inline const glm::mat4& getProjectionMatrix() const {return projection;}
			inline const glm::mat4& getViewMatrix() const {return view;}
			inline const glm::mat4& getViewProjection() const {return viewProjection;}
		
		private:
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 viewProjection;

			glm::vec2 position {0.f, 0.f};
			float rotation {0.f};
	};
}