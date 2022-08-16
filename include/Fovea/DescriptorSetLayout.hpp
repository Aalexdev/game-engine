#pragma once

#include "../vulkan/vulkan.h"
#include "builders/DescriptorSetLayoutBuilder.hpp"

namespace Fovea{
	class DescriptorSetLayout{
		friend class DescriptorWriter;
		public:
			DescriptorSetLayout() = default;
			DescriptorSetLayout(const DescriptorSetLayoutBuilder &builder){
				initialize(builder);
			}
			~DescriptorSetLayout();
			void initialize(const DescriptorSetLayoutBuilder &builder);

			VkDescriptorSetLayout getDescriptorSetLayout();

		private:
			VkDescriptorSetLayout setLayout;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
	};
}