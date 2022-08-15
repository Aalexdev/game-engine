#pragma once

#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "CommandPool.hpp"
#include "Renderer.hpp"
#include "Pipeline.hpp"
#include "PipelineLibrary.hpp"
#include "RenderTargetLibrary.hpp"
#include "DescriptorSetLibrary.hpp"
#include "TextureLibrary.hpp"

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
		RenderTargetLibrary renderTargetLibrary;
		DescriptorSetLibrary descriptorSetLibrary;
		TextureLibrary textureLibrary;

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

		WaitIdle wait;
	};

	Core& getInstance();
};