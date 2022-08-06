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

	typedef struct FoveaColor{
		float r;
		float g;
		float b;
		float a;
	} FoveaColor;

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

	void FoveaInitialize(void *window);

	void FoveaSetClearColor(const FoveaColor &color);

	void FoveaOnWindowResized(uint32_t width, uint32_t height);

	void FoveaBeginFrame();
	void FoveaEndFrame();

	// =================== shaders ====================

	typedef struct FoveaShader{
		uint64_t id;
	} FoveaShader;

	typedef enum FoveaShaderSample{
		FoveaShaderSample_1,
		FoveaShaderSample_2,
		FoveaShaderSample_4,
		FoveaShaderSample_8,
		FoveaShaderSample_16,
		FoveaShaderSample_32,
		FoveaShaderSample_64,
	} FoveaShaderSample;

	typedef struct FoveaShaderCreateInfo{
		FoveaShaderSample sample;

		const char* vertexFilepath;
		const char* fragmentFilepath;
		const char* geometryFilepath;
		const char* computeFilepath;
	} FoveaShaderCreateInfo;

	void FoveaDefaultCreateInfo(FoveaShaderCreateInfo *createInfo);

	FoveaShader FoveaGetShaderFromName(const char *name);

	FoveaShader FoveaCreateShader(const char *name, FoveaShaderCreateInfo *createInfo);

	void FoveaDestroyShader(FoveaShader shader);

	void FoveaUseShader(FoveaShader shader);

#ifdef __cplusplus
	}
#endif

#endif