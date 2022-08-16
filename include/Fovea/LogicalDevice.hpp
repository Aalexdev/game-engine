#pragma once

#include "builders/LogicalDeviceBuilder.hpp"
#include "../vulkan/vulkan.h"

#include <array>

namespace Fovea{
	class LogicalDevice{
		public:
			~LogicalDevice();

			void initialize(LogicalDeviceBuilder &builder);

			VkDevice getDevice() const;

			void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory);

			VkQueue getQueue(PhysicalDeviceFamily family);

		private:
			VkDevice device;
			std::array<VkQueue, static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> queues;
	};
}