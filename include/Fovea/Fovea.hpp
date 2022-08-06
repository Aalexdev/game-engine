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

typedef uint64_t FoveaShader;
typedef uint64_t FoveaRenderTarget;

typedef enum FoveaBool{
	Fovea_False = 0,
	Fovea_True = 1,
} FoveaBool;

typedef enum FoveaFormats{
	FoveaFormat_Float,
	FoveaFormat_Float_vec2,
	FoveaFormat_Float_vec3,
	FoveaFormat_Float_vec4,
	FoveaFormat_Int,
	FoveaFormat_Int_vec2,
	FoveaFormat_Int_vec3,
	FoveaFormat_Int_vec4,
	FoveaFormat_Uint,
	FoveaFormat_Uint_vec2,
	FoveaFormat_Uint_vec3,
	FoveaFormat_Uint_vec4,
} FoveaFormats;

typedef enum FoveaImageFormat{
	FoveaImageFormat_R8,
	FoveaImageFormat_R8G8,
	FoveaImageFormat_R8G8B8,
	FoveaImageFormat_R8G8B8A8,
	FoveaImageFormat_R16,
	FoveaImageFormat_R16G16,
	FoveaImageFormat_R16G16B16,
	FoveaImageFormat_R16G16B16A16,
	FoveaImageFormat_R32,
	FoveaImageFormat_R32G32,
	FoveaImageFormat_R32G32B32,
	FoveaImageFormat_R32G32B32A32,
	FoveaImageFormat_R64,
	FoveaImageFormat_R64G64,
	FoveaImageFormat_R64G64B64,
	FoveaImageFormat_R64G64B64A64,
	FoveaImageFormat_R4G4,
	FoveaImageFormat_R4G4B4A4,
	FoveaImageFormat_R5G5B5A1,
	FoveaImageFormat_R5B6G5,
	FoveaImageFormat_D32,
	FoveaImageFormat_D32_S8,
	FoveaImageFormat_D24_S8,
} FoveaImageFormat;

typedef enum FoveaImageTiling{
	FoveaImageTiling_Linear,
	FoveaImageTiling_Nearest,
} FoveaImageTiling;

typedef enum FoveaSample{
	FoveaSample_1,
	FoveaSample_2,
	FoveaSample_4,
	FoveaSample_8,
	FoveaSample_16,
	FoveaSample_32,
	FoveaSample_64,
} FoveaSample;

typedef enum FoveaShaderType{
	FoveaShaderType_Graphic,
	FoveaShaderType_Compute
} FoveaShaderType;

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

typedef struct FoveaShaderCreateInfo{
	FoveaSample sample;
	FoveaShaderType type;
	uint32_t pushConstantSize;

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

void FoveaInitialize(void *window);

void FoveaSetClearColor(const FoveaColor &color);

void FoveaOnWindowResized(uint32_t width, uint32_t height);

void FoveaBeginFrame();
void FoveaEndFrame();

// =================== shaders ====================

void FoveaDefaultCreateInfo(FoveaShaderCreateInfo *createInfo);

FoveaShader FoveaGetShaderFromName(const char *name);

FoveaShader FoveaCreateShader(const char *name, FoveaShaderCreateInfo *createInfo);

void FoveaDestroyShader(FoveaShader shader);

void FoveaUseShader(FoveaShader shader);

void FoveaSetShaderPushConstant(FoveaShader shader, void *data);

// ========================= render target =========================

FoveaRenderTarget FoveaCreateRenderTarget(const char *name, FoveaRenderTargetCreateInfo *createInfo);

void FoveaDestroyRenderTarget(FoveaRenderTarget renderTarget);
FoveaRenderTarget getRenderTargetFromName(const char *name);

void FoveaBeginRenderTarget(FoveaRenderTarget renderTarget);
void FoveaEndRenderTarget(FoveaRenderTarget renderTarget);

	
#ifdef __cplusplus
	}
#endif

#endif