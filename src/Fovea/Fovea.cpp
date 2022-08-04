#include "Fovea/core.hpp"
#include "Fovea/Fovea.hpp"

#include "Fovea/SwapChain.hpp"

namespace Fovea{

	void initializeInstance(void *window){
		InstanceBuilder builder;

		builder.enableValidationLayers(true);
		getInstance().instance.initialize(builder, window);
	}

	void initializePhysicalDevice(){
		PhysicalDeviceBuidler builder;

		builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		builder.requireFeature(PhysicalDeviceFeature::WIDE_LINES);
		builder.requireFeature(PhysicalDeviceFeature::LARGE_POINTS);
		builder.requireFeature(PhysicalDeviceFeature::SHADER_FLOAT_64);

		builder.requireFamily(PhysicalDeviceFamily::FAMILY_GRAPHIC);
		builder.requireFamily(PhysicalDeviceFamily::FAMILY_PRESENT);
		getInstance().physicalDevice.initialize(builder);
	}

	void initializeLogicalDevice(){
		LogicalDeviceBuilder builder;
		builder.requireQueue(PhysicalDeviceFamily::FAMILY_GRAPHIC);
		builder.requireQueue(PhysicalDeviceFamily::FAMILY_PRESENT);
		builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		builder.setQueuePriority(PhysicalDeviceFamily::FAMILY_GRAPHIC, 1);
		getInstance().logicalDevice.initialize(builder);
	}

	void initializeRenderCommandPool(){
		CommandPoolBuilder builder;

		builder.setFamily(PhysicalDeviceFamily::FAMILY_GRAPHIC);
		builder.setFlag(CommandPoolFlags::FLAG_TRANSIENT | CommandPoolFlags::FLAG_RESET_BUFFER);
		getInstance().renderCommandPool.initialize(builder);
	}

	void initializeRenderer(){
		Renderer &renderer = getInstance().renderer;

		renderer.initialize();

		renderer.setClearColor(0.1, 0.1, 0.1, 1.0);
	}

	void initialize(void* window){
		initializeInstance(window);
		initializePhysicalDevice();
		initializeLogicalDevice();
		initializeRenderCommandPool();
		initializeRenderer();
	}

	void setClearColor(const ColorRGBA &color){
		getInstance().renderer.setClearColor(color.r, color.g, color.b, color.a);
	}

	void setClearColor(const ColorRGB &color){
		getInstance().renderer.setClearColor(color.r, color.g, color.b, 1.f);
	}

	void onWindowResized(uint32_t width, uint32_t height){
		getInstance().renderer.windowResized(width, height);
	}

	VkCommandBuffer& frameCommandBuffer(){
		static VkCommandBuffer commandBuffer;
		return commandBuffer;
	}

	void beginFrame(){
		frameCommandBuffer() = getInstance().renderer.beginFrame();
	}

	void endFrame(){
		Renderer &renderer = getInstance().renderer;
		{
			VkCommandBuffer& commandBuffer = frameCommandBuffer();
			renderer.beginSwapChainRenderPass(commandBuffer);

			renderer.endSwapChainRenderPass(commandBuffer);
		}
		
		renderer.endFrame();
	}
}