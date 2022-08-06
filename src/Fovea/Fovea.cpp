#include "Fovea/core.hpp"
#include "Fovea/Fovea.hpp"

#include "Fovea/Pipeline.hpp"

using namespace Fovea;

void initializeInstance(void *window){
	InstanceBuilder builder;

	builder.enableValidationLayers(true);
	builder.requireValidationLayer("VK_LAYER_KHRONOS_validation");

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
struct PushConstant{
	int a;
};

void FoveaInitialize(void* window){
	initializeInstance(window);
	initializePhysicalDevice();
	initializeLogicalDevice();
	initializeRenderCommandPool();
	initializeRenderer();
}

void FoveaSetClearColor(const FoveaColor &color){
	getInstance().renderer.setClearColor(color.r, color.g, color.b, color.a);
}

void FoveaOnWindowResized(uint32_t width, uint32_t height){
	getInstance().renderer.windowResized(width, height);
}

VkCommandBuffer& frameCommandBuffer(){
	static VkCommandBuffer commandBuffer;
	return commandBuffer;
}

void FoveaBeginFrame(){
	frameCommandBuffer() = getInstance().renderer.beginFrame();
}

void FoveaEndFrame(){
	Renderer &renderer = getInstance().renderer;
	{
		VkCommandBuffer& commandBuffer = frameCommandBuffer();
		renderer.beginSwapChainRenderPass(commandBuffer);

		auto pipeline = getInstance().pipelineLibrary.get(0);
		pipeline->bind(frameCommandBuffer());

		vkCmdDraw(frameCommandBuffer(), 3, 1, 0, 0);

		renderer.endSwapChainRenderPass(commandBuffer);
	}

	renderer.endFrame();
}

void FoveaDefaultCreateInfo(FoveaShaderCreateInfo *createInfo){
	createInfo->sample = FoveaShaderSample_1;
}

VkSampleCountFlagBits FoveaSampleToVkSample(FoveaShaderSample sample){
	switch (sample){
		case FoveaShaderSample_1: return VK_SAMPLE_COUNT_1_BIT;
		case FoveaShaderSample_2: return VK_SAMPLE_COUNT_2_BIT;
		case FoveaShaderSample_4: return VK_SAMPLE_COUNT_4_BIT;
		case FoveaShaderSample_8: return VK_SAMPLE_COUNT_8_BIT;
		case FoveaShaderSample_16: return VK_SAMPLE_COUNT_16_BIT;
		case FoveaShaderSample_32: return VK_SAMPLE_COUNT_32_BIT;
		case FoveaShaderSample_64: return VK_SAMPLE_COUNT_64_BIT;
	}
	return VK_SAMPLE_COUNT_1_BIT;
}

FoveaShader FoveaCreateShader(const char *name, FoveaShaderCreateInfo *createInfo){
	PipelineBuilder builder;

	if (createInfo->vertexFilepath != nullptr) builder.setShaderStage(PipelineStage::VERTEX, createInfo->vertexFilepath);
	if (createInfo->fragmentFilepath != nullptr) builder.setShaderStage(PipelineStage::FRAGMENT, createInfo->fragmentFilepath);
	if (createInfo->geometryFilepath != nullptr) builder.setShaderStage(PipelineStage::GEOMETRY, createInfo->geometryFilepath);
	if (createInfo->computeFilepath != nullptr) builder.setShaderStage(PipelineStage::COMPUTE, createInfo->computeFilepath);
	builder->multisampleInfo.rasterizationSamples = FoveaSampleToVkSample(createInfo->sample);

	builder.setRenderPass(getInstance().renderer.getSwapChain().getRenderPass());

	return {getInstance().pipelineLibrary.push(&builder, name)};
}

void FoveaDestroyShader(FoveaShader shader){
	getInstance().pipelineLibrary.erase(shader.id);
}

FoveaShader FoveaGetShaderFromName(const char *name){
	return {getInstance().pipelineLibrary.getIDFromName(name)};
}

void FoveaUseShader(FoveaShader shader){
	auto pipeline = getInstance().pipelineLibrary.get(shader.id);
	pipeline->bind(frameCommandBuffer());

	vkCmdDraw(frameCommandBuffer(), 3, 1, 0, 0);
}