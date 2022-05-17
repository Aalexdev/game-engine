#pragma once

#include "../../core.hpp"
#include <libs/glm/glm.hpp>

namespace engine::ECS::components{
	struct ENGINE_API BoxCollider{
		using FixturePtr = void*;

		glm::vec2 offset = glm::vec2(0.f);
		glm::vec2 size = glm::vec2(0.5f);
		bool scaledSize = true;
		bool isSensor = false;
		FixturePtr runtimeFixture;

		std::string material;

		BoxCollider() = default;
		BoxCollider(const BoxCollider &) = default;
	};
}