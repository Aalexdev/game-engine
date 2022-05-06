#include "engine/renderer/Camera.hpp"

#include <libs/glm/gtc/matrix_transform.hpp>

namespace engine{
	Camera::Camera(){
		projection = glm::mat4(1.f);
		update();
	}

	void Camera::orthographic(float left, float right, float bottom, float top, float near, float far){
		projection = glm::ortho(left, right, bottom, top, near, far);
		update();
	}


	Camera::Camera(const glm::mat4& projection) : projection{projection}{
		update();
	}

	void Camera::update(){
        glm::mat4 transform = glm::translate(glm::mat4(1.f), {position.x, position.y, 0.f}) * glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1.f));
        view = glm::inverse(transform);
        viewProjection = projection * view;
	}
}