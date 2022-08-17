#ifndef __FOVEA__H__
#define __FOVEA__H__

#ifndef _STDINT_H
	typedef char int8_t;
	typedef unsigned char uint8_t;
	typedef signed char sint8_t;
	typedef short int16_t;
	typedef unsigned short uint16_t;
	typedef signed short sint16_t;
	typedef int int32_t;
	typedef unsigned int uint32_t;
	typedef signed int sint32_t;
	typedef long long int64_t;
	typedef unsigned long long uint64_t;
	typedef signed long long sint64_t;
#endif

#ifdef __cplusplus
	extern "C"{
#endif

#define FOVEA_NONE (uint64_t)-1
#define FOVEA_WHOLE_SIZE (~0ULL)
#define FOVEA_DEPTH_ATTACHMENT (uint32_t)-1

typedef uint64_t FoveaShader;
typedef uint64_t FoveaRenderTarget;
typedef uint64_t FoveaDescriptorSet;
typedef uint64_t FoveaTexture;

typedef enum FoveaBool{
	Fovea_False = 0,
	Fovea_True = 1,
} FoveaBool;

typedef enum FoveaFormat{
	FoveaFormat_Float = 0,
	FoveaFormat_Float_vec2 = 1,
	FoveaFormat_Float_vec3 = 2,
	FoveaFormat_Float_vec4 = 3,
	FoveaFormat_Int = 4,
	FoveaFormat_Int_vec2 = 5,
	FoveaFormat_Int_vec3 = 6,
	FoveaFormat_Int_vec4 = 7,
	FoveaFormat_Uint = 8,
	FoveaFormat_Uint_vec2 = 9,
	FoveaFormat_Uint_vec3 = 10,
	FoveaFormat_Uint_vec4 = 11,
} FoveaFormat;

typedef enum FoveaImageFormat{
	FoveaImageFormat_R8 = 0,
	FoveaImageFormat_R8G8 = 1,
	FoveaImageFormat_R8G8B8 = 2,
	FoveaImageFormat_R8G8B8A8 = 3,
	FoveaImageFormat_R16 = 4,
	FoveaImageFormat_R16G16 = 5,
	FoveaImageFormat_R16G16B16 = 6,
	FoveaImageFormat_R16G16B16A16 = 7,
	FoveaImageFormat_R32 = 8,
	FoveaImageFormat_R32G32 = 9,
	FoveaImageFormat_R32G32B32 = 10,
	FoveaImageFormat_R32G32B32A32 = 11,
	FoveaImageFormat_R64 = 12,
	FoveaImageFormat_R64G64 = 13,
	FoveaImageFormat_R64G64B64 = 14,
	FoveaImageFormat_R64G64B64A64 = 15,
	FoveaImageFormat_R4G4 = 16,
	FoveaImageFormat_R4G4B4A4 = 17,
	FoveaImageFormat_R5G5B5A1 = 18,
	FoveaImageFormat_R5B6G5 = 19,
	FoveaImageFormat_D32 = 20,
	FoveaImageFormat_D32_S8 = 21,
	FoveaImageFormat_D24_S8 = 22,
} FoveaImageFormat;

typedef enum FoveaImageTiling{
	FoveaImageTiling_Linear = 0,
	FoveaImageTiling_Nearest = 1,
} FoveaImageTiling;

typedef enum FoveaSample{
	FoveaSample_1 = 0,
	FoveaSample_2 = 1,
	FoveaSample_4 = 2,
	FoveaSample_8 = 3,
	FoveaSample_16 = 4,
	FoveaSample_32 = 5,
	FoveaSample_64 = 6,
} FoveaSample;

typedef enum FoveaShaderType{
	FoveaShaderType_Graphic = 0,
	FoveaShaderType_Compute = 1,
} FoveaShaderType;

typedef enum FoveaShaderStage{
	FoveaShaderStage_Vertex = 1 << 0,
	FoveaShaderStage_Geometry = 1 << 1,
	FoveaShaderStage_Fragment = 1 << 2,
	FoveaShaderStage_Compute = 1 << 3,
} FoveaShaderStage;

typedef enum FoveaTextureAddressMode{
	FoveaTextureAddressMode_Repeat = 0,
	FoveaTextureAddressMode_MirroredRepeat = 1,
	FoveaTextureAddressMode_ClampToEdge = 2,
	FoveaTextureAddressMode_ClampToBorder = 3,
} FoveaTextureAddressMode;

typedef enum FoveaDescriptorType{
	FoveaDescriptorType_Texture = 0,
	FoveaDescriptorType_Buffer = 1,
} FoveaDescriptorType;

typedef enum FoveaTopology{
	FoveaTopology_Quad = 0,
	FoveaTopology_Trigone = 1,
	FoveaTopology_Line = 2,
} FoveaTopology;

typedef struct FoveaColor{
	float r;
	float g;
	float b;
	float a;
} FoveaColor;

typedef struct FoveaUIVec2{
	uint32_t width;
	uint32_t height;
} FoveaUIVec2;

typedef struct FoveaShaderVertexAttribute{
	FoveaFormat format;
	uint32_t offset;
} FoveaShaderVertexAttribute;

typedef struct FoveaShaderCreateInfo{
	FoveaSample sample;
	FoveaShaderType type;
	uint32_t pushConstantSize;
	FoveaRenderTarget target;
	FoveaShader base;
	uint32_t vertexInputSize;
	FoveaShaderVertexAttribute* vertexAttributes;
	uint32_t vertexAttributsCount;
	FoveaDescriptorSet* descriptorSets;
	uint32_t descriptorSetCount;

	const char* vertexFilepath;
	const char* fragmentFilepath;
	const char* geometryFilepath;
	const char* computeFilepath;
} FoveaShaderCreateInfo;

typedef struct FoveaRenderTargetAttachment{
	FoveaImageFormat format;
	FoveaImageTiling tiling;
	FoveaSample sample;
	FoveaColor clearColor;
} FoveaRenderTargetAttachment;

typedef struct FoveaRenderTargetCreateInfo{
	uint32_t colorAttachmentCount;
	FoveaRenderTargetAttachment* colorAttachments;
	FoveaImageFormat depthFormat;
	FoveaBool depthStencilEnabled;
	float depthClearValue;
	uint32_t stencilClearValue;
	FoveaUIVec2 size;
} FoveaRenderTargetCreateInfo;

typedef struct FoveaDescriptorInfo{
	uint32_t binding;
	int stage;
	FoveaDescriptorType type;
	
	uint32_t bufferSize;
	uint32_t textureCount;
	FoveaTexture* textures;

} FoveaDescriptorInfo;

typedef struct FoveaDescriptorSetCreateInfo{
	FoveaDescriptorInfo* descriptors;
	uint32_t descriptorCount;
	uint32_t setCount;
} FoveaDescriptorSetCreateInfo;

typedef struct FoveaTextureCreateInfo{
	FoveaImageTiling magFilter;
	FoveaImageTiling minFilter;
	FoveaTextureAddressMode addressModeX;
	FoveaTextureAddressMode addressModeY;
	FoveaBool anisotropy;
	FoveaBool normalizedCoords;

	FoveaImageTiling tiling;
	FoveaSample samples;

} FoveaTextureCreateInfo;

// ========================= base functions =========================

void FoveaInitialize(void *window);

void FoveaSetClearColor(const FoveaColor color);

void FoveaOnWindowResized(uint32_t width, uint32_t height);

void FoveaBeginFrame(void);

void FoveaEndFrame(void);

void FoveaSetScene(void *v, uint32_t vertexCount);

void FoveaSetSceneVertexSize(uint32_t size);

void FoveaRenderScene(void);

void FoveaSetSceneData(uint32_t offset, uint32_t count, void* data);

void FoveaFlushSceneData(uint32_t offset, uint32_t count);

void FoveaSetGeneralUsageVertexSize(uint32_t size);

void FoveaSetGeneralUsage(void *v, uint32_t vertexCount);

void FoveaSetGeneralUsageData(uint32_t offset, uint32_t count, void *data);

void FoveaFlushGeneralUsageData(uint32_t offset, uint32_t count);

void FoveaRenderGeneralUsageData(void);

void FoveaSetGeneralUsageTopology(FoveaTopology typology);

void FoveaBeginSwapChainRenderPass(void);

void FoveaEndSwapChainRenderPass(void);

// =================== shaders ====================

void FoveaDefaultShaderCreateInfo(FoveaShaderCreateInfo *createInfo);

FoveaShader FoveaCreateShader(FoveaShaderCreateInfo *createInfo);

void FoveaDestroyShader(FoveaShader shader);

void FoveaUseShader(FoveaShader shader, uint32_t setsIndex[]);

void FoveaSetShaderPushConstant(FoveaShader shader, void *data);

FoveaShader FoveaReserveShader(void);

void FoveaLoadResevedShader(FoveaShader shader, FoveaShaderCreateInfo *createInfo);

// ========================= render target =========================

FoveaRenderTarget FoveaCreateRenderTarget(FoveaRenderTargetCreateInfo *createInfo);

void FoveaDestroyRenderTarget(FoveaRenderTarget renderTarget);

void FoveaBeginRenderTarget(FoveaRenderTarget renderTarget);

void FoveaEndRenderTarget(FoveaRenderTarget renderTarget);

void FoveaResizeRenderTarget(FoveaRenderTarget renderTarget, FoveaUIVec2 size);

FoveaRenderTarget FoveaReserveRenderTarget(void);

void FoveaLoadReservedRenderTarget(FoveaRenderTarget renderTarget, FoveaRenderTargetCreateInfo* createInfo);

// ========================= descriptor set =========================

FoveaDescriptorSet FoveaCreateDescriptorSet(FoveaDescriptorSetCreateInfo* createInfo);

void FoveaDestroyDescriptorSet(FoveaDescriptorSet descriptorSet);

void FoveaWriteToDescriptorSetBuffer(FoveaDescriptorSet descriptorSet, uint32_t setIndex, uint32_t binding, void* data);

void FoveaSetDescriptorSetTexture(FoveaDescriptorSet descriptorSet, FoveaTexture texture, uint32_t setIndex, uint32_t binding, uint32_t textureIndex);

FoveaDescriptorSet FoveaReserveDescriptorSet(void);

void FoveaLoadReservedDescriptorSet(FoveaDescriptorSet descriptorSet, FoveaDescriptorSetCreateInfo* createInfo);

// ========================= descriptor set =========================

FoveaTextureCreateInfo FoveaDefaultTextureCreateInfo(void);

FoveaTexture FoveaCreateTextureFromRenderTarget(FoveaRenderTarget renderTarget, uint32_t attachment, FoveaTextureCreateInfo* createInfo);

FoveaTexture FoveaCreateTextureFromPath(const char* path, FoveaTextureCreateInfo* createInfo);

FoveaTexture* FoveaCreateTexturesFromPaths(const char* paths[], FoveaTextureCreateInfo* createInfos, uint32_t textureCount);

FoveaTexture FoveaCreateTextureFromData(FoveaImageFormat format, FoveaUIVec2 size, void* data, FoveaTextureCreateInfo* createInfo);

void FoveaDestroyTexture(FoveaTexture texture);

FoveaTexture FoveaReserveTexture(void);

void FoveaLoadReservedTextureFromRenderTarget(FoveaTexture texture, FoveaRenderTarget renderTarget, uint32_t attachment, FoveaTextureCreateInfo* createInfo);

void FoveaLoadReservedTextureFromPath(FoveaTexture texture, const char* path, FoveaTextureCreateInfo* createInfo);

void FoveaLoadReservedTextureFromData(FoveaTexture texture, FoveaImageFormat format, FoveaUIVec2 size, void* data, FoveaTextureCreateInfo* createInfo);

#ifdef __cplusplus
	}
#endif

#endif