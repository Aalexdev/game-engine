#pragma once

#include "../../core.hpp"
#include "../../pch.hpp"
#include "Joint.hpp"

namespace engine::ECS::components{
	struct ENGINE_API DistanceJoint{
		std::list<Joint> joints;

		DistanceJoint() = default;
		DistanceJoint(const DistanceJoint &other){
			for (auto &j : other.joints){
				joints.push_back(j);
			}
		}
	};
}