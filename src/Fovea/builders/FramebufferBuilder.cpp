#include "Fovea/builders/FramebufferBuilder.hpp"
#include <cassert>

namespace Fovea{
	void FramebufferBuilder::setAttachments(FramebufferAttachments attachments){
		this->attachments = attachments;
	}

	void FramebufferBuilder::addAttachmentImage(uint8_t attachmentIndex, VkImage image){
		assert(attachmentIndex < attachments.attachments.size() && "invalid attachment index");
		attachments.attachments[static_cast<size_t>(attachmentIndex)].image = image;
	}

	void FramebufferBuilder::addAttachmentImageView(uint8_t attachmentIndex, VkImageView imageView){
		assert(attachmentIndex < attachments.attachments.size() && "invalid attachment index");
		attachments.attachments[static_cast<size_t>(attachmentIndex)].imageView = imageView;
	}

	void FramebufferBuilder::addRenderPass(VkRenderPass renderPass){
		this->renderPass = renderPass;
	}

	void FramebufferBuilder::setExtent(VkExtent2D extent){
		this->extent = extent;
	}

	void FramebufferBuilder::enableDepthAttachment(VkFormat depthFormat, bool enabled){
		depthBufferEnabled = enabled;
		this->depthFormat = depthFormat;
	}

	void FramebufferBuilder::addDepthAttachmentImage(VkImage depthImage){
		this->depthImage = depthImage;
	}

	void FramebufferBuilder::addDepthAttachmentImageView(VkImageView dephtImageView){
		this->depthImageView = dephtImageView;
	}
}