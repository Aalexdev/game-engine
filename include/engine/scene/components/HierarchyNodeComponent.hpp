#pragma once

#include "../../core.hpp"
#include "../../dataStruct/Set.hpp"
#include "../../UUID.hpp"
#include "../../pch.hpp"

namespace engine::components{
	struct ENGINE_API HierarchyNode{
		UUID parent;
		std::list<UUID> childs;
	};
}