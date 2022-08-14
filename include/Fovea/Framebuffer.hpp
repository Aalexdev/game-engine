#pragma once

#include "builders/FramebufferBuilder.hpp"

namespace Fovea{
	class Framebuffer{
		friend class RenderTarget;
		public:
			Framebuffer() = default;
			Framebuffer(FramebufferBuilder &builder);
			~Framebuffer();

			void initialize(FramebufferBuilder &builder);
			void destroy();

			VkImage getAttachmentImage(uint8_t attachmentIndex);
			VkImageView getAttachmentImageView(uint8_t attachmentIndex);
			VkFormat getAttachmentFormat(uint8_t attachmentIndex);
			VkImage getDepthAttachmentImage();
			VkDeviceMemory getDepthAttachmentMemory();
			VkImageView getDepthAttachmentImageView();
			VkFormat getDepthAttachmentFormat();
			VkFramebuffer getFramebuffer();
			VkExtent2D getExtent();

			void resize(VkExtent2D extent);
		
		private:
			VkImage createColorAttachmentImage(FramebufferBuilder &builder, FramebufferAttachments::Attachment &attachment, VkDeviceMemory *imageMemory);
			VkImageView createColorAttachmentImageView(FramebufferBuilder &builder, uint8_t attachmentIndex);

			VkImage createDepthAttachmentImage(FramebufferBuilder &builder);
			VkImageView createDepthAttachmentImageView(FramebufferBuilder &builder);

			VkRenderPass createRenderPass(FramebufferBuilder &builder);

			VkFramebuffer createFramebuffer(FramebufferBuilder &builder);

			template<typename T>
			struct CustomableMember{
				CustomableMember<T>(const T &other) {member = other;}
				CustomableMember<T>& operator=(const T &other) {member = other; return *this;}
				operator T&() {return member;}
				T& operator*() {return member;}
				T* operator->() {return &member;}

				T member;
				bool custom = false;
			};

			struct ColorAttachment{
				VkFormat format;
				VkSampleCountFlagBits samples;
				VkImageTiling tiling;
				CustomableMember<VkImage> image = VK_NULL_HANDLE;
				CustomableMember<VkImageView> imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory;
			};

			CustomableMember<VkRenderPass> renderPass = VK_NULL_HANDLE;
			VkFramebuffer framebuffer;
			VkExtent2D extent;

			std::vector<ColorAttachment> colorAttachments;

			bool depthAttachmentEnabled = false;
			VkFormat depthFormat;
			CustomableMember<VkImage> depthImage = VK_NULL_HANDLE;
			CustomableMember<VkImageView> depthImageView = VK_NULL_HANDLE;
			VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
	};
}