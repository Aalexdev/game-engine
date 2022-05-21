#pragma once

#include "../../core.hpp"
#include "../../pch.hpp"
#include <libs/glm/glm.hpp>
#include "../../UUID.hpp"

namespace engine::ECS::components{
	struct ENGINE_API SpringJoint{
		struct Joint{
			UUID joinedEntity = UUID::INVALID_UUID;

			glm::vec2 anchorA = {0, 0};
			glm::vec2 anchorB = {0, 0};

			float frequencyHertz = 0;
			float dampingRatio = 0;

			void* runtimeJoint = nullptr;
		};

		std::list<Joint> joints;
		
		SpringJoint() = default;
		SpringJoint(const SpringJoint &other){
			for (auto &j : other.joints){
				joints.push_back(j);
			}
		}
	};
}