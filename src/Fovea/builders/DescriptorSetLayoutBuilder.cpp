#include "Fovea/builders/DescriptorSetLayoutBuilder.hpp"

#include <cassert>

namespace Fovea{
	void DescriptorSetLayoutBuilder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32_t count){
		assert(bindings.count(binding) == 0 && "binding already used");

		VkDescriptorSetLayoutBinding layout = {};
		layout.binding = binding;
		layout.descriptorCount = count;
		layout.descriptorType = descriptorType;
		layout.stageFlags = shaderStages;

		bindings[binding] = layout;
	}
}