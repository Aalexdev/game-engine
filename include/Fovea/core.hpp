#pragma once

#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "CommandPool.hpp"
#include "Renderer.hpp"

namespace Fovea{
	struct Core{
		Instance instance;
		PhysicalDevice physicalDevice;
		LogicalDevice logicalDevice;
		CommandPool renderCommandPool;
		Renderer renderer;
	};

	Core& getInstance();
};