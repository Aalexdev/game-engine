#include "Fovea/core.hpp"
#include "Fovea/DescriptorSetLayout.hpp"

#include <stdexcept>

namespace Fovea{
	DescriptorSetLayout::~DescriptorSetLayout(){
		vkDestroyDescriptorSetLayout(getInstance().logicalDevice.getDevice(), setLayout, nullptr);
	}
	
	void DescriptorSetLayout::initialize(const DescriptorSetLayoutBuilder &builder){
		bindings = builder.bindings;

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
		for (auto b : bindings){
			layoutBindings.push_back(b.second);
		}

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		createInfo.pBindings = layoutBindings.data();
		
		if (vkCreateDescriptorSetLayout(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &setLayout) != VK_SUCCESS){
			throw std::runtime_error("failed to create descriptor set layout");
		}
		
	}
	
	VkDescriptorSetLayout DescriptorSetLayout::getDescriptorSetLayout(){
		return setLayout;
	}
}