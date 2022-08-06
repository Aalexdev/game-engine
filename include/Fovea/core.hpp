#pragma once

#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "CommandPool.hpp"
#include "Renderer.hpp"
#include "Pipeline.hpp"
#include "PipelineLibrary.hpp"

namespace Fovea{

	class WaitIdle{
		public:
			~WaitIdle();
	};

	struct Core{
		Instance instance;
		PhysicalDevice physicalDevice;
		LogicalDevice logicalDevice;
		CommandPool renderCommandPool;
		Renderer renderer;
		PipelineLibrary pipelineLibrary;

		WaitIdle wait;
	};

	Core& getInstance();
};