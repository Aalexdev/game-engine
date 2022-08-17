#pragma once

#include "SwapChain.hpp"
#include "Buffer.hpp"
#include "../vulkan/vulkan.h"

#include <memory>
#include <vector>
#include <cassert>
#include <queue>

namespace Fovea{
	class Renderer{
		public:
			~Renderer();

			void initialize(uint32_t sceneVertexBufferSize, uint32_t generalUsageVertexBufferSize);

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

			enum class Topology{
				Quad = 0,
				Trigone = 1,
				Line = 1
			};

			SwapChain &getSwapChain() noexcept {return *swapChain;}

			VkCommandBuffer beginFrame();

			void endFrame();

			void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f);

			void setViewPortPosition(const float &x = 0.f, const float &y = 0.f) {viewport.x = x; viewport.y = y;}

			void setViewPortSize(const float &width, const float &height) {viewport.width = width; viewport.height = height;}

			void windowResized(uint32_t width, uint32_t height);

			void setScene(void* v, uint32_t vertexCount);

			void setSceneVertexSize(uint32_t size, size_t minOffsetAlignement);

			void renderScene(VkCommandBuffer commandBuffer);

			void setSceneData(uint32_t offset, uint32_t count, void* data);

			void flushSceneData(uint32_t offset, uint32_t count);

			void setGeneralUsageVertexSize(uint32_t size, uint32_t minOffsetAlignement);

			void setGeneralUsageData(void* v, uint32_t vertexCount);

			void renderGeneralUsageData(VkCommandBuffer commandBuffer);

			void setGeneralUsageTopology(Topology typology);

			void setGeneralUsageData(uint32_t offset, uint32_t count, void* data);

			void flushGeneralUsageData(uint32_t offset, uint32_t count);
			
		private:
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void createSceneVertexBuffer(uint32_t count);
			void createSceneIndexBuffer(uint32_t count);
			void createGeneralUsageBuffers(uint32_t VbufferSize, uint32_t IBufferSize);

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

			Buffer sceneVertexBuffer;
			Buffer indexBuffer;

			uint32_t sceneVertexSize = 10;
			uint32_t sceneAlignement = 10;
			uint32_t sceneVertexBufferUsedSize = 0;
			uint32_t sceneIndexUsed = 0;
			uint32_t maxSceneVertexSize = 0;

			Buffer generalUsageVertexBuffer;
			Buffer generalUsageIndexBuffer;

			uint32_t generalUsageVertexSize = 10;
			uint32_t generalUsageAlignement = 10;
			uint32_t generalUsageVertexBufferUsedSize = 0;
			uint32_t generalUsageIndexUsed = 0;
			uint32_t maxGeneralUsageVertexSize = 0;
			Topology generalUsageTopology = Topology::Quad;
	};
}