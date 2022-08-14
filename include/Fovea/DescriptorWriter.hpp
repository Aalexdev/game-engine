#pragma once

#include "DescriptorSetLayout.hpp"
#include "DescriptorPool.hpp"

#include "../vulkan/vulkan.h"

#include <vector>

namespace Fovea{
	class DescriptorWriter{
		public:
			DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool){
				initialize(setLayout, pool);
			}

			void initialize(DescriptorSetLayout &setLayout, DescriptorPool &pool);
			
		
			void writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
			void writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
			
			bool build(VkDescriptorSet &set);
			void overwrite(VkDescriptorSet &set);

		private:
			DescriptorSetLayout* setLayout;
			DescriptorPool* pool;
			std::vector<VkWriteDescriptorSet> writes;
	};
}