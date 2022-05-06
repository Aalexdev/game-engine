#pragma once

#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>

namespace engine::components{
	struct ENGINE_API Transform{
		glm::vec2 translation = {0.f, 0.f};
		glm::vec2 scale = {1.f, 1.f};
		float rotation = 0.f;

		glm::mat4 getTransform() const{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), this->rotation, {0.f, 0.f, 1.f});

			return glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * rotation * glm::scale(glm::mat4(1.f), {scale.x, scale.y, 1.f});
		}
	};
}