#pragma once

#include "builders/DescriptorPoolBuilder.hpp"

namespace Fovea{
	class DescriptorPool{
		public:
			DescriptorPool(DescriptorPoolBuilder &builder){
				initialize(builder);
			}

			~DescriptorPool();

			void initialize(DescriptorPoolBuilder &builder);

			bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor);
			void freeDescriptors(std::vector<VkDescriptorSet> &descriptors);
			void resetPool();
			VkDescriptorPool getPool();

		private:
			VkDescriptorPool pool;
	};
}