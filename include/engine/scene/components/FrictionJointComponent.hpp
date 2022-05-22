#pragma once

#include "../../core.hpp"
#include "../../pch.hpp"
#include "../../UUID.hpp"

namespace engine::ECS::components{
	struct ENGINE_API FrictionJoint{
		struct Joint{
			UUID joinedEntity = UUID::INVALID_UUID;
			float maxForce = 0.f;
			float maxTorque = 0.f;

			void *runtimeJoint = nullptr;
		};

		std::list<Joint> joints;

		FrictionJoint() = default;
		FrictionJoint(const FrictionJoint &other){
			for (auto &j : other.joints){
				joints.push_back(j);
			}
		}
	};
}