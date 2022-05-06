#pragma once

#include "../pch.hpp"
#include "../core.hpp"
#include "../renderer/Framebuffer.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlFramebuffer : public Framebuffer{
		public:
			OpenGlFramebuffer(const FramebufferLayout &layout);
			~OpenGlFramebuffer();

			virtual void bind() override;
			virtual void unBind() override;

			virtual void rebuild() override;
			virtual void resize(uint32_t width, uint32_t height) override;

			// return the color attachment at the given slot index, if non existent, return 0
			virtual uint32_t getColorAttachment(uint32_t slot) const override;
			virtual uint32_t getDepthAttachment() const override {return depthAttachment;}

			virtual uint32_t getWidth() const override {return width;}
			virtual uint32_t getHeight() const override {return height;}

			virtual void getPixel(uint32_t attachment, uint32_t x, uint32_t y, void* pixel) override;
			virtual void clear(uint32_t attachment, void* clearData) override;

			virtual bool bound() const override {return isBind;}
		
		private:
			void destroyAttachments();

			uint32_t width;
			uint32_t height;

			uint32_t framebuffer = 0;

			std::vector<FramebufferTextureSpecification> colorAttachmentsSpecs;
			FramebufferTextureSpecification depthAttachmentSpecs = FramebufferTextureFormat::NONE;
			
			std::vector<uint32_t> colorAttachments;
			uint32_t depthAttachment = 0;

			uint16_t samples = 1;
			bool isBind = false;
	};
}