#pragma once

#include "SwapChain.hpp"
#include "Buffer.hpp"
#include "../vulkan/vulkan.h"

#include <memory>
#include <vector>
#include <cassert>

namespace Fovea{
	class Renderer{
		public:
			~Renderer();

			void initialize(size_t vertexBufferSize = 5000);

			VkRenderPass getSwapChainRenderPass() const {return swapChain->getRenderPass();}

			bool isFrameInProgress() const {return isFrameStarted;}

			VkCommandBuffer getCurrentCommandBuffer() const {
				assert(isFrameStarted && "Cannot get command buffer whene frame not in progress");
				return commandBuffers[currentFrameIndex];
			};

			int getFrameIndex() const {
				assert(isFrameStarted && "Cannot get frame index whene frame not in progress");
				return currentFrameIndex;
			}

			SwapChain &getSwapChain() noexcept {return *swapChain;}

			VkCommandBuffer beginFrame();

			void endFrame();

			void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f);

			void setViewPortPosition(const float &x = 0.f, const float &y = 0.f) {viewport.x = x; viewport.y = y;}

			void setViewPortSize(const float &width, const float &height) {viewport.width = width; viewport.height = height;}

			void windowResized(uint32_t width, uint32_t height);

			void resizeVertexBuffer(size_t size);

			void setInstanceSize(size_t size, size_t minAlignementOffset);

			void drawQuad(void *v0, void *v1, void *v2, void *v3);

			void flush();
			
		private:
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void createVertexBuffer(uint32_t count);
			void createIndexBuffer(uint32_t count);
			void copyStaginBuffers();

			std::unique_ptr<SwapChain> swapChain = nullptr;
			std::vector<VkCommandBuffer> commandBuffers;

			uint32_t currentImageIndex = 0;
			int currentFrameIndex = 0;
			bool isFrameStarted = false;
			bool isWindowResized = false;

			VkClearColorValue clearColor = {0.f, 0.f, 0.f, 0.f};
			VkViewport viewport;
			VkRect2D scissor;
			VkExtent2D windowExtent = {1080, 720};

			Buffer staginVertexBuffer;
			Buffer vertexBuffer;
			Buffer indexBuffer;

			size_t alignementSize = 10;
			size_t instanceSize = 10;
			uint32_t indexCount = 0;
			uint32_t maxIndices = 0;
			size_t usedVertexSize = 0;
			size_t maxVertexSize = 0;

			uint32_t drawCalls = 0;
	};
}