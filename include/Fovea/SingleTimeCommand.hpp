#pragma once

#include "../vulkan/vulkan.h"

namespace Fovea{
	class SingleTimeCommand{
		public:
			SingleTimeCommand();
			~SingleTimeCommand();

			static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
			static void copyBuffer(VkBuffer src, VkBuffer dst, VkBufferCopy copy);

			operator VkCommandBuffer() const {return commandBuffer;}
		private:
			void begin();
			void end();

			VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	};
}