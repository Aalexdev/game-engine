#include "Fovea/builders/SwapChainBuilder.hpp"
#include <cassert>

namespace Fovea{
	void SwapChainBuilder::setRefreshMode(SwapChainRefreshMode refreshMode){
		this->refreshMode = refreshMode;
	}

	void SwapChainBuilder::setSurfaceFormat(VkSurfaceFormatKHR format){
		this->format = format;
	}

	void SwapChainBuilder::setSwapchainBufferCount(uint8_t count){
		assert(count >= 2 && "cannot set less than 2 frame buffers for the swapChain");
		frameBufferCount = count;
	}

	void SwapChainBuilder::setExtent(VkExtent2D extent){
		this->extent = extent;
	}

}