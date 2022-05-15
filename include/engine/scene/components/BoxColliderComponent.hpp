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

		// TODO ; move into physics material
		float density = 1.f;
		float friction = 0.5f;
		float restitution = 0.f;
		float restitutionThreshold = 0.5f;

		FixturePtr runtimeFixture;

		BoxCollider() = default;
		BoxCollider(const BoxCollider &) = default;
	};
}