#pragma once

#include "../core.hpp"
#include "../UUID.hpp"

namespace engine{
	struct ENGINE_API PhysicMaterial{
		float density = 1.f;
		float friction = 0.5f;
		float restitution = 0.f;
		float restitutionThreshold = 0.5f;

		std::string name;
	};
}