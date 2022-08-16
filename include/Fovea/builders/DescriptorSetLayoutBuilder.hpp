#pragma once

#include "../../vulkan/vulkan.h"

#include <unordered_map>

namespace Fovea{
	class DescriptorSetLayoutBuilder{
		friend class DescriptorSetLayout;
		public:

			void AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32_t count = 1);

		private:
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

	};
}