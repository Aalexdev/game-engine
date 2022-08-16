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

			DescriptorSetLayout& getLayout();
			DescriptorPool& getPool();
			VkDescriptorSet* getSets();
			VkDescriptorSet getSet(uint32_t index);

			void* getBuffer(uint32_t setIndex, uint32_t binding);
			void writeBuffer(uint32_t setIndex, uint32_t binding, void* data);

			void setDescriptorImage(uint32_t setIndex, uint32_t binding, uint32_t index, VkDescriptorImageInfo info);
		
		private:
			void createPool(DescriptorSetBuilder &builder);
			void createSetLayout(DescriptorSetBuilder &builder);
			void createBuffers(DescriptorSetBuilder &builder);
			void createDescriptorSets(DescriptorSetBuilder &builder);

			struct BufferRange{
				uint32_t offset;
				uint32_t size;
				uint32_t alignement;
			};

			// store the builder for when the user want to overwrite a texture
			DescriptorSetBuilder builder;

			DescriptorPool pool;
			DescriptorSetLayout layout;
			BufferRange* buffers;
			Buffer* buffer = nullptr;
			VkDescriptorSet* sets = nullptr;
			uint32_t descriptorSetCount = 0;
	};
}