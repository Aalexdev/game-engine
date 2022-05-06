#pragma once

#include "../../core.hpp"

#include <libs/glm/glm.hpp>

namespace engine::components{
	struct ENGINE_API CircleRenderer{

		glm::vec4 color = glm::vec4(1.f);
		float thickness = 1.f;
		float fade = 0.005f;


		CircleRenderer(const CircleRenderer &) = default;
		CircleRenderer() = default;
	};
}