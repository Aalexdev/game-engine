#pragma once

#include "../../core.hpp"
#include "../../UUID.hpp"
#include <libs/glm/glm.hpp>

namespace engine{	
	struct ENGINE_API Joint{
		UUID entityB = UUID::INVALID_UUID;
		void* runtimeJoint = nullptr;

		glm::vec2 anchorOffsetA = {0.f, 0.f}; // the offset of the joint on the entity A
		glm::vec2 anchorOffsetB = {0.f, 0.f}; // the offset of the joint on the entity B
	};
}