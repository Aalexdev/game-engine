#pragma once

#include "../../core.hpp"

namespace engine::ECS::components{
	struct ENGINE_API CircleCollider{
		using FixturePtr = void*;

		glm::vec2 position = glm::vec2(0.f);
		float radius = 0.5;
		bool scaledSize = true; // the scale is based on the minimal value between the scale width and scale height
		bool isSensor = false;
		FixturePtr runtimeFixture;
		
		std::string material;


		CircleCollider() = default;
		CircleCollider(const CircleCollider &) = default;
		~CircleCollider() = default;
	};
}