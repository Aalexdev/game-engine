#include "Fovea/RenderTarget.hpp"
#include "Fovea/Framebuffer.hpp"
#include "Fovea/core.hpp"

#include <stdexcept>

namespace Fovea{
	RenderTarget::~RenderTarget(){
		if (!customFramebuffer){
			delete framebuffer;
		}

		vkDestroyRenderPass(getInstance().logicalDevice.getDevice(), renderPass, nullptr);
	}

	void RenderTarget::initialize(RenderTargetBuilder &builder){
		if (builder.fb != nullptr){
			framebuffer = builder.fb;
			customFramebuffer = true;
		} else {
			framebuffer = new Framebuffer(builder.framebufferBuilder);
		}

		clearValues.resize(framebuffer->colorAttachments.size() + static_cast<uint32_t>(framebuffer->depthAttachmentEnabled));

		uint32_t index = 0;
		if (builder.framebufferBuilder.depthBufferEnabled){
			clearValues[index].depthStencil = {builder.depthClear, builder.stencilClear};
		}

		for (int i=0; i<builder.clearColors.size(); i++){
			VkClearColorValue color;
			color.float32[0] = builder.clearColors[i].r;
			color.float32[1] = builder.clearColors[i].g;
			color.float32[2] = builder.clearColors[i].b;
			color.float32[3] = builder.clearColors[i].a;

			clearValues[index].color = color;
			index++;
		}

		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		viewport.width = 1.f;
		viewport.height = 1.f;

		scissor.offset = {0, 0};
		scissor.extent = {1, 1};

		createRenderPass(builder);
	}

	void RenderTarget::beginRenderPass(VkCommandBuffer commandBuffer){
		VkRenderPassBeginInfo beginInfo = {};

		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass;
		beginInfo.framebuffer = framebuffer->getFramebuffer();
		beginInfo.renderArea.offset = {0, 0};
		beginInfo.renderArea.extent = framebuffer->getExtent();

		beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		beginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		viewport.width = static_cast<float>(framebuffer->getExtent().width);
		viewport.height = static_cast<float>(framebuffer->getExtent().height);
		scissor.extent = framebuffer->getExtent();

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void RenderTarget::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}

	void RenderTarget::createRenderPass(RenderTargetBuilder &builder){
		bool depthAttachment = framebuffer->depthAttachmentEnabled;
		auto &bd = builder.framebufferBuilder;
	
		std::vector<VkAttachmentDescription> attachmentDescriptions(bd.attachments.attachments.size() + static_cast<size_t>(bd.depthBufferEnabled));
		std::vector<VkAttachmentReference> colorAttachmentsRefs(bd.attachments.attachments.size());

		VkAttachmentReference depthAttachmentRef = {};

		uint32_t attachmentIndex = 0;
		if (depthAttachment){
			auto &attachment = attachmentDescriptions[attachmentIndex];
			attachment.format = bd.depthFormat;
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			depthAttachmentRef.attachment = attachmentIndex;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			attachmentIndex++;
		}

		for (int i=0; i<bd.attachments.attachments.size(); i++){
			auto &attachment = attachmentDescriptions[attachmentIndex];
			auto &bdAttachment = bd.attachments.attachments[i];

			attachment.format = bdAttachment.format;
			attachment.samples = bdAttachment.samples;
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			colorAttachmentsRefs[i].attachment = attachmentIndex;
			colorAttachmentsRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			attachmentIndex++;
		}

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentsRefs.size());
		subpass.pColorAttachments = colorAttachmentsRefs.data();

		if (depthAttachment){
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
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &renderPass) != VK_SUCCESS){
			throw std::runtime_error("failed to create render target render pass");
		}
	}

	VkRenderPass RenderTarget::getRenderPass(){
		return renderPass;
	}

	void RenderTarget::resize(VkExtent2D extent){
		framebuffer->resize(extent);
	}

}