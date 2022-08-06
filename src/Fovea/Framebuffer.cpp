#include "Fovea/Framebuffer.hpp"
#include "Fovea/core.hpp"

#include <stdexcept>
#include <cassert>

namespace Fovea{
	Framebuffer::Framebuffer(FramebufferBuilder &builder){
		initialize(builder);
	}

	Framebuffer::~Framebuffer(){
		destroy();
	}

	void Framebuffer::destroy(){
		VkDevice device = getInstance().logicalDevice.getDevice();
		for (auto &color : colorAttachments){
			if (!color.image.custom){
				vkDestroyImage(device, color.image, nullptr);
				vkFreeMemory(device, color.imageMemory, nullptr);
			}

			if (!color.imageView.custom){
				vkDestroyImageView(device, color.imageView, nullptr);
			}
		}

		if (depthAttachmentEnabled){
			if (!depthImage.custom){
				vkDestroyImage(device, depthImage, nullptr);
			}

			vkFreeMemory(device, depthImageMemory, nullptr);

			if (!depthImageView.custom){
				vkDestroyImageView(device, depthImageView, nullptr);
			}
		}

		if (!renderPass.custom){
			vkDestroyRenderPass(device, renderPass, nullptr);
		}

		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	void Framebuffer::initialize(FramebufferBuilder &builder){
		colorAttachments.resize(builder.attachments.attachments.size());
		extent = builder.extent;

		for (int i=0; i<static_cast<int>(colorAttachments.size()); i++){
			auto &build = builder.attachments.attachments[i];
			auto &attachment = colorAttachments[i];

			attachment.format = build.format;

			if (build.image == VK_NULL_HANDLE){
				attachment.image = createColorAttachmentImage(builder, build, &attachment.imageMemory);
			} else {
				attachment.image.custom = true;
				attachment.image = build.image;
			}

			if (build.imageView == VK_NULL_HANDLE){
				attachment.imageView = createColorAttachmentImageView(builder, i);
			} else {
				attachment.imageView.custom = true;
				attachment.imageView = build.imageView;
			}
		}

		depthAttachmentEnabled = builder.depthBufferEnabled;

		if (depthAttachmentEnabled){
			depthFormat = builder.depthFormat;

			if (builder.depthImage == VK_NULL_HANDLE){
				depthImage = createDepthAttachmentImage(builder);
			} else {
				depthImage.custom = true;
				depthImage = builder.depthImage;
			}

			if (builder.depthImageView == VK_NULL_HANDLE){
				depthImageView = createDepthAttachmentImageView(builder);
			} else {
				depthImageView.custom = true;
				depthImageView = builder.depthImageView;
			}

		}

		if (builder.renderPass == VK_NULL_HANDLE){
			renderPass = createRenderPass(builder);
		} else {
			renderPass.custom = true;
			renderPass = builder.renderPass;
		}

		framebuffer = createFramebuffer(builder);
	}

	VkImage Framebuffer::createColorAttachmentImage(FramebufferBuilder &builder, FramebufferAttachments::Attachment &attachment, VkDeviceMemory *imageMemory){
		VkImageCreateInfo createInfo = {};

		uint32_t queueFamily = getInstance().physicalDevice.getFamily(PhysicalDeviceFamily::FAMILY_GRAPHIC).family;

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

		createInfo.extent.width = builder.extent.width;
		createInfo.extent.height = builder.extent.height;
		createInfo.extent.depth = 1;

		createInfo.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = attachment.format;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = attachment.samples;
		createInfo.tiling = attachment.tiling;
		createInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.pQueueFamilyIndices = &queueFamily;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage image = VK_NULL_HANDLE;
		getInstance().logicalDevice.createImageWithInfo(createInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, *imageMemory);
		return image;
	}

	VkImageView Framebuffer::createColorAttachmentImageView(FramebufferBuilder &builder, uint8_t attachmentIndex){
		auto &attachment = colorAttachments[static_cast<size_t>(attachmentIndex)];

		VkImageSubresourceRange subResourceRange = {};
		subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subResourceRange.baseMipLevel = 0;
		subResourceRange.levelCount = 1;
		subResourceRange.baseArrayLayer = 0;
		subResourceRange.layerCount = 1;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.format = attachment.format;
		createInfo.image = attachment.image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.subresourceRange = subResourceRange;

		VkImageView imageView = VK_NULL_HANDLE;
		if (vkCreateImageView(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &imageView) != VK_SUCCESS){
			throw std::runtime_error("failed to create color attachment image view");
		}

		return imageView;
	}

	VkImage Framebuffer::createDepthAttachmentImage(FramebufferBuilder &builder){
		uint32_t queueFamily = getInstance().physicalDevice.getFamily(PhysicalDeviceFamily::FAMILY_GRAPHIC).family;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

		createInfo.extent.width = builder.extent.width;
		createInfo.extent.height = builder.extent.height;
		createInfo.extent.depth = 1;

		createInfo.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = depthFormat;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.pQueueFamilyIndices = &queueFamily;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage image = VK_NULL_HANDLE;
		getInstance().logicalDevice.createImageWithInfo(createInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, depthImageMemory);

		return image;
	}

	VkImageView Framebuffer::createDepthAttachmentImageView(FramebufferBuilder &builder){
		VkImageSubresourceRange subResourceRange = {};
		subResourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		subResourceRange.baseMipLevel = 0;
		subResourceRange.levelCount = 1;
		subResourceRange.baseArrayLayer = 0;
		subResourceRange.layerCount = 1;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = depthImage;
		createInfo.format = depthFormat;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.subresourceRange = subResourceRange;

		VkImageView imageView = VK_NULL_HANDLE;
		if (vkCreateImageView(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &imageView) != VK_SUCCESS){
			throw std::runtime_error("failed to create depth attachment image view");
		}

		return imageView;
	}

	VkRenderPass Framebuffer::createRenderPass(FramebufferBuilder &builder){
		size_t attachmentCount = colorAttachments.size() + static_cast<size_t>(depthAttachmentEnabled);
		std::vector<VkAttachmentDescription> attachmentDescriptions(attachmentCount);

		uint32_t attachmentIndex = 0;
		if (depthAttachmentEnabled){
			auto &attachment = attachmentDescriptions[attachmentIndex];
			attachment.format = depthFormat;
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			attachmentIndex++;
		}

		for (int i=0; i<static_cast<int>(colorAttachments.size()); i++){
			auto &description = attachmentDescriptions[attachmentIndex];

			description.format = colorAttachments[i].format;
			description.samples = builder.attachments.attachments[i].samples;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			attachmentIndex++;
		}

		attachmentIndex = 0;
		VkAttachmentReference depthAttachmentRef;
		std::vector<VkAttachmentReference> colorAttachmentRefs(colorAttachments.size());

		if (depthAttachmentEnabled){
			depthAttachmentRef.attachment = attachmentIndex;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			attachmentIndex++;
		}

		for (int i=0; i<static_cast<int>(colorAttachments.size()); i++){
			auto &ref = colorAttachmentRefs[i];
			ref.attachment = attachmentIndex;
			ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			attachmentIndex++;
		}

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		
		if (depthAttachmentEnabled){
			subpass.pDepthStencilAttachment = &depthAttachmentRef;
		} else {
			subpass.pDepthStencilAttachment = nullptr;
		}

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
		createInfo.pAttachments = attachmentDescriptions.data();
		createInfo.pSubpasses = &subpass;
		createInfo.subpassCount = 1;
		createInfo.pDependencies = &dependency;
		createInfo.dependencyCount = 1;
		
		VkRenderPass renderPass = VK_NULL_HANDLE;
		if (vkCreateRenderPass(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &renderPass) != VK_SUCCESS){
			throw std::runtime_error("failed to create render pass");
		}

		return renderPass;
	}

	VkFramebuffer Framebuffer::createFramebuffer(FramebufferBuilder &builder){
		std::vector<VkImageView> attachments(static_cast<uint32_t>(colorAttachments.size()) + static_cast<uint32_t>(depthAttachmentEnabled));

		{
			uint32_t attachmentIndex = 0;
			if (depthAttachmentEnabled){
				attachments[attachmentIndex] = depthImageView;
				attachmentIndex++;
			}

			for (auto &a : colorAttachments){
				attachments[attachmentIndex] = a.imageView;
				attachmentIndex++;
			}
		}

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		createInfo.width = extent.width;
		createInfo.height = extent.height;
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		
		VkFramebuffer framebuffer = VK_NULL_HANDLE;
		if (vkCreateFramebuffer(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &framebuffer) != VK_SUCCESS){
			throw std::runtime_error("failed to create framebuffer");
		}
		
		return framebuffer;
	}

	VkImage Framebuffer::getAttachmentImage(uint8_t attachmentIndex){
		assert(attachmentIndex < colorAttachments.size());
		return colorAttachments[attachmentIndex].image;
	}

	VkImage Framebuffer::getDepthAttachmentImage(){
		return depthImage;
	}

	VkDeviceMemory Framebuffer::getDepthAttachmentMemory(){
		return depthImageMemory;
	}

	VkFramebuffer Framebuffer::getFramebuffer(){
		return framebuffer;
	}

	VkImageView Framebuffer::getAttachmentImageView(uint8_t attachmentIndex){
		assert(attachmentIndex < colorAttachments.size());
		return colorAttachments[attachmentIndex].imageView;
	}

	VkFormat Framebuffer::getAttachmentFormat(uint8_t attachmentIndex){
		assert(attachmentIndex < colorAttachments.size());
		return colorAttachments[attachmentIndex].format;
	}

	VkImageView Framebuffer::getDepthAttachmentImageView(){
		return depthImageView;
	}

	VkFormat Framebuffer::getDepthAttachmentFormat(){
		return depthFormat;
	}

	VkExtent2D Framebuffer::getExtent(){
		return extent;
	}

}