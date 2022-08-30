#include "Fovea/core.hpp"
#include "Fovea/Fovea.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <pthread.h>

using namespace Fovea;

static inline VkFormat FoveaFormatToVkFormat(FoveaFormat format){
	switch (format){
		case FoveaFormat_Float: return VK_FORMAT_R32_SFLOAT;
		case FoveaFormat_Float_vec2: return VK_FORMAT_R32G32_SFLOAT;
		case FoveaFormat_Float_vec3: return VK_FORMAT_R32G32B32_SFLOAT;
		case FoveaFormat_Float_vec4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case FoveaFormat_Int: return VK_FORMAT_R32_SINT;
		case FoveaFormat_Int_vec2: return VK_FORMAT_R32G32_SINT;
		case FoveaFormat_Int_vec3: return VK_FORMAT_R32G32B32_SINT;
		case FoveaFormat_Int_vec4: return VK_FORMAT_R32G32B32A32_SINT;
		case FoveaFormat_Uint: return VK_FORMAT_R32_UINT;
		case FoveaFormat_Uint_vec2: return VK_FORMAT_R32G32_UINT;
		case FoveaFormat_Uint_vec3: return VK_FORMAT_R32G32B32_UINT;
		case FoveaFormat_Uint_vec4: return VK_FORMAT_R32G32B32A32_UINT;
	}
	return VK_FORMAT_R32_SFLOAT;
}

static inline VkFormat FoveaImageFormatToVkFormat(FoveaImageFormat format){
	switch (format){
		case FoveaImageFormat_R8: return VK_FORMAT_R8_SRGB;
		case FoveaImageFormat_R8G8: return VK_FORMAT_R8G8_SRGB;
		case FoveaImageFormat_R8G8B8: return VK_FORMAT_R8G8B8_SRGB;
		case FoveaImageFormat_R8G8B8A8: return VK_FORMAT_R8G8B8A8_SRGB;
		case FoveaImageFormat_R16: return VK_FORMAT_R16_SFLOAT;
		case FoveaImageFormat_R16G16: return VK_FORMAT_R16G16_SFLOAT;
		case FoveaImageFormat_R16G16B16: return VK_FORMAT_R16G16B16_SFLOAT;
		case FoveaImageFormat_R16G16B16A16: return VK_FORMAT_R16G16B16A16_SFLOAT;
		case FoveaImageFormat_R32: return VK_FORMAT_R32_SFLOAT;
		case FoveaImageFormat_R32G32: return VK_FORMAT_R32G32_SFLOAT;
		case FoveaImageFormat_R32G32B32: return VK_FORMAT_R32G32B32_SFLOAT;
		case FoveaImageFormat_R32G32B32A32: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case FoveaImageFormat_R64: return VK_FORMAT_R64_SFLOAT;
		case FoveaImageFormat_R64G64: return VK_FORMAT_R64G64_SFLOAT;
		case FoveaImageFormat_R64G64B64: return VK_FORMAT_R64G64B64_SFLOAT;
		case FoveaImageFormat_R64G64B64A64: return VK_FORMAT_R64G64B64A64_SFLOAT;
		case FoveaImageFormat_R4G4: return VK_FORMAT_R4G4_UNORM_PACK8;
		case FoveaImageFormat_R4G4B4A4: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
		case FoveaImageFormat_R5G5B5A1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
		case FoveaImageFormat_R5B6G5: return VK_FORMAT_R5G6B5_UNORM_PACK16;
		case FoveaImageFormat_D32: return VK_FORMAT_D32_SFLOAT;
		case FoveaImageFormat_D32_S8: return VK_FORMAT_D32_SFLOAT_S8_UINT;
		case FoveaImageFormat_D24_S8: return VK_FORMAT_D24_UNORM_S8_UINT;
	}
	return VK_FORMAT_R8G8B8_UINT;
}

static inline uint32_t FoveaImageFormatToPixelSize(FoveaImageFormat format){
	switch (format){
		case FoveaImageFormat_R8: return 1;
		case FoveaImageFormat_R8G8: return 2;
		case FoveaImageFormat_R8G8B8: return 3;
		case FoveaImageFormat_R8G8B8A8: return 4;
		case FoveaImageFormat_R16: return 2;
		case FoveaImageFormat_R16G16: return 4;
		case FoveaImageFormat_R16G16B16: return 6;
		case FoveaImageFormat_R16G16B16A16: return 8;
		case FoveaImageFormat_R32: return 4;
		case FoveaImageFormat_R32G32: return 8;
		case FoveaImageFormat_R32G32B32: return 16;
		case FoveaImageFormat_R32G32B32A32: return 32;
		case FoveaImageFormat_R64: return 8;
		case FoveaImageFormat_R64G64: return 16;
		case FoveaImageFormat_R64G64B64: return 32;
		case FoveaImageFormat_R64G64B64A64: return 64;
		case FoveaImageFormat_R4G4: return 1;
		case FoveaImageFormat_R4G4B4A4: return 2;
		case FoveaImageFormat_R5G5B5A1: return 2;
		case FoveaImageFormat_R5B6G5: return 2;
		case FoveaImageFormat_D32: return 4;
		case FoveaImageFormat_D32_S8: return 5;
		case FoveaImageFormat_D24_S8: return 4;
	}
	return 0;
}

static inline VkImageTiling FoveaImageTilingToVulkanImageTiling(FoveaImageTiling tiling){
	switch (tiling){
		case FoveaImageTiling_Linear: return VK_IMAGE_TILING_LINEAR;
		case FoveaImageTiling_Nearest: return VK_IMAGE_TILING_OPTIMAL;
	}
	return VK_IMAGE_TILING_LINEAR;
}

static inline VkFilter FoveaImageTilingToVulkanFilter(FoveaImageTiling tiling){
	switch (tiling){
		case FoveaImageTiling_Linear: return VK_FILTER_LINEAR;
		case FoveaImageTiling_Nearest: return VK_FILTER_NEAREST;
	}
	return VK_FILTER_LINEAR;
}

static inline VkSampleCountFlagBits FoveaSampleToVkSample(FoveaSample sample){
	switch (sample){
		case FoveaSample_1: return VK_SAMPLE_COUNT_1_BIT;
		case FoveaSample_2: return VK_SAMPLE_COUNT_2_BIT;
		case FoveaSample_4: return VK_SAMPLE_COUNT_4_BIT;
		case FoveaSample_8: return VK_SAMPLE_COUNT_8_BIT;
		case FoveaSample_16: return VK_SAMPLE_COUNT_16_BIT;
		case FoveaSample_32: return VK_SAMPLE_COUNT_32_BIT;
		case FoveaSample_64: return VK_SAMPLE_COUNT_64_BIT;
	}
	return VK_SAMPLE_COUNT_1_BIT;
}

static inline int FoveaShaderTypeToPipelineStage(FoveaShaderType type){
	switch (type){
		case FoveaShaderType_Graphic: return VK_SHADER_STAGE_ALL_GRAPHICS;
		case FoveaShaderType_Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
	}
	return 0;
}

static inline VkDescriptorType FoveaDescriptorTypeToVkDescriptorType(FoveaDescriptorType type){
	switch (type){
		case FoveaDescriptorType_Buffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case FoveaDescriptorType_Texture: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	}
	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}

static inline VkSamplerAddressMode FoveaTextureAddressModeToVkSamplerAddressMode(FoveaTextureAddressMode mode){
	switch (mode){
		case FoveaTextureAddressMode_ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case FoveaTextureAddressMode_ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case FoveaTextureAddressMode_MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case FoveaTextureAddressMode_Repeat:  return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
}

static inline VkPipelineStageFlags FoveaShaderStagePipelineStageFlags(int stages){
	VkPipelineStageFlags vkStage = 0;
	if (stages & FoveaShaderStage_Compute) vkStage |= VK_SHADER_STAGE_COMPUTE_BIT;
	if (stages & FoveaShaderStage_Fragment) vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
	if (stages & FoveaShaderStage_Geometry) vkStage |= VK_SHADER_STAGE_GEOMETRY_BIT;
	if (stages & FoveaShaderStage_Vertex) vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
	return vkStage;
}

static inline Renderer::Topology FoveaTopologyToRendererTopology(FoveaTopology topology){
	switch (topology){
		case FoveaTopology_Quad: return Renderer::Topology::Quad;
		case FoveaTopology_Trigone: return Renderer::Topology::Trigone;
		case FoveaTopology_Line: return Renderer::Topology::Line;
	}
	return Renderer::Topology::Quad;
}

static inline VkPolygonMode FoveaPolygonModeToVkPolygonMode(FoveaPolygonMode mode){
	switch (mode){
		case FoveaPolygonMode_Fill: return VK_POLYGON_MODE_FILL;
		case FoveaPolygonMode_Line: return VK_POLYGON_MODE_LINE;
		case FoveaPolygonMode_Point: return VK_POLYGON_MODE_POINT;
	}
	return VK_POLYGON_MODE_FILL;
}

static inline RenderTarget::ClearColor FoveaColorToRenderTargetClearColor(FoveaColor color){
	return {color.r, color.g, color.b, color.a};
}

static inline VkExtent2D FoveaUIVec2ToVkExtent(FoveaUIVec2 vec){
	return {vec.width, vec.height};
}

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
	renderer.initialize(50000, 5000);
	renderer.setClearColor(0.1, 0.1, 0.1, 1.0);
}

void FoveaInitialize(void* window){
	initializeInstance(window);
	initializePhysicalDevice();
	initializeLogicalDevice();
	initializeRenderCommandPool();
	initializeRenderer();
}

void FoveaSetClearColor(const FoveaColor color){
	getInstance().renderer.setClearColor(color.r, color.g, color.b, color.a);
}

void FoveaOnWindowResized(uint32_t width, uint32_t height){
	getInstance().renderer.windowResized(width, height);
}

static inline VkCommandBuffer& frameCommandBuffer(){
	return getInstance().commandBuffer;
}

void FoveaBeginFrame(void){
	frameCommandBuffer() = getInstance().renderer.beginFrame();
}

void FoveaBeginSwapChainRenderPass(void){
	VkCommandBuffer& commandBuffer = frameCommandBuffer();
	getInstance().renderer.beginSwapChainRenderPass(commandBuffer);
}

void FoveaEndSwapChainRenderPass(void){
	getInstance().renderer.endSwapChainRenderPass(frameCommandBuffer());
}

void FoveaEndFrame(void){
	getInstance().renderer.endFrame();
}

void FoveaSetScene(void *v, uint32_t vertexCount){
	getInstance().renderer.setScene(v, vertexCount);
}

void FoveaSetSceneVertexSize(uint32_t size){
	getInstance().renderer.setSceneVertexSize(size, getInstance().physicalDevice.getProperties().limits.nonCoherentAtomSize);
}

uint32_t FoveaGetSceneVertexSize(){
	return getInstance().renderer.getSceneVertexSize();
}

void* getSceneBuffer(){
	return getInstance().renderer.getSceneBuffer();
}

uint32_t FoveaGetGeneraUsageVertexSize(){
	return getInstance().renderer.getGeneralUsageVertexSize();
}

void* getGeneralUsageBuffer(){
	return getInstance().renderer.getGeneralUsageBuffer();
}

void FoveaRenderScene(void){
	getInstance().renderer.renderScene(frameCommandBuffer());
}

void FoveaDefaultShaderCreateInfo(FoveaShaderCreateInfo *createInfo){
	createInfo->sample = FoveaSample_1;
	createInfo->type = FoveaShaderType_Graphic;
	createInfo->pushConstantSize = 0;
	createInfo->base = FOVEA_NONE;
	createInfo->target = FOVEA_NONE;
	createInfo->vertexAttributsCount = 0;
	createInfo->vertexAttributes = nullptr;
	createInfo->vertexInputSize = 0;
	createInfo->descriptorSetCount = 0;
	createInfo->descriptorSets = nullptr;
}

PipelineBuilder FoveaCreatePipelineBuilderFromShaderCreateInfo(FoveaShaderCreateInfo *createInfo){
	PipelineBuilder builder;

	if (createInfo->vertexFilepath != nullptr) builder.setShaderStage(PipelineStage::VERTEX, createInfo->vertexFilepath);
	if (createInfo->fragmentFilepath != nullptr) builder.setShaderStage(PipelineStage::FRAGMENT, createInfo->fragmentFilepath);
	if (createInfo->geometryFilepath != nullptr) builder.setShaderStage(PipelineStage::GEOMETRY, createInfo->geometryFilepath);
	if (createInfo->computeFilepath != nullptr) builder.setShaderStage(PipelineStage::COMPUTE, createInfo->computeFilepath);
	builder->multisampleInfo.rasterizationSamples = FoveaSampleToVkSample(createInfo->sample);

	if (createInfo->target != FOVEA_NONE){
		builder.setRenderPass(getInstance().renderTargetLibrary.get(createInfo->target)->getRenderPass());
	} else {
		builder.setRenderPass(getInstance().renderer.getSwapChain().getRenderPass());
	}
	
	builder.setPushConstant(createInfo->pushConstantSize, FoveaShaderTypeToPipelineStage(createInfo->type));

	if (createInfo->base != FOVEA_NONE){
		builder.setBase(getInstance().pipelineLibrary.get(createInfo->base));
	}

	PipelineVertexDescription vertexDescription;

	if (createInfo->vertexInputSize > 0){
		vertexDescription.attributeDescriptions.resize(createInfo->vertexAttributsCount);
		for (int i=0; i<createInfo->vertexAttributsCount; i++){
			auto &des = vertexDescription.attributeDescriptions[i];
			auto &bui = createInfo->vertexAttributes[i];

			des.binding = 0;
			des.location = i;
			des.format = FoveaFormatToVkFormat(bui.format);
			des.offset = bui.offset;
		}

		vertexDescription.bindingDescription.binding = 0;
		vertexDescription.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vertexDescription.bindingDescription.stride = createInfo->vertexInputSize;

		builder.setVertexDescription(vertexDescription);
	}

	for (uint32_t i=0; i<createInfo->descriptorSetCount; i++){
		auto d = getInstance().descriptorSetLibrary.get(createInfo->descriptorSets[i]);
		builder.pushSet(d);
	}

	builder->rasterizationInfo.polygonMode = FoveaPolygonModeToVkPolygonMode(createInfo->polygonMode);
	builder->rasterizationInfo.lineWidth = createInfo->lineWidth;
	builder->depthStencilInfo.depthTestEnable = createInfo->depthTest;
	builder->colorBlendAttachment.blendEnable = createInfo->blending;

	return builder;
}

FoveaShader FoveaCreateShader(FoveaShaderCreateInfo *createInfo){
	PipelineBuilder builder = FoveaCreatePipelineBuilderFromShaderCreateInfo(createInfo);
	Pipeline* pipeline = new Pipeline(&builder);
	return static_cast<FoveaShader>(getInstance().pipelineLibrary.push(pipeline));
}

void FoveaDestroyShader(FoveaShader shader){
	getInstance().pipelineLibrary.erase(shader);
}

void FoveaUseShader(FoveaShader shader, uint32_t setsIndex[]){
	auto pipeline = getInstance().pipelineLibrary.get(shader);
	pipeline->bind(frameCommandBuffer(), setsIndex);
}

void FoveaSetShaderPushConstant(FoveaShader shader, void *data){
	getInstance().pipelineLibrary.get(shader)->bindPushConstant(frameCommandBuffer(), data);
}

FoveaShader FoveaReserveShader(void){
	return getInstance().pipelineLibrary.reserve();
}

void FoveaLoadResevedShader(FoveaShader shader, FoveaShaderCreateInfo *createInfo){
	PipelineBuilder builder = FoveaCreatePipelineBuilderFromShaderCreateInfo(createInfo);
	Pipeline* pipeline = new Pipeline(&builder);
	getInstance().pipelineLibrary.set(shader, pipeline);
}

RenderTargetBuilder RenderTargetBuilderFromFoveaRenderTargetCreateInfo(FoveaRenderTargetCreateInfo *createInfo){
	RenderTargetBuilder builder;

	FramebufferAttachments attachments;

	attachments.attachments.resize(createInfo->colorAttachmentCount);

	for (uint32_t i=0; i<createInfo->colorAttachmentCount; i++){
		auto &attachment = attachments.attachments[i];
		auto &info = createInfo->colorAttachments[i];

		attachment.format = FoveaImageFormatToVkFormat(info.format);
		attachment.tiling = FoveaImageTilingToVulkanImageTiling(info.tiling);
		attachment.samples = FoveaSampleToVkSample(info.sample);
	}

	builder->setAttachments(attachments);
	builder->setExtent(FoveaUIVec2ToVkExtent(createInfo->size));

	for (uint32_t i=0; i<createInfo->colorAttachmentCount; i++){
		builder.setAttachmentClearColor(i, FoveaColorToRenderTargetClearColor(createInfo->colorAttachments[i].clearColor));
	}

	if (createInfo->depthStencilEnabled == Fovea_True){
		builder->enableDepthAttachment(FoveaImageFormatToVkFormat(createInfo->depthFormat));
		builder.setDepthStencilClearColor(createInfo->depthClearValue, createInfo->stencilClearValue);
	}
	return builder;
}

FoveaRenderTarget FoveaCreateRenderTarget(FoveaRenderTargetCreateInfo *createInfo){
	RenderTargetBuilder builder = RenderTargetBuilderFromFoveaRenderTargetCreateInfo(createInfo);
	RenderTarget* renderTarget = new RenderTarget(builder);
	return static_cast<FoveaRenderTarget>(getInstance().renderTargetLibrary.push(renderTarget));
}

void FoveaLoadReservedRenderTarget(FoveaRenderTarget renderTarget, FoveaRenderTargetCreateInfo* createInfo){
	RenderTargetBuilder builder = RenderTargetBuilderFromFoveaRenderTargetCreateInfo(createInfo);
	RenderTarget* target = new RenderTarget(builder);
	getInstance().renderTargetLibrary.set(renderTarget, target);
}

void FoveaDestroyRenderTarget(FoveaRenderTarget renderTarget){
	getInstance().renderTargetLibrary.erase(renderTarget);
}

void FoveaBeginRenderTarget(FoveaRenderTarget renderTarget){
	getInstance().renderTargetLibrary.get(renderTarget)->beginRenderPass(frameCommandBuffer());
}

void FoveaEndRenderTarget(FoveaRenderTarget renderTarget){
	getInstance().renderTargetLibrary.get(renderTarget)->endRenderPass(frameCommandBuffer());
}

void FoveaResizeRenderTarget(FoveaRenderTarget renderTarget, FoveaUIVec2 size){
	getInstance().renderTargetLibrary.get(renderTarget)->resize(FoveaUIVec2ToVkExtent(size));
}

DescriptorSetBuilder DescriptorSetBuilderFromFoveaDescriptorSetCreateInfo(FoveaDescriptorSetCreateInfo *createInfo){
	DescriptorSetBuilder builder;

	builder.setDescriptorSetCount(createInfo->setCount);
	
	std::vector<DescriptorSetBuilder::Descriptor> descriptors(createInfo->descriptorCount);
	for (uint32_t i=0; i<createInfo->descriptorCount; i++){
		auto &d = descriptors[i];
		auto &db = createInfo->descriptors[i];

		std::vector<VkDescriptorImageInfo> info(db.textureCount);
		for (uint32_t k=0; k<db.textureCount; k++){
			auto descriptorInfo = getInstance().textureLibrary.get(db.textures[k])->getDescriptorInfo();
			info[k] = descriptorInfo;
		}

		d.binding = db.binding;
		d.bufferSize = db.bufferSize;
		d.imageCount = db.textureCount;
		d.imageInfos = info;
		d.type = FoveaDescriptorTypeToVkDescriptorType(db.type);
		d.stage = FoveaShaderStagePipelineStageFlags(db.stage);
	}

	builder.setDescriptors(descriptors);
	return builder;
}

FoveaDescriptorSet FoveaCreateDescriptorSet(FoveaDescriptorSetCreateInfo* createInfo){
	DescriptorSetBuilder builder = DescriptorSetBuilderFromFoveaDescriptorSetCreateInfo(createInfo);
	DescriptorSet* descriptorSet = new DescriptorSet(builder);
	return static_cast<FoveaDescriptorSet>(getInstance().descriptorSetLibrary.push(descriptorSet));
}

void FoveaLoadReservedDescriptorSet(FoveaDescriptorSet descriptorSet, FoveaDescriptorSetCreateInfo* createInfo){
	DescriptorSetBuilder builder = DescriptorSetBuilderFromFoveaDescriptorSetCreateInfo(createInfo);
	DescriptorSet* descriptor = new DescriptorSet(builder);
	getInstance().descriptorSetLibrary.set(descriptorSet, descriptor);
}

FoveaRenderTarget FoveaReserveRenderTarget(void){
	return getInstance().renderTargetLibrary.reserve();
}

void FoveaDestroyDescriptorSet(FoveaDescriptorSet descriptorSet){
	getInstance().descriptorSetLibrary.erase(descriptorSet);
}

void FoveaWriteToDescriptorSetBuffer(FoveaDescriptorSet descriptorSet, uint32_t setIndex, uint32_t binding, void* data){
	getInstance().descriptorSetLibrary.get(descriptorSet)->writeBuffer(setIndex, binding, data);
}

void FoveaSetDescriptorSetTexture(FoveaDescriptorSet descriptorSet, FoveaTexture texture, uint32_t setIndex, uint32_t binding, uint32_t textureIndex){
	auto t = getInstance().textureLibrary.get(texture);
	getInstance().descriptorSetLibrary.get(descriptorSet)->setDescriptorImage(setIndex, binding, textureIndex, t->getDescriptorInfo());
}

FoveaDescriptorSet FoveaReserveDescriptorSet(void){
	return getInstance().descriptorSetLibrary.reserve();
}

static inline TextureBuilder FoveaTextureCreateInfoToTextureBuilder(FoveaTextureCreateInfo *info){
	TextureBuilder builder;
	
	builder.magFilter = FoveaImageTilingToVulkanFilter(info->magFilter);
	builder.minFilter = FoveaImageTilingToVulkanFilter(info->minFilter);
	builder.addressModeU = FoveaTextureAddressModeToVkSamplerAddressMode(info->addressModeX);
	builder.addressModeV = FoveaTextureAddressModeToVkSamplerAddressMode(info->addressModeY);
	builder.anisotropy = info->anisotropy;
	builder.normalizeCoords = info->normalizedCoords;
	builder.tiling = FoveaImageTilingToVulkanImageTiling(info->tiling);
	builder.samples = FoveaSampleToVkSample(info->samples);
	return builder;
}

FoveaTextureCreateInfo FoveaDefaultTextureCreateInfo(void){
	FoveaTextureCreateInfo info;
	info.magFilter = FoveaImageTiling_Nearest;
	info.minFilter = FoveaImageTiling_Nearest;
	info.addressModeX = FoveaTextureAddressMode_ClampToBorder;
	info.addressModeY = FoveaTextureAddressMode_ClampToBorder;
	info.anisotropy = Fovea_False;
	info.normalizedCoords = Fovea_True;
	info.tiling = FoveaImageTiling_Nearest;
	info.samples = FoveaSample_1;
	return info;
}

FoveaTexture FoveaCreateTextureFromRenderTarget(FoveaRenderTarget renderTarget, uint32_t attachment, FoveaTextureCreateInfo* createInfo){
	auto framebuffer = getInstance().renderTargetLibrary.get(renderTarget)->getFramebuffer();
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	Texture *texture = new Texture(*framebuffer, attachment, builder);

	return getInstance().textureLibrary.push(texture);
}

FoveaTexture FoveaCreateTextureFromPath(const char* path, FoveaTextureCreateInfo* createInfo){
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	Texture *texture = new Texture(path, builder);

	return getInstance().textureLibrary.push(texture);
}

FoveaTexture FoveaCreateTextureFromData(FoveaImageFormat format, FoveaUIVec2 size, void* data, FoveaTextureCreateInfo* createInfo){
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	Texture *texture = new Texture(FoveaImageFormatToVkFormat(format), FoveaUIVec2ToVkExtent(size), data, FoveaImageFormatToPixelSize(format), builder);

	return getInstance().textureLibrary.push(texture);
}

void FoveaDestroyTexture(FoveaTexture texture){
	getInstance().textureLibrary.erase(texture);
}

FoveaTexture FoveaReserveTexture(void){
	return getInstance().textureLibrary.reserve();
}

void FoveaLoadReservedTextureFromRenderTarget(FoveaTexture texture, FoveaRenderTarget renderTarget, uint32_t attachment, FoveaTextureCreateInfo* createInfo){
	auto framebuffer = getInstance().renderTargetLibrary.get(renderTarget)->getFramebuffer();
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	
	Texture *t = new Texture(*framebuffer, attachment, builder);
	getInstance().textureLibrary.set(texture, t);
}

void FoveaLoadReservedTextureFromPath(FoveaTexture texture, const char* path, FoveaTextureCreateInfo* createInfo){
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	Texture *t = new Texture(path, builder);

	getInstance().textureLibrary.set(texture, t);
}

void FoveaLoadReservedTextureFromData(FoveaTexture texture, FoveaImageFormat format, FoveaUIVec2 size, void* data, FoveaTextureCreateInfo* createInfo){
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(createInfo);
	Texture *t = new Texture(FoveaImageFormatToVkFormat(format), FoveaUIVec2ToVkExtent(size), data, FoveaImageFormatToPixelSize(format), builder);

	getInstance().textureLibrary.set(texture, t);
}

static inline FoveaImageFormat channelCountToImageFormat(int channel){
	printf("%d\n", channel);
	switch (channel){
		case 1: return FoveaImageFormat_R8;
		case 2: return FoveaImageFormat_R8G8;
		case 3: return FoveaImageFormat_R8G8B8;
		case 4: return FoveaImageFormat_R8G8B8A8;
	}
	return FoveaImageFormat_R8G8B8;
}


struct LoadTextureAsyncData{
	FoveaTexture *texture;
	FoveaTextureCreateInfo* createInfo;
	const char* path;
	pthread_mutex_t *lock;
};

void* loadTextureAsync(void *ptr){
	LoadTextureAsyncData* info = static_cast<LoadTextureAsyncData*>(ptr);
	TextureBuilder builder = FoveaTextureCreateInfoToTextureBuilder(info->createInfo);
	
	int w, h, channel;
	void* pixels = stbi_load(info->path, &w, &h, &channel, STBI_default);
	FoveaImageFormat format = channelCountToImageFormat(channel);

	pthread_mutex_lock(info->lock);
	FoveaLoadReservedTextureFromData(*info->texture, format, {static_cast<uint32_t>(w), static_cast<uint32_t>(h)}, pixels, info->createInfo);
	pthread_mutex_unlock(info->lock);

	stbi_image_free(pixels);
	return nullptr;
}

FoveaTexture* FoveaCreateTexturesFromPaths(const char* paths[], FoveaTextureCreateInfo* createInfos, uint32_t textureCount){
	FoveaTexture *textures = new FoveaTexture[textureCount];
	pthread_t *threads = new pthread_t[textureCount];
	LoadTextureAsyncData* datas = new LoadTextureAsyncData[textureCount];
	pthread_mutex_t lock;
	pthread_mutex_init(&lock, nullptr);

	for (uint32_t i=0; i<textureCount; i++){
		textures[i] = getInstance().textureLibrary.reserve();
		datas[i].createInfo = &createInfos[i];
		datas[i].lock = &lock;
		datas[i].path = paths[i];
		datas[i].texture = &textures[i];
		pthread_create(&threads[i], nullptr, &loadTextureAsync, &datas[i]);
	}

	for (uint32_t i=0; i<textureCount; i++){
		pthread_join(threads[i], nullptr);
	}

	pthread_mutex_destroy(&lock);
	delete[] threads;
	delete[] datas;

	return textures;
}

void FoveaSetSceneData(uint32_t offset, uint32_t size, void* data){
	getInstance().renderer.setSceneData(offset, size, data);
}

void FoveaFlushSceneData(uint32_t offset, uint32_t size){
	getInstance().renderer.flushSceneData(offset, size);
}

void FoveaSetGeneralUsageVertexSize(uint32_t size){
	getInstance().renderer.setGeneralUsageVertexSize(size, getInstance().physicalDevice.getProperties().limits.nonCoherentAtomSize);
}

void FoveaSetGeneralUsage(void *v, uint32_t vertexCount){
	getInstance().renderer.setGeneralUsageData(v, vertexCount);
}

void FoveaSetGeneralUsageData(uint32_t offset, uint32_t count, void *data){
	getInstance().renderer.setGeneralUsageData(offset, count, data);
}

void FoveaFlushGeneralUsageData(uint32_t offset, uint32_t count){
	getInstance().renderer.flushGeneralUsageData(offset, count);
}

void FoveaRenderGeneralUsageData(void){
	getInstance().renderer.renderGeneralUsageData(frameCommandBuffer());
}

void FoveaSetGeneralUsageTopology(FoveaTopology topology){
	getInstance().renderer.setGeneralUsageTopology(FoveaTopologyToRendererTopology(topology));
}

void FoveaRenderQuadScene(void *v0, void *v1, void *v2, void *v3){
	getInstance().renderer.renderQuadScene(v0, v1, v2, v3);
}

void FoveaRenderGeneralUsageQuad(void *v0, void *v1, void *v2, void *v3){
	getInstance().renderer.renderGeneralUsageQuad(v0, v1, v2, v3);
}

void FoveaRenderGeneralUsageTrigone(void *v0, void *v1, void *v2){
	getInstance().renderer.renderGeneralUsageTrigone(v0, v1, v2);
}

void FoveaRenderGeneralUsageLine(void *v0, void *v1){
	getInstance().renderer.renderGeneralUsageLine(v0, v1);
}