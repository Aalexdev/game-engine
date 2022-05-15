#pragma once

#include "../../core.hpp"
#include "../../renderer/SubTexture2D.hpp"

#include <libs/glm/glm.hpp>

namespace engine::ECS::components{
	struct ENGINE_API Sprite{
		glm::vec4 color = {1.f, 1.f, 1.f, 1.f};
		Ref<SubTexture2D> texture = nullptr;
		uint16_t batchGroup = 0;
	};
}