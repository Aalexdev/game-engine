#include "Fovea/builders/RenderTargetBuilder.hpp"
#include <cassert>

namespace Fovea{
	void RenderTargetBuilder::setFramebuffer(Framebuffer &framebuffer){
		fb = &framebuffer;
	}

	
	void RenderTargetBuilder::setDepthStencilClearColor(float depth, uint32_t stencil){
		depthClear = depth;
		stencilClear = stencil;
	}

	void RenderTargetBuilder::setAttachmentClearColor(uint32_t attachment, ClearColor color){
		if (clearColors.size() < framebufferBuilder.attachments.attachments.size()){
			clearColors.resize(framebufferBuilder.attachments.attachments.size());
		}

		assert(attachment < clearColors.size());
		clearColors[attachment] = color;
	}

	void RenderTargetBuilder::setFramebufferBuilder(FramebufferBuilder &builder){
		framebufferBuilder = builder;
	}

	
}