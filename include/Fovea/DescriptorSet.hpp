#pragma once

#include "builders/DescriptorSetBuilder.hpp"
#include "../vulkan/vulkan.h"

#include "Fovea/Buffer.hpp"
#include "Fovea/DescriptorPool.hpp"
#include "Fovea/DescriptorSetLayout.hpp"

namespace Fovea{
	class DescriptorSet{
		public:
			DescriptorSet() = default;
			DescriptorSet(DescriptorSetBuilder &builder){
				initialize(builder);
			}

			~DescriptorSet();

			void initialize(DescriptorSetBuilder &builder);
		
		private:
			void createPool(DescriptorSetBuilder &builder);
			void createSetLayout(DescriptorSetBuilder &builder);
			void createBuffers(DescriptorSetBuilder &builder);
			void createDescriptorSets(DescriptorSetBuilder &builder);

			struct BufferRange{
				uint32_t offset;
				uint32_t size;
			};

			DescriptorPool pool;
			DescriptorSetLayout layout;
			BufferRange* buffers;
			Buffer* buffer = nullptr;
			VkDescriptorSet* sets = nullptr;
			uint32_t descriptorSetCount = 0;
	};
}