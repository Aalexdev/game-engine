#include "engine/pch.hpp"
#include "engine/UUID.hpp"

#include <random>

namespace engine{

	static std::random_device randomDevice;
	static std::mt19937_64 engine{randomDevice()};
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	uint64_t UUID::generateUUID(){
		uint64_t id = uniformDistribution(engine);
		if (id == INVALID_UUID) return generateUUID();
		return id;
	}
}