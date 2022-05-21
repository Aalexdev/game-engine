#pragma once

#include "../../core.hpp"
#include "../../pch.hpp"
#include "../../UUID.hpp"

#include <libs/glm/glm.hpp>

namespace engine::ECS::components{
	struct ENGINE_API PrismaticJoint{
		struct Joint{
			UUID joinedEntity = UUID::INVALID_UUID;

			glm::vec2 translation = {0.5f, 0.5f};

			bool limits = false;
			float minTranslation = 0.f;
			float maxTranslation = 1.f;

			bool motor = false;
			float maxForce = 1.f;
			float speed = 0.f;

			void *runtimeJoint = nullptr;
		};

		std::list<Joint> joints;

		PrismaticJoint() = default;
		PrismaticJoint(const PrismaticJoint &other){
			for (auto &j : other.joints){
				joints.push_back(j);
			}
		}
	};
}