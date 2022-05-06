#pragma once

#include "../../core.hpp"

namespace engine::components{
	struct ENGINE_API Tag{
		std::string tag;

		Tag(const std::string &tag) : tag{tag} {}
		operator const std::string&() const {return tag;} 
	};
}