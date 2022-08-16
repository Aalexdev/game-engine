#include "Fovea/SwapChain.hpp"
#include "Fovea/core.hpp"

#include <limits>
#include <stdexcept>

namespace Fovea{
	SwapChain::SwapChain(std::shared_ptr<SwapChain> previous){
		oldSwapChain = previous;
	}

	SwapChain::~SwapChain(){
		framebuffers.clear();

		vkDestroySwapchainKHR(getInstance().logicalDevice.getDevice(),swapchain, nullptr);

		vkDestroyRenderPass(getInstance().logicalDevice.getDevice(), renderPass, nullptr);

		for (size_t i=0; i<static_cast<size_t>(framesInFlight); i++) {
			vkDestroySemaphore(getInstance().logicalDevice.getDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(getInstance().logicalDevice.getDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(getInstance().logicalDevice.getDevice(), inFlightFences[i], nullptr);
		}
	}
	
	void SwapChain::create(){
		createSwapchain();
		createRenderPass();
		createFramebuffers();
		createSyncObjects();
		oldSwapChain = nullptr;
	}

	void SwapChain::initialize(SwapChainBuilder &builder){
		extent = builder.extent;
		refreshType = builder.refreshMode;
		
		framebuffers.clear();
		create();
	}

	VkFormat SwapChain::findDepthFormat(){
		return getInstance().physicalDevice.findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	void SwapChain::createSwapchain(){
		auto swapChainSupport = getInstance().physicalDevice.getSwapChainSupport();

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		framesInFlight = imageCount;
		

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = getInstance().instance.getSurface();
		
		// color buffer
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = {getInstance().physicalDevice.getFamily(PhysicalDeviceFamily::FAMILY_GRAPHIC).family, getInstance().physicalDevice.getFamily(PhysicalDeviceFamily::FAMILY_PRESENT).family};

		if (queueFamilyIndices[0] != queueFamilyIndices[1]){
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = oldSwapChain == nullptr ? VK_NULL_HANDLE : oldSwapChain->swapchain;

		if (vkCreateSwapchainKHR(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS){
			throw std::runtime_error("failed to create the swapchain");
		}

		vkGetSwapchainImagesKHR(getInstance().logicalDevice.getDevice(), swapchain, &imageCount, nullptr);
		images.resize(imageCount);
		vkGetSwapchainImagesKHR(getInstance().logicalDevice.getDevice(), swapchain, &imageCount, images.data());

		imageFormat = surfaceFormat.format;
		extent = extent;
	}

	void SwapChain::createFramebuffers(){
		depthFormat = findDepthFormat();

		framebuffers.resize(images.size());

		for (int i=0; i<images.size(); i++){
			FramebufferBuilder builder;
			builder.setExtent(extent);
			builder.addRenderPass(renderPass);

			FramebufferAttachments attachments = {
				{.format = imageFormat, .image = images[i]}
			};

			builder.enableDepthAttachment(depthFormat);
			builder.setAttachments(attachments);
			framebuffers[i].initialize(builder);
		}
	}

	void SwapChain::createSyncObjects(){
		imageAvailableSemaphores.resize(framesInFlight);
		renderFinishedSemaphores.resize(framesInFlight);
		inFlightFences.resize(framesInFlight);
		imagesInFlight.resize(images.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < static_cast<size_t>(framesInFlight); i++) {
			if (vkCreateSemaphore(getInstance().logicalDevice.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(getInstance().logicalDevice.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(getInstance().logicalDevice.getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)

				throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats){
		for (const auto &availableFormat : availableFormats) {
			if (availableFormat.format == wantedSurfaceFormat.format && availableFormat.colorSpace == wantedSurfaceFormat.colorSpace) 
				return availableFormat;
		}

		fprintf(stderr, "WARNING :: failed to found the wanted swap chain surface format (format : %d color space : %d) set as the first available format \n", static_cast<int>(wantedSurfaceFormat.format), static_cast<int>(wantedSurfaceFormat.colorSpace));

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes){
		for (const auto &presentMode : availablePresentModes){
			if (presentMode == static_cast<VkPresentModeKHR>(refreshType))
				return presentMode;
		}

		fprintf(stderr, "WARNING :: failed to found the wanted swap chain present mode : %d | set as V-sync VK_PRESENT_MODE_FIFO_KHR\n", refreshType);

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities){
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		else {
			VkExtent2D actualExtent = windowExtent;
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	VkFramebuffer SwapChain::getFrameBuffer(uint8_t index){
		return framebuffers[index].getFramebuffer();
	}

	VkImageView SwapChain::getImageView(uint8_t index){
		return framebuffers[index].getAttachmentImageView(0);
	}

	VkRenderPass SwapChain::getRenderPass(){
		return renderPass;
	}

	uint8_t SwapChain::getImageCount(){
		return images.size();
	}

	VkFormat SwapChain::getImageFormat(){
		return imageFormat;
	}
	
	VkExtent2D SwapChain::getExtent(){
		return extent;
	}

	uint8_t SwapChain::getFrameInFlightCount(){
		return framesInFlight;
	}

	VkResult SwapChain::acquireNextImage(uint32_t *imageIndex){
		LogicalDevice &device = getInstance().logicalDevice;
		vkWaitForFences(device.getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		VkResult result = vkAcquireNextImageKHR(device.getDevice(), swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);
		return result;
	}

	VkResult SwapChain::submitCommandBuffer(VkCommandBuffer* buffers, uint32_t *imageIndex){
		LogicalDevice &device = getInstance().logicalDevice;
		
		if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(device.getDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
		
		imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffers;

		VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device.getDevice(), 1, &inFlightFences[currentFrame]);
		if (vkQueueSubmit(device.getQueue(PhysicalDeviceFamily::FAMILY_GRAPHIC), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {swapchain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = imageIndex;

		auto result = vkQueuePresentKHR(device.getQueue(PhysicalDeviceFamily::FAMILY_PRESENT), &presentInfo);

		currentFrame = (currentFrame + 1) % framesInFlight;

		return result;
	}

	bool SwapChain::compareFormats(const SwapChain& swapChain){
		return swapChain.depthFormat == depthFormat && swapChain.imageFormat == imageFormat;
	}

	SwapChainRefreshMode SwapChain::getRefreshMode(){
		return refreshType;
	} 

	VkImage SwapChain::getCurrentImage(){
		return framebuffers[currentFrame].getAttachmentImage(0);
	}

	void SwapChain::createRenderPass(){
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 1;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments = {depthAttachment, colorAttachment};

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(getInstance().logicalDevice.getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
			throw std::runtime_error("failed to create render pass");	
	}
}