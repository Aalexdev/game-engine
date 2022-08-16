#include "Fovea/core.hpp"
#include "Fovea/DescriptorPool.hpp"

#include <cassert>

namespace Fovea{
	DescriptorPool::~DescriptorPool(){
		vkDestroyDescriptorPool(getInstance().logicalDevice.getDevice(), pool, nullptr);
	}

	void DescriptorPool::initialize(DescriptorPoolBuilder &builder){
		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount = static_cast<uint32_t>(builder.poolSizes.size());
		createInfo.pPoolSizes = builder.poolSizes.data();
		createInfo.flags = builder.poolFlags;
		createInfo.maxSets = builder.maxSets;

		if (vkCreateDescriptorPool(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &pool) != VK_SUCCESS){
			throw std::runtime_error("failed to create descriptor pool");
		}
	}

	bool DescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor){
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;

		return vkAllocateDescriptorSets(getInstance().logicalDevice.getDevice(), &allocInfo, &descriptor) == VK_SUCCESS;
	}

	void DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors){
		vkFreeDescriptorSets(getInstance().logicalDevice.getDevice(), pool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
	}

	void DescriptorPool::resetPool(){
		vkResetDescriptorPool(getInstance().logicalDevice.getDevice(), pool, 0);
	}

	VkDescriptorPool DescriptorPool::getPool(){
		return pool;
	}
}