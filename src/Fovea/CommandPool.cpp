#include "Fovea/CommandPool.hpp"
#include "Fovea/core.hpp"

#include <stdexcept>

namespace Fovea{
	CommandPool::~CommandPool(){
		vkDestroyCommandPool(getInstance().logicalDevice.getDevice(), commandPool, nullptr);
	}

	void CommandPool::initialize(CommandPoolBuilder &builder){
		flags = builder.flags;
		family = builder.family;
		
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		createInfo.queueFamilyIndex = getInstance().physicalDevice.getFamily(family).family;
		createInfo.flags = builder.flags;

		if (vkCreateCommandPool(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &commandPool)){
			throw std::runtime_error("failed to create a command pool");
		}
	}

	VkCommandPool CommandPool::getCommandPool(){
		return commandPool;
	}
}