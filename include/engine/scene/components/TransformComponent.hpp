#pragma once

#include "../../core.hpp"
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>

namespace engine::components{
	struct ENGINE_API Transform{
		glm::vec2 translation = {0.f, 0.f};
		glm::vec2 scale = {1.f, 1.f};
		float rotation = 0.f;

		glm::mat4 transform; // curent transform
		glm::mat4 lastTransform; // the transform during the last frame
	};
}