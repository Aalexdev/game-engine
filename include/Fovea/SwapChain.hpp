#pragma once

#include "builders/SwapChainBuilder.hpp"
#include "Framebuffer.hpp"
#include <memory>

namespace Fovea{
	class SwapChain{
		public:
			~SwapChain();
			SwapChain() = default;
			SwapChain(std::shared_ptr<SwapChain> previous);
			void initialize(SwapChainBuilder &builder);

			SwapChain(const SwapChain &) = delete;
			SwapChain& operator=(const SwapChain &) = delete;

			VkFormat findDepthFormat();
			VkFramebuffer getFrameBuffer(uint8_t index);
			VkImageView getImageView(uint8_t index);
			VkRenderPass getRenderPass();
			uint8_t getImageCount();
			VkFormat getImageFormat();
			VkExtent2D getExtent();
			uint8_t getFrameInFlightCount();

			VkResult acquireNextImage(uint32_t *imageIndex);
			VkResult submitCommandBuffer(VkCommandBuffer* buffers, uint32_t *imageIndex);

			bool compareFormats(const SwapChain& swapChain);

			SwapChainRefreshMode getRefreshMode(); 
			VkImage getCurrentImage();

		
		private:
			void create();
			void createSwapchain();
			void createFramebuffers();
			void createSyncObjects();
			void createRenderPass();

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);


			VkSwapchainKHR swapchain = VK_NULL_HANDLE;
			std::shared_ptr<SwapChain> oldSwapChain = nullptr;
			VkExtent2D windowExtent;
			std::vector<Framebuffer> framebuffers;

			std::vector<VkImage> images;
			std::vector<VkSemaphore> imageAvailableSemaphores;
			std::vector<VkSemaphore> renderFinishedSemaphores;
			std::vector<VkFence> inFlightFences;
			std::vector<VkFence> imagesInFlight;
			size_t currentFrame = 0;
			size_t framesInFlight;

			VkRenderPass renderPass = VK_NULL_HANDLE;

			SwapChainRefreshMode refreshType = SwapChainRefreshMode::FIFO_MODE;
			VkSurfaceFormatKHR wantedSurfaceFormat = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

			VkFormat imageFormat;
			VkFormat depthFormat;
			VkExtent2D extent;
	};
}