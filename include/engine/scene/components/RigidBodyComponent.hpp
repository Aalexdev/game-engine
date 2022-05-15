#pragma once

#include "../../core.hpp"

namespace engine::ECS::components{
	struct ENGINE_API RigidBody{
		using BodyPtr = void*;

		enum class BodyType{
			STATIC = 0,
			DYNAMIC = 1,
			KINEMATIC = 2
		};

		BodyType type = BodyType::STATIC;
		bool fixedRotation = false;
		glm::vec2 linearVelocity = {0, 0};
		float angularVelocity = 0;
		bool bullet = false;
		float gravityScale = 1.f;

		BodyPtr runtimeBody = nullptr;

		RigidBody() = default;
		RigidBody(const RigidBody &) = default;
	};
}