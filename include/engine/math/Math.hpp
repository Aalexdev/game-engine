#pragma once

#include "../core.hpp"
#include <libs/glm/glm.hpp>

namespace engine::math{
	ENGINE_API bool decomposeTransform(const glm::mat4 &mat, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation);
	ENGINE_API bool decomposeTransform2D(const glm::mat4 &mat, glm::vec2 &translation, glm::vec2 &scale, float &rotation);
}