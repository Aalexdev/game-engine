#pragma once

#include "FramebufferBuilder.hpp"
#include "../Framebuffer.hpp"

#include <vector>

namespace Fovea{
	class RenderTargetBuilder{
		friend class RenderTarget;
		public:
			struct ClearColor{
				float r=0.f, g=0.f, b=0.f, a=0.f;
			};

			FramebufferBuilder* operator->() {return &framebufferBuilder;}
			FramebufferBuilder& operator*() {return framebufferBuilder;}

			void setFramebufferBuilder(FramebufferBuilder &builder);
			void setFramebuffer(Framebuffer &framebuffer);

			void setDepthStencilClearColor(float depth, uint32_t stencil);
			void setAttachmentClearColor(uint32_t attachment, ClearColor color);

		private:
			std::vector<ClearColor> clearColors;
			float depthClear = 1.f;
			uint32_t stencilClear = 0;

			FramebufferBuilder framebufferBuilder;
			Framebuffer* fb = nullptr;
	};
}