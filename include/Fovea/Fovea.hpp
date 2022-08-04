#pragma once

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

namespace Fovea{

	struct ColorRGBA{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 0.f;
	};

	struct ColorRGB{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
	};


	void initialize(void *window);

	void setClearColor(const ColorRGBA &color);

	void setClearColor(const ColorRGB &color);

	void onWindowResized(uint32_t width, uint32_t height);

	void beginFrame();

	void endFrame();
}