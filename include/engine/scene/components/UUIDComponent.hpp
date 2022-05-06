#pragma once

#include "../../core.hpp"
#include "../../UUID.hpp"

namespace engine::components{
	struct UUID{
		engine::UUID id;

		UUID() = default;
		UUID(const UUID &) = default;
		UUID(engine::UUID id) : id{id} {}

		operator engine::UUID() const {return id;}
		operator uint64_t() const {return id;}	
	};
}