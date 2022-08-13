#pragma once

#include "../vulkan/vulkan.h"

namespace Fovea{
	class Buffer{
		public:
			Buffer() = default;
			~Buffer();

			// avoid copy
			Buffer(const Buffer &) = delete;
			Buffer &operator=(const Buffer &) = delete;

			void alloc(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryProperties, VkDeviceSize minOffsetAlignement = 1);

			VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			void unmap();
			void write(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			void writeToIndex(void* data, int index);
			VkResult flushIndex(int index);
			VkDescriptorBufferInfo descriptorInfoForIndex(int index);
			VkResult invalidateIndex(int index);

			VkBuffer getBuffer() const;
			void* getMappedMemory() const;
			uint32_t getInstanceCount() const;
			VkDeviceSize getInstanceSize() const;
			VkDeviceSize getAlignmentSize() const;
			VkBufferUsageFlags getUsageFlags() const;
			VkMemoryPropertyFlags getMemoryPropertyFlags() const;
			VkDeviceSize getBufferSize() const;

		private:
			uint32_t instanceCount;
			VkDeviceSize instanceSize;

			VkBuffer buffer = VK_NULL_HANDLE;
			VkDeviceMemory memory = VK_NULL_HANDLE;
			void* mapped = nullptr;

			VkDeviceSize bufferSize;
			VkDeviceSize alignmentSize;
			VkBufferUsageFlags usageFlags;
			VkMemoryPropertyFlags memoryPropertyFlags;
	};
}