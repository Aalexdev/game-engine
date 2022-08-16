#pragma once

#include "../../vulkan/vulkan.h"

namespace Fovea{
	enum class SwapChainRefreshMode{
		MAILBOX_MODE,
		IMMEDIATE_MODE,
		FIFO_MODE,
		VSync = FIFO_MODE,
	};

	class SwapChainBuilder{
		friend class SwapChain;
		public:
			void setRefreshMode(SwapChainRefreshMode refreshMode);
			void setSurfaceFormat(VkSurfaceFormatKHR format);

			// ! cannot be less than 2 buffers !
			void setSwapchainBufferCount(uint8_t count); 

			void setExtent(VkExtent2D extent);

		
		private:
			SwapChainRefreshMode refreshMode = SwapChainRefreshMode::FIFO_MODE; // V-Sync
			VkSurfaceFormatKHR format;
			VkExtent2D extent;
			uint8_t frameBufferCount = 2;
	};
}