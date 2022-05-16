#pragma once

#include "../../core.hpp"

namespace engine::ECS::components{
	struct ENGINE_API CircleCollider{
		using FixturePtr = void*;

		glm::vec2 position = glm::vec2(0.f);
		float radius = 1;
		bool scaledSize = true;
		bool isSensor = false;
		
		// TODO : move into physics material
		float density = 1.f;
		float friction = 0.5f;
		float restitution = 0.f;
		float restitutionThreshold = 0.5f;

		FixturePtr runtimeFixture;

		CircleCollider() = default;
		CircleCollider(const CircleCollider &) = default;
		~CircleCollider() = default;
	};
}