#pragma once
#include "builders/RenderTargetBuilder.hpp"
#include "Framebuffer.hpp"

#include "../vulkan/vulkan.h"

namespace Fovea{
	class RenderTarget{
		public:
			using ClearColor = RenderTargetBuilder::ClearColor;

			RenderTarget() = default;
			RenderTarget(RenderTargetBuilder &builder){
				initialize(builder);
			}

			~RenderTarget();

			void initialize(RenderTargetBuilder &builder);
			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);
			VkRenderPass getRenderPass();

			void resize(VkExtent2D extent);
		
		private:
			void createRenderPass(RenderTargetBuilder &builder);

			std::vector<VkClearValue> clearValues;
			bool customFramebuffer = false;
			Framebuffer* framebuffer = nullptr;

			VkRenderPass renderPass = nullptr;
			VkViewport viewport;
			VkRect2D scissor;
	};
}