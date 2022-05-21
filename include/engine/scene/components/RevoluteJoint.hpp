#pragma once

#include "../../core.hpp"
#include "../../pch.hpp"
#include "../../UUID.hpp"
#include <libs/glm/glm.hpp>

namespace engine::ECS::components{
	struct ENGINE_API RevoluteJoint{
		struct Joint{
			UUID joinedEntity = UUID::INVALID_UUID;

			glm::vec2 anchor = {0.f, 0.f};

			bool limits = false;
			float minAngle = 0.f; // in deg
			float maxAngle = 360.f; // in deg

			bool motor = false;
			float maxTorque = 10.f;
			float speed = 0.f;

			void* runtimeJoint = nullptr;
		};

		std::list<Joint> joints;

		RevoluteJoint() = default;
		RevoluteJoint(const RevoluteJoint &other){
			for (auto &j : other.joints){
				joints.push_back(j);
			}
		}
	};
}